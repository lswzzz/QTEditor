#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class AddSprite : public Sprite
{
public:
	static AddSprite* create();

	void setStartPos(cocos2d::Point pos);
	void setMovedPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	std::vector<cocos2d::Point> getRange(int gridW, int gridH);
private:
	AddSprite();
	virtual ~AddSprite();
	CustomCommand _customCommand;
	bool drawRect;
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	cocos2d::Point endedPos;
};

