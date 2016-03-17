#include "EdgeSprite.h"

EdgeSprite::EdgeSprite()
{
	init();
	isDraw = false;
}

EdgeSprite* EdgeSprite::create()
{
	auto ret = new EdgeSprite();
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

bool EdgeSprite::init()
{
	Sprite::init();
	drawnode = DrawNode::create();
	this->addChild(drawnode);
	return true;
}

void EdgeSprite::remove(float dt)
{
	this->removeFromParent();
	this->release();
}

EdgeSprite::~EdgeSprite()
{
	if (drawnode){
		drawnode->removeFromParent();
	}
}

void EdgeSprite::startDraw(Point pos, Size size)
{
	drawnode->clear();
	this->setPosition(pos);
	this->size = size;
	isDraw = true;
	drawnode->drawRect(Point(0, 0), Point(size.width, size.height), Color4F(Color4B(0x00, 0xFF, 0x00, 0xff)));//Color4F(Color4B(0x48, 0x3d, 0x8b, 0xff)));
}

void EdgeSprite::endDraw()
{
	isDraw = false;
	drawnode->clear();
}

void EdgeSprite::move(Point pos)
{
	moved = pos;
	this->setPosition(this->getPosition() + moved-start);
	start = moved;
}

void EdgeSprite::startPos(Point pos)
{
	start = pos;
}

void EdgeSprite::clearDraw()
{
	if (drawnode)drawnode->clear();
}