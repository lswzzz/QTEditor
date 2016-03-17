#include "ImageTreeWidget.h"
#include "QMenu"
#include "FontChina.h"
#include "MyLogger.h"
#include "QFileDialog"
#include "MySqlLiteManager.h"
#include "cocos2d.h"
#include "Global.h"
#include "QMessageBox"
#include "LayerManagerWidget.h"
#include "QColor"
#include "GlobalConstant.h"
#include "ImgTransPath.h"
#include "ResMap.h"
#include "ResourceView.h"
#include "ExteralView.h"

USING_NS_CC;

ImageTreeWidget::ImageTreeWidget(QWidget *parent)
	:BaseResTreeWidget(parent)
{
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editFileOrDirName(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemClick(QTreeWidgetItem*, int)));
	init();
	dialog = new ResModifyDirDialog();
	dialog->isResourceDialog(type__);
	newDirName = "newFolder";
}

void ImageTreeWidget::init()
{
	type__ = Image_Type::NormalBlock;
	hasPrePacker = true;
	relativeDir = g_c_relaResourceDir;
	relativeListDir = g_c_rela_ResourceListDir;
	ImgTransPath::updatePath();
	imagePath = FileUtils::getInstance()->getSearchPaths().at(0).c_str();
	listPath = imagePath;
	imagePath += relativeDir;
	listPath += relativeListDir;
}

ImageTreeWidget::~ImageTreeWidget()
{
}

void ImageTreeWidget::itemClick(QTreeWidgetItem* item, int columns)
{
	if (item->data(0, Qt::UserRole).type() == QVariant::Int){
		g_filePath = ImgTransPath::getListImagePathByIndex(item->data(0, Qt::UserRole).toInt()).toStdString();
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

void ImageTreeWidget::itemChanged(QTreeWidgetItem* item, int columns)
{
	if (check){
		if (item->data(0, Qt::UserRole).type() == QVariant::Bool){
			if (curDirExist(getSrcName(item->text(0)))){
				item->setText(0, oldName);
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("当前文件夹已经存在"));
			}
			else{
				QString newDir = getDirPath(item);
				QString oldDir = getDirPath(item).section("/", 0, -3) + "/" + oldName;
				QDir dir;
				dir.rename(oldDir, newDir);
				resetChildItemMapData(item);
				SortItem(item);
			}
		}
		else if (item->data(0, Qt::UserRole).type() == QVariant::Int){
			if (curResExist(getSrcName(item->text(0)))){
				item->setText(0, oldName);
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("当前文件已经存在"));
			}
			else{
				QString oldPath = getDirPath(item->parent()) + oldName;
				QString newPath = getDirPath(item->parent()) + getSrcName(item->text(0));
				QFile::rename(oldPath, newPath);
				resetItemMapData(item);
				SortItem(item);
			}

		}
		check = false;
	}
}

void ImageTreeWidget::editFileOrDirName(QTreeWidgetItem* item, int columns)
{
	oldName = getSrcName(item->text(0));
	check = true;
}

void ImageTreeWidget::deleteCurrentResOrDir()
{
	auto* item = this->currentItem();
	if (item->data(0, Qt::UserRole).type() == QVariant::Bool){
		if (!deleteDirNoRefItem(item)){
			return;
		}
		QString path = getCurDirPath();
		DeleteDirectory(path, true);
	}
	else{
		if (getItemRefCount(item) > 0){
			addConsoleInfo("包含引用资源，删除失败!");
			return;
		}
		deleteRes(item->data(0, Qt::UserRole).toInt());
	}
	removeFromItemMap(item);
	delete item;
	g_filePath = "";
	g_ImageType = Image_Type::NormalBlock;
	addConsoleInfo("删除成功");
}

void ImageTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (this->topLevelItemCount() > 0){
		QModelIndex clickedIndex = indexAt(event->pos());
		if (clickedIndex.isValid() == false){
			this->setCurrentItem(nullptr);
		}
		else{
			auto item = this->currentItem();
			if (item && item->data(0, Qt::UserRole).type() == QVariant::Bool){
				g_filePath = "";
				setCocCursor(Cursor_Type::Normal);
				HelloWorld* scene = (HelloWorld*)g_scene;
				scene->resetState();
			}
		}
	}
	QTreeWidget::mousePressEvent(event);
}

void ImageTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
	QTreeWidgetItem *item = currentItem();
	if (!this->childAt(event->pos())){
		if (item && item->data(0, Qt::UserRole).type() == QVariant::Int){
			startDrag();
		}
	}
	else{
  		//QTreeWidget::mouseMoveEvent(event);
	}
}

void ImageTreeWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QTreeWidget::mouseReleaseEvent(event);
}

void ImageTreeWidget::addToPrePackerResource()
{
	auto *item = currentItem();
	if (item){
		item->setBackgroundColor(0, QColor(125, 125, 125, 255));
		prePackerList << item->data(0, Qt::UserRole).toString();
	}
}

void ImageTreeWidget::actionAddRes()
{
	QStringList openFiles = QFileDialog::getOpenFileNames(this, FontChina::G2U("添加资源文件"), MySqlLiteManager::getInstance()->getOpenDir(), tr("Image Files (*.png *.jpg *.bmp)"));
	if (openFiles.size() > 0){
		auto dir__ = openFiles.at(0).section("/", 0, -2);
		MySqlLiteManager::getInstance()->setOpenDir(openFiles.at(0).section("/", 0, -2));
		for (int i = 0; i < openFiles.size(); i++){
			addRes(openFiles.at(i));
		}
	}
}

void ImageTreeWidget::actionAddDir()
{
	addDir(true);
}

void ImageTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* ResMenu = new QMenu(this);
	QModelIndex clickedIndex = indexAt(event->pos());
	if (clickedIndex.isValid() == true){
		QAction* deleteac = new QAction(FontChina::G2U("删除"), this);
		connect(deleteac, SIGNAL(triggered()), this, SLOT(deleteCurrentResOrDir()));
		ResMenu->addAction(deleteac);
		if (hasPrePacker && this->currentItem()->data(0, Qt::UserRole).type() == QVariant::Int){
			QAction* add_Pre = new QAction(FontChina::G2U("添加到预打包"), this);
			connect(add_Pre, SIGNAL(triggered()), this, SLOT(addToPrePackerResource()));
			ResMenu->addAction(add_Pre);
		}
		if (this->currentItem()->data(0, Qt::UserRole).type() == QVariant::Int){
			QAction* moveDir = new QAction(FontChina::G2U("移动到其他目录"), this);
			connect(moveDir, SIGNAL(triggered()), this, SLOT(moveResToDirDialog()));
			ResMenu->addAction(moveDir);
		}
		if (this->currentItem()->data(0, Qt::UserRole).type() == QVariant::Int){
			QAction* findLayerQuote = new QAction(FontChina::G2U("检索相关的引用层"), this);
			connect(findLayerQuote, SIGNAL(triggered()), this, SLOT(findResourceLayerQuote()));
			ResMenu->addAction(findLayerQuote);
		}
	}
	QAction* addRes = new QAction(FontChina::G2U("添加资源"), this);
	QAction* addDir = new QAction(FontChina::G2U("添加文件夹"), this);
	connect(addRes, SIGNAL(triggered()), this, SLOT(actionAddRes()));
	connect(addDir, SIGNAL(triggered()), this, SLOT(actionAddDir()));
	ResMenu->addAction(addRes);
	ResMenu->addAction(addDir);
	ResMenu->exec(QCursor::pos());
}

void ImageTreeWidget::moveResToDirDialog()
{
	dialog->showDir();
}

void ImageTreeWidget::findResourceLayerQuote()
{
	auto *item = currentItem();
	int index = item->data(0, Qt::UserRole).toInt();
	std::string listPath = ImgTransPath::getListPngName(index).toStdString();
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto vec = scene->getForeManager()->getALLImageLayer();
	for (size_t i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		if (layer->containsInImageMap(listPath)){
			addConsoleInfo("层%s包含该图片", layer->getTagName().c_str());
		}
	}
}

QTreeWidgetItem* ImageTreeWidget::addRes(QString sourceFile, bool warn)
{
	QString toFile = getCurDirPath();
	QString relaFile = getCurDirRelaPath();
	QString listFile, listFilename;
	QString name = sourceFile.section("/", -1);
	int index = ResMap::getImageIndex();
	mutex__.lock();
	ResMap::addImageIndex();
	mutex__.unlock();
	toFile += name;
	relaFile += name;
	listFilename = QString::number(index) + "." + name.section(".", -1);
	listFile = listPath + listFilename;
	ResMap::addImageTypeInfo(index, name.section(".", -1));
	ResMap::addRealImagePathInfo(index, toFile);
	ResMap::addRelaImagePathInfo(index, relaFile);
	ResMap::addRelaImagePathIndexInfo(relaFile, index);
	ResMap::oldPathMap.insert(name, relaFile);
	QFile::copy(sourceFile, toFile);
	QFile::copy(sourceFile, listFile);
	auto texture = TextureCache::getInstance()->addImage(listFile.toStdString());
	ResMap::addSinglePngTextureInfo(index, texture);
	if (curResExist(name)){
		if (warn)QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("文件已加入"));
		return nullptr;
	}
	else{
		auto newItem = new QTreeWidgetItem();
		newItem->setIcon(0, QIcon(listFile));
		newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
		newItem->setData(0, Qt::UserRole, index);
		newItem->setText(0, name);
		addResItemInCurFolder(newItem);
		//这里要修改
		_itemMap.insert(std::pair<std::string, ItemData*>(listFile.toStdString(), new ItemData(newItem)));
		SortItem(newItem);
		return newItem;
	}
}

QTreeWidgetItem* ImageTreeWidget::addDir(bool autoname, QString dir)
{
	auto newItem = new QTreeWidgetItem();
	newItem->setIcon(0, QIcon(":/icon/Resources/folder.ico"));
	newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
	auto curDir = getCurFolderItem();
	if (autoname){
		newItem->setText(0, getNewFolderName(curDir));
	}
	else{
		newItem->setText(0, dir);
	}
	newItem->setData(0, Qt::UserRole, true);
	addDirItemInCurFolder(newItem);
	createRealDir(newItem);
	SortItem(newItem);
	return newItem;
}