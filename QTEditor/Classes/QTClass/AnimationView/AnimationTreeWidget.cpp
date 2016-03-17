#include "AnimationTreeWidget.h"
#include "Global.h"
#include "GlobalConstant.h"
#include "QMenu"
#include "FontChina.h"
#include "QFileDialog"
#include "MySqlLiteManager.h"
#include "ImgTransPath.h"
#include "LayerManagerWidget.h"
#include "QMessageBox"
#include "ResMap.h"

AnimationTreeWidget::AnimationTreeWidget(QWidget *parent)
	:BaseResTreeWidget(parent)
{
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editFileOrDirName(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemClick(QTreeWidgetItem*, int)));
	init();
	dialog = new ResModifyDirDialog();
	dialog->isResourceDialog(type__);
	newDirName = "newSpriteFrame";
}

AnimationTreeWidget::~AnimationTreeWidget()
{
}

void AnimationTreeWidget::init()
{
	type__ = Image_Type::AnimationBlock;
	hasPrePacker = false;
	relativeDir = g_c_relaFrameDir;
	relativeListDir = g_c_rela_FrameListDir;
	imagePath = FileUtils::getInstance()->getSearchPaths().at(0).c_str();
	listPath = imagePath;
	imagePath += relativeDir;
	listPath += relativeListDir;
}

void AnimationTreeWidget::changedResRef(std::string name, int count)
{
	BaseResTreeWidget::changedResRef(name, count);
}

QString AnimationTreeWidget::getCurrentAnimationName()
{
	auto item = currentItem();
	if (item->data(0, Qt::UserRole).type() == QVariant::Int)
	{
		item = item->parent();
	}
	return getSrcName(item->text(0));
}

QString AnimationTreeWidget::getAnimationName(QTreeWidgetItem* item)
{
	if (item->data(0, Qt::UserRole).type() == QVariant::Int){
		item = item->parent();
	}
	return getSrcName(item->text(0));
}

void AnimationTreeWidget::itemClick(QTreeWidgetItem* item, int columns)
{
	auto vec = getDirChildRes(item);
	if (item->data(0, Qt::UserRole).type() == QVariant::Bool && vec.size() > 0){
		auto itemChild = vec.at(0);
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
			if (!layer->isSportState()){
				QMessageBox::warning(NULL, "warning", FontChina::G2U("静态层不能创建动画"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return;
			}
		}
		g_filePath = ImgTransPath::getListImagePathByIndex(itemChild->data(0, Qt::UserRole).toInt()).toStdString();
		g_animationName = getSrcName(item->text(0)).toStdString();
		g_TemplateFile = "";
		g_ImageType = type__;
		layerManager->add_ImageChild();
	}
}

void AnimationTreeWidget::itemChanged(QTreeWidgetItem* item, int columns)
{
	if (check){
		if (item->data(0, Qt::UserRole).type() == QVariant::Bool){
			if (curAnimationExist(getSrcName(item->text(0)), item)){
				item->setText(0, oldName);
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("当前动画已经存在"));
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
			if (curSpriteFrameExist(getSrcName(item->text(0)))){
				item->setText(0, oldName);
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("当前帧动画已经存在"));
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

void AnimationTreeWidget::editFileOrDirName(QTreeWidgetItem* item, int columns)
{
	oldName = getSrcName(item->text(0));
	check = true;
}

void AnimationTreeWidget::deleteCurrentResOrDir()
{
	auto* item = this->currentItem();
	if (item->data(0, Qt::UserRole).type() == QVariant::Bool){
		if (!curAnimationNoRefItem(item)){
			addConsoleInfo("包含引用动画,删除失败!");
			return;
		}
		QString path = getCurDirPath();
		DeleteDirectory(path, true);
		removeFromItemMap(item);
	}
	else{
		auto itemParent = item->parent();
		if (getAnimationItemRefCount(itemParent) > 0){
			addConsoleInfo("包含引用动画，删除失败!");
			return;
		}
		deleteRes(item->data(0, Qt::UserRole).toInt());
	}
	delete item;
	g_filePath = "";
	g_ImageType = Image_Type::NormalBlock;
	addConsoleInfo("删除成功");
}

void AnimationTreeWidget::actionAddSingleFrame()
{
	QStringList openFiles = QFileDialog::getOpenFileNames(this, FontChina::G2U("添加帧动画"), MySqlLiteManager::getInstance()->getOpenDir(), tr("Image Files (*.png *.jpg *.bmp)"));
	if (openFiles.size() > 0){
		auto dir__ = openFiles.at(0).section("/", 0, -2);
		MySqlLiteManager::getInstance()->setOpenDir(openFiles.at(0).section("/", 0, -2));
		for (int i = 0; i < openFiles.size(); i++){
			addSingleFrame(openFiles.at(i));
		}
	}
}

void AnimationTreeWidget::actionAddFrameDir()
{
	addFrameDir(true);
}

//void AnimationTreeWidget::moveResToDirDialog()
//{
//	dialog->showDir();
//}

void AnimationTreeWidget::findFrameLayerQuote()
{
	auto *item = currentItem();
	if (item->data(0, Qt::UserRole).type() == QVariant::Int){
		item = item->parent();
	}
	std::string animationname = getSrcName(item->text(0)).toStdString();
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto vec = scene->getForeManager()->getALLImageLayer();
	for (size_t i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		if (layer->containsInAnimationMap(animationname)){
			addConsoleInfo("层%s包含该动画", layer->getTagName().c_str());
		}
	}
}

QTreeWidgetItem* AnimationTreeWidget::addSingleFrame(QString sourceFile, bool warn)
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
	QFile::copy(sourceFile, toFile);
	QFile::copy(sourceFile, listFile);
	auto texture = TextureCache::getInstance()->addImage(listFile.toStdString());
	ResMap::addSinglePngTextureInfo(index, texture);
	if (curResExist(name)){
		if (warn)QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("帧动画已加入"));
		return nullptr;
	}
	else{
		auto newItem = new QTreeWidgetItem();
		newItem->setIcon(0, QIcon(listFile));
		newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
		newItem->setData(0, Qt::UserRole, index);
		newItem->setText(0, name);
		addResItemInCurFolder(newItem);
		SortItem(newItem);
		return newItem;
	}
}

QTreeWidgetItem* AnimationTreeWidget::addFrameDir(bool autoname, QString dir)
{
	auto newItem = new QTreeWidgetItem();
	newItem->setIcon(0, QIcon(":/icon/Resources/frame.png"));
	newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
	if (autoname){
		dir = getNewAnimationName();
		newItem->setText(0, dir);
	}
	else{
		newItem->setText(0, dir);
	}
	newItem->setData(0, Qt::UserRole, true);
	addDirItemInCurFolder(newItem);
	//这里要修改
	_itemMap.insert(std::pair<std::string, ItemData*>(dir.toStdString(), new ItemData(newItem)));
	createRealDir(newItem);
	SortItem(newItem);
	return newItem;
}

void AnimationTreeWidget::mousePressEvent(QMouseEvent *event)
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

void AnimationTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
	QTreeWidgetItem *item = currentItem();
	if (!this->childAt(event->pos())){
		if (item && item->data(0, Qt::UserRole).type() == QVariant::Bool){
			startDrag();
		}
	}
	else{
		//QTreeWidget::mouseMoveEvent(event);
	}
}

void AnimationTreeWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QTreeWidget::mouseReleaseEvent(event);
}

void AnimationTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* ResMenu = new QMenu(this);
	QModelIndex clickedIndex = indexAt(event->pos());
	if (clickedIndex.isValid() == true){
		QAction* deleteac = new QAction(FontChina::G2U("删除"), this);
		connect(deleteac, SIGNAL(triggered()), this, SLOT(deleteCurrentResOrDir()));
		ResMenu->addAction(deleteac);
		/*if (this->currentItem()->data(0, Qt::UserRole).type() == QVariant::Int){
			QAction* moveDir = new QAction(FontChina::G2U("移动到其他动画中"), this);
			connect(moveDir, SIGNAL(triggered()), this, SLOT(moveResToDirDialog()));
			ResMenu->addAction(moveDir);
			}*/
		if (this->currentItem()->data(0, Qt::UserRole).type() == QVariant::Int){
			QAction* findLayerQuote = new QAction(FontChina::G2U("检索相关的引用层"), this);
			connect(findLayerQuote, SIGNAL(triggered()), this, SLOT(findFrameLayerQuote()));
			ResMenu->addAction(findLayerQuote);
		}
		QAction* addRes = new QAction(FontChina::G2U("添加单帧"), this);
		connect(addRes, SIGNAL(triggered()), this, SLOT(actionAddSingleFrame()));
		ResMenu->addAction(addRes);
	}
	QAction* addDir = new QAction(FontChina::G2U("添加动画文件夹"), this);
	connect(addDir, SIGNAL(triggered()), this, SLOT(actionAddFrameDir()));
	ResMenu->addAction(addDir);
	ResMenu->exec(QCursor::pos());
}

bool AnimationTreeWidget::curAnimationExist(QString animationname)
{
	return curAnimationExist(animationname, nullptr);
}

bool AnimationTreeWidget::curAnimationExist(QString animationname, QTreeWidgetItem* compareItem)
{
	int size = this->topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto itemChild = this->topLevelItem(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool) {
			if (getSrcName(itemChild->text(0)) == animationname && itemChild != compareItem){
				return true;
			}
			else if (curAnimationExistInSubItem(itemChild, animationname, compareItem)){
				return true;
			}
		}
	}
	return false;
}

bool AnimationTreeWidget::curSpriteFrameExist(QString framename)
{
	return curResExist(framename);
}

bool AnimationTreeWidget::curAnimationExistInSubItem(QTreeWidgetItem* item, QString animationname, QTreeWidgetItem* compareItem)
{
	int size = item->childCount();
	for (int i = 0; i < size; i++){
		auto itemChild = item->child(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool) {
			if (getSrcName(itemChild->text(0)) == animationname && itemChild != compareItem){
				return true;
			}
			else if (curAnimationExistInSubItem(itemChild, animationname, compareItem)){
				return true;
			}
		}
	}
	return false;
}

QString AnimationTreeWidget::getNewAnimationName()
{
	int index = 0;
	QString animationname = newDirName + QString::number(index);
	while (curAnimationExist(animationname)){
		index++;
		animationname = newDirName + QString::number(index);
	}
	return animationname;
}

bool AnimationTreeWidget::curAnimationNoRefItem(QTreeWidgetItem* item)
{
	bool canDelete = true;
	if (getAnimationItemRefCount(item) > 0)return false;
	for (size_t i = 0; i < item->childCount(); i++){
		auto itemChild = item->child(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			if (!curAnimationNoRefItem(itemChild)){
				canDelete = false;
			}
		}
	}
	return canDelete;
}

int AnimationTreeWidget::getAnimationItemRefCount(QTreeWidgetItem* item)
{
	QString animationname = getSrcName(item->text(0));
	return _itemMap[animationname.toStdString()]->getRefCount();
}

QVector<QString> AnimationTreeWidget::getSinpleFrameNameListQString(QString animationName)
{
	auto item = getAnimationItem(animationName);
	return getSinpleFrameNameListQString(item);
}

std::vector<int> AnimationTreeWidget::getSinpleFrameNameList(QString animationName)
{
	auto item = getAnimationItem(animationName);
	return getSinpleFrameNameList(item);
}

QVector<QString> AnimationTreeWidget::getSinpleFrameNameListQString(QTreeWidgetItem* item)
{
	auto vec = getSinpleFrameNameList(item);
	QVector<QString> StringVec;
	for (size_t i = 0; i < vec.size(); i++){
		auto index = vec.at(i);
		QString imageName = ImgTransPath::getRealImageName(index);
		StringVec.push_back(imageName);
	}
	return StringVec;
}

std::vector<int> AnimationTreeWidget::getSinpleFrameNameList(QTreeWidgetItem* item)
{
	std::vector<int> vec;
	int size = item->childCount();
	for (size_t i = 0; i < size; i++){
		auto itemChild = item->child(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Int){
			vec.push_back(itemChild->data(0, Qt::UserRole).toInt());
		}
	}
	return vec;
}

std::vector<int> AnimationTreeWidget::getCurrentSinpleFrameList()
{
	auto item = currentItem();
	if (item->data(0, Qt::UserRole).type() == QVariant::Int){
		item = item->parent();
	}
	return getSinpleFrameNameList(item);
}

QVector<QString> AnimationTreeWidget::getCurrentSinpleFrameNameListQString()
{
	auto item = currentItem();
	if (item->data(0, Qt::UserRole).type() == QVariant::Int){
		item = item->parent();
	}
	return getSinpleFrameNameListQString(item);
}

QTreeWidgetItem* AnimationTreeWidget::getAnimationItem(QString animationname)
{
	int size = this->topLevelItemCount();
	for (size_t i = 0; i < size; i++){
		auto itemChild = this->topLevelItem(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			if (getSrcName(itemChild->text(0)) == animationname){
				return itemChild;
			}
			else{
				auto itemChildChild = getAnimationItemFromSubItem(animationname, itemChild);
				if (itemChildChild)return itemChildChild;
			}
		}
	}
	return nullptr;
}

QTreeWidgetItem* AnimationTreeWidget::getAnimationItemFromSubItem(QString animationname, QTreeWidgetItem* item)
{
	int size = item->childCount();
	for (size_t i = 0; i < size; i++){
		auto itemChild = item->child(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			if (getSrcName(itemChild->text(0)) == animationname){
				return itemChild;
			}
			else{
				auto itemChildChild = getAnimationItemFromSubItem(animationname, itemChild);
				if (itemChildChild)return itemChildChild;
			}
		}
	}
	return nullptr;
}