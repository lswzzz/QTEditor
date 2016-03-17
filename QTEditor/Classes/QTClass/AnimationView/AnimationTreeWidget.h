#pragma once
#include "BaseResTreeWidget.h"

class AnimationTreeWidget : public BaseResTreeWidget
{
	Q_OBJECT
public:
	
	AnimationTreeWidget(QWidget *parent = 0);
	virtual ~AnimationTreeWidget();
	QVector<QString> getSinpleFrameNameListQString(QString animationName);
	std::vector<int> getSinpleFrameNameList(QString animationName);
	QVector<QString> getSinpleFrameNameListQString(QTreeWidgetItem* item);
	std::vector<int> getSinpleFrameNameList(QTreeWidgetItem* item);
	QVector<QString> getCurrentSinpleFrameNameListQString();
	std::vector<int> getCurrentSinpleFrameList();
	QString getCurrentAnimationName();
	QString getAnimationName(QTreeWidgetItem* item);
	void changedResRef(std::string name, int count);
	bool curAnimationExist(QString animationname);
	bool curAnimationExist(QString animationname, QTreeWidgetItem* compareItem);
	bool curSpriteFrameExist(QString framename);
	QTreeWidgetItem* addSingleFrame(QString sourceFile, bool warn = true);
	QTreeWidgetItem* addFrameDir(bool autoname, QString dir = "");
	QTreeWidgetItem* getAnimationItem(QString animationname);
	QTreeWidgetItem* getAnimationItemFromSubItem(QString animationname, QTreeWidgetItem* item);
private slots:
	void itemClick(QTreeWidgetItem* item, int columns);
	void itemChanged(QTreeWidgetItem* item1, int columns);
	void editFileOrDirName(QTreeWidgetItem* item, int columns);
	void deleteCurrentResOrDir();
	void actionAddSingleFrame();
	void actionAddFrameDir();
	//void moveResToDirDialog();
	void findFrameLayerQuote();
protected:
	virtual void init();
protected:
	QString getNewAnimationName();
	bool curAnimationNoRefItem(QTreeWidgetItem* item);
	bool curAnimationExistInSubItem(QTreeWidgetItem* item, QString animationname, QTreeWidgetItem* compareItem);
	int getAnimationItemRefCount(QTreeWidgetItem* item);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);
	void contextMenuEvent(QContextMenuEvent *event);
};

