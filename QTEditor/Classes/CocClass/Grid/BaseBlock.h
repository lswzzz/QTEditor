#pragma once
#include "cocos2d.h"

using namespace cocos2d;

//必须从一个无效的-1开始
const int Type_None = -1;
//创建方格子
const int Type_Box = 0;
//创建斜体
const int Type_Italic = 1;
//创建浮体
const int Type_Fluid = 2;
//创建图片层
const int Type_Foce_Image_Layer = 3;
//移动图片层
const int Type_Move_Foce_Image_Layer = 4;
//删除网格
const int Type_Delete_Block = 5;
//指定方格子的方向
const int Type_Select_Direction = 6;
//删除图片层
const int Type_Delete_Foce_Image_Layer = 7;
//创建子图片
const int Type_Foce_Image_Child = 8;
//移动子图片
const int Type_Move_Foce_Image_Child = 9;
//删除子图片
const int Type_Delete_Foce_Image_Child = 10;
//多选格子
const int Type_Multi_Select_Block = 11;
//多选精灵
const int Type_Multi_Select_Sprite = 12;
//移动多选精灵
const int Type_Move_Multi_Select_Sprite = 13;

const int Italic_Left2Right_Up = 1;
const int Italic_Right2Left_Down = 2;
const int Italic_Left2Right_Down = 3;
const int Italic_Right2Left_Up = 4;

struct drawData{
	cocos2d::Point pos;
	cocos2d::Point start;
	cocos2d::Point end;
	int type;
};

class BaseBlock : public Sprite
{
public:
	
	static BaseBlock* create(float width, float height);
	
	virtual void initData(float width, float height);
	virtual void setTag(int tag);
	virtual int getTag();

	void setType(int type);
	//判断当前是盒子还是斜体还是流体
	int getType();
	void Remove();
	void clearDraw();

	unsigned int* getTargetPo_Index();
	
	unsigned int* getTargetTr_Index();
	
protected:
	BaseBlock();
	virtual ~BaseBlock();
	int m_type;
protected:
	float width;
	float height;
	std::string fileName;
	unsigned int* targetPo_Index;
	unsigned int* targetTr_Index;
	int tag;
};

