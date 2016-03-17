#include "spriteitemmanagerdialog.h"
#include "ui_spriteitemmanagerdialog.h"
#include "FontChina.h"
#include "QLayout"
#include "Global.h"
#include "LayerManagerWidget.h"
#include "HelloWorldScene.h"

SpriteItemManagerDialog::SpriteItemManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpriteItemManagerDialog)
{
    ui->setupUi(this);
	_setLayout();
	_connect();
}

SpriteItemManagerDialog::~SpriteItemManagerDialog()
{
    delete ui;
}

void SpriteItemManagerDialog::_setLayout()
{
	layerWidget = new QTreeWidget;
	layerWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	QStringList headers2;
	layerWidget->setHeaderLabels(headers2 << FontChina::G2U("层"));
	sureButton = new QPushButton(FontChina::G2U("确定"));
	cancelButton = new QPushButton(FontChina::G2U("取消"));
	QGridLayout* layout = new QGridLayout;
	layout->addWidget(layerWidget, 0, 0, 1, 2);
	layout->addWidget(sureButton, 1, 0, 1, 1);
	layout->addWidget(cancelButton, 1, 1, 1, 1);
	setLayout(layout);
}

void SpriteItemManagerDialog::_connect()
{
	connect(sureButton, SIGNAL(clicked()), this, SLOT(on_sureButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
}

void SpriteItemManagerDialog::on_sureButton_clicked()
{
	auto item = layerWidget->currentItem();
	if (item){
		moveSprites();
	}
	this->setVisible(false);
}
void SpriteItemManagerDialog::on_cancelButton_clicked()
{
	this->setVisible(false);
}

void SpriteItemManagerDialog::removeAllItem()
{
	auto count = layerWidget->topLevelItemCount();
	for (int i = 0; i < count; i++){
		auto item = layerWidget->topLevelItem(0);
		delete item;
	}
}

void SpriteItemManagerDialog::addShowLayer(QList<QTreeWidgetItem*> list)
{
	removeAllItem();
	for (int i = 0; i < list.size(); i++){
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0, list.at(i)->text(1));
		layerWidget->addTopLevelItem(item);
	}
}

void SpriteItemManagerDialog::addMoveSprite(QList<QTreeWidgetItem*> list)
{
	spriteList = list;
}

void SpriteItemManagerDialog::moveSprites()
{
	auto newparent = layerWidget->currentItem()->text(0);
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto layerManagerWidget = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	auto newLayer = scene->getLayerFromForeManager(newparent.toStdString());
	std::vector<ImageSprite*> spriteVec;
	std::map<std::string, ImageSpriteLayer*> layerMap;
	for (int i = 0; i < spriteList.size(); i++){
		auto olditem = spriteList.at(i);
		if (!olditem->parent())continue;
		auto parentname = olditem->parent()->text(1);
		if (parentname == newparent)continue;
		auto childname = olditem->text(1);
		auto level = olditem->text(2).toInt();
		auto sprite = scene->getSpriteChildFromForeManager(parentname.toStdString(), childname.toStdString());
		auto layer = scene->getLayerFromForeManager(parentname.toStdString());
		layerMap.insert(std::pair<std::string, ImageSpriteLayer*>(parentname.toStdString(), layer));
		layer->subResRefFromSprite(sprite);
		spriteVec.push_back(sprite);
		layerManagerWidget->addItemSpriteAsyn(newparent, childname, level, sprite);
	}
	newLayer->addOtherLayerCopySprites(spriteVec, false);
	for (int i = 0; i < spriteList.size(); i++){
		auto olditem = spriteList.at(i);
		if (olditem->parent()){
			delete olditem;
		}
	}
	std::map<std::string, ImageSpriteLayer*>::iterator iter;
	for (iter = layerMap.begin(); iter != layerMap.end(); iter++){
		iter->second->removeSpriteOver();
	}
	newLayer->addSpriteOver();
}