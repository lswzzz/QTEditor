#pragma once
#include "HelloWorldScene.h"
#include "resmodifydirdialog.h"
#include "BaseResTreeWidget.h"

class ImageTreeWidget : public BaseResTreeWidget
{
	Q_OBJECT
public:
	ImageTreeWidget(QWidget *parent = 0);
	virtual ~ImageTreeWidget();

public slots:
	void itemClick(QTreeWidgetItem* item, int columns);
	void itemChanged(QTreeWidgetItem* item1, int columns);
	void editFileOrDirName(QTreeWidgetItem* item, int columns);
	void deleteCurrentResOrDir();
	void addToPrePackerResource();
	void actionAddRes();
	void actionAddDir();
	void moveResToDirDialog();
	void findResourceLayerQuote();
	QTreeWidgetItem* addRes(QString sourceFile, bool warn = true);
	QTreeWidgetItem* addDir(bool autoname, QString dir = "");
protected:
	
	virtual void init();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);
	void contextMenuEvent(QContextMenuEvent *event);
};

