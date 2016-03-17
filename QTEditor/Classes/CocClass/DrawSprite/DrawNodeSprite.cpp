#include "DrawNodeSprite.h"
#include "ImageSprite.h"

DrawNodeSprite::DrawNodeSprite()
{
	drawNode = NULL;
}

DrawNodeSprite* DrawNodeSprite::create(Point pos1, Point pos2, float time, bool remove)
{
	DrawNodeSprite* ret = new DrawNodeSprite();
	if (ret && ret->init(pos1, pos2)){
		if (remove){
			ret->runSchedule(time);
		}
		return ret;
	}
	return NULL;
}

DrawNodeSprite* DrawNodeSprite::create(Rect rect, float time, bool remove)
{
	DrawNodeSprite* ret = new DrawNodeSprite();
	if (ret && ret->init(rect)){
		if (remove){
			ret->runSchedule(time);
		}
		return ret;
	}
	return NULL;
}

DrawNodeSprite* DrawNodeSprite::create(int width, int height)
{
	DrawNodeSprite* ret = new DrawNodeSprite();
	if (ret && ret->init(Size(width, height))){
		return ret;
	}
	return NULL;
}

bool DrawNodeSprite::init(Size size)
{
	if (!Sprite::init()){
		return false;
	}
	drawNode = DrawNode::create();
	addChild(drawNode);
	drawNode->drawRect(Point(-size.width / 2, -size.height / 2), Point(size.width / 2, size.height / 2), Color4F(Color4B(0xFF, 0xD7, 0x00, 0xFF)));
	return true;
}

bool DrawNodeSprite::init(Point pos1, Point pos2)
{
	if (!Sprite::init()){
		return false;
	}
	drawNode = DrawNode::create();
	addChild(drawNode);
	drawNode->drawLine(pos1, pos2, Color4F(0, 1.0, 0, 1.0));
	return true;
}

bool DrawNodeSprite::init(Rect rect)
{
	if (!Sprite::init()){
		return false;
	}
	drawNode = DrawNode::create();
	addChild(drawNode);
	Point pos1, pos2;
	pos1 = Point(rect.getMinX(), rect.getMinY());
	pos2 = Point(rect.getMaxX(), rect.getMaxY());
	drawNode->drawSolidRect(pos1, pos2, Color4F(0, 1.0, 0, 1.0));
	return true;
}

void DrawNodeSprite::runSchedule(float time)
{
	scheduleOnce(schedule_selector(DrawNodeSprite::remove), time);
}

void DrawNodeSprite::remove(float dt)
{
	this->removeFromParent();
	this->release();
}

void DrawNodeSprite::Delete()
{
	this->removeFromParent();
	this->release();
}

DrawNodeSprite::~DrawNodeSprite()
{
	if (drawNode){
		drawNode->removeFromParent();
	}
}
