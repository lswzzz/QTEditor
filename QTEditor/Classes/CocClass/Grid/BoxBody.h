#pragma once
#include "BaseBlock.h"

//这个跟UI的ComboBox对应不要修改
typedef enum _One_Way{
	NONE = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4,
}One_Way;

class BoxBody : virtual public BaseBlock
{
public:
	
	static BoxBody* create(std::string file, float width, float height);
	void DrawGrid();
	void DrawDirection();
	void setWay(One_Way way);
	One_Way getWay();
protected:
	BoxBody();
	virtual ~BoxBody();
	DrawNode* drawnode;
	One_Way m_way;
};

