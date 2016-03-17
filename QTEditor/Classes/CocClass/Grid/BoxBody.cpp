#include "BoxBody.h"
#include "Global.h"
#include "GridSpriteLayer.h"
BoxBody::BoxBody()
{
	drawnode = NULL;
	m_way = NONE;
}

BoxBody::~BoxBody()
{
	if (drawnode){
		drawnode->removeFromParent();
	}
}

BoxBody* BoxBody::create(std::string file, float width, float height)
{
	auto sprite = new BoxBody();
	if (sprite){
		sprite->initData(width, height);
		return sprite;
	}
	else{
		return NULL;
	}
}

void BoxBody::DrawDirection()
{
	GridSpriteLayer* parent = dynamic_cast<GridSpriteLayer*>(this->getParent());
	parent->drawDirection(this);
}

void BoxBody::DrawGrid()
{
	GridSpriteLayer* parent = dynamic_cast<GridSpriteLayer*>(this->getParent());
	parent->addBox(this);
}

void BoxBody::setWay(One_Way way)
{
	m_way = way;
}

One_Way BoxBody::getWay()
{
	return m_way;
}