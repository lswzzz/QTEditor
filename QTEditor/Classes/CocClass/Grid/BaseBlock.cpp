#include "BaseBlock.h"
#include "GridSpriteLayer.h"

BaseBlock::BaseBlock()
{
	m_type = Type_None;
	targetTr_Index = new unsigned int[5];
	targetPo_Index = new unsigned int[7];
	for (int i = 0; i < 5; i++){
		targetTr_Index[i] = -1;
	}
	for (int i = 0; i < 7; i++){
		targetPo_Index[i] = -1;
	}
}

BaseBlock::~BaseBlock()
{
	if (targetTr_Index)delete[] targetTr_Index;
	if (targetPo_Index)delete[] targetPo_Index;
}

BaseBlock* BaseBlock::create(float width, float height)
{
	auto sprite = new BaseBlock();
	if (sprite){
		sprite->init();
		return sprite;
	}
	return NULL;
}

void BaseBlock::initData(float width, float height)
{
	this->width = width;
	this->height = height;
	this->init();
	this->setContentSize(Size(width, height));
}

void BaseBlock::setTag(int tag)
{
	this->tag = tag;
}

int BaseBlock::getTag()
{
	return this->tag;
}

void BaseBlock::setType(int type)
{
	m_type = type;
}

int BaseBlock::getType()
{
	return m_type;
}

void BaseBlock::Remove()
{
	if (this->getParent()){
		GridSpriteLayer* parent = dynamic_cast<GridSpriteLayer*>(this->getParent());
		if (parent){
			parent->RemoveDraw(this, this->getType());
		}
	}
}

void BaseBlock::clearDraw()
{
	if (this->getParent()){
		GridSpriteLayer* parent = dynamic_cast<GridSpriteLayer*>(this->getParent());
		if (parent){
			parent->ClearBlockDraw(this, this->getType());
		}
	}
}

unsigned int* BaseBlock::getTargetPo_Index()
{
	return targetPo_Index;
}

unsigned int* BaseBlock::getTargetTr_Index()
{
	return targetTr_Index;
}
