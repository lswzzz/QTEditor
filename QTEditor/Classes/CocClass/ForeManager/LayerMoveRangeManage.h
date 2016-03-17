#pragma once
#include "cocos2d.h"

USING_NS_CC;

class LayerMoveRangeManage : public Node
{
public:
	struct DrawData{
		DrawNode* drawnode;
		int col1;
		int row1;
		int col2;
		int row2;
		bool isDraw;
	};

	int getGridWidth(){ return gridWidth; };
	int getGridHeight(){ return gridHeight; };
	static LayerMoveRangeManage* create();
	void setGridData(int gridWidth, int gridHeight);
	void addGrid(int count, int direction);
	Vec2 gridPosChangeToGLPosStart(Vec2 pos, bool isOpen);
	Vec2 gridPosChangeToGLPosEnd(Vec2 pos, bool isOpen);
	//gridPos
	void resetDrawData(std::string tagname, std::string newtagname);
	void addDrawData(std::string tagname);
	void deleteDrawRect(std::string tagname);
	void setDrawDataRect(std::string tagname, Vec2 startPos, Vec2 endedPos, bool isOpen = false);
	void setDrawDataRect(DrawData* data);
	Vec2 getLayerStartPos(std::string tagname);
	Vec2 getLayerEndedPos(std::string tagname);
	DrawNode* getLayerDrawNode(std::string tagname);
	DrawData* getLayerData(std::string tagname);
private:
	std::map<std::string, DrawData*> drawMap_;
	int gridWidth;
	int gridHeight;
};

