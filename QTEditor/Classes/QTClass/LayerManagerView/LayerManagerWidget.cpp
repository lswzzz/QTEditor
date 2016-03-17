#include "LayerManagerWidget.h"
#include "FontChina.h"
#include <QLayout> 
#include "Global.h"
#include "qabstractitemview.h"
#include "HelloWorldScene.h"
#include <qlistwidget.h>
#include <qmessagebox.h>
#include <QHeaderView>
#include <qaction.h>
#include "ControlView.h"
#include "ImageSprite.h"
#include <QThreadPool>
#include <TemplateView.h>
#include "MyLogger.h"
#include "ImgTransPath.h"
#include "QTEditor.h"
#include "keyboardmappingdialog.h"

QColor LayerManagerWidget::spriteColor;
QColor LayerManagerWidget::layerColor;

LayerManagerWidget::LayerManagerWidget(char* title, QSize size)
:BaseDockWidget(title, size)
{
	addlayer = new QPushButton(FontChina::G2U("添加层"));
	addlayer->setObjectName("add_Button");
	delet = new QPushButton(FontChina::G2U("删除"));
	delet->setObjectName("delet_Button");
	up = new QPushButton(FontChina::G2U("上移"));
	up->setObjectName("up_Button");
	down = new QPushButton(FontChina::G2U("下移"));
	down->setObjectName("down_Button");
	sortLayer = new QPushButton(FontChina::G2U("按层级排序"));
	imageTreeWidget = new LayerManagerTreeWidget();
	imageTreeWidget->setObjectName("image_treeWidget");
	imageTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	lockButton = new QPushButton(QIcon(":/icon/Resources/lock.png"), "");
	showButton = new QPushButton(QIcon(":/icon/Resources/show.ico"), "");
	lockButton->setStyleSheet(" border: none; ");
	showButton->setStyleSheet(" border: none; ");
	lockButton->setMaximumSize(QSize(30, 30));
	showButton->setMaximumSize(QSize(30, 30));
	findButton = new QPushButton(QIcon(":/icon/Resources/open.ico"), "");
	findButton->setStyleSheet(" border: none; ");
	findButton->setMaximumSize(QSize(30, 30));
	gridButton = new QPushButton(QIcon(":/icon/Resources/grid.png"), "");
	gridButton->setStyleSheet(" border: none; ");
	gridButton->setMaximumSize(QSize(30, 30));
	sortSpriteButton = new QPushButton(QIcon(":/icon/Resources/Sort.ico"), "");
	sortSpriteButton->setStyleSheet(" border: none; ");
	sortSpriteButton->setMaximumSize(QSize(30, 30));
	outputButton = new QPushButton(QIcon(":/icon/Resources/output.ico"), "");
	outputButton->setStyleSheet(" border: none; ");
	outputButton->setMaximumSize(QSize(30, 30));

	lockButton->setToolTip(FontChina::G2U("锁定或解锁所有层"));
	showButton->setToolTip(FontChina::G2U("显示或隐藏所有层"));
	findButton->setToolTip(FontChina::G2U("查询一个节点"));
	sortSpriteButton->setToolTip(FontChina::G2U("排序当前层的所有精灵"));
	outputButton->setToolTip(FontChina::G2U("设置所有图层导出与否"));
	gridButton->setToolTip(FontChina::G2U("显示或隐藏网格"));

	QStringList headers;
	QStringList headers2;
	imageTreeWidget->setHeaderLabels(headers2 << FontChina::G2U("下标") << FontChina::G2U("层名称") << FontChina::G2U("层级"));
	QTreeWidgetItem* item2 = dynamic_cast<QTreeWidgetItem*>(imageTreeWidget->headerItem());
	item2->setSizeHint(0, QSize(50, 25));
	item2->setSizeHint(1, QSize(250, 25));
	item2->setSizeHint(2, QSize(50, 25));

	imageTreeWidget->show();
	dialog = new SpriteItemManagerDialog;
	selectDialog = new SelectSprite;

	_setLayout();
	_connect();
	is_Alt_Key = false;
	spriteColor = QColor(255, 0, 0, 255);
	layerColor = QColor(255, 0, 0, 255);
}

void LayerManagerWidget::keyPressEvent(QKeyEvent *e)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	if (e->key() == Qt::Key_Delete){

	}
	else if (e->key() == Qt::Key_Shift){
		scene->setShiftKeyStates(true);
	}
	else if (e->key() == Qt::Key_Backspace){
		//重置当前操作为Type_None;
		scene->resetState();
		g_TemplateFile = "";
		scene->setType(Type_None);
	}
	else if (e->key() == Qt::Key_Space){
		setCocCursor(Cursor_Type::Move_Scene);
		scene->setSpaceKeyStates(true);
	}
	else if (e->key() == Qt::Key_Alt){
		is_Alt_Key = true;
	}
}

void LayerManagerWidget::keyReleaseEvent(QKeyEvent* e)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	if (e->key() == Qt::Key_Delete){
		on_deletNode_Button_clicked();
	}
	else if (e->key() == Qt::Key_Shift){
		scene->setShiftKeyStates(false);
	}
	else if (e->key() == Qt::Key_Space){
		scene->setSpaceKeyStates(false);
		setCocCursor(Cursor_Type::Normal);
	}
	else if (e->key() == Qt::Key_Alt){
		is_Alt_Key = false;
	}
	else if (e->key() == Qt::Key_V){
		if (is_Alt_Key){
			scene->foreManager->PasteSpriteFromMultiSelect();
		}
	}
	else{
		auto editor = dynamic_cast<QTEditor*>(g_editor);
		editor->getKeyBoardMappingDialog()->catchedKeyEvent((Qt::Key)e->key());
	}
}

void LayerManagerWidget::_setLayout()	
{
	QVBoxLayout* vlayout = new QVBoxLayout(widget);
	QHBoxLayout* hlayout = new QHBoxLayout(widget);
	hlayout->addWidget(addlayer);
	hlayout->addWidget(delet);
	hlayout->addWidget(up);
	hlayout->addWidget(down);
	hlayout->addWidget(sortLayer);
	
	QHBoxLayout* hlayout3 = new QHBoxLayout(widget);
	hlayout3->addWidget(findButton);
	hlayout3->addWidget(lockButton);
	hlayout3->addWidget(showButton);
	hlayout3->addWidget(gridButton);

	QHBoxLayout* hlayout4 = new QHBoxLayout(widget);
	hlayout4->addWidget(outputButton);
	hlayout4->addWidget(sortSpriteButton);

	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addLayout(hlayout4, 0, 0);
	gridLayout->addWidget(new QLabel, 0, 1, 1, 3);
	gridLayout->addLayout(hlayout3, 0, 4);

	vlayout->addLayout(gridLayout);
	vlayout->addWidget(imageTreeWidget);
	vlayout->addLayout(hlayout);
	widget->setLayout(vlayout);
}

void LayerManagerWidget::_connect()
{
	connect(sortLayer, SIGNAL(clicked()), this, SLOT(on_sortLayerByZorder_clicked()));
	connect(addlayer, SIGNAL(clicked()), this, SLOT(on_addLayer_Button_clicked()));
	connect(delet, SIGNAL(clicked()), this, SLOT(on_deletNode_Button_clicked()));
	connect(up, SIGNAL(clicked()), this, SLOT(on_up_Button_clicked()));
	connect(down, SIGNAL(clicked()), this, SLOT(on_down_Button_clicked()));
	connect(lockButton, SIGNAL(clicked()), this, SLOT(on_lockButton_clicked()));
	connect(showButton, SIGNAL(clicked()), this, SLOT(on_showButton_clicked()));
	connect(findButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));
	connect(sortSpriteButton, SIGNAL(clicked()), this, SLOT(on_SortSpriteByZorder_clicked()));
	connect(outputButton, SIGNAL(clicked()), this, SLOT(on_OutputButton_clicked()));
	connect(gridButton, SIGNAL(clicked()), this, SLOT(on_GridButton_clicked()));
}

void LayerManagerWidget::on_addLayer_Button_clicked()
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	QString layername = imageTreeWidget->getNewDefaultLayerName();
	int z_order = imageTreeWidget->getNewDefaultLayerZOrder();
	scene->addForeImageLayer(layername.toUtf8().data(), z_order);
	scene->setType(Type_Foce_Image_Layer);
	addItemLayerAndSort(layername, z_order);
}

void LayerManagerWidget::deleteSpriteChildItem(std::string layername, std::string spritename)
{
	if (imageTreeWidget->deleteSpriteItem(layername, spritename)){
		ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
		if (controlView->targetPropertiesIsSprite()){
			controlView->clearProperties();
		}
	}
}

void LayerManagerWidget::deleteSpriteLayerItem(std::string layername)
{
	if (imageTreeWidget->deleteLayerItem(layername)){
		ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
		if (!controlView->targetPropertiesIsSprite()){
			controlView->clearProperties();
		}
	}
}

//将当前使用统一模板的精灵全部选中
void LayerManagerWidget::SelectedAllUseTemplate(QString templateFile)
{
	SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	if (item){
		auto list = imageTreeWidget->selectedItems();
		for (int i = 0; i < list.size(); i++){
			imageTreeWidget->setItemSelected(list.at(i), false);
		}
		SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(item->parent());
		if (parent){
			item = parent;
		}
		int size = item->childCount();
		QString parentStr = item->text(1);
		for (int i = 0; i < size; i++){
			auto itemChild = item->child(i);
			QString childStr = itemChild->text(1);
			auto sprite = scene->getSpriteChildFromForeManager(parentStr.toStdString(), childStr.toStdString());
			if (sprite->getTemplateFile() == templateFile.toStdString()){
				imageTreeWidget->setItemSelected(itemChild, true);
			}
		}
	}
}

SpriteTreeItem* LayerManagerWidget::getCurrentSpriteTreeItem()
{
	SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(item->parent());
	if (parent){
		return item;
	}
	else{
		return nullptr;
	}
}

SpriteTreeItem* LayerManagerWidget::getSpriteTreeItem(std::string parent, std::string child)
{
	return imageTreeWidget->getSpriteTreeItem(parent, child);
}

SpriteTreeItem* LayerManagerWidget::getLayerItem(std::string layername)
{
	return imageTreeWidget->getLayerItem(layername);
}

std::string LayerManagerWidget::getLayerNameByIndex(int index)
{
	auto item = imageTreeWidget->topLevelItem(index);
	std::string layername = item->text(1).toStdString();
	if (item->text(0).toInt() != index){
		addConsoleInfo("错误item的序号错误layer是%s", layername.c_str());
	}
	return layername;
}

int LayerManagerWidget::getLayerIndex(std::string& layername)
{
	auto item = imageTreeWidget->getLayerItem(layername);
	return item->text(0).toInt();
}

void LayerManagerWidget::add_ImageChild()
{
	SpriteTreeItem* rootItem = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	if (rootItem == NULL){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("请先创建一个层"));
		return;
	}
	HelloWorld* scene = (HelloWorld*)g_scene;
	SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(rootItem->parent());
	if (parent){
		scene->setType(Type_Foce_Image_Child, FontChina::G2U(parent->text(1).toUtf8().data()));
	}
	else{
		scene->setType(Type_Foce_Image_Child, FontChina::G2U(rootItem->text(1).toUtf8().data()));
	}
}

void LayerManagerWidget::on_deletNode_Button_clicked()
{
	MyLogger::getInstance()->addInfo("the LayerManagerWidget::on_delet_Button_clicked be called");
	SpriteTreeItem* item;
	item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	if (item == NULL){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("请先选择一个目标"));
		return;
	}
	QMessageBox::StandardButton rb = QMessageBox::warning(this, FontChina::G2U("谨慎"), FontChina::G2U("你确认删除吗"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (rb == QMessageBox::No){
		return;
	}
	bool containsLayer = false;
	QList<QTreeWidgetItem*> list = imageTreeWidget->selectedItems();
	HelloWorld* scene = (HelloWorld*)g_scene;
	QVector<QString> layerNameVec;
	for (int i = 0; i < list.size(); i++){
		item = dynamic_cast<SpriteTreeItem*>(list.at(i));
		SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(item->parent());
		QString str = item->text(1);
		auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
		if (parent == NULL){
			containsLayer = true;
			auto layer = scene->getLayerFromForeManager(str.toStdString());
			auto vec = layer->getAllSpriteNode();
			for (int k = 0; k < vec.size(); k++){
				auto sprite = dynamic_cast<ImageSprite*>(vec.at(k));
				templateView->itemDeleteSprite(sprite);
			}
			scene->deleteLayerFromForeManager(str.toUtf8().data());
		}
		else{
			QString strparent = parent->text(1);
			auto sprite = scene->getSpriteChildFromForeManager(strparent.toStdString(), str.toStdString());
			templateView->itemDeleteSprite(sprite);
			scene->deleteSprtteChildFromForeManager(strparent.toUtf8().data(), str.toUtf8().data());
			if (!layerNameVec.contains(strparent)){
				layerNameVec.push_back(strparent);
			}
		}
	}
	for (int i = 0; i < layerNameVec.size(); i++){
		auto layer = scene->getLayerFromForeManager(layerNameVec.at(i).toStdString());
		layer->removeSpriteOver();
	}
	for (int i = 0; i < list.size(); i++){
		delete list.at(i);
	}
	if (containsLayer){
		on_sortLayerByZorder_clicked();
	}
	scene->setType(Type_None);
	ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
	controlView->clearProperties();
}

//http://www.oschina.net/question/234345_48839
void LayerManagerWidget::on_up_Button_clicked()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	if (!item)return;
	QModelIndex mindex = imageTreeWidget->currentIndex();
	int index = mindex.row();
	SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(item->parent());
	if (parent == NULL){
		SpriteTreeItem* above = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->itemAbove(item));
		if (above == NULL){
			return;
		}
		if (above->parent() != NULL){
			above = dynamic_cast<SpriteTreeItem*>(above->parent());
		}
		if (index > 0){
			swapLayerItem(item, above);
			imageTreeWidget->setCurrentItem(item);
		}
	}
	else{
		SpriteTreeItem* above = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->itemAbove(item));
		if (index > 0){
			swapSpriteItem(item, above);
			imageTreeWidget->setCurrentItem(item);
		}
	}
}

void LayerManagerWidget::on_down_Button_clicked()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	if (!item)return;
	QModelIndex mindex = imageTreeWidget->currentIndex();
	SpriteTreeItem* parent = dynamic_cast<SpriteTreeItem*>(item->parent());
	if (parent == NULL){
		int index = mindex.row();
		int count = imageTreeWidget->topLevelItemCount();
		int belowindex = index + 1;
		if (index >= count - 1){
			return;
		}
		SpriteTreeItem* below = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->topLevelItem(belowindex));
		QString text = below->text(1);
		if (count - 1 > index){
			swapLayerItem(item, below);
			imageTreeWidget->setCurrentItem(item);
		}
	}
	else{
		SpriteTreeItem* below = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->itemBelow(item));
		int index = mindex.row();
		int count = item->parent()->childCount();
		if (count-1 > index){
			swapSpriteItem(item, below);
			imageTreeWidget->setCurrentItem(item);
		}
	}
}

void LayerManagerWidget::swapSpriteItem(SpriteTreeItem* item1, SpriteTreeItem* item2)
{
	QTreeWidgetItem* itemParent = item1->parent();
	QString strparent = itemParent->text(1);
	int index1 = itemParent->indexOfChild(item1);
	int index2 = itemParent->indexOfChild(item2);
	QString zOrder1 = item1->text(2);
	QString zOrder2 = item2->text(2);
	QString tag1 = item1->text(1);
	QString tag2 = item2->text(1);
	if (index1 < index2){
		itemParent->takeChild(index1);
		itemParent->takeChild(index2 - 1);
		itemParent->insertChild(index1, item2);
		itemParent->insertChild(index2, item1);
	}
	else{
		itemParent->takeChild(index2);
		itemParent->takeChild(index1 - 1);
		itemParent->insertChild(index2, item1);
		itemParent->insertChild(index1, item2);
	}
	item1->setText(0, QString::number(index2));
	item2->setText(0, QString::number(index1));
	item1->setText(2, zOrder2);
	item2->setText(2, zOrder1);
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->changeSpriteChildZOrderFromForeManager(strparent.toUtf8().data(), tag1.toUtf8().data(), zOrder2.toInt());
	scene->changeSpriteChildZOrderFromForeManager(strparent.toUtf8().data(), tag2.toUtf8().data(), zOrder1.toInt());
}

//只能交换相邻的
void LayerManagerWidget::swapLayerItem(SpriteTreeItem* item1, SpriteTreeItem* item2)
{
	int index1 = imageTreeWidget->indexOfTopLevelItem(item1);
	int index2 = imageTreeWidget->indexOfTopLevelItem(item2);
	QString zOrder1 = item1->text(2);
	QString zOrder2 = item2->text(2);
	QString tag1 = item1->text(1);
	QString tag2 = item2->text(1);
	if (index1 < index2){
		imageTreeWidget->takeTopLevelItem(index1);
		imageTreeWidget->takeTopLevelItem(index2 - 1);
		imageTreeWidget->insertTopLevelItem(index1, item2);
		imageTreeWidget->insertTopLevelItem(index2, item1);
	}
	else{
		imageTreeWidget->takeTopLevelItem(index2);
		imageTreeWidget->takeTopLevelItem(index1 - 1);
		imageTreeWidget->insertTopLevelItem(index2, item1);
		imageTreeWidget->insertTopLevelItem(index1, item2);
	}
	item1->setText(0, QString::number(index2));
	item2->setText(0, QString::number(index1));
	item1->setText(2, zOrder2);
	item2->setText(2, zOrder1);
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->changeLayerZOrderFromForeManager(tag1.toUtf8().data(), zOrder2.toInt());
	scene->changeLayerZOrderFromForeManager(tag2.toUtf8().data(), zOrder1.toInt());
	scene->getLayerFromForeManager(tag1.toUtf8().data())->setLayerIndex(index2);
	scene->getLayerFromForeManager(tag2.toUtf8().data())->setLayerIndex(index1);
	updateForeManageLayerStruct();
}

void LayerManagerWidget::hideNode()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	QTreeWidgetItem* item = imageTreeWidget->currentItem();
	if (item->parent())
	{
		item->setIcon(0, QIcon(":/icon/Resources/no.ico"));
		item->setTextColor(0, spriteColor);
		scene->setSpriteChildVisibleFromForeManager(item->parent()->text(1).toUtf8().data(), item->text(1).toUtf8().data(), false);
	}
	else{
		setLayerVisible(item, false);
	}
}

void LayerManagerWidget::showNode()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	QTreeWidgetItem* item = imageTreeWidget->currentItem();
	if (item->parent())
	{
		item->setIcon(0, QIcon(":/icon/Resources/yes.ico"));
		item->setTextColor(0, QColor(0, 0, 0));
		scene->setSpriteChildVisibleFromForeManager(item->parent()->text(1).toUtf8().data(), item->text(1).toUtf8().data(), true);
	}
	else{
		setLayerVisible(item, true);
	}
}

void LayerManagerWidget::moveSpriteOtherLayer()
{
	QList<QTreeWidgetItem*> list;
	for (int i = 0; i < imageTreeWidget->topLevelItemCount(); i++){
		list.push_back(imageTreeWidget->topLevelItem(i));
	}
	dialog->addShowLayer(list);
	dialog->addMoveSprite(imageTreeWidget->selectedItems());
	dialog->show();
}

void LayerManagerWidget::locatioinSprite()
{
	auto item = imageTreeWidget->currentItem();
	if (item->parent()){
		HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
		auto sprite = scene->getSpriteChildFromForeManager(item->parent()->text(1).toUtf8().data(), item->text(1).toUtf8().data());
		auto layer = scene->getLayerFromForeManager(item->parent()->text(1).toUtf8().data());
		auto pos = sprite->getPosition() + layer->getPosition();
		scene->moveSceneToPos(pos);
	}
}

//静态或动态
void LayerManagerWidget::setState()
{
	QTreeWidgetItem* item = imageTreeWidget->currentItem();
	if (item && item->parent() == NULL){
		if (item->data(2, Qt::UserRole).toBool()){
			setLayerState(item, false);
		}
		else{
			setLayerState(item, true);
		}
	}
}

void LayerManagerWidget::lockLayer()
{
	QTreeWidgetItem* item = imageTreeWidget->currentItem();
	if (item && item->parent() == NULL){
		if (!item->data(1, Qt::UserRole).toBool()){
			setLayerLockState(item, true);
		}
		else{
			setLayerLockState(item, false);
		}
	}
}

void LayerManagerWidget::dontOutputLayer()
{
	QTreeWidgetItem* item = imageTreeWidget->currentItem();
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	if (item && item->parent() == NULL){
		QColor color = item->textColor(1);
		if (color == QColor(0, 0, 0)){
			item->setTextColor(1, layerColor);
			auto layer = scene->getLayerFromForeManager(item->text(1).toLocal8Bit().data());
			layer->setDontOutputState(true);
		}
		else{
			item->setTextColor(1, QColor(0, 0, 0));
			auto layer = scene->getLayerFromForeManager(item->text(1).toLocal8Bit().data());
			layer->setDontOutputState(false);
		}
	}
}

void LayerManagerWidget::setLayerDontOutputStateIcon(std::string parent, bool dontOuput)
{
	auto item = getLayerItem(parent);
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	if (item){
		if (dontOuput){
			item->setTextColor(1, layerColor);
		}
		else{
			item->setTextColor(1, QColor(0, 0, 0));
		}
	}
}

void LayerManagerWidget::on_lockButton_clicked()
{
	static bool lock = false;
	lock = !lock;
	for (int i = 0; i < imageTreeWidget->topLevelItemCount(); i++){
		auto item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->topLevelItem(i));
		if (item){
			setLayerLockState(item, lock);
		}
	}
}

void LayerManagerWidget::on_showButton_clicked()
{
	static bool show = true;
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	show = !show;
	for (int i = 0; i < imageTreeWidget->topLevelItemCount(); i++){
		auto item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->topLevelItem(i));
		if (item){
			setLayerVisible(item, show);
		}
	}
}

void LayerManagerWidget::on_findButton_clicked()
{
	selectDialog->setTreeWidget(imageTreeWidget);
	selectDialog->show();
}

void LayerManagerWidget::on_GridButton_clicked()
{
	static bool show = false;
	show = !show;
	HelloWorld* scene = (HelloWorld*)g_scene;
	if (show){
		scene->setOptionShowOrHide(1);
	}
	else{
		scene->setOptionShowOrHide(3);
	}
}

void LayerManagerWidget::on_SortSpriteByZorder_clicked()
{
	auto item = imageTreeWidget->currentItem();
	if (item){
		if (item->parent()){
			imageTreeWidget->sortChildItem(item->parent());
		}
		else{
			imageTreeWidget->sortChildItem(item);
		}
	}
}

void LayerManagerWidget::on_OutputButton_clicked()
{
	static bool dooutput = false;
	dooutput = !dooutput;
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	for (int i = 0; i < imageTreeWidget->topLevelItemCount(); i++){
		auto item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->topLevelItem(i));
		if (dooutput){
			item->setTextColor(1, QColor(255, 0, 0));
			scene->getLayerFromForeManager(item->text(1).toStdString())->setDontOutputState(dooutput);
		}
		else{
			item->setTextColor(1, QColor(0, 0, 0));
			scene->getLayerFromForeManager(item->text(1).toStdString())->setDontOutputState(dooutput);
		}
	}
}

int LayerManagerWidget::getItemCount()
{
	return imageTreeWidget->topLevelItemCount();
}

int LayerManagerWidget::getCurrentRootIndex()
{
	auto item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	if (!item){
		return -1;
	}
	if (item->parent()){
		return imageTreeWidget->indexOfTopLevelItem(item->parent());
	}
	else{
		return imageTreeWidget->indexOfTopLevelItem(item);
	}
}

int LayerManagerWidget::getCurrentRootChildCount()
{
	auto item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	if (item->parent()){
		return item->parent()->childCount();
	}
	else{
		return item->childCount();
	}
}

int LayerManagerWidget::getCurrentRootChildIndex()
{
	auto item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	if (item->parent()){
		return item->parent()->indexOfChild(item) + 1;
	}
	else{
		return item->childCount();
	}
}

void LayerManagerWidget::removeAllItem()
{
	imageTreeWidget->removeAllItem();
}

//用于打开的时候进行数据获取
bool LayerManagerWidget::addItemSpriteAsyn(QString parent, QString name, int zorder, Node* node)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	auto layerItem = imageTreeWidget->getLayerItem(parent.toStdString());
	if (layerItem){
		SpriteTreeItem* newSpriteItem = addItemDefaultWay(parent, name, zorder);
		ImageSprite* sprite;
		if (node){
			sprite = dynamic_cast<ImageSprite*>(node);
		}
		else{
			sprite = scene->getSpriteChildFromForeManager(parent.toStdString(), name.toStdString());
		}
		newSpriteItem->setSpriteTag(sprite->getId());
		asyncGenerateSpriteItemIcon(newSpriteItem, sprite);
		return true;
	}
	return false;
}

void LayerManagerWidget::GenerateSpriteItemIcon(SpriteTreeItem* child, ImageSprite* sprite)
{
	if (sprite->isHasEntityProperties()){
		child->setIcon(2, QIcon(ImgTransPath::getListImagePath(sprite)));
		if (sprite->isVisible()){
			child->setIcon(0, QIcon(":/icon/Resources/yes.ico"));
			child->setTextColor(0, QColor(0, 0, 0));
		}
		else{
			child->setIcon(0, QIcon(":/icon/Resources/no.ico"));
			child->setTextColor(0, spriteColor);
		}
	}
	else{
		if (sprite->isVisible()){
			child->setTextColor(0, QColor(0, 0, 0));
		}
		else{
			child->setTextColor(0, spriteColor);
		}
	}
	QString templateFile = sprite->getTemplateFile().c_str();
	if (templateFile != ""){
		child->setIcon(1, QIcon(":/icon/Resources/template.ico"));
	}
}

void LayerManagerWidget::asyncGenerateSpriteItemIcon(SpriteTreeItem* childItem, ImageSprite* sprite)
{
	openFileAddItemThread* itemThread = new openFileAddItemThread;
	itemThread->child = childItem;
	itemThread->sprite = sprite;
	itemThread->setAutoDelete(true);
	QThreadPool* threadPool = QThreadPool::globalInstance();
	threadPool->start(itemThread);
	//threadPool->waitForDone();
}

int LayerManagerWidget::addItemLayerNoSort(QString layername, int zorder, int index)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	int i = imageTreeWidget->topLevelItemCount();
	if (index > i)addConsoleInfo("错误, 导入的层数据紊乱");
	SpriteTreeItem* newItem = imageTreeWidget->createNewLayerItem(layername, zorder, index);
	imageTreeWidget->insertTopLevelItem(index, newItem);
	imageTreeWidget->setCurrentItem(newItem);
	return index;
}

int LayerManagerWidget::addItemLayerAndSort(QString layername, int zorder)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	//int i = imageTreeWidget->topLevelItemCount();
	auto item = imageTreeWidget->currentItem();
	int i;
	if (item){
		if (item->parent())item = item->parent();
		i = item->text(0).toInt() + 1;
	}
	else{
		i = 0;
	}
	
	SpriteTreeItem* newItem = imageTreeWidget->createNewLayerItem(layername, zorder, i);
	imageTreeWidget->insertTopLevelItem(i, newItem);
	imageTreeWidget->setCurrentItem(newItem);
	imageTreeWidget->sortParentItem();
	imageTreeWidget->sortImageLayerIndex();
	imageTreeWidget->sortSceneLayerByLayerItem();
	return newItem->text(0).toInt();
}

int LayerManagerWidget::addItemSpriteByNode(QString layername, Node* node, int zorder, bool isMulti)
{
	auto sprite = dynamic_cast<ImageSprite*>(node);
	QString spritename = sprite->getTagName().c_str();
	auto layerItem = imageTreeWidget->getLayerItem(layername.toStdString());
	int childCount = layerItem->childCount();
	SpriteTreeItem* newSpriteItem = addItemDefaultWay(layername, spritename, zorder);
	if (!isMulti){
		auto list = imageTreeWidget->selectedItems();
		for (int i = 0; i < list.size(); i++){
			imageTreeWidget->setItemSelected(list.at(i), false);
		}
		imageTreeWidget->setCurrentItem(newSpriteItem);
	}
	newSpriteItem->setSpriteTag(sprite->getId());
	GenerateSpriteItemIcon(newSpriteItem, sprite);
	return childCount;
}

int LayerManagerWidget::addItemSprite(QString layername, QString name, int zorder, bool isMulti)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	ImageSprite* sprite = getSprite(layername.toStdString(), name.toStdString());
	return addItemSpriteByNode(layername, sprite, zorder, isMulti);
}

SpriteTreeItem* LayerManagerWidget::addItemDefaultWay(QString layername, QString spritename, int zorder)
{
	auto layerItem = imageTreeWidget->getLayerItem(layername.toStdString());
	int childCount = layerItem->childCount();
	auto newSpriteItem = imageTreeWidget->createNewSpriteItem(spritename, zorder, childCount);
	layerItem->addChild(newSpriteItem);
	return newSpriteItem;
}

void LayerManagerWidget::contextMenuEvent(QContextMenuEvent *event)
{
	SpriteTreeItem* item = dynamic_cast<SpriteTreeItem*>(imageTreeWidget->currentItem());
	bool isRoot = false;
	if (!item)return;
	if (item->parent() == NULL){
		isRoot = true;
	}
	else{
		isRoot = false;
	}
	QMenu* menu;
	QAction* showAction;
	QAction* hideAction;
	QAction* stateAction;
	QAction* lockAction;
	QAction* dontOutputAction;
	menu = new QMenu(this);
	showAction = new QAction(tr(FontChina::G2U("显示")), this);
	hideAction = new QAction(tr(FontChina::G2U("隐藏")), this);
	menu->addAction(showAction);
	menu->addAction(hideAction);
	connect(showAction, SIGNAL(triggered()), this, SLOT(showNode()));
	connect(hideAction, SIGNAL(triggered()), this, SLOT(hideNode()));
	if (isRoot){
		auto scene = static_cast<HelloWorld*>(g_scene);
		auto layer = scene->getLayerFromForeManager(item->text(1).toLocal8Bit().data());
		if (layer->isSportState()){
			stateAction = new QAction(tr(FontChina::G2U("静态层")), this);
		}
		else{
			stateAction = new QAction(tr(FontChina::G2U("动态层")), this);
		}
		if (layer->isLock()){
			lockAction = new QAction(tr(FontChina::G2U("取消锁定")), this);
		}
		else{
			lockAction = new QAction(tr(FontChina::G2U("锁定")), this);
		}
		if (layer->isDontOutput()){
			dontOutputAction = new QAction(tr(FontChina::G2U("导出当前层")), this);
		}
		else{
			dontOutputAction = new QAction(tr(FontChina::G2U("不导出当前层")), this);
		}
		menu->addAction(stateAction);
		menu->addAction(lockAction);
		menu->addAction(dontOutputAction);
		connect(stateAction, SIGNAL(triggered()), this, SLOT(setState()));
		connect(lockAction, SIGNAL(triggered()), this, SLOT(lockLayer()));
		connect(dontOutputAction, SIGNAL(triggered()), this, SLOT(dontOutputLayer()));
	}
	else{
		QAction* moveSpriteAction = new QAction(tr(FontChina::G2U("移动到其他层")), this);
		QAction* locationAction = new QAction(tr(FontChina::G2U("定位")), this);
		menu->addAction(moveSpriteAction);
		menu->addAction(locationAction);
		connect(moveSpriteAction, SIGNAL(triggered()), this, SLOT(moveSpriteOtherLayer()));
		connect(locationAction, SIGNAL(triggered()), this, SLOT(locatioinSprite()));
	}
	if (item){
		menu->exec(QCursor::pos());
	}

}

int LayerManagerWidget::getCurrItemIndex()
{
	return SpriteTreeItem::getCurrTag();
}

void LayerManagerWidget::setCurrItemIndex(int count)
{
	SpriteTreeItem::setCurrTagCount(count);
}

bool LayerManagerWidget::setLayerVisibleIcon(QTreeWidgetItem* item, bool visible)
{
	if (item){
		if (visible){
			item->setIcon(0, QIcon(":/icon/Resources/yes.ico"));
			item->setData(0, Qt::UserRole, true);
		}
		else{
			item->setIcon(0, QIcon(":/icon/Resources/no.ico"));
			item->setData(0, Qt::UserRole, false);
		}
		return true;
	}
	else{
		return false;
	}
}

bool LayerManagerWidget::setChildIcon(int parentindex, int index, std::string filepath)
{
	QTreeWidgetItem* parent = imageTreeWidget->topLevelItem(parentindex);
	if (parent){
		QTreeWidgetItem* child = parent->child(index);
		if (child){
			//child->setIcon(2, QIcon(filepath.c_str()));
			return true;
		}
		return false;
	}
	return false;
}

void LayerManagerWidget::setLayerStateIcon(QTreeWidgetItem* item, bool state)
{
	if (item){
		if (state == false){
			item->setIcon(2, QIcon(":/icon/Resources/static.png"));
			item->setData(2, Qt::UserRole, false);
		}
		else{
			item->setIcon(2, QIcon(":/icon/Resources/dynamic.png"));
			item->setData(2, Qt::UserRole, true);
		}
	}
}

void LayerManagerWidget::setLayerLockStateIcon(QTreeWidgetItem* item, bool isLock)
{
	if (item){
		if (isLock == true){
			item->setIcon(1, QIcon(":/icon/Resources/lock.png"));
			item->setData(1, Qt::UserRole, true);
		}
		else{
			item->setIcon(1, QIcon(":/icon/Resources/unlock.png"));
			item->setData(1, Qt::UserRole, false);
		}
	}
}

void LayerManagerWidget::setLayerStateIcon(int index, bool state)
{
	QTreeWidgetItem* item = imageTreeWidget->topLevelItem(index);
	setLayerStateIcon(item, state);
}

void LayerManagerWidget::setLayerLockStateIcon(int index, bool isLock)
{
	QTreeWidgetItem* item = imageTreeWidget->topLevelItem(index);
	setLayerLockStateIcon(item, isLock);
}

bool LayerManagerWidget::setLayerVisibleIcon(int index, bool visible)
{
	QTreeWidgetItem* item = imageTreeWidget->topLevelItem(index);
	return setLayerVisibleIcon(item, visible);
}

std::string LayerManagerWidget::getCurrentLayerName()
{
	auto item = imageTreeWidget->currentItem();
	if (item){
		auto o_item = item->parent();
		if (o_item){
			return o_item->text(1).toLocal8Bit().data();
		}
		else{
			return item->text(1).toLocal8Bit().data();
		}
	}
	return "";
}

std::string LayerManagerWidget::getCurrentSpriteName()
{
	auto item = imageTreeWidget->currentItem();
	if (item){
		if (item->parent()){
			return item->text(1).toLocal8Bit().data();
		}
	}
	return "";
}

ImageSprite* LayerManagerWidget::getCurrentSprite()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	std::string layername = getCurrentLayerName();
	std::string spritename = getCurrentSpriteName();
	if (layername != "" && spritename != ""){
		ImageSprite* sprite = scene->getSpriteChildFromForeManager(layername, spritename);
		return sprite;
	}
	return nullptr;
}

ImageSprite* LayerManagerWidget::getSprite(std::string parentname, std::string tagname)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	return scene->foreManager->getImageChildFromTag(parentname, tagname);
}

void LayerManagerWidget::setCurrentLayer(std::string name)
{
	auto size = imageTreeWidget->topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto item = imageTreeWidget->topLevelItem(i);
		QString str = name.c_str();
		if (item->text(1) == str){
			imageTreeWidget->setCurrentItem(item);
			return;
		}
	}
}

void LayerManagerWidget::setCurrentSpriteChild(std::string parentname, std::string tagname)
{
	QString str = parentname.c_str();
	QString tag = tagname.c_str();
	auto size = imageTreeWidget->topLevelItemCount();
	for (int i = 0; i < size; i++){
		auto item = imageTreeWidget->topLevelItem(i);
		if (item->text(1) == str){
			auto childCount = item->childCount();
			for (int k = 0; k < childCount; k++){
				auto child = item->child(k);
				if (child->text(1) == tag){
					imageTreeWidget->setCurrentItem(child);
					return;
				}
			}
		}
	}
}

void LayerManagerWidget::on_sortLayerByZorder_clicked()
{
	imageTreeWidget->sortParentItem();
	imageTreeWidget->sortImageLayerIndex();
	imageTreeWidget->sortSceneLayerByLayerItem();
	addConsoleInfo("排序层完成");
}

void LayerManagerWidget::updateForeManageLayerStruct()
{
	std::vector<std::string> vec;
	auto count = imageTreeWidget->topLevelItemCount();
	for (size_t i = 0; i < count; i++){
		auto item = imageTreeWidget->topLevelItem(i);
		vec.push_back(item->text(1).toStdString());
	}
	auto scene = static_cast<HelloWorld*>(g_scene);
	auto foreManager = scene->getForeManager();
	foreManager->updateImageVecOrder(vec);
}

void LayerManagerWidget::collapseCurrentLayer()
{
	auto item = imageTreeWidget->currentItem();
	if (item->parent())item = item->parent();
	imageTreeWidget->collapseItem(item);
}

void LayerManagerWidget::expandCurrentLayer()
{
	auto item = imageTreeWidget->currentItem();
	if (item->parent())item = item->parent();
	imageTreeWidget->expandItem(item);

}

void LayerManagerWidget::touchLayerIconEvent()
{
	if (imageTreeWidget->touchIcon){
		auto item = imageTreeWidget->currentItem();
		auto index = imageTreeWidget->currentIndex().row();
		if (imageTreeWidget->touchcol == 0){
			setLayerVisible(item, !item->data(0, Qt::UserRole).toBool());
		}
		else if (imageTreeWidget->touchcol == 1){
			setLayerLockState(item, !item->data(1, Qt::UserRole).toBool());
		}
		else{
			setLayerState(item, !item->data(2, Qt::UserRole).toBool());
		}
	}
}

void LayerManagerWidget::touchSpriteIconEvent()
{

}

void LayerManagerWidget::setLayerVisible(QTreeWidgetItem* item, bool visible)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->setLayerVisibleFromForeManager(item->text(1).toUtf8().data(), visible);
	auto index = imageTreeWidget->currentIndex().row();
	setLayerVisibleIcon(item, visible);
}

void LayerManagerWidget::setLayerLockState(QTreeWidgetItem* item, bool isLock)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->foreManager->setLayerLockState(item->text(1).toUtf8().data(), isLock);
	auto index = imageTreeWidget->currentIndex().row();
	setLayerLockStateIcon(item, isLock);
}

void LayerManagerWidget::setLayerState(QTreeWidgetItem* item, bool isSport)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	scene->setLayerStateFromForeManager(item->text(1).toUtf8().data(), isSport);
	auto index = imageTreeWidget->currentIndex().row();
	setLayerStateIcon(item, isSport);
}

LayerManagerWidget::~LayerManagerWidget()
{
}

