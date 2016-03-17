#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class BMFontLayer : public Layer
{
public:
	BMFontLayer(int row, int col);
	virtual bool init();
	void addLabel(int tag, cocos2d::Point pos);
	void delLabel(int tag);
	~BMFontLayer();

private:
	int row;
	int col;
	LabelAtlas*** vec;
};

