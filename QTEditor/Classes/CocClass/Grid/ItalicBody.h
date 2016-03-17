#pragma once
#include "BoxBody.h"
#include "FluidBody.h"

class ItalicBody : public BoxBody, public FluidBody
{
public:
	
	static ItalicBody* create(std::string file, float width, float height);
	void addDrawNode(cocos2d::Point pos1, cocos2d::Point pos2, int type);
	cocos2d::Point getStartPos();
	cocos2d::Point getEndPos();
	void setStartPos(cocos2d::Point pos);
	void setEndPos(cocos2d::Point pos);
	void setPos1(cocos2d::Point pos);
	cocos2d::Point getPos1();
	void setPos2(cocos2d::Point pos);
	cocos2d::Point getPos2();
	int getDrawType();
	bool isNeed();
private:
	ItalicBody();
	~ItalicBody();
	DrawNode* drawnode;
	int draw_type;
	cocos2d::Point poss[10];
	//start 和 end是导出的时候使用的
	cocos2d::Point start;
	cocos2d::Point end;
	//这两个是在导出配置文件的时候使用的
	cocos2d::Point pos1, pos2;
	bool need;
};

