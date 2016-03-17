#pragma once
#include <qwidget.h>
#include <qtreewidget.h>

class SpriteTreeItem : public QTreeWidgetItem
{
public:
	SpriteTreeItem();
	~SpriteTreeItem();
	void setTag(int tag);
	int getTag();
	//保存sprite的tag
	void setSpriteTag(int tag);
	int getSpriteTag();
	static int getCurrTag();
	static void setCurrTagCount(int count);
private:
	int __tag__;
	int spriteTag;
	static int currTag;
};

