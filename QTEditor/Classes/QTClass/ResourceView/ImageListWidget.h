#pragma once
#include <qlistwidget.h>
#include <qtreewidget.h>
#include "HelloWorldScene.h"

class ImageListWidget : public QListWidget
{
	Q_OBJECT
public:
	ImageListWidget(QWidget *parent = 0);
	~ImageListWidget();
	//这个删除不能全删要保留下一个
	void removeAllItem();
	void widgetVisible(bool state);
	virtual QTreeWidget* getTreeWidget();
	virtual void update();
protected slots:
	void deleteResOrDir();
	virtual void itemClicked(QListWidgetItem * item);
	void itemDoubleClicked(QListWidgetItem* item);
	void actionAddRes();
	void actionAddDir();
protected:
	void copyFolderFromTreeItem(QListWidgetItem* listItem, QTreeWidgetItem* treeItem);
	void copyResFromTreeItem(QListWidgetItem* listItem, QTreeWidgetItem* treeItem);
	void FlushFromTreeWidget();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);
	void contextMenuEvent(QContextMenuEvent *event);
protected:
	void startDrag();
	Image_Type type__;
};

