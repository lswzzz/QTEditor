#include "ImageListWidget.h"
#include <qdrag.h>
#include <qmimedata.h>
#include <qevent.h>
#include "Global.h"
#include "FontChina.h"
#include <qmenu.h>
#include <qcolor.h>
#include "LayerManagerWidget.h"
#include "QMessageBox"
#include "FontChina.h"
#include "MyLogger.h"
#include "QFileDialog"
#include "MySqlLiteManager.h"
#include "ResourceView.h"
#include "ImgTransPath.h"

ImageListWidget::ImageListWidget(QWidget *parent)
	:QListWidget(parent)
{
	connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
	connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
	this->setViewMode(QListView::IconMode);
	this->setIconSize(QSize(50, 50));
	type__ = Image_Type::NormalBlock;
}

ImageListWidget::~ImageListWidget()
{
}

void ImageListWidget::removeAllItem()
{
	int size = this->count();
	for (int i = 0; i < size; i++){
		auto item = this->takeItem(0);
		if (item)
			delete item;
	}
}

void ImageListWidget::itemClicked(QListWidgetItem * item)
{
	if (item->data(Qt::UserRole).type() == QVariant::Int){
		g_filePath = ImgTransPath::getListImagePathByIndex(item->data(Qt::UserRole).toInt()).toStdString();
		g_TemplateFile = "";
		g_ImageType = type__;
		LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
		auto name = layerManager->getCurrentLayerName();
		int count = layerManager->getItemCount();
		if (count > 0 && name != ""){
			auto scene = static_cast<HelloWorld*>(g_scene);
			auto layer = scene->getLayerFromForeManager(name);
			if (layer->isLock())
			{
				QMessageBox::warning(NULL, "warning", FontChina::G2U("当前层已经被锁定"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return;
			}
		}
		layerManager->add_ImageChild();
	}
	else{
		g_filePath = "";
		g_TemplateFile = "";
		setCocCursor(Cursor_Type::Normal);
		HelloWorld* scene = (HelloWorld*)g_scene;
		scene->resetState();
	}
}

void ImageListWidget::itemDoubleClicked(QListWidgetItem* item)
{
	if (item->data(Qt::UserRole).type() == QVariant::Bool){
		auto treeWidget = static_cast<ImageTreeWidget*>(getTreeWidget());
		if (item->data(Qt::UserRole).toBool() == false){
			treeWidget->setCurFolder("");
		}
		else{
			treeWidget->setCurFolder(item->text());
		}
		FlushFromTreeWidget();
	}
}

void ImageListWidget::actionAddRes()
{
	auto treeWidget = static_cast<ImageTreeWidget*>(getTreeWidget());
	treeWidget->actionAddRes();
	FlushFromTreeWidget();
}

void ImageListWidget::actionAddDir()
{
	auto treeWidget = static_cast<ImageTreeWidget*>(getTreeWidget());
	treeWidget->actionAddDir();
	FlushFromTreeWidget();
}

void ImageListWidget::widgetVisible(bool state)
{
	if (state){
		FlushFromTreeWidget();
	}
}

void ImageListWidget::update()
{
	FlushFromTreeWidget();
}

QTreeWidget* ImageListWidget::getTreeWidget()
{
	auto resourceView = static_cast<ResourceView*>(g_resourceView);
	return resourceView->getTreeWidget();
}

void ImageListWidget::copyFolderFromTreeItem(QListWidgetItem* listItem, QTreeWidgetItem* treeItem)
{
	listItem->setIcon(QIcon(":/icon/Resources/folder.ico"));
	listItem->setText(treeItem->text(0));
	listItem->setData(Qt::UserRole, treeItem->data(0, Qt::UserRole).toBool());
}

void ImageListWidget::copyResFromTreeItem(QListWidgetItem* listItem, QTreeWidgetItem* treeItem)
{
	int index = treeItem->data(0, Qt::UserRole).toInt();
	QString ss = ImgTransPath::getListImagePathByIndex(treeItem->data(0, Qt::UserRole).toInt());
	listItem->setIcon(QIcon(ss));
	listItem->setText(treeItem->text(0));
	listItem->setTextColor(treeItem->textColor(0));
	listItem->setData(Qt::UserRole, treeItem->data(0, Qt::UserRole).toInt());
}

void ImageListWidget::FlushFromTreeWidget()
{
	removeAllItem();
	auto treeWidget = static_cast<ImageTreeWidget*>(getTreeWidget());
	auto dirVec = treeWidget->getChildFolder();
	auto resVec = treeWidget->getChildRes();
	if (!treeWidget->isTop()){
		auto itemChild = new QListWidgetItem();
		itemChild->setIcon(QIcon(":/icon/Resources/folder_Back.ico"));
		itemChild->setText(FontChina::G2U("返回上一级"));
		itemChild->setData(Qt::UserRole, false);
		this->insertItem(this->count(), itemChild);
	}
	for (int i = 0; i < dirVec.size(); i++){
		auto itemChild = new QListWidgetItem();
		copyFolderFromTreeItem(itemChild, dirVec.at(i));
		this->insertItem(this->count(), itemChild);
	}
	for (int i = 0; i < resVec.size(); i++){
		auto itemChild = new QListWidgetItem();
		copyResFromTreeItem(itemChild, resVec.at(i));
		this->insertItem(this->count(), itemChild);
	}
}

void ImageListWidget::mousePressEvent(QMouseEvent *event)
{
	QModelIndex clickedIndex = indexAt(event->pos());
	if (clickedIndex.isValid() == false){
		this->setCurrentItem(nullptr);
	}
	else{
		this->setCurrentIndex(clickedIndex);
		auto item = this->currentItem();
		if (item->data(Qt::UserRole).type() == QVariant::Bool){
			g_filePath = "";
			setCocCursor(Cursor_Type::Normal);
			HelloWorld* scene = (HelloWorld*)g_scene;
			scene->resetState();
		}
	}
	QListWidget::mousePressEvent(event);
}

void ImageListWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QListWidget::mouseReleaseEvent(event);
}

void ImageListWidget::mouseMoveEvent(QMouseEvent *event)
{
	QListWidgetItem *item = currentItem();
	if (item && item->data(Qt::UserRole).type() == QVariant::Int){
		startDrag();
	}
}

void ImageListWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* ResMenu = new QMenu(this);
	QModelIndex clickedIndex = indexAt(event->pos());
	if (clickedIndex.isValid() == true){
		QAction* deleteac = new QAction(FontChina::G2U("删除"), this);
		connect(deleteac, SIGNAL(triggered()), this, SLOT(deleteResOrDir()));
		ResMenu->addAction(deleteac);
	}
	QAction* addRes = new QAction(FontChina::G2U("添加资源"), this);
	QAction* addDir = new QAction(FontChina::G2U("添加文件夹"), this);
	connect(addRes, SIGNAL(triggered()), this, SLOT(actionAddRes()));
	connect(addDir, SIGNAL(triggered()), this, SLOT(actionAddDir()));
	ResMenu->addAction(addRes);
	ResMenu->addAction(addDir);
	ResMenu->exec(QCursor::pos());
}

void ImageListWidget::startDrag()
{
	auto *item = currentItem();
	if (item) {
		setEditorCursor(Cursor_Type::addSprite);
		itemClicked(item);
	}
}

void ImageListWidget::deleteResOrDir()
{
	auto item = this->currentItem();
	auto treeWidget = static_cast<ImageTreeWidget*>(getTreeWidget());
	if (item->data(Qt::UserRole).type() == QVariant::Bool){
		treeWidget->deleteResOrDir(item->text(), true);
	}
	else{
		treeWidget->deleteResOrDir(item->text(), false);
	}
	g_filePath = "";
	g_ImageType = Image_Type::NormalBlock;
	delete item;
	addConsoleInfo("删除成功");
}
