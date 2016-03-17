#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class AdsorbentLayer : public Layer
{
public:
	static AdsorbentLayer* create();
	void setGrid(int row, int col, int rowheight, int colwidth);
    void setShowLine(cocos2d::Point pos, cocos2d::Size size);
	void setShowLine(cocos2d::Point pos, Node* node);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onDraw(const Mat4 &transform, uint32_t flags);
	void setIsDraw(bool isdraw);
	bool getIsDraw();
	cocos2d::Point getTargetPos();
private:
	AdsorbentLayer();
	virtual ~AdsorbentLayer();
	CustomCommand _customCommand;
	int row;
	int col;
	int rowheight;
	int colwidth;
    cocos2d::Size trans;
	cocos2d::Point pos1, pos2, pos3, pos4;
    cocos2d::Point targetPos;
	bool isDraw;
};

