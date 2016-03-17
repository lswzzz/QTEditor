#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class DeleteSprite : public Sprite
{
public:
	
	static DeleteSprite* create();

	void setStartPos(cocos2d::Point pos);
	void setMovedPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	std::vector<cocos2d::Point> getRange(int gridW, int gridH);
	//type 1 表示删除网格 type 2 表示删除图片
	void setType(int type);
	int getType();
private:
	DeleteSprite();
	virtual ~DeleteSprite();
	CustomCommand _customCommand;
	bool drawRect;
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	cocos2d::Point endedPos;
	int m_type;
};

