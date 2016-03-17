#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class SelectDirectionSprite : public Sprite
{
public:
	static SelectDirectionSprite* create();
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	std::vector<cocos2d::Point> getRange(int gridW, int gridH);
	void setStartPos(cocos2d::Point pos);
	void setMovedPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	//0上 1下 2左 3右 4 无
	void setType(int type);
	int getType();
private:
	SelectDirectionSprite();
	virtual ~SelectDirectionSprite();
	CustomCommand _customCommand;
	bool drawRect;
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	cocos2d::Point endedPos;
	int m_type;
};

