#include "BaseResTreeWidget.h"
#include "QDir"
#include "ImgTransPath.h"
#include "GlobalConstant.h"
#include "ResMap.h"
#include "FontChina.h"
#include "QMessageBox"
#include "ResourceView.h"
#include "ExteralView.h"
#include "AnimationView.h"

USING_NS_CC;

ItemData::ItemData(QTreeWidgetItem* item) {
	_item = item;
	_refCount = 0;
	_item->setTextColor(0, QColor(126, 126, 126, 255));
}

void ItemData::appendRefCount(int n) {
	_refCount += n;
	if (_refCount < 0) _refCount = 0;

	QString name = BaseResTreeWidget::getSrcName(_item->text(0));
	if (_refCount == 0) {
		_item->setTextColor(0, QColor(126, 126, 126, 255));
	}
	else {
		stringstream ss;
		ss << _refCount;

		name += " (*";
		name += ss.str().c_str();
		name += ")";
		_item->setTextColor(0, QColor(0, 102, 255, 255));
	}
	_item->setText(0, name);
}

BaseResTreeWidget::BaseResTreeWidget(QWidget *parent)
	:QTreeWidget(parent)
{
	check = false;
	this->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
}

void BaseResTreeWidget::initResourceDir()
{
	QDir* dir = new QDir();
	if (dir->exists(imagePath)){
		DeleteDirectory(imagePath);
	}
	if (!dir->exists(imagePath)){
		dir->mkdir(imagePath);
	}
	if (dir->exists(listPath)){
		DeleteDirectory(listPath);
	}
	if (!dir->exists(listPath)){
		dir->mkdir(listPath);
	}
	delete dir;
}

BaseResTreeWidget::~BaseResTreeWidget()
{
	for (auto itr = _itemMap.begin(); itr != _itemMap.end(); itr++) {
		delete itr->second;
	}
	_itemMap.clear();
}

QString BaseResTreeWidget::getSrcName(QString name) {
	if (name.at(name.size() - 1) == QString(")")) {
		int pos = name.lastIndexOf(" (*");
		if (pos == -1) {
			return name;
		}
		else {
			return name.left(pos);
		}
	}
	else {
		return name;
	}
}

void BaseResTreeWidget::SortItem(QTreeWidgetItem* item)
{
	if (item->parent()){
		auto itemParent = item->parent();
		int index = -1;
		int before = -1;
		for (int i = 0; i < itemParent->childCount(); i++){
			auto itemChild = itemParent->child(i);
			if (itemChild == item){
				index = i;
			}
			else if (item->data(0, Qt::UserRole).type() == QVariant::Int && itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
				before = i;
			}
			else if (item->data(0, Qt::UserRole).type() == itemChild->data(0, Qt::UserRole).type() && itemChild->text(0) < item->text(0)){
				before = i;
			}
		}
		itemParent->takeChild(index);
		if (before > index){
			itemParent->insertChild(before, item);
		}
		else{
			itemParent->insertChild(before + 1, item);
		}
	}
	else{
		int index = -1;
		int before = -1;
		for (int i = 0; i < this->topLevelItemCount(); i++){
			auto itemChild = this->topLevelItem(i);
			if (itemChild == item){
				index = i;
			}
			else if (item->data(0, Qt::UserRole).type() == QVariant::Int && itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
				before = i;
			}
			else if (item->data(0, Qt::UserRole).type() == itemChild->data(0, Qt::UserRole).type() && itemChild->text(0) < item->text(0)){
				before = i;
			}
		}
		this->takeTopLevelItem(index);
		if (before > index){
			this->insertTopLevelItem(before, item);
		}
		else{
			this->insertTopLevelItem(before + 1, item);
		}
	}
}

int BaseResTreeWidget::getItemIndex(QTreeWidgetItem* item)
{
	auto itemParent = item->parent();
	if (itemParent){
		int size = itemParent->childCount();
		for (int i = 0; i < size; i++){
			auto itemChild = itemParent->child(i);
			if (itemChild == item)return i;
		}
	}
	else{
		int size = this->topLevelItemCount();
		for (int i = 0; i < size; i++){
			auto itemChild = this->topLevelItem(i);
			if (itemChild == item)return i;
		}
	}
	return -1;
}

//返回值 true当前文件夹可以删除
bool BaseResTreeWidget::deleteDirNoRefItem(QTreeWidgetItem* item)
{
	bool canDelete = true;
	auto count = item->childCount();
	QVector<QTreeWidgetItem*> itemVec;
	for (size_t i = 0; i < item->childCount(); i++){
		auto itemChild = item->child(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			if (deleteDirNoRefItem(itemChild)){
				QString path = getDirPath(itemChild);
				DeleteDirectory(path, true);
				itemVec.push_back(itemChild);
			}
			else{
				canDelete = false;
			}
		}
		else{
			if (getItemRefCount(itemChild) <= 0){
				deleteRes(itemChild->data(0, Qt::UserRole).toInt());
				removeFromItemMap(itemChild);
				itemVec.push_back(itemChild);
			}
			else{
				canDelete = false;
			}
		}
	}
	for (size_t i = 0; i < itemVec.size(); i++){
		delete itemVec.at(i);
	}
	return canDelete;
}

int BaseResTreeWidget::getItemRefCount(QTreeWidgetItem* item)
{
	auto listFile = ImgTransPath::getListImagePathByIndex(item->data(0, Qt::UserRole).toInt());
	return _itemMap[listFile.toStdString()]->getRefCount();
}

void BaseResTreeWidget::deleteRes(int index)
{
	QString image = ResMap::getImageRealPath(index);
	DeleteFileFile(image);
}

void BaseResTreeWidget::startDrag()
{
	auto *item = currentItem();
	if (item) {
		setEditorCursor(Cursor_Type::addSprite);
		itemClicked(item, 0);
	}
}

QStringList BaseResTreeWidget::getPackerResourceList()
{
	return prePackerList;
}

void BaseResTreeWidget::clearPackerResourceList()
{
	prePackerList.clear();
}

void BaseResTreeWidget::removeAllItem(bool removeRealDir)
{
	int size = this->topLevelItemCount();
	for (int i = 0; i < size; i++){
		this->takeTopLevelItem(0);
	}
	if (removeRealDir){
		DeleteDirectory(imagePath, false);
	}
}

QString BaseResTreeWidget::getNewFolderName(QTreeWidgetItem* itemParent)
{
	if (!itemParent){
		int size = this->topLevelItemCount();
		int count = 0;
		for (int i = 0; i < size; i++){
			auto item = this->topLevelItem(i);
			if (item->data(0, Qt::UserRole).type() == QVariant::Bool && getSrcName(item->text(0)).contains(newDirName)) {
				auto str = getSrcName(item->text(0)).mid(9);
				if (str.toInt() >= count){
					count = str.toInt() + 1;
				}
			}
		}
		return newDirName + QString::number(count);
	}
	else{
		int size = itemParent->childCount();
		int count = 0;
		for (int i = 0; i < size; i++){
			auto item = itemParent->child(i);
			if (item->data(0, Qt::UserRole).type() == QVariant::Bool && getSrcName(item->text(0)).contains(newDirName)) {
				auto str = getSrcName(item->text(0)).mid(9);
				if (str.toInt() >= count){
					count = str.toInt() + 1;
				}
			}
		}
		return newDirName + QString::number(count);
	}
}

bool BaseResTreeWidget::curResExist(QString res)
{
	auto item = getCurFolderItem();
	if (item){
		int size = item->childCount();
		int repeatCount = 0;
		for (int i = 0; i < size; i++){
			auto itemChild = item->child(i);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Int && getSrcName(itemChild->text(0)) == res){
				if (check){
					repeatCount++;
					if (repeatCount > 1)
						return true;
				}
				else{
					return true;
				}
			}
		}
	}
	else{
		int size = this->topLevelItemCount();
		int repeatCount = 0;
		for (int i = 0; i < size; i++){
			auto itemChild = this->topLevelItem(i);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Int && getSrcName(itemChild->text(0)) == res){
				if (check){
					repeatCount++;
					if (repeatCount > 1)
						return true;
				}
				else{
					return true;
				}
			}
		}
	}
	return false;
}

void BaseResTreeWidget::addResItemInCurFolder(QTreeWidgetItem* item)
{
	auto curItem = getCurFolderItem();
	if (curItem){
		curItem->insertChild(curItem->childCount(), item);
	}
	else{
		this->insertTopLevelItem(this->topLevelItemCount(), item);
	}
}

void BaseResTreeWidget::addDirItemInCurFolder(QTreeWidgetItem* item)
{
	auto curItem = getCurFolderItem();
	if (curItem){
		curItem->insertChild(curItem->childCount(), item);
		sortList(curItem);
	}
	else{
		this->insertTopLevelItem(this->topLevelItemCount(), item);
		sortList(nullptr);
	}
}

void BaseResTreeWidget::createRealDir(QTreeWidgetItem* item)
{
	QString createDir = getDirPath(item);
	QDir dir;
	if (!dir.exists(createDir)){
		dir.mkdir(createDir);
	}
}

QString BaseResTreeWidget::getDirPath(QTreeWidgetItem* itemDir)
{
	if (!itemDir){
		return imagePath;
	}
	else{
		auto itemParent = itemDir->parent();
		if (itemParent){
			return getDirPath(itemParent) + getSrcName(itemDir->text(0)) + "/";
		}
		else{
			return imagePath + getSrcName(itemDir->text(0)) + "/";
		}
	}
}

QString BaseResTreeWidget::getDirRelaPath(QTreeWidgetItem* itemDir)
{
	if (!itemDir){
		switch (type__){
		case Image_Type::NormalBlock:
			return g_c_relaResourceDirRoot;
			break;
		case Image_Type::Entity:
			return g_c_relaExteralDirRoot;
			break;
		case Image_Type::AnimationBlock:
			return g_c_relaFrameDirRoot;
			break;
		}
	}
	else{
		auto itemParent = itemDir->parent();
		if (itemParent){
			return getDirRelaPath(itemParent) + getSrcName(itemDir->text(0)) + "/";
		}
		else{
			switch (type__){
			case Image_Type::NormalBlock:
				return g_c_relaResourceDirRoot + getSrcName(itemDir->text(0)) + "/";
				break;
			case Image_Type::Entity:
				return g_c_relaExteralDirRoot + getSrcName(itemDir->text(0)) + "/";
				break;
			case Image_Type::AnimationBlock:
				return g_c_relaFrameDirRoot + getSrcName(itemDir->text(0)) + "/";
				break;
			}
		}
	}
}

QString BaseResTreeWidget::getResItemRelaPath(QTreeWidgetItem* itemRes)
{
	return getDirRelaPath(itemRes->parent()) + getSrcName(itemRes->text(0));
}

QString BaseResTreeWidget::getResItemRealPath(QTreeWidgetItem* itemRes)
{
	return getDirPath(itemRes->parent()) + getSrcName(itemRes->text(0));
}

QString BaseResTreeWidget::getCurDirPath()
{
	auto item = getCurFolderItem();
	QString path = getDirPath(item);
	return path;
}

QString BaseResTreeWidget::getCurDirRelaPath()
{
	auto item = getCurFolderItem();
	QString path = getDirRelaPath(item);
	return path;
}

//获取当前的文件夹如果当前是一个资源的话会获取到他的父节点
QTreeWidgetItem* BaseResTreeWidget::getCurFolderItem()
{
	auto curItem = this->currentItem();
	if (curItem){
		if (curItem->data(0, Qt::UserRole).type() == QVariant::Bool){
			return curItem;
		}
		else{
			return curItem->parent();
		}
	}
	else{
		return nullptr;
	}
}

void BaseResTreeWidget::setCurFolder(QString folderOther)
{
	auto vec = getChildFolder();
	if (folderOther == ""){
		auto item = getCurFolderItem();
		this->setCurrentItem(item->parent());
		return;
	}
	else{
		for (int i = 0; i < vec.size(); i++){
			auto item = vec.at(i);
			if (getSrcName(item->text(0)) == folderOther) {
				this->setCurrentItem(item);
				return;
			}
		}
	}
}

void BaseResTreeWidget::deleteResOrDir(QString name, bool isDir)
{
	QVector<QTreeWidgetItem*> vec;
	if (isDir){
		vec = getChildFolder();
	}
	else{
		vec = getChildRes();
	}
	for (int i = 0; i < vec.size(); i++){
		auto item = vec.at(i);
		if (getSrcName(item->text(0)) == name) {
			removeFromItemMap(item);
			delete item;
			return;
		}
	}
}

QVector<QTreeWidgetItem*> BaseResTreeWidget::getChildFolder()
{
	return getChild(true);
}

QVector<QTreeWidgetItem*> BaseResTreeWidget::getChildRes()
{
	return getChild(false);
}

bool BaseResTreeWidget::isTop()
{
	if (getCurFolderItem()){
		return false;
	}
	return true;
}

QVector<QTreeWidgetItem*> BaseResTreeWidget::getChild(bool isDir)
{
	QVector<QTreeWidgetItem*> vec;
	auto parent = getCurFolderItem();
	if (parent){
		int size = parent->childCount();
		for (int i = 0; i < size; i++){
			auto itemChild = parent->child(i);
			if (isDir && itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
				vec.push_back(itemChild);
			}
			if (!isDir && itemChild->data(0, Qt::UserRole).type() == QVariant::Int){
				vec.push_back(itemChild);
			}
		}
	}
	else{
		int size = topLevelItemCount();
		for (int i = 0; i < size; i++){
			auto itemChild = this->topLevelItem(i);
			if (isDir && itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
				vec.push_back(itemChild);
			}
			if (!isDir && itemChild->data(0, Qt::UserRole).type() == QVariant::Int){
				vec.push_back(itemChild);
			}
		}
	}
	return vec;
}

//当修改文件夹名称的时候调用
bool BaseResTreeWidget::curDirExist(QString dir)
{
	auto item = currentItem()->parent();
	if (item){
		int size = item->childCount();
		int count = 0;
		for (int i = 0; i < size; i++){
			auto itemChild = item->child(i);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool && getSrcName(itemChild->text(0)) == dir) {
				count++;
			}
		}
		if (count > 1)return true;
	}
	else{
		int size = this->topLevelItemCount();
		int count = 0;
		for (int i = 0; i < size; i++){
			auto itemChild = this->topLevelItem(i);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool && getSrcName(itemChild->text(0)) == dir) {
				count++;
			}
		}
		if (count > 1)return true;
	}
	return false;
}

void BaseResTreeWidget::resetDirName(QString key)
{

}

void BaseResTreeWidget::resetResName(QString key)
{

}

void BaseResTreeWidget::sortList(QTreeWidgetItem* item)
{
	QVector<QTreeWidgetItem*> dirVec;
	QVector<QTreeWidgetItem*> resVec;
	if (item){
		auto size = item->childCount();
		for (int i = 0; i < size; i++){
			auto itemChild = item->child(0);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
				dirVec.push_back(itemChild);
			}
			else{
				resVec.push_back(itemChild);
			}
			item->removeChild(itemChild);
		}
		for (int i = 0; i < dirVec.size(); i++){
			item->addChild(dirVec.at(i));
		}
		for (int i = 0; i < resVec.size(); i++){
			item->addChild(resVec.at(i));
		}
	}
	else{
		int size = this->topLevelItemCount();
		for (int i = 0; i < size; i++){
			auto itemChild = this->topLevelItem(0);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
				dirVec.push_back(itemChild);
			}
			else{
				resVec.push_back(itemChild);
			}
			this->takeTopLevelItem(0);
		}
		for (int i = 0; i < dirVec.size(); i++){
			this->insertTopLevelItem(this->topLevelItemCount(), dirVec.at(i));
		}
		for (int i = 0; i < resVec.size(); i++){
			this->insertTopLevelItem(this->topLevelItemCount(), resVec.at(i));
		}
	}
}

void BaseResTreeWidget::changedResRef(std::string path, int count) {
	auto itr = _itemMap.find(path);
	if (itr != _itemMap.end()) {
		ItemData* data = itr->second;
		data->appendRefCount(count);
	}
	if (!this->isVisible()){
		switch (type__){
		case Image_Type::NormalBlock:
		{
			ResourceView* view = (ResourceView*)g_resourceView;
			view->getListWidget()->update();
		}
			break;
		case Image_Type::Entity:
		{
			ExteralView* view = (ExteralView*)g_exteralView;
			view->getListWidget()->update();
		}
			break;
		case Image_Type::AnimationBlock:
			break;
		}
	}
}

void BaseResTreeWidget::removeFromItemMap(QTreeWidgetItem* item) {
	for (auto itr = _itemMap.begin(); itr != _itemMap.end(); itr++) {
		if (itr->second->getItem() == item) {
			delete itr->second;
			_itemMap.erase(itr);

			return;
		}
	}
}

void BaseResTreeWidget::resetChildItemMapData(QTreeWidgetItem* item)
{
	int size = item->childCount();
	for (int i = 0; i < size; i++){
		auto itemChild = item->child(i);
		if (itemChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			resetChildItemMapData(itemChild);
		}
		else{
			resetItemMapData(itemChild);
		}
	}
}

void BaseResTreeWidget::resetItemMapData(QTreeWidgetItem* item)
{
	int index = item->data(0, Qt::UserRole).toInt();
	QString path = getResItemRealPath(item);
	QString relaPath = getResItemRelaPath(item);
	ResMap::addRealImagePathInfo(index, path);
	ResMap::addRelaImagePathInfo(index, relaPath);
}

QVector<QTreeWidgetItem*> BaseResTreeWidget::getCurSelectRes()
{
	auto vec = selectedItems();
	QVector<QTreeWidgetItem*> vecMove;
	for (int i = 0; i < vec.size(); i++){
		auto item = vec.at(i);
		if (item->data(0, Qt::UserRole).type() == QVariant::Int){
			vecMove.push_back(item);
		}
	}
	return vecMove;
}

void BaseResTreeWidget::moveResToDir(QVector<QTreeWidgetItem*> vec, QTreeWidgetItem* item)
{
	for (int i = 0; i < vec.size(); i++){
		auto itemChild = vec.at(i);
		if (!inDirHasSameRes(getSrcName(itemChild->text(0)), item)){
			int index = itemChild->data(0, Qt::UserRole).toInt();
			QString realPath = ResMap::getImageRealPath(index);
			QString newRealPath = getDirPath(item);
			QString newRelaPath = getDirRelaPath(item);
			newRelaPath += getSrcName(itemChild->text(0));
			newRealPath += getSrcName(itemChild->text(0));
			QFile::copy(realPath, newRealPath);
			DeleteFileFile(realPath);
			ResMap::addRealImagePathInfo(index, newRealPath);
			ResMap::addRelaImagePathInfo(index, newRelaPath);
			ResMap::addRelaImagePathIndexInfo(newRealPath, index);
			if (itemChild->parent()){
				int index_ = getItemIndex(itemChild);
				itemChild->parent()->takeChild(index_);
			}
			else{
				int index_ = getItemIndex(itemChild);
				this->takeTopLevelItem(index_);
			}
			if (item){
				item->insertChild(item->childCount(), itemChild);
			}
			else{
				this->insertTopLevelItem(this->topLevelItemCount(), itemChild);
			}
		}
	}
}

QVector<QTreeWidgetItem*> BaseResTreeWidget::getDirChildRes(QTreeWidgetItem* item)
{
	QVector<QTreeWidgetItem*> vec;
	if (item){
		int size = item->childCount();
		for (int i = 0; i < size; i++){
			auto itemChild = item->child(i);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Int){
				vec.push_back(itemChild);
			}
		}
	}
	else{
		int size = this->topLevelItemCount();
		for (int i = 0; i < size; i++){
			auto itemChild = this->topLevelItem(i);
			if (itemChild->data(0, Qt::UserRole).type() == QVariant::Int){
				vec.push_back(itemChild);
			}
		}
	}
	return vec;
}

bool BaseResTreeWidget::inDirHasSameRes(QString res, QTreeWidgetItem* item)
{
	auto vec = getDirChildRes(item);
	for (int i = 0; i < vec.size(); i++){
		auto itemChild = vec.at(i);
		if (getSrcName(itemChild->text(0)) == res){
			return true;
		}
	}
	return false;
}

void BaseResTreeWidget::setCurRes(int listpngindex)
{
	std::string str = ImgTransPath::getListImagePathByIndex(listpngindex).toStdString();
	auto data = _itemMap[str];
	setCurrentItem(data->getItem());
}