#pragma once
#include "BaseDockWidget.h"
#include <qpushbutton.h>
#include <qtreewidget.h>
#include <qcombobox.h>
#include "SpriteTreeItem.h"
#include <qmenu.h>
#include "ImageSprite.h"
#include <QThread>
#include <QRunnable>
#include "spriteitemmanagerdialog.h"
#include "selectsprite.h"
#include "LayerManagerTreeWidget.h"

class LayerManagerWidget : public BaseDockWidget
{
	Q_OBJECT
public:
	explicit LayerManagerWidget(char* title, QSize size);
	virtual ~LayerManagerWidget();
private slots:
	void on_addLayer_Button_clicked();
	void on_deletNode_Button_clicked();
	void on_up_Button_clicked();
	void on_down_Button_clicked();
	void on_sortLayerByZorder_clicked();
	void hideNode();
	void showNode();
	void moveSpriteOtherLayer();
	void locatioinSprite();
	void setState();
	void lockLayer();
	void dontOutputLayer();
	void on_lockButton_clicked();
	void on_showButton_clicked();
	void on_findButton_clicked();
	void on_GridButton_clicked();
	void on_SortSpriteByZorder_clicked();
	void on_OutputButton_clicked();

public:
	void updateForeManageLayerStruct();
	void deleteSpriteChildItem(std::string layername, std::string spritename);
	void deleteSpriteLayerItem(std::string layername);
	void SelectedAllUseTemplate(QString templateFile);
	SpriteTreeItem* getCurrentSpriteTreeItem();
	SpriteTreeItem* getSpriteTreeItem(std::string parent, std::string child);
	SpriteTreeItem* getLayerItem(std::string layername);
	std::string getLayerNameByIndex(int index);
	int         getLayerIndex(std::string& layername);

	void add_ImageChild();
	void removeAllItem();

	bool addItemSpriteAsyn(QString parent, QString name, int level, Node* node = nullptr);
	int addItemLayerNoSort(QString layername, int zorder, int index);
	int addItemLayerAndSort(QString layername, int zorder);
	int addItemSpriteByNode(QString layername, Node* sprite, int zorder, bool isMulti = false);
	int addItemSprite(QString layername, QString name, int zorder, bool isMulti = false);
	SpriteTreeItem* addItemDefaultWay(QString layername, QString spritename, int zorder);

	void contextMenuEvent(QContextMenuEvent *event);
	int getItemCount();
	int getCurrentRootIndex();
	int getCurrentRootChildCount();
	//如果当前选中的是root那么返回当前数量child
	int getCurrentRootChildIndex();
	//获取当前item生成的最大值
	int getCurrItemIndex();
	void setCurrItemIndex(int count);

	void setLayerDontOutputStateIcon(std::string parent, bool dontOuput);
	void setLayerStateIcon(QTreeWidgetItem* item, bool state);
	void setLayerLockStateIcon(QTreeWidgetItem* item, bool isLock);
	void setLayerStateIcon(int index, bool state);
	void setLayerLockStateIcon(int index, bool isLock);
	bool setLayerVisibleIcon(int index, bool visible = true);
	bool setLayerVisibleIcon(QTreeWidgetItem* item, bool visible = true);
	bool setChildIcon(int parentindex, int index, std::string filepath);

	std::string getCurrentLayerName();
	std::string getCurrentSpriteName();
	void setCurrentLayer(std::string name);
	void setCurrentSpriteChild(std::string parentname, std::string tagname);
	ImageSprite* getCurrentSprite();
	ImageSprite* getSprite(std::string parentname, std::string tagname);

	void collapseCurrentLayer();
	void expandCurrentLayer();

	void touchLayerIconEvent();
	void touchSpriteIconEvent();

	void setLayerVisible(QTreeWidgetItem* item, bool visible);
	void setLayerLockState(QTreeWidgetItem* item, bool isLock);
	void setLayerState(QTreeWidgetItem* item, bool isSport);

	void swapSpriteItem(SpriteTreeItem* item1, SpriteTreeItem* item2);
	void swapLayerItem(SpriteTreeItem* item1, SpriteTreeItem* item2);
	
protected:
	void _setLayout();
	void _connect();
	
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void keyReleaseEvent(QKeyEvent* e);
	
	//void static setItemIcon2(int index, SpriteTreeItem* item, QString image);
	void static GenerateSpriteItemIcon(SpriteTreeItem* child, ImageSprite* sprite);
	void asyncGenerateSpriteItemIcon(SpriteTreeItem* child, ImageSprite* sprite);
protected:
	QPushButton* addlayer;
	QPushButton* delet;
	QPushButton* up;
	QPushButton* down;
	QPushButton* sortLayer;
	LayerManagerTreeWidget* imageTreeWidget;
	QString imageString;
	QString imageZ_order;
	bool is_Alt_Key;

	QPushButton* lockButton;
	QPushButton* showButton;
	QPushButton* findButton;
	QPushButton* sortSpriteButton;
	QPushButton* outputButton;
	QPushButton* gridButton;
	SpriteItemManagerDialog* dialog;
	SelectSprite* selectDialog;

	static QColor			spriteColor;
	static QColor			layerColor;
	class openFileAddItemThread :public QRunnable{
	public:
		void run(){
			GenerateSpriteItemIcon(child, sprite);
		}
	public:
		SpriteTreeItem* child;
		ImageSprite* sprite;
	};
	friend class HelloWorldScene;
	friend class InvincibleFriendClass;
};

