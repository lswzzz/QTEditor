#pragma once
#include "cocos2d.h"
#include "AddSprite.h"
#include "GridSpriteLayer.h"
#include "DeleteSprite.h"
#include "SelectDirectionSprite.h"
#include "TouchSprite.h"
#include "BaseBlock.h"
#include "ItalicBody.h"
#include "BoxBody.h"
#include "MultiselectBlock.h"

using namespace cocos2d;

class GridManagerLayer : public Layer
{
public:
	static GridManagerLayer* create();
	virtual bool init();
	
	void setSelectDirectionSpriteStartPos(cocos2d::Point pos);
	void setSelectDirectionSpriteMovedPos(cocos2d::Point pos);
	void setSelectDirectionSpriteEndedPos(cocos2d::Point pos);
	void setAddSpriteStartPos(cocos2d::Point pos);
	void setAddSpriteMovedPos(cocos2d::Point pos);
	void setAddSpriteEndedPos(cocos2d::Point pos);
	void setDeleteSpriteStartPos(cocos2d::Point pos);
	void setDeleteSpriteMovedPos(cocos2d::Point pos);
	void setDeleteSpriteEndedPos(cocos2d::Point pos);
	void setTouchSpriteMovedPos(cocos2d::Point pos);
	void setTouchSpriteEndedPos(cocos2d::Point pos);
	void setSelectDirectionType(int type);
	void setMultiSelectBlockStartPos(cocos2d::Point pos);
	void setMultiSelectBlockMovedPos(cocos2d::Point pos);
	void setMultiSelectBlockEndedPos(cocos2d::Point pos);

	void setStartPos(cocos2d::Point pos);
	void setMovedPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	void createGrid(int row, int col, int height, int width, bool byWinSize = true);
	bool addGrid(int count, int direction);
	void setDrawColor(Color4B b);
	Color4B getDrawColor();
	//1 start 0 end
	cocos2d::Point setItalicPosAndTouchPos(int type);
	int getTouchSpriteType();
	GridSpriteLayer* getGridSpriteLayer();
	DrawNode* getDrawNode();
	void addBlock(BaseBlock* block, int row, int col);
	BaseBlock* getBlock(int row, int col);
	int getBBSize();
	BaseBlock*** getBBvec();

	void clearDraw();
	void pressUp();
	void pressDown();
	void pressRight();
	void pressLeft();
	void releaseUp();
	void releaseDown();
	void releaseLeft();
	void releaseRight();

protected:
	GridManagerLayer();
	virtual ~GridManagerLayer();
	void setGridSpriteLayerStartPos(cocos2d::Point pos);
	void setGridSpriteLayerMovedPos(cocos2d::Point pos);
	void setGridSpriteLayerEndedPos(cocos2d::Point pos);

private:
	AddSprite* addSprite;
	GridSpriteLayer* gridSpriteLayer;
	DrawNode* drawnode;
	DeleteSprite* deleteSprite;
	SelectDirectionSprite* selectDirectionSprite;
	TouchSprite* touchSprite;
	MultiselectBlock* multiselectBlock;
	cocos2d::Point startPos;
	cocos2d::Point movedPos;
	cocos2d::Point endedPos;
	cocos2d::Point ItalicPos;
	bool is_Shift_Key;
	BaseBlock*** BBvec;
	int BBSize;
	int row;
	int col;
	Color4B gridColor;
	friend class HelloWorld;
};

