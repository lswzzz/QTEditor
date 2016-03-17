#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class DrawNodeSprite : public Sprite
{
public:
	
	static DrawNodeSprite* create(cocos2d::Point pos1, cocos2d::Point pos2, float time, bool remove = false);
	static DrawNodeSprite* create(cocos2d::Rect rect, float time, bool remove = false);
	static DrawNodeSprite* create(int width, int height);
	virtual bool init(cocos2d::Point pos1, cocos2d::Point pos2);
	virtual bool init(cocos2d::Rect rect);
	virtual bool init(cocos2d::Size size);

	void Delete();
	
protected:
	DrawNodeSprite();
	virtual ~DrawNodeSprite();
	void runSchedule(float time);
	void remove(float dt);
protected:
	DrawNode* drawNode;
};

