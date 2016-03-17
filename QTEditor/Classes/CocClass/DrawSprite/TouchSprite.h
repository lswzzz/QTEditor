#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class TouchSprite : public Sprite
{
public:
	
	static TouchSprite* create();
	void setStartPos(cocos2d::Point pos);
	void setMovedPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	//设置当前的状态是否被吸附 状态值有1 吸附和 0 不吸附
	void setType(int type);
	//当前的状态是否被吸附 状态值有1 吸附和 0 不吸附
	int getType();
private:
	TouchSprite();
	virtual ~TouchSprite();
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	cocos2d::Point endedPos;
	CustomCommand _customCommand;
	bool drawLine;
	int m_type;
};

