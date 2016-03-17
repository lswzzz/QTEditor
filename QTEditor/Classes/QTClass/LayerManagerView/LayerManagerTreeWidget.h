#pragma once
#include "QTreeWidget.h"
#include "QMouseEvent"
#include "QHeaderView"
#include "QItemDelegate.h"
#include "ImageSpriteLayer.h"
#include "SpriteTreeItem.h"

class LayerManagerItemDelegate : public QItemDelegate {
	Q_OBJECT
public:
	LayerManagerItemDelegate() {
	}
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
		QStyleOptionViewItem  viewOption(option);
		viewOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::ForegroundRole).value<QColor>());
		viewOption.palette.setColor(QPalette::Highlight, QColor(230, 230, 230, 255));
		QItemDelegate::paint(painter, viewOption, index);
	}
};

class LayerManagerTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	LayerManagerTreeWidget(QWidget *parent = 0);
	~LayerManagerTreeWidget();
	
private slots:
	void					itemClick(QTreeWidgetItem* item1, int columns);
	void					itemChanged(QTreeWidgetItem* item1, int columns);
	void					RenameNodeName(QTreeWidgetItem* item1, int columns);
public:
	struct inlinedata{
		int index;
		int z_order;
		QString name;
		SpriteTreeItem* item;
	};

	void					mousePressEvent(QMouseEvent *event);
	void					mouseMoveEvent(QMouseEvent *event);
	void					mouseReleaseEvent(QMouseEvent* event);
	
	bool					inCurrentIconRect(QPoint pos);

	ImageSpriteLayer*		getItemMappingLayer(QTreeWidgetItem* item);
	ImageSpriteLayer*		resetSceneCurrentLayerAndGetReturn(QTreeWidgetItem* item);

	void					setShowEdgeLayer(QTreeWidgetItem* item);
	void					setShowEdgeSprite(QTreeWidgetItem* itemParent, QTreeWidgetItem* itemChild);

	void					clearControlView();
	void					setValueToControlViewLayer(QTreeWidgetItem* item);
	void					setValueToControlViewSprite(QTreeWidgetItem* itemSprite);

	void					resetSpriteTagName(QTreeWidgetItem* itemSprite);
	void					resetLayerTagName(QTreeWidgetItem* itemLayer);
	bool					checkSpriteRepeated(QTreeWidgetItem* itemSprite);
	bool					checkLayerRepeated(QTreeWidgetItem* itemLayer);
	bool					checkItemZOrder(QTreeWidgetItem* item);

	QString					getNewDefaultLayerName();
	int						getNewDefaultLayerZOrder();

	bool					deleteSpriteItem(std::string layername, std::string spritename);
	bool					deleteLayerItem(std::string layername);

	SpriteTreeItem*			createNewLayerItem(QString layername, int zorder, int index);
	SpriteTreeItem*			createNewSpriteItem(QString name, int ozrder, int index);

	SpriteTreeItem*			getSpriteTreeItem(std::string parent, std::string child);
	SpriteTreeItem*			getLayerItem(std::string layername);

	void					removeAllItem();

	void					sortParentItem();
	void					sortSceneLayerByLayerItem();
	void					sortChildItem(QTreeWidgetItem* itemparent);
	void					sortSceneSpriteByChildItem(QTreeWidgetItem* itemparent);
	void					sortalgorithm(inlinedata* data, int size);
	void					sortImageLayerIndex();
	bool touchIcon;
	int touchcol;
	bool check;
	QString imageString;
	QString imageZ_order;
	QString imageIndex;
};

