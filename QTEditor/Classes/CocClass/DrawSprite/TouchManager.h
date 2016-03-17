#pragma once
#include "cocos2d.h"

using namespace cocos2d;
class TouchManager : public Layer
{
public:
	
	static TouchManager* create();
	virtual bool init();
	void addTouchEvent();
	void addMouseEvent();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void startTouchManage();
	void endTouchManage();

	void onMouseScroll(Event* event);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	int getEdgeDirection();
	void sceneMoveToScenePos(cocos2d::Point pos);

private:
	TouchManager();
	virtual ~TouchManager();
	CustomCommand _customCommand;
private:
	bool isDrawEdge;
	cocos2d::Point startPos;
	cocos2d::Point endedPos;
	cocos2d::Point movedPos;
	cocos2d::Point scenePosition;
	int direction;
	bool manage;
};

