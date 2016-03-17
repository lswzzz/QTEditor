#pragma once
#include "QTreeWidget"
#include "HelloWorldScene.h"
#include "resmodifydirdialog.h"

class ItemData {
public:
	ItemData(QTreeWidgetItem* item);
	virtual ~ItemData() {}
	unsigned int getRefCount() { return _refCount; }
	void appendRefCount(int n);
	QTreeWidgetItem* getItem() { return _item; }

protected:
	QTreeWidgetItem* _item;
	int _refCount;
};

class BaseResTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:

	BaseResTreeWidget(QWidget *parent = 0);
	virtual ~BaseResTreeWidget();

	QStringList getPackerResourceList();
	void clearPackerResourceList();
	void removeAllItem(bool removeRealDir);
	bool curResExist(QString res);
	QVector<QTreeWidgetItem*> getChildFolder();
	QVector<QTreeWidgetItem*> getChildRes();
	QVector<QTreeWidgetItem*> getDirChildRes(QTreeWidgetItem* item);
	bool inDirHasSameRes(QString res, QTreeWidgetItem* item);
	bool isTop();
	QTreeWidgetItem* getCurFolderItem();
	void setCurFolder(QString folderOther);
	void deleteResOrDir(QString name, bool isDir);
	QString getResItemRelaPath(QTreeWidgetItem* itemRes);
	QString getResItemRealPath(QTreeWidgetItem* itemRes);
	void initResourceDir();
	QVector<QTreeWidgetItem*> getCurSelectRes();
	void moveResToDir(QVector<QTreeWidgetItem*> vec, QTreeWidgetItem* item);
	static QString getSrcName(QString name);
	void SortItem(QTreeWidgetItem* item);
	int getItemIndex(QTreeWidgetItem* item);
	void setCurRes(int listpngindex);
	virtual void changedResRef(std::string path, int count);

protected:
	void startDrag();
	QString getNewFolderName(QTreeWidgetItem* itemParent);
	void addResItemInCurFolder(QTreeWidgetItem* item);
	void addDirItemInCurFolder(QTreeWidgetItem* item);
	QVector<QTreeWidgetItem*> getChild(bool isDir);
	QString getCurDirPath();
	QString getCurDirRelaPath();
	QString getDirPath(QTreeWidgetItem* itemDir);
	QString getDirRelaPath(QTreeWidgetItem* itemDir);
	bool curDirExist(QString dir);
	void sortList(QTreeWidgetItem* item);
	void removeFromItemMap(QTreeWidgetItem* item);
	void createRealDir(QTreeWidgetItem* item);

	void deleteRes(int index);
	void resetDirName(QString key);
	void resetResName(QString key);
	void resetChildItemMapData(QTreeWidgetItem* item);
	void resetItemMapData(QTreeWidgetItem* item);
	bool deleteDirNoRefItem(QTreeWidgetItem* item);
	virtual int getItemRefCount(QTreeWidgetItem* item);
protected:
	QStringList prePackerList;
	bool check;
	QString oldName;
	QString relativeDir;
	QString relativeListDir;
	Image_Type type__;
	bool hasPrePacker;
	std::map<std::string, ItemData*> _itemMap;
	QString imagePath;
	QString listPath;
	ResModifyDirDialog* dialog;
	std::mutex mutex__;
	QString newDirName;
};

