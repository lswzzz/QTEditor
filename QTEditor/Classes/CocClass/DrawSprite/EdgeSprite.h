#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class EdgeSprite : public Sprite
{
public:
	static EdgeSprite* create();
	virtual bool init();
	void remove(float dt);
	void clearDraw();
    void startDraw(cocos2d::Point pos, cocos2d::Size size);
	void endDraw();
	void move(cocos2d::Point pos);
	void startPos(cocos2d::Point pos);
private:
	EdgeSprite();
	virtual ~EdgeSprite();
	cocos2d::Size size;
	cocos2d::Point start, moved;
	DrawNode* drawnode;
	bool isDraw;
};

