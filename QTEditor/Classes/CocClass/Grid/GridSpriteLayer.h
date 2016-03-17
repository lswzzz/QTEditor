#pragma once
#include "cocos2d.h"
#include "BoxBody.h"
#include "BaseBlock.h"
#include "BoxBody.h"
#include "ItalicBody.h"
#include "FluidBody.h"

using namespace cocos2d;

class GridSpriteLayer : public DrawNode
{
public:
	static GridSpriteLayer* create(int row, int col, int width, int height);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	bool addBox(BoxBody* box);
	bool drawDirection(BoxBody* box);
	bool addItalic(ItalicBody* italic, cocos2d::Point pos1, cocos2d::Point pos2, bool &need);
	bool addFluid(FluidBody* fluid);
	void RemoveDraw(BaseBlock* block, int type);
	void ClearBlockDraw(BaseBlock* block, int type);
	void drawLine(const cocos2d::Point &origin, const cocos2d::Point &destination, const Color4F &color, int tag);
    void drawSolidPoly(const cocos2d::Point *poli, unsigned int numberOfPoints, const Color4F &color, int tag);
	void drawPolygon(const cocos2d::Point *verts, int count, const Color4F &fillColor, float borderWidth, const Color4F &borderColor, int tag);
	virtual void setDirty(bool dirty);
	int getTag();
	void setBaseBlock(BaseBlock*** vec);
	void setGrid(int row, int col);
	//当所有测试都通过可以添加斜体了返回算法计算的所有点
	int getPoints(cocos2d::Point pos1, cocos2d::Point pos2, drawData* data, int type);
	//添加斜体所用到的检测函数
	void addBoxAndItalic(cocos2d::Point pos1, cocos2d::Point pos2, int& BBSize);
	//计算斜体使用到的方法 用于检测用户滑动的方向
	void CorrectionRange(int &x1, int &y1, int &x2, int &y2, int type, cocos2d::Point pos1, cocos2d::Point pos2);
	//测试添加斜体时两个点的距离
	bool testDirstance(cocos2d::Point pos1, cocos2d::Point pos2);
	
	void setStartPos(cocos2d::Point pos);
	void setEndedPos(cocos2d::Point pos);
	void updateGrid();
	int getMinX();
	int getMinY();
	int getMaxX();
	int getMaxY();
	int getGridCount();
protected:
	GridSpriteLayer();
	virtual ~GridSpriteLayer();
	void ensureCapacityIndex(int count);
	void ensureCapacityGLLineIndex(int count);
	void removeBox(BoxBody* box);
	void removeItalic(ItalicBody* italic);
	void removeFluid(FluidBody* fluid);
protected:
	int inline_tag;
	int row;
	int col;
	int height;
	int width;
	cocos2d::Point startPos;
	cocos2d::Point endedPos;
	int* _buffer_Index;
	int* _bufferLine_Index;
	int _buffer_Index_Capacity;
	int _bufferLine_Index_Capacity;
	BaseBlock*** vec;
	unsigned int targetTr_Index;
	unsigned int targetPo_Index;
	void onDraw(const Mat4 &transform, uint32_t flags);
	void onDrawGLLine(const Mat4 &transform, uint32_t flags);
	void onDrawGLPoint(const Mat4 &transform, uint32_t flags);

	CustomCommand _customCommand;
};

