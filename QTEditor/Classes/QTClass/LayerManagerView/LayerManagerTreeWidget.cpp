#include "LayerManagerTreeWidget.h"
#include "SpriteTreeItem.h"
#include "MyLogger.h"
#include "HelloWorldScene.h"
#include "ImageSpriteLayer.h"
#include "LayerManagerWidget.h"
#include "ControlView.h"
#include "FontChina.h"
#include "QMessageBox"

LayerManagerTreeWidget::LayerManagerTreeWidget(QWidget *parent)
	:QTreeWidget(parent)
{
	check = false;
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(RenameNodeName(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemClick(QTreeWidgetItem*, int)));
	setItemDelegate(new LayerManagerItemDelegate());
}

LayerManagerTreeWidget::~LayerManagerTreeWidget()
{
}

//http://www.4byte.cn/question/1923818/position-of-icon-in-qtreewidgetitem.html
bool LayerManagerTreeWidget::inCurrentIconRect(QPoint pos)
{
	touchIcon = false;
	QModelIndex clickedIndex = indexAt(pos);
	QTreeWidgetItem* item = currentItem();
	if (item->parent())return false;
	touchcol = clickedIndex.column();
	// make sure the event was on a valid item
	if (clickedIndex.isValid() == false){
		return false;
	}
	int treeX = header()->sectionViewportPosition(0);
	int rootX = visualRect(rootIndex()).x();
	QRect vrect = visualRect(clickedIndex);
	int itemX = treeX + vrect.x() - rootX;
	QRect iconRect = QRect(itemX,
		vrect.y(),
		vrect.height(),
		vrect.height());
	QRect textRect = QRect(itemX + iconRect.width(),
		vrect.y(),
		vrect.width() - iconRect.width(),
		vrect.height());
	if (!item->icon(touchcol).isNull()){
		if (iconRect.contains(pos))
		{
			touchIcon = true;
			return true;
		}
	}
	return false;
}

void LayerManagerTreeWidget::mousePressEvent(QMouseEvent *event)
{
	QTreeWidget::mousePressEvent(event);
}

void LayerManagerTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
	QTreeWidget::mouseMoveEvent(event);
}

void LayerManagerTreeWidget::mouseReleaseEvent(QMouseEvent* event)
{
	inCurrentIconRect(event->pos());
	QTreeWidget::mouseReleaseEvent(event);
}

void LayerManagerTreeWidget::itemClick(QTreeWidgetItem* item, int columns)
{
	MyLogger::getInstance()->addInfo("the LayerManagerWidget::imageitemClick be called, the args itemname is " + item->text(1) + ", columns is " + QString::number(columns));
	QList<QTreeWidgetItem*> list = selectedItems();
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	auto parent = item->parent();
	ImageSpriteLayer* layer = nullptr;
	auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	layerManager->touchLayerIconEvent();
	if (parent){
		layer = resetSceneCurrentLayerAndGetReturn(item->parent());
	}
	else{
		layer = resetSceneCurrentLayerAndGetReturn(item);
	}
	if (list.size() > 1){
		scene->setType(Type_None);
		clearControlView();
	}
	else{
		if (layer){
			if (parent){
				if (!layer->isLock()){
					setShowEdgeSprite(parent, item);
				}
				//讲值赋给controlView
				setValueToControlViewSprite(item);
			}
			else{
				if (!layer->isLock()){
					setShowEdgeLayer(item);
				}
				setValueToControlViewLayer(item);
			}
		}
	
		if (layer && !layer->isLock()){
			if (parent){
				QStringList listData;
				listData.push_back(item->parent()->text(1));
				listData.push_back(item->text(1));
				scene->setType(Type_Move_Foce_Image_Child, listData);
			}
			else{
				//暂时屏蔽移动层的功能
				//scene->setType(Type_Move_Foce_Image_Layer, (void*)item->text(1).toUtf8().data());
			}
		}
	}
}

void LayerManagerTreeWidget::itemChanged(QTreeWidgetItem* item1, int columns)
{
	if (!touchIcon && check){

		if (columns == 1){
			SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(item1->parent());
			if (parent){
				resetSpriteTagName(item1);
			}
			else{
				resetLayerTagName(item1);
			}
		}
		else if (columns == 2){
			SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(item1->parent());
			if (checkItemZOrder(item1)){
				int z_order = item1->text(2).toInt();
				HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
				if (parent == NULL){
					scene->changeLayerZOrderFromForeManager(item1->text(1).toUtf8().data(), z_order);
				}
				else{
					scene->changeSpriteChildZOrderFromForeManager(parent->text(1).toUtf8().data(), item1->text(1).toUtf8().data(), z_order);
				}
			}
		}
		else{
			if (item1->text(0) != imageIndex){
				item1->setText(0, imageIndex);
			}
		}
	}
	check = false;
}

void LayerManagerTreeWidget::RenameNodeName(QTreeWidgetItem* item1, int columns)
{
	imageString = item1->text(1);
	imageZ_order = item1->text(2);
	imageIndex = item1->text(0);
	check = true;
}

ImageSpriteLayer* LayerManagerTreeWidget::getItemMappingLayer(QTreeWidgetItem* item)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	return scene->getForeManager()->getImageLayerFromTag(item->text(1).toStdString());
}

ImageSpriteLayer* LayerManagerTreeWidget::resetSceneCurrentLayerAndGetReturn(QTreeWidgetItem* item)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->getForeManager()->setChooseLayer(item->text(1).toLocal8Bit().data());
	if (scene->getForeManager()->getTargetLayer()->getTagName() == item->text(1).toStdString()){
		return  scene->getForeManager()->getTargetLayer();
	}
	return nullptr;
}

void LayerManagerTreeWidget::setShowEdgeLayer(QTreeWidgetItem* item)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	std::string tagname = item->text(1).toUtf8().data();
	scene->showNodeEdge(tagname, "", true);
}

void LayerManagerTreeWidget::setShowEdgeSprite(QTreeWidgetItem* itemParent, QTreeWidgetItem* itemChild)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	std::string tagname = itemChild->text(1).toUtf8().data();
	std::string parentname = itemParent->text(1).toUtf8().data();
	scene->showNodeEdge(parentname, tagname, false);
}

void LayerManagerTreeWidget::clearControlView()
{
	ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
	controlView->clearProperties();
}

void LayerManagerTreeWidget::setValueToControlViewLayer(QTreeWidgetItem* item)
{
	ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	controlView->show();
	controlView->raise();
	std::string tagname = item->text(1).toUtf8().data();
	ImageSpriteLayer* layer_ = scene->getLayerFromForeManager(tagname);
	if (layer_){
		controlView->setValueFromLayer(layer_);
	}
	else{
		MyLogger::getInstance()->addError("Error the LayerManagerTreeWidget::setValueToLayer be called, layer is null");
	}
}

void LayerManagerTreeWidget::setValueToControlViewSprite(QTreeWidgetItem* itemSprite)
{
	ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	controlView->show();
	controlView->raise();
	std::string tagname = itemSprite->text(1).toUtf8().data();
	std::string parentname = itemSprite->parent()->text(1).toUtf8().data();
	ImageSprite* sprite = scene->getSpriteChildFromForeManager(parentname, tagname);
	controlView->setValueFromChildSprite(sprite);
}

void LayerManagerTreeWidget::resetSpriteTagName(QTreeWidgetItem* itemSprite)
{
	QString str = itemSprite->text(1);
	QString strparent = itemSprite->parent()->text(1);
	if (!checkSpriteRepeated(itemSprite)){
		HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
		scene->changeSpriteChildTagFromForeManager(strparent.toStdString(), imageString.toStdString(), str.toStdString());
	}
}

void LayerManagerTreeWidget::resetLayerTagName(QTreeWidgetItem* itemLayer)
{
	if (!checkLayerRepeated(itemLayer)){
		QString str = itemLayer->text(1);
		HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
		scene->changeLayerTagNameFromForeManager(imageString.toStdString(), str.toStdString());
	}
}

//检测子item是否重名
bool LayerManagerTreeWidget::checkSpriteRepeated(QTreeWidgetItem* itemSprite)
{
	QTreeWidgetItem* itemParent = itemSprite->parent();
	QString str = itemSprite->text(1);
	QString strParent = itemParent->text(1);
	int sizeChild = itemParent->childCount();
	for (int j = 0; j < sizeChild; j++){
		auto itemChild = itemParent->child(j);
		if (itemChild != itemSprite && itemChild->text(1) == str && itemChild){
			if (g_openConfile){
				auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
				auto itemss = dynamic_cast<SpriteTreeItem*>(itemSprite);
				auto sprite = layerManager->getSprite(strParent.toStdString(), itemChild->text(1).toStdString());
				itemss->setText(1, itemss->text(1) + "__" + QString::number(itemss->getTag()));
				sprite->setTagName(itemss->text(1).toStdString());
			}
			else{
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("精灵名称重名!"));
				itemSprite->setText(1, imageString);
				return true;
			}
		}
	}
	return false;
}

//检测层名是否重复
bool LayerManagerTreeWidget::checkLayerRepeated(QTreeWidgetItem* itemLayer)
{
	int size = topLevelItemCount();
	QString str = itemLayer->text(1);
	for (int i = 0; i < size; i++){
		SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(topLevelItem(i));
		if (item != itemLayer && item->text(1) == str){
			if (g_openConfile){
				auto itemss = dynamic_cast<SpriteTreeItem*>(itemLayer);
				itemss->setText(1, itemss->text(1) + "__" + QString::number(itemss->getTag()));
				HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
				scene->changeLayerTagNameFromForeManager(imageString.toUtf8().data(), str.toUtf8().data());
			}
			else{
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("层名称重名!"));
				itemLayer->setText(1, imageString);
				return true;
			}
		}
	}
	return false;
}

//检测item的Z坐标是否正确
bool LayerManagerTreeWidget::checkItemZOrder(QTreeWidgetItem* item)
{
	QString z_order = item->text(2);
	if (-1 == isDigitStr(z_order)){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("必须输入一个正确的数字!"));
		item->setText(2, imageZ_order);
		return false;
	}
	return true;
}

QString	LayerManagerTreeWidget::getNewDefaultLayerName()
{
	QString layername = "layer_";
	int size = topLevelItemCount();
	int count = 0;
	for (int i = 0; i < size; i++){
		auto item = topLevelItem(i);
		auto str = item->text(1).mid(6);
		if (str.toInt() >= count){
			count = str.toInt() + 1;
		}
	}
	layername += QString::number(count);
	return layername;
}

int	LayerManagerTreeWidget::getNewDefaultLayerZOrder()
{
	SpriteTreeItem* currItem = dynamic_cast<SpriteTreeItem*>(currentItem());
	int z_order = topLevelItemCount();
	if (currItem){
		if (currItem->parent()){
			z_order = currItem->parent()->text(2).toInt();
		}
		else{
			z_order = currItem->text(2).toInt();
		}
	}
	return z_order;
}

bool LayerManagerTreeWidget::deleteSpriteItem(std::string layername, std::string spritename)
{
	auto itemSprite = getSpriteTreeItem(layername, spritename);
	if (itemSprite){
		delete itemSprite;
		return true;
	}
	return false;
}

bool LayerManagerTreeWidget::deleteLayerItem(std::string layername)
{
	int size = topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto item = dynamic_cast<SpriteTreeItem*>(topLevelItem(i));
		if (item->text(1) == layername.c_str()){
			delete item;
			return true;
		}
	}
	return false;
}

SpriteTreeItem*	LayerManagerTreeWidget::createNewLayerItem(QString layername, int zorder, int index)
{
	SpriteTreeItem* newItem = new SpriteTreeItem();
	newItem->setIcon(0, QIcon(":/icon/Resources/yes.ico"));
	newItem->setIcon(1, QIcon(":/icon/Resources/unlock.png"));
	newItem->setIcon(2, QIcon(":/icon/Resources/dynamic.png"));
	newItem->setData(0, Qt::UserRole, true);
	newItem->setData(1, Qt::UserRole, false);
	newItem->setData(2, Qt::UserRole, true);
	newItem->setText(0, QString::number(index));
	newItem->setText(1, layername);
	newItem->setText(2, QString::number(zorder));
	newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
	return newItem;
}

SpriteTreeItem*	LayerManagerTreeWidget::createNewSpriteItem(QString name, int ozrder, int index)
{
	SpriteTreeItem* newItem = new SpriteTreeItem();
	newItem->setText(0, QString::number(index));
	newItem->setText(1, name);
	newItem->setText(2, QString::number(ozrder));
	newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
	return newItem;
}

SpriteTreeItem* LayerManagerTreeWidget::getSpriteTreeItem(std::string parentname, std::string childname)
{
	int size = topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto itemLayer = dynamic_cast<SpriteTreeItem*>(topLevelItem(i));
		auto childCount = itemLayer->childCount();
		if (itemLayer->text(1) == parentname.c_str()){
			for (int k = 0; k < childCount; k++){
				auto childitem = dynamic_cast<SpriteTreeItem*>(itemLayer->child(k));
				if (childitem->text(1) == childname.c_str()){
					return childitem;
				}
			}
		}
	}
	return nullptr;
}

SpriteTreeItem*	LayerManagerTreeWidget::getLayerItem(std::string layername)
{
	int size = topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto itemLayer = dynamic_cast<SpriteTreeItem*>(topLevelItem(i));
		if (itemLayer->text(1) == layername.c_str()){
			return itemLayer;
		}
	}
	return nullptr;
}

void LayerManagerTreeWidget::removeAllItem()
{
	int size = topLevelItemCount();
	while (topLevelItemCount() > 0){
		SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(takeTopLevelItem(0));
		while (item->childCount() > 0){
			SpriteTreeItem* itemchild = dynamic_cast<SpriteTreeItem*>(item->child(0));
			delete itemchild;
		}
		delete item;
	}
}

void LayerManagerTreeWidget::sortParentItem()
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(currentItem());
	SpriteTreeItem* saveItem = item;
	if (item == nullptr)return;

	if (item->parent()){
		item = dynamic_cast<SpriteTreeItem*>(item->parent());
		saveItem = item;
	}
	QString str = item->text(1);
	int size = topLevelItemCount();
	inlinedata* data = new inlinedata[size];
	SpriteTreeItem** items = new SpriteTreeItem*[size];
	for (int i = 0; i < size; i++){
		SpriteTreeItem* itemc = dynamic_cast<SpriteTreeItem*>(topLevelItem(i));
		items[i] = itemc;
	}
	for (int i = 0; i < size; i++){
		takeTopLevelItem(0);
	}
	for (int i = 0; i < size; i++){
		data[i].index = i;
		SpriteTreeItem* itemc = dynamic_cast<SpriteTreeItem*>(items[i]);
		data[i].z_order = itemc->text(2).toInt();
		data[i].name = itemc->text(1);
		data[i].item = itemc;
	}
	sortalgorithm(data, size);
	for (int i = 0; i < size; i++){
		SpriteTreeItem* itemc = data[i].item;
		itemc->setText(0, QString::number(i));
		itemc->setText(2, QString::number(data[i].z_order));
		insertTopLevelItem(i, itemc);
	}
	setCurrentItem(saveItem);
	delete[] items;
	delete[] data;
}

void LayerManagerTreeWidget::sortSceneLayerByLayerItem()
{
	int size = topLevelItemCount();
	HelloWorld* scene = (HelloWorld*)g_scene;
	for (int i = 0; i < size; i++){
		auto item = topLevelItem(i);
		auto layername = item->text(1).toStdString();
		auto layerzorder = item->text(2).toInt();
		scene->changeLayerZOrderFromForeManager(layername, layerzorder);
	}
}

void LayerManagerTreeWidget::sortChildItem(QTreeWidgetItem* itemparent)
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	int size = itemparent->childCount();
	SpriteTreeItem** items = new SpriteTreeItem*[size];
	inlinedata* data = new inlinedata[size];
	for (int i = 0; i < size; i++){
		SpriteTreeItem* itemc = dynamic_cast<SpriteTreeItem*>(itemparent->child(i));
		items[i] = itemc;
	}
	for (int i = 0; i < size; i++){
		itemparent->takeChild(i);
	}
	for (int i = 0; i < size; i++){
		data[i].index = i;
		SpriteTreeItem* itemc = dynamic_cast<SpriteTreeItem*>(items[i]);
		data[i].z_order = itemc->text(2).toInt();
		data[i].name = itemc->text(1);
		data[i].item = itemc;
	}
	sortalgorithm(data, size);
	for (int i = 0; i < size; i++){
		SpriteTreeItem* itemc = data[i].item;
		itemc->setText(0, QString::number(i));
		itemc->setText(2, QString::number(data[i].z_order));
		itemparent->insertChild(i, itemc);
	}
	delete[] items;
	delete[] data;
}

void LayerManagerTreeWidget::sortImageLayerIndex()
{
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	int size = topLevelItemCount();
	for (size_t i = 0; i < size; i++){
		auto layerItem = topLevelItem(i);
		auto layer = scene->getLayerFromForeManager(layerItem->text(1).toStdString());
		layer->setLayerIndex(layerItem->text(0).toInt());
	}
}

void LayerManagerTreeWidget::sortSceneSpriteByChildItem(QTreeWidgetItem* itemparent)
{
	int size = itemparent->childCount();
	auto layername = itemparent->text(1).toStdString();
	HelloWorld* scene = (HelloWorld*)g_scene;
	for (int i = 0; i < size; i++){
		auto itemChild = itemparent->child(i);
		auto spritename = itemChild->text(1).toStdString();
		auto spritezorder = itemChild->text(2).toInt();
		scene->changeSpriteChildZOrderFromForeManager(layername, spritename, spritezorder);
	}
}

void LayerManagerTreeWidget::sortalgorithm(inlinedata* data, int size)
{
	inlinedata temp;
	for (int i = 0; i < size; i++){
		for (int j = i; j < size; j++){
			if (data[i].z_order > data[j].z_order){
				temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}
	}
}