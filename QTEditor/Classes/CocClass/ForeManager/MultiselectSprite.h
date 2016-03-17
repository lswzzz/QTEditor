#pragma once
#include "cocos2d.h"
#include "ImageSprite.h"

using namespace cocos2d;

class MultiselectSprite : public Sprite
{
public:
	static MultiselectSprite* create();
	virtual bool init();
	void setStartPos(cocos2d::Point pos);
	void setMovedPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	
	cocos2d::Rect getRect();
	bool MultiSelect(Vector<Node*> vec);
	void clearDrawNode();
	void moveSize(cocos2d::Point size);
	Vector<Node*> getVec();
	void setVec(Vector<Node*> vec);
	bool addSimpleNode(ImageSprite* sprite);
	bool isTouchMultiSelectSprite(cocos2d::Point pos);
protected:
	MultiselectSprite();
	virtual ~MultiselectSprite();
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	void addDrawNode();
	
private:
	Vector<Node*> _vec;
	CustomCommand _customCommand;
	DrawNode* drawnode;
	bool drawRect;
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	cocos2d::Point endedPos;
	int cur_static;
	bool isSimple;
};

