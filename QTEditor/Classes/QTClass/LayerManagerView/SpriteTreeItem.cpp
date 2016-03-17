#include "SpriteTreeItem.h"

int SpriteTreeItem::currTag = 0;
SpriteTreeItem::SpriteTreeItem()
:QTreeWidgetItem()
{
	__tag__ = currTag;
	currTag++;
}


SpriteTreeItem::~SpriteTreeItem()
{
}

void SpriteTreeItem::setTag(int tag)
{
	__tag__ = tag;
}

int SpriteTreeItem::getTag()
{
	return __tag__;
}

void SpriteTreeItem::setSpriteTag(int tag)
{
	spriteTag = tag;
}

int SpriteTreeItem::getSpriteTag()
{
	return spriteTag;
}

int SpriteTreeItem::getCurrTag()
{
	return currTag;
}

void SpriteTreeItem::setCurrTagCount(int count)
{
	currTag = count;
}