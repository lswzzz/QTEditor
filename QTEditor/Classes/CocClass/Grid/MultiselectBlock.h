#pragma once
#include "cocos2d.h"
#include "BaseBlock.h"

using namespace cocos2d;

class MultiselectBlock : public Sprite
{
public:
	static MultiselectBlock* create();
	virtual bool init();
	void setStartPos(cocos2d::Point pos);
	void setMovedPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	void setBBvec(BaseBlock*** vec, int row, int col, int size, int gridW, int gridH);
	bool ** getSelectVec(){ return selectVec; };
	void clearDrawNode();
	void up();
	void down();
	void left();
	void right();

protected:
	MultiselectBlock();
	virtual ~MultiselectBlock();
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	std::vector<cocos2d::Point> getRange(int gridW, int gridH);
	void addToVec(cocos2d::Point loc);
	void deleteSelectVec();
	void createSelectVec();
	void addDrawNode();
	void clearDraw();
	void resetVec(int direct);
	void resetDrawUp();
	void resetDrawDown();
	void resetDrawLeft();
	void resetDrawRight();
	bool checkMoveUp();
	bool checkMoveDown();
	bool checkMoveLeft();
	bool checkMoveRight();
private:
	bool ** selectVec;
	CustomCommand _customCommand;
	DrawNode* drawnode;
	BaseBlock*** BBvec;
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	cocos2d::Point endedPos;
	int gridW;
	int gridH;
	int row;
	int col;
	//int size;
	bool drawRect;
	int cur_static;
};

