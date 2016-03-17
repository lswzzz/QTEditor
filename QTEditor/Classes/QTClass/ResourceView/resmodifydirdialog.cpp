#include "resmodifydirdialog.h"
#include "ui_resmodifydirdialog.h"
#include "FontChina.h"
#include "QGridLayout"
#include "Global.h"
#include "ResourceView.h"
#include "ExteralView.h"
#include "ImageTreeWidget.h"

ResModifyDirDialog::ResModifyDirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResModifyDirDialog)
{
    ui->setupUi(this);
	_setLayout();
	_connect();
}

ResModifyDirDialog::~ResModifyDirDialog()
{
    delete ui;
}

void ResModifyDirDialog::_setLayout()
{
	treeWidget = new ResModifyTreeWidget();
	treeWidget->setHeaderHidden(true);
	sureButton = new QPushButton(FontChina::G2U("确定"));
	cancelButton = new QPushButton(FontChina::G2U("取消"));
	QGridLayout* layout = new QGridLayout;
	layout->addWidget(treeWidget, 0, 0, 1, 2);
	layout->addWidget(sureButton, 1, 0, 1, 1);
	layout->addWidget(cancelButton, 1, 1, 1, 1);
	setLayout(layout);
}

void ResModifyDirDialog::_connect()
{
	connect(sureButton, SIGNAL(clicked()), this, SLOT(on_sureButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
}

void ResModifyDirDialog::isResourceDialog(Image_Type type_)
{
	this->res_Type = type_;
}

void ResModifyDirDialog::showDir()
{
	this->removeAllItem();
	this->show();
	switch (res_Type){
	case Image_Type::NormalBlock:
	{
		auto resourceView = static_cast<ResourceView*>(g_resourceView);
		auto imageTree = resourceView->getTreeWidget();
		int size = imageTree->topLevelItemCount();
		for (int i = 0; i < size; i++){
			auto imageItem = imageTree->topLevelItem(i);
			if (imageItem->data(0, Qt::UserRole).type() == QVariant::Bool){
				auto treeItem = createNewTreeItem(imageItem);
				treeWidget->insertTopLevelItem(treeWidget->topLevelItemCount(), treeItem);
				createChildItem(treeItem, imageItem);
			}
		}
	}
		break;
	case Image_Type::Entity:
	{
		auto exteralView = static_cast<ExteralView*>(g_exteralView);
		auto imageTree = exteralView->getTreeWidget();
		int size = imageTree->topLevelItemCount();
		for (int i = 0; i < size; i++){
			auto imageItem = imageTree->topLevelItem(i);
			if (imageItem->data(0, Qt::UserRole).type() == QVariant::Bool){
				auto treeItem = createNewTreeItem(imageItem);
				treeWidget->insertTopLevelItem(treeWidget->topLevelItemCount(), treeItem);
				createChildItem(treeItem, imageItem);
			}
		}
	}
		break;
	case Image_Type::AnimationBlock:
		break;
	}
}

void ResModifyDirDialog::createChildItem(QTreeWidgetItem* itemParent, QTreeWidgetItem* dirParent)
{
	int size = dirParent->childCount();
	for (int i = 0; i < size; i++){
		auto itemDirChild = dirParent->child(i);
		if (itemDirChild->data(0, Qt::UserRole).type() == QVariant::Bool){
			auto treeItem = createNewTreeItem(itemDirChild);
			itemParent->insertChild(itemParent->childCount(), treeItem);
			createChildItem(treeItem, itemDirChild);
		}
	}
}

QTreeWidgetItem* ResModifyDirDialog::createNewTreeItem(QTreeWidgetItem* itemData)
{
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText(0, itemData->text(0));
	item->setTextColor(0, itemData->textColor(0));
	_map.insert(item, itemData);
	return item;
}

void ResModifyDirDialog::on_sureButton_clicked()
{
	auto item = treeWidget->currentItem();
	switch (res_Type){
	case Image_Type::NormalBlock:
	{
		auto resourceView = static_cast<ResourceView*>(g_resourceView);
		auto imageTree = resourceView->getTreeWidget();
		if (item){
			auto imageItem = _map[item];
			imageTree->moveResToDir(imageTree->getCurSelectRes(), imageItem);
		}
		else{
			imageTree->moveResToDir(imageTree->getCurSelectRes(), nullptr);
		}
	}
		break;
	case Image_Type::Entity:
	{
		auto exteralView = static_cast<ExteralView*>(g_exteralView);
		auto imageTree = exteralView->getTreeWidget();
		if (item){
			auto imageItem = _map[item];
			imageTree->moveResToDir(imageTree->getCurSelectRes(), imageItem);
		}
		else{
			imageTree->moveResToDir(imageTree->getCurSelectRes(), nullptr);
		}
	}
		break;
	case Image_Type::AnimationBlock:
		break;
	}
	this->setVisible(false);
}

void ResModifyDirDialog::on_cancelButton_clicked()
{
	this->setVisible(false);
}

void ResModifyDirDialog::removeAllItem()
{
	auto count = treeWidget->topLevelItemCount();
	for (int i = 0; i < count; i++){
		auto item = treeWidget->topLevelItem(0);
		removeChildItem(item);
		delete item;
	}
	_map.clear();
}

void ResModifyDirDialog::removeChildItem(QTreeWidgetItem* item)
{
	int count = item->childCount();
	for (int i = 0; i < count; i++){
		auto itemChild = item->child(i);
		removeChildItem(itemChild);
		delete itemChild;
	}
}