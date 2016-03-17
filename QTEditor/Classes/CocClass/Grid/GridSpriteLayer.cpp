#include "GridSpriteLayer.h"
#include "Global.h"

static Vec2 v2fzero(0.0f, 0.0f);
static inline Vec2 v2f(float x, float y)
{
	Vec2 ret(x, y);
	return ret;
}
static inline Vec2 v2fadd(const Vec2 &v0, const Vec2 &v1)
{
	return v2f(v0.x + v1.x, v0.y + v1.y);
}
static inline Vec2 v2fsub(const Vec2 &v0, const Vec2 &v1)
{
	return v2f(v0.x - v1.x, v0.y - v1.y);
}
static inline Vec2 v2fmult(const Vec2 &v, float s)
{
	return v2f(v.x * s, v.y * s);
}
static inline Vec2 v2fperp(const Vec2 &p0)
{
	return v2f(-p0.y, p0.x);
}
static inline Vec2 v2fneg(const Vec2 &p0)
{
	return v2f(-p0.x, -p0.y);
}
static inline float v2fdot(const Vec2 &p0, const Vec2 &p1)
{
	return  p0.x * p1.x + p0.y * p1.y;
}
static inline Vec2 v2fforangle(float _a_)
{
	return v2f(cosf(_a_), sinf(_a_));
}
static inline Vec2 v2fnormalize(const Vec2 &p)
{
	Vec2 r = Vec2(p.x, p.y).getNormalized();
	return v2f(r.x, r.y);
}
static inline Vec2 __v2f(const Vec2 &v)
{
	//#ifdef __LP64__
	return v2f(v.x, v.y);
	// #else
	//     return * ((Vec2*) &v);
	// #endif
}

static inline Tex2F __t(const Vec2 &v)
{
	return *(Tex2F*)&v;
}

static int grid_W;
static int grid_H;

GridSpriteLayer::GridSpriteLayer()
{
	inline_tag = 12345;
	vec = NULL;
	_buffer_Index = NULL;
	_bufferLine_Index = NULL;
	_buffer_Index_Capacity = 0;
	_bufferLine_Index_Capacity = 0;
}

GridSpriteLayer::~GridSpriteLayer()
{
	if (_buffer_Index)free(_buffer_Index);
	if (_bufferLine_Index)free(_bufferLine_Index);
}

GridSpriteLayer* GridSpriteLayer::create(int row, int col, int width, int height)
{
	GridSpriteLayer* ret = new GridSpriteLayer();
	if (ret && ret->init()){
		ret->ensureCapacityIndex(180);
		ret->ensureCapacityGLLineIndex(90);
		ret->row = row;
		ret->col = col;
		ret->width = width;
		ret->height = height;
		grid_W = width;
		grid_H = height;
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return NULL;
}

bool GridSpriteLayer::addBox(BoxBody* box)
{
	Point position = box->getPosition();
	Point pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y - box->getContentSize().height / 2 + 2);
	Point pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y - box->getContentSize().height / 2 + 2);
	Point pos3 = Point(position.x + box->getContentSize().width / 2 - 2, position.y + box->getContentSize().height / 2 - 2);
	Point pos4 = Point(position.x - box->getContentSize().width / 2 + 2, position.y + box->getContentSize().height / 2 - 2);
	Point poss[4] = { pos1, pos2, pos3, pos4 };
	this->drawSolidPoly(poss, 4, Color4F(0.5f, 0.5f, 0.5f, 0.5f), box->getTag());
	//在这里创建在BaseBlock销毁的时候删除
	unsigned int* indexs_Tr = new unsigned int[2];
	unsigned int* indexs_Po = new unsigned int[1];
	switch (box->getWay()){
	case UP:
	{
			   pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y + box->getContentSize().height / 2 - 2);
			   pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y + box->getContentSize().height / 2 - 2);
			   this->drawLine(pos1, pos2, Color4F(0.0f, 1.0f, 0.0f, 1.0f), box->getTag());
	}
		break;
	case DOWN:
	{
				 pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y - box->getContentSize().height / 2 + 2);
				 pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y - box->getContentSize().height / 2 + 2);
				 this->drawLine(pos1, pos2, Color4F(0.0f, 1.0f, 0.0f, 1.0f), box->getTag());
	}
		break;
	case LEFT:
	{
				 pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y - box->getContentSize().height / 2 + 2);
				 pos2 = Point(position.x - box->getContentSize().width / 2 + 2, position.y + box->getContentSize().height / 2 - 2);
				 this->drawLine(pos1, pos2, Color4F(0.0f, 1.0f, 0.0f, 1.0f), box->getTag());
	}
		break;
	case RIGHT:
	{
				  pos1 = Point(position.x + box->getContentSize().width / 2 - 2, position.y - box->getContentSize().height / 2 + 2);
				  pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y + box->getContentSize().height / 2 - 2);
				  this->drawLine(pos1, pos2, Color4F(0.0f, 1.0f, 0.0f, 1.0f), box->getTag());
	}
		break;
	case NONE:
	{
				 pos1 = Point(0, 0);
				 pos2 = Point(0, 0);
				 this->drawLine(pos1, pos2, Color4F(0.5f, 0.5f, 0.5f, 0.5f), box->getTag());
				 /* pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y + box->getContentSize().height / 2 - 2);
				  pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y + box->getContentSize().height / 2 - 2);
				  this->drawLine(pos1, pos2, Color4F(0.0f, 1.0f, 0.0f, 1.0f), box->getTag());*/
				 break;
	}
		break;
	}
	indexs_Tr[0] = targetTr_Index;
	indexs_Tr[1] = targetTr_Index + 3;
	indexs_Po[0] = targetPo_Index;
	unsigned int* targetTr_Index = box->getTargetTr_Index();
	unsigned int* targetPo_Index = box->getTargetPo_Index();
	for (int i = 0; i < 2; i++){
		targetTr_Index[i] = indexs_Tr[i];
	}
	targetPo_Index[0] = indexs_Po[0];
	delete []indexs_Tr;
	delete []indexs_Po;
	return true;
}

bool GridSpriteLayer::addItalic(ItalicBody* italic, Point pos1, Point pos2, bool &need)
{
	//添加斜体
	Point position = italic->getPosition();
	Point origin = Point(position.x - italic->getContentSize().width / 2, position.y - italic->getContentSize().height / 2);
	Point tarPos = Point(position.x + italic->getContentSize().width / 2, position.y + italic->getContentSize().height / 2);
	Size italic_size = Size(italic->getContentSize().width, italic->getContentSize().height);

	//1添加背景
	Point pos_1 = Point(position.x - italic->getContentSize().width / 2 + 2, position.y - italic->getContentSize().height / 2 + 2);
	Point pos_2 = Point(position.x + italic->getContentSize().width / 2 - 2, position.y - italic->getContentSize().height / 2 + 2);
	Point pos_3 = Point(position.x + italic->getContentSize().width / 2 - 2, position.y + italic->getContentSize().height / 2 - 2);
	Point pos_4 = Point(position.x - italic->getContentSize().width / 2 + 2, position.y + italic->getContentSize().height / 2 - 2);
	Point posbg[4] = { pos_1, pos_2, pos_3, pos_4 };
	this->drawSolidPoly(posbg, 4, Color4F(0.5f, 0.5f, 0.5f, 0.5f), italic->getTag());
	
	unsigned int* indexs_Tr = new unsigned int[5];
	unsigned int* indexs_Po = NULL;
	indexs_Tr[0] = targetTr_Index;
	indexs_Tr[1] = targetTr_Index + 3;
	Point* poss = new Point[7];
	poss[0].x = origin.x + pos1.x * italic->getContentSize().width;
	poss[0].y = origin.y + italic_size.height - pos1.y * italic_size.height;
	poss[1].x = origin.x + pos2.x * italic->getContentSize().width;
	poss[1].y = origin.y + italic_size.height - pos2.y * italic_size.height;
	need = false;
	italic->setPos1(pos1);
	italic->setPos2(pos2);
	if (pos1.distance(pos2) < 0.4f){
		need = true;
		indexs_Po = new unsigned int[7];
	}
	if (need){
		switch (italic->getDrawType()){
		case Italic_Left2Right_Down:
			poss[2].y = origin.y + 2;
			poss[2].x = poss[1].x;
			poss[3].x = tarPos.x - 2;
			poss[3].y = origin.y + 2;
			poss[4].y = tarPos.y - 2;
			poss[4].x = tarPos.x - 2;
			poss[5].x = origin.x + 2;
			poss[5].y = tarPos.y - 2;
			poss[6].x = origin.x + 2;
			poss[6].y = poss[0].y;
			this->drawLine(poss[0], poss[1], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[0] = targetPo_Index;
			this->drawLine(poss[1], poss[2], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[1] = targetPo_Index;
			this->drawLine(poss[2], poss[3], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[2] = targetPo_Index;
			this->drawLine(poss[3], poss[4], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[3] = targetPo_Index;
			this->drawLine(poss[4], poss[5], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[4] = targetPo_Index;
			this->drawLine(poss[5], poss[6], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[5] = targetPo_Index;
			this->drawLine(poss[6], poss[0], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[6] = targetPo_Index;
			break;
		case Italic_Right2Left_Down:
			poss[2].y = origin.y + 2;
			poss[2].x = poss[1].x;
			poss[3].x = origin.x + 2;
			poss[3].y = origin.y + 2;
			poss[4].x = origin.x + 2;
			poss[4].y = tarPos.y - 2;
			poss[5].x = tarPos.x - 2;
			poss[5].y = tarPos.y - 2;
			poss[6].x = tarPos.x - 2;
			poss[6].y = poss[0].y;
			this->drawLine(poss[0], poss[1], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[0] = targetPo_Index;
			this->drawLine(poss[1], poss[2], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[1] = targetPo_Index;
			this->drawLine(poss[2], poss[3], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[2] = targetPo_Index;
			this->drawLine(poss[3], poss[4], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[3] = targetPo_Index;
			this->drawLine(poss[4], poss[5], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[4] = targetPo_Index;
			this->drawLine(poss[5], poss[6], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[5] = targetPo_Index;
			this->drawLine(poss[6], poss[0], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[6] = targetPo_Index;
			
			break;
		case Italic_Right2Left_Up:
			poss[2].y = poss[1].y;
			poss[2].x = tarPos.x - 2;
			poss[3].y = origin.y + 2;
			poss[3].x = tarPos.x - 2;
			poss[4].x = origin.x + 2;
			poss[4].y = origin.y + 2;
			poss[5].x = origin.x + 2;
			poss[5].y = tarPos.y - 2;
			poss[6].x = poss[0].x;
			poss[6].y = tarPos.y - 2;
			this->drawLine(poss[0], poss[1], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[0] = targetPo_Index;
			this->drawLine(poss[1], poss[2], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[1] = targetPo_Index;
			this->drawLine(poss[2], poss[3], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[2] = targetPo_Index;
			this->drawLine(poss[3], poss[4], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[3] = targetPo_Index;
			this->drawLine(poss[4], poss[5], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[4] = targetPo_Index;
			this->drawLine(poss[5], poss[6], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[5] = targetPo_Index;
			this->drawLine(poss[6], poss[0], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[6] = targetPo_Index;
			
			break;
		case Italic_Left2Right_Up:
			poss[2].y = poss[1].y;
			poss[2].x = origin.x + 2;
			poss[3].x = origin.x + 2;
			poss[3].y = origin.y + 2;
			poss[4].x = tarPos.x - 2;
			poss[4].y = origin.y + 2;
			poss[5].x = tarPos.x - 2;
			poss[5].y = tarPos.y - 2;
			poss[6].x = poss[0].x;
			poss[6].y = tarPos.y - 2;
			this->drawLine(poss[0], poss[1], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[0] = targetPo_Index;
			this->drawLine(poss[1], poss[2], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[1] = targetPo_Index;
			this->drawLine(poss[2], poss[3], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[2] = targetPo_Index;
			this->drawLine(poss[3], poss[4], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[3] = targetPo_Index;
			this->drawLine(poss[4], poss[5], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[4] = targetPo_Index;
			this->drawLine(poss[5], poss[6], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[5] = targetPo_Index;
			this->drawLine(poss[6], poss[0], Color4F(1.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
			indexs_Po[6] = targetPo_Index;
			
		default:
			break;
		}
	}
	switch (italic->getDrawType()){
	case Italic_Left2Right_Down:
		italic->setStartPos(pos1);
		italic->setEndPos(pos2);
		poss[2].y = origin.y;
		poss[2].x = poss[1].x;
		poss[3].x = origin.x;
		poss[3].y = origin.y;
		poss[4].y = poss[0].y;
		poss[4].x = origin.x;
		if (need)
			this->drawSolidPoly(poss, 5, Color4F(0.8f, 0.7f, 1.0f, 0.5f), italic->getTag());
		else
			this->drawSolidPoly(poss, 5, Color4F(0.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
		break;
	case Italic_Right2Left_Down:
		italic->setStartPos(pos2);
		italic->setEndPos(pos1);
		poss[2].y = origin.y;
		poss[2].x = poss[1].x;
		poss[3].x = tarPos.x;
		poss[3].y = origin.y;
		poss[4].x = tarPos.x;
		poss[4].y = poss[0].y;
		if (need)
			this->drawSolidPoly(poss, 5, Color4F(0.8f, 0.7f, 1.0f, 0.5f), italic->getTag());
		else
			this->drawSolidPoly(poss, 5, Color4F(0.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
		break;
	case Italic_Right2Left_Up:
		italic->setStartPos(pos2);
		italic->setEndPos(pos1);
		poss[2].x = tarPos.x;
		poss[2].y = poss[1].y;
		poss[3].y = tarPos.y;
		poss[3].x = tarPos.x;
		poss[4].x = poss[0].x;
		poss[4].y = tarPos.y;
		if (need)
			this->drawSolidPoly(poss, 5, Color4F(0.8f, 0.7f, 1.0f, 0.5f), italic->getTag());
		else
			this->drawSolidPoly(poss, 5, Color4F(0.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
		break;
	case Italic_Left2Right_Up:
		italic->setStartPos(pos1);
		italic->setEndPos(pos2);
		poss[2].y = poss[1].y;
		poss[2].x = origin.x;
		poss[3].x = origin.x;
		poss[3].y = tarPos.y;
		poss[4].x = poss[0].x;
		poss[4].y = tarPos.y;
		if (need)
			this->drawSolidPoly(poss, 5, Color4F(0.8f, 0.7f, 1.0f, 0.5f), italic->getTag());
		else
			this->drawSolidPoly(poss, 5, Color4F(0.0f, 0.0f, 1.0f, 0.5f), italic->getTag());
	default:
		break;
	}
	indexs_Tr[2] = targetTr_Index;
	indexs_Tr[3] = targetTr_Index + 3;
	indexs_Tr[4] = targetTr_Index + 6;
	unsigned int* targetTr_Index = italic->getTargetTr_Index();
	unsigned int* targetPo_Index = italic->getTargetPo_Index();
	for (int i = 0; i < 5; i++){
		targetTr_Index[i] = indexs_Tr[i];
	}
	if (indexs_Po){
		for (int i = 0; i < 7; i++){
			targetPo_Index[i] = indexs_Po[i];
		}
	}
	delete []indexs_Tr;
	if(indexs_Po)delete []indexs_Po;
	delete[] poss;
	return true;
}

bool GridSpriteLayer::addFluid(FluidBody* fluid)
{
	Point position = fluid->getPosition();
	Point pos1 = Point(position.x - fluid->getContentSize().width / 2 + 2, position.y - fluid->getContentSize().height / 2 + 2);
	Point pos2 = Point(position.x + fluid->getContentSize().width / 2 - 2, position.y - fluid->getContentSize().height / 2 + 2);
	Point pos3 = Point(position.x + fluid->getContentSize().width / 2 - 2, position.y + fluid->getContentSize().height / 2 - 2);
	Point pos4 = Point(position.x - fluid->getContentSize().width / 2 + 2, position.y + fluid->getContentSize().height / 2 - 2);
	DrawNode;
	Point poss[4] = { pos1, pos2, pos3, pos4 };
	this->drawSolidPoly(poss, 4, Color4F(0.5f, 0.5f, 0.5f, 0.5f), fluid->getTag());
	return true;
}

bool GridSpriteLayer::drawDirection(BoxBody* box)
{
	unsigned int* indexs_Po = box->getTargetPo_Index();
	One_Way way = box->getWay();
	Point position = box->getPosition();
	Point pos1, pos2;
	switch (way){
	case NONE:
	{
				 V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + indexs_Po[0]);
				 *(point) = { pos1, Color4B(127, 127, 127, 127), Tex2F(0.0, 0.0) };
				 *(point + 1) = { pos2, Color4B(127, 127, 127, 127), Tex2F(0.0, 0.0) };
	}
		break;
	case UP:
	{
			   pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y + box->getContentSize().height / 2 - 2);
			   pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y + box->getContentSize().height / 2 - 2);
			   V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + indexs_Po[0]);
			   *(point) = { pos1, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
			   *(point + 1) = { pos2, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
	}
		break;
	case DOWN:
	{
				 pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y - box->getContentSize().height / 2 + 2);
				 pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y - box->getContentSize().height / 2 + 2);
				 V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + indexs_Po[0]);
				 *(point) = { pos1, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
				 *(point + 1) = { pos2, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
	}
		break;
	case LEFT:
	{
				 pos1 = Point(position.x - box->getContentSize().width / 2 + 2, position.y - box->getContentSize().height / 2 + 2);
				 pos2 = Point(position.x - box->getContentSize().width / 2 + 2, position.y + box->getContentSize().height / 2 - 2);
				 V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + indexs_Po[0]);
				 *(point) = { pos1, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
				 *(point + 1) = { pos2, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
	}
		break;
	case RIGHT:
	{
				  pos1 = Point(position.x + box->getContentSize().width / 2 - 2, position.y - box->getContentSize().height / 2 + 2);
				  pos2 = Point(position.x + box->getContentSize().width / 2 - 2, position.y + box->getContentSize().height / 2 - 2);
				  V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + indexs_Po[0]);
				  *(point) = { pos1, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
				  *(point + 1) = { pos2, Color4B(0, 255, 0, 255), Tex2F(0.0, 0.0) };
	}
		break;
	}
	_dirtyGLLine = true;
	return true;
}

void GridSpriteLayer::drawLine(const Vec2 &origin, const Vec2 &destination, const Color4F &color, int tag)
{
	ensureCapacityGLLine(2);
	ensureCapacityGLLineIndex(1);

	V2F_C4B_T2F *point = (V2F_C4B_T2F*)(_bufferGLLine + _bufferCountGLLine);

	V2F_C4B_T2F a = { origin, Color4B(color), Tex2F(0.0, 0.0) };
	V2F_C4B_T2F b = { destination, Color4B(color), Tex2F(0.0, 0.0) };
	//添加下标
	int count = _bufferCountGLLine / 2;
	int* index = (int*)(_bufferLine_Index + count);
	*index = tag;

	//指定位置偏移
	targetPo_Index = _bufferCountGLLine;
	*point = a;
	*(point + 1) = b;

	_bufferCountGLLine += 2;
	_dirtyGLLine = true;
}

void GridSpriteLayer::drawSolidPoly(const Vec2 *poli, unsigned int numberOfPoints, const Color4F &color, int tag)
{
	drawPolygon(poli, numberOfPoints, color, 0.0, Color4F(0.0, 0.0, 0.0, 0.0), tag);
}

void GridSpriteLayer::drawPolygon(const Vec2 *verts, int count, const Color4F &fillColor, float borderWidth, const Color4F &borderColor, int tag)
{
	CCASSERT(count >= 0, "invalid count value");

	bool outline = (borderColor.a > 0.0 && borderWidth > 0.0);

	auto  triangle_count = outline ? (3 * count - 2) : (count - 2);
	auto vertex_count = 3 * triangle_count;
	ensureCapacity(vertex_count);
	ensureCapacityIndex(vertex_count / 3);

	V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
	V2F_C4B_T2F_Triangle *cursor = triangles;

	int count_Index = _bufferCount / 3;
	int* index = (int*)(_buffer_Index + count_Index);
	for (int i = 0; i < triangle_count; i++){
		*(index + i) = tag;
	}

	targetTr_Index = _bufferCount;
	for (int i = 0; i < count - 2; i++)
	{
		V2F_C4B_T2F_Triangle tmp = {
			{ verts[0], Color4B(fillColor), __t(v2fzero) },
			{ verts[i + 1], Color4B(fillColor), __t(v2fzero) },
			{ verts[i + 2], Color4B(fillColor), __t(v2fzero) },
		};

		*cursor++ = tmp;
	}

	if (outline)
	{
		struct ExtrudeVerts { Vec2 offset, n; };
		struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts)*count);
		memset(extrude, 0, sizeof(struct ExtrudeVerts)*count);

		for (int i = 0; i < count; i++)
		{
			Vec2 v0 = __v2f(verts[(i - 1 + count) % count]);
			Vec2 v1 = __v2f(verts[i]);
			Vec2 v2 = __v2f(verts[(i + 1) % count]);

			Vec2 n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
			Vec2 n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));

			Vec2 offset = v2fmult(v2fadd(n1, n2), 1.0 / (v2fdot(n1, n2) + 1.0));
			struct ExtrudeVerts tmp = { offset, n2 };
			extrude[i] = tmp;
		}

		for (int i = 0; i < count; i++)
		{
			int j = (i + 1) % count;
			Vec2 v0 = __v2f(verts[i]);
			Vec2 v1 = __v2f(verts[j]);

			Vec2 n0 = extrude[i].n;

			Vec2 offset0 = extrude[i].offset;
			Vec2 offset1 = extrude[j].offset;

			Vec2 inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
			Vec2 inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
			Vec2 outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
			Vec2 outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));

			V2F_C4B_T2F_Triangle tmp1 = {
				{ inner0, Color4B(borderColor), __t(v2fneg(n0)) },
				{ inner1, Color4B(borderColor), __t(v2fneg(n0)) },
				{ outer1, Color4B(borderColor), __t(n0) }
			};
			*cursor++ = tmp1;

			V2F_C4B_T2F_Triangle tmp2 = {
				{ inner0, Color4B(borderColor), __t(v2fneg(n0)) },
				{ outer0, Color4B(borderColor), __t(n0) },
				{ outer1, Color4B(borderColor), __t(n0) }
			};
			*cursor++ = tmp2;
		}

		free(extrude);
	}
	_bufferCount += vertex_count;
	_dirty = true;
}

void GridSpriteLayer::RemoveDraw(BaseBlock* block, int type)
{
	switch (type){
		case Type_Box:
			{	
						 auto box = dynamic_cast<BoxBody*>(block);
						 removeBox(box);
			}
			block->removeFromParent();
			block->release();
			break;
		case Type_Italic:
			{
						auto italic = dynamic_cast<ItalicBody*>(block);
						removeItalic(italic);
			}
			block->removeFromParent();
			block->release();
			break;
		case Type_Fluid:
			break;
	}
}

void GridSpriteLayer::ClearBlockDraw(BaseBlock* block, int type)
{
	switch (type){
	case Type_Box:
	{
		auto box = dynamic_cast<BoxBody*>(block);
		removeBox(box);
	}
		break;
	case Type_Italic:
	{
		auto italic = dynamic_cast<ItalicBody*>(block);
		removeItalic(italic);
	}
		break;
	case Type_Fluid:
		break;
	}
}

void GridSpriteLayer::ensureCapacityIndex(int count)
{
	CCASSERT(count >= 0, "capacity must be >= 0");
	int Index_Count = _bufferCount / 3;
	if (Index_Count + count > _buffer_Index_Capacity){
		_buffer_Index_Capacity += MAX(_buffer_Index_Capacity, count);
		_buffer_Index = (int*)realloc(_buffer_Index, _buffer_Index_Capacity * sizeof(int));
	}
}

void GridSpriteLayer::ensureCapacityGLLineIndex(int count)
{
	CCASSERT(count >= 0, "capacity must be >= 0");
	int Index_Count = _bufferCountGLLine / 2;
	if (Index_Count + count > _bufferLine_Index_Capacity){
		int capcount = _bufferLine_Index_Capacity;
		_bufferLine_Index_Capacity += MAX(_bufferLine_Index_Capacity, count);
		_bufferLine_Index = (int*)realloc(_bufferLine_Index, _bufferLine_Index_Capacity * sizeof(int));
	}
}

void GridSpriteLayer::removeBox(BoxBody* box)
{
	//box的偏移数组
	unsigned int* point_Index = box->getTargetPo_Index();
	unsigned int* triangle_Index = box->getTargetTr_Index();
	for (int i = 0; i < 2; i++){
		//最后一个三角形的位置
		_bufferCount -= 3;
		//计算出三角形的tag下标
		int count_Index = _bufferCount / 3;
		//对应的实际地址
		int* index = (int*)(_buffer_Index + count_Index);
		//对应的tag
		int tag = *index;
		int row1 = tag / col;
		int col1 = tag % col;
		auto block = vec[row1][col1];
		if (block == NULL){
			addConsoleInfo("Box Buffer 错误");
			continue;
		}
		//获取到这个block的所有索引下标
		unsigned int* targetTr_Index = block->getTargetTr_Index();
		int tr_index = -9999;
		bool isItalic = false;
		switch (block->getType()){
		case Type_Box:
			for (int j = 0; j < 2; j++){
				if (*(targetTr_Index + j) == _bufferCount){
					tr_index = j;
					break;
				}
			}
			break;
		case Type_Italic:
			for (int j = 0; j < 5; j++){
				if (*(targetTr_Index + j) == _bufferCount){
					tr_index = j;
					isItalic = true;
					break;
				}
			}
			break;
		case Type_Fluid:
			break;
		}
		//最后一个三角形
		V2F_C4B_T2F_Triangle* other_triangle = (V2F_C4B_T2F_Triangle*)(_buffer + _bufferCount);
		//block的下标位置更新
		V2F_C4B_T2F_Triangle* triangle = (V2F_C4B_T2F_Triangle*)(_buffer + triangle_Index[i]);
		V2F_C4B_T2F_Triangle tmp = *other_triangle;
		//对box位置的三角形赋值
		*other_triangle = *triangle;
		*triangle = tmp;
		index = (int*)(_buffer_Index + triangle_Index[i] / 3);
		int* other_index = (int*)(_buffer_Index + targetTr_Index[tr_index]/3);
		*index = tag;
		*other_index = box->getTag();
		unsigned int temp = targetTr_Index[tr_index];
		targetTr_Index[tr_index] = triangle_Index[i];
		triangle_Index[i] = temp;
		if (isItalic){
			for (int j = 0; j < 5; j++){
				for (int k = j + 1; k < 5; k++){
					if (targetTr_Index[j] > targetTr_Index[k]){
						V2F_C4B_T2F_Triangle* boxTr = (V2F_C4B_T2F_Triangle*)(_buffer + targetTr_Index[j]);
						V2F_C4B_T2F_Triangle* italicTr = (V2F_C4B_T2F_Triangle*)(_buffer + targetTr_Index[k]);
						V2F_C4B_T2F_Triangle* tmp = new V2F_C4B_T2F_Triangle();
						*tmp = *boxTr;
						*boxTr = *italicTr;
						*italicTr = *tmp;
						int tmpindex = targetTr_Index[j];
						targetTr_Index[j] = targetTr_Index[k];
						targetTr_Index[k] = tmpindex;
						delete tmp;
					}
				}
			}
		}
	}
	for (int i = 0; i < 1; i++){
		_bufferCountGLLine -=2;
		int count_Index = _bufferCountGLLine / 2;
		int* index = (int*)(_bufferLine_Index + count_Index);
		int tag = *index;
		int row1 = tag / col;
		int col1 = tag % col;
		auto block = vec[row1][col1];
		if (block == NULL){
			addConsoleInfo("Box Line错误");
			continue;
		}
		unsigned int* targetPo_Index = block->getTargetPo_Index();
		int po_index = 0;
		bool isItalic = false;
		switch (block->getType()){
		case Type_Box:
			break;
		case Type_Italic:
			for (int j = 0; j < 7; j++){
				if (*(targetPo_Index + j) == _bufferCountGLLine){
					po_index = j;
					isItalic = true;
					break;
				}
			}
			break;
		case Type_Fluid:
			break;
		}
		targetPo_Index[po_index] = point_Index[i];
		V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + point_Index[i]);
		V2F_C4B_T2F *a = (V2F_C4B_T2F *)(_bufferGLLine + _bufferCountGLLine);
		V2F_C4B_T2F *b = (V2F_C4B_T2F *)(_bufferGLLine + _bufferCountGLLine + 1);
		*(point) = *a;
		*(point + 1) = *b;
		index = (int*)(_bufferLine_Index + point_Index[i] / 2);
		*index = tag;
		/*V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + point_Index[i]);
		V2F_C4B_T2F *a = (V2F_C4B_T2F *)(_bufferGLLine + _bufferCountGLLine);
		V2F_C4B_T2F *b = (V2F_C4B_T2F *)(_bufferGLLine + _bufferCountGLLine + 1);
		V2F_C4B_T2F* tmp1 = a;
		V2F_C4B_T2F* tmp2 = b;
		*a = *(point);
		*b = *(point + 1);
		*(point) = *tmp1;
		*(point + 1) = *tmp2;
		index = (int*)(_bufferLine_Index + point_Index[i] / 2);
		int* other_index = (int*)(_bufferLine_Index + targetPo_Index[po_index] / 2);
		int temp = *index;
		*index = tag;
		*other_index = temp;
		targetPo_Index[po_index] = point_Index[i];*/
	}
}

void GridSpriteLayer::removeItalic(ItalicBody* italic)
{
	//box的偏移数组
	unsigned int* point_Index = italic->getTargetPo_Index();
	unsigned int* triangle_Index = italic->getTargetTr_Index();
	int loopindex = -1;
	for (int i = 0; i < 5; i++){
		//最后一个三角形的位置
		_bufferCount -= 3;
		//计算出三角形的tag下标
		int count_Index = _bufferCount / 3;
		//对应的实际地址
		int* index = (int*)(_buffer_Index + count_Index);
		//对应的tag
		int tag = *index;
		int row1 = tag / col;
		int col1 = tag % col;
		if (tag == italic->getTag()){
			continue;
		}
		else{
			loopindex = loopindex + 1;
		}
		auto block = vec[row1][col1];
		if (block == NULL){
			addConsoleInfo("Italic Buffer错误");
			continue;
		}
		//获取到这个block的所有索引下标
		unsigned int* targetTr_Index = block->getTargetTr_Index();
		int tr_index = -9999;
		bool isItalic = false;
		switch (block->getType()){
		case Type_Box:
			for (int j = 0; j < 2; j++){
				if (*(targetTr_Index + j) == _bufferCount){
					tr_index = j;
					break;
				}
			}
			break;
		case Type_Italic:
			for (int j = 0; j < 5; j++){
				if (*(targetTr_Index + j) == _bufferCount){
					tr_index = j;
					isItalic = true;
					break;
				}
			}
			break;
		case Type_Fluid:
			break;
		}
		
		//最后一个三角形
		V2F_C4B_T2F_Triangle* other_triangle = (V2F_C4B_T2F_Triangle*)(_buffer + _bufferCount);
		V2F_C4B_T2F_Triangle* triangle = (V2F_C4B_T2F_Triangle*)(_buffer + triangle_Index[loopindex]);
		V2F_C4B_T2F_Triangle tmp = *other_triangle;
		//对box位置的三角形赋值
		*other_triangle = *triangle;
		*triangle = tmp;
		index = (int*)(_buffer_Index + triangle_Index[loopindex] / 3);
		int* other_index = (int*)(_buffer_Index + targetTr_Index[tr_index]/3);
		*index = tag;
		*other_index = italic->getTag();
		unsigned int temp = targetTr_Index[tr_index];
		targetTr_Index[tr_index] = triangle_Index[loopindex];
		triangle_Index[loopindex] = temp;
		if (isItalic){
			for (int j = 0; j < 5; j++){
				for (int k = j+1; k < 5; k++){
					if (targetTr_Index[j] > targetTr_Index[k]){
						V2F_C4B_T2F_Triangle* boxTr = (V2F_C4B_T2F_Triangle*)(_buffer + targetTr_Index[j]);
						V2F_C4B_T2F_Triangle* italicTr = (V2F_C4B_T2F_Triangle*)(_buffer + targetTr_Index[k]);
						V2F_C4B_T2F_Triangle* tmp = new V2F_C4B_T2F_Triangle();
						*tmp = *boxTr;
						*boxTr = *italicTr;
						*italicTr = *tmp;
						int tmpindex = targetTr_Index[j];
						targetTr_Index[j] = targetTr_Index[k];
						targetTr_Index[k] = tmpindex;
					}
				}
			}
		}
	}
	if(italic->isNeed()){
		for (int i = 0; i < 7; i++){
			_bufferCountGLLine -= 2;
			//获取到最后一个的下标
			int count_Index = _bufferCountGLLine / 2;
			int* index = (int*)(_bufferLine_Index + count_Index);
			int tag = *index;
			int row1 = tag / col;
			int col1 = tag % col;
			auto block = vec[row1][col1];
			if (block == NULL){
				addConsoleInfo("Italic Line 错误");
				continue;
			}
			//获取到最后一块的下标数组
			unsigned int* targetPo_Index = block->getTargetPo_Index();
			int po_index = 0;
			bool isItalic = false;
			switch (block->getType()){
			case Type_Box:
				break;
			case Type_Italic:
				for (int j = 0; j < 7; j++){
					if (*(targetPo_Index + j) == _bufferCountGLLine){
						po_index = j;
						isItalic = true;
						break;
					}
				}
				break;
			case Type_Fluid:
				break;
			}
			targetPo_Index[po_index] = point_Index[i];
			V2F_C4B_T2F* point = (V2F_C4B_T2F*)(_bufferGLLine + point_Index[i]);
			V2F_C4B_T2F *a = (V2F_C4B_T2F *)(_bufferGLLine + _bufferCountGLLine);
			V2F_C4B_T2F *b = (V2F_C4B_T2F *)(_bufferGLLine + _bufferCountGLLine + 1);
			point->colors.a = 0;
			(point + 1)->colors.a = 0;
			*(point) = *a;
			*(point + 1) = *b;
			index = (int*)(_bufferLine_Index + point_Index[i] / 2);
			*index = tag;
			if (isItalic){

			}
		}
	}
}

void GridSpriteLayer::removeFluid(FluidBody* fluid)
{

}

void GridSpriteLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (_bufferCount)
	{
		_customCommand.init(_globalZOrder, transform, flags);
		_customCommand.func = CC_CALLBACK_0(GridSpriteLayer::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
	}

	if (_bufferCountGLPoint)
	{
		_customCommandGLPoint.init(_globalZOrder, transform, flags);
		_customCommandGLPoint.func = CC_CALLBACK_0(GridSpriteLayer::onDrawGLPoint, this, transform, flags);
		renderer->addCommand(&_customCommandGLPoint);
	}

	if (_bufferCountGLLine)
	{
		_customCommandGLLine.init(_globalZOrder, transform, flags);
		_customCommandGLLine.func = CC_CALLBACK_0(GridSpriteLayer::onDrawGLLine, this, transform, flags);
		renderer->addCommand(&_customCommandGLLine);
	}
}

void GridSpriteLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = getGLProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);

	GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	if (_dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);

		_dirty = false;
	}
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(_vao);
	}
	else
	{
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// vertex
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		// texcood
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
	}

	glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(0);
	}

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
	CHECK_GL_ERROR_DEBUG();
}

void GridSpriteLayer::onDrawGLLine(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR);
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);

	if (_dirtyGLLine)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
		_dirtyGLLine = false;
	}
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(_vaoGLLine);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
		// vertex
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		// texcood
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
	}
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, _bufferCountGLLine);

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCountGLLine);
	CHECK_GL_ERROR_DEBUG();
}

void GridSpriteLayer::onDrawGLPoint(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE);
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);

	if (_dirtyGLPoint)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);

		_dirtyGLPoint = false;
	}

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(_vaoGLPoint);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
	}

	glDrawArrays(GL_POINTS, 0, _bufferCountGLPoint);

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCountGLPoint);
	CHECK_GL_ERROR_DEBUG();
}

void GridSpriteLayer::setDirty(bool dirty)
{
	_dirty = dirty;
	_dirtyGLLine = dirty;
}

int GridSpriteLayer::getTag()
{
	return inline_tag;
}

void GridSpriteLayer::setBaseBlock(BaseBlock*** vec)
{
	this->vec = vec;
}

void GridSpriteLayer::setGrid(int row, int col)
{
	this->row = row;
	this->col = col;
}

int GridSpriteLayer::getPoints(Point pos1, Point pos2, drawData* data, int type)
{
	float k = (pos1.y - pos2.y) / (pos1.x - pos2.x);
	float k_ = (pos1.x - pos2.x) / (pos1.y - pos2.y);
	int x1 = pos1.x / grid_W;
	int x2 = pos2.x / grid_W;
	int y1 = pos1.y / grid_H;
	int y2 = pos2.y / grid_H;
	this->CorrectionRange(x1, y1, x2, y2, type, pos1, pos2);
	int sizeX = abs(x1 - x2) + 1;
	int sizeY = abs(y1 - y2) + 1;
	//char str[256];
	//sprintf(str, "当前画斜体,起点和终点分别是(%d, %d), (%d, %d)", x1, y1, x2, y2);
	//addConsoleInfo(str);
	/*sprintf(str, "斜体的网格横跨宽度大小是%d, 网格横跨高度大小是%d", sizeX, sizeY);
	addConsoleInfo(str);*/
	int num = 0;
	Point start, end;
	//处理当明明拖动的是终点但是这里却是起点
	if (k > 0){
		if (type == Italic_Right2Left_Up){
			float y_1 = y1 * grid_H;
			float x_1 = (y_1 - pos1.y + k * pos1.x) / k;
			float x_2 = x1 * grid_W;
			float y_2 = k * (x_2 - pos1.x) + pos1.y;
			if (y_1 >= y1 * grid_H  && x_1 >= x1 * grid_W){
				pos1 = Point(x_1, y_1);
			}
			else if (y_2 >= y1 * grid_H  && x_2 >= x1 * grid_W){
				pos1 = Point(x_2, y_2);
			}
		}
	}
	else{
		if (type == Italic_Left2Right_Up){
			float y_1 = y1 * grid_H;
			float x_1 = (y_1 - pos1.y + k * pos1.x) / k;
			float x_2 = (x1 + 1)* grid_W;
			float y_2 = k * (x_2 - pos1.x) + pos1.y;
			if (y_2 >= y1 * grid_H  && x_2 >= x1 * grid_W){
				pos1 = Point(x_2, y_2);
				//addConsoleInfo("2");
			}
			else if (y_1 >= y1 * grid_H  && x_1 >= x1 * grid_W){
				pos1 = Point(x_1, y_1);
				//addConsoleInfo("1");
			}
		}
	}
	start = pos1;
	for (int i = 0; i < sizeX; i++){
		if (k < 0){
			end.x = (x1 - i) * grid_W;
			end.y = k * end.x - k * pos1.x + pos1.y;
		}
		else{
			end.x = (x1 + i + 1) * grid_W;
			end.y = k * end.x - k * pos1.x + pos1.y;
		}
		int startx = start.x / grid_W;
		if (k < 0 && i == 0 && startx == start.x / grid_W){
			startx--;
		}
		int endx = end.x / grid_W;
		int starty = start.y / grid_H;
		int endy = end.y / grid_H;
		int size_Y = abs(starty - endy) + 1;
		Point* ss = new Point[size_Y + 1];
		ss[0] = start;
		ss[size_Y] = end;
		for (int s = 1; s < size_Y; s++){
			if (s == 1){
				ss[s].y = (starty + 1)* grid_H;
			}
			else{
				ss[s].y = ss[s - 1].y + grid_H;
			}
			ss[s].x = k_ * ss[s].y - k_ * pos1.y + pos1.x;
		}
		for (int j = 0; j < size_Y; j++){
			if ((starty + j) > y2)
				break;
			Point pos;
			if (k < 0)
				pos.x = endx;
			else
				pos.x = startx;
			pos.y = starty + j;
			Point one = ss[j] - Point(startx*grid_W, starty*grid_H);
			Point two = ss[j + 1] - Point(startx*grid_W, starty*grid_H);
			one = Point(one.x / grid_W, one.y / grid_H - j);
			two = Point(two.x / grid_W, two.y / grid_H - j);
			if (i > 0 && k < 0){
				one.x += 1;
				two.x += 1;
			}
			if (abs(one.x - two.x)>0.01 && abs(one.y - two.y)>0.01){
				data[num].pos = pos;
				data[num].start = one;
				data[num].end = two;
				data[num].type = type;
				num++;
			}
		}
		delete[]ss;
		start = end;
	}
	return num;
}

void GridSpriteLayer::addBoxAndItalic(Point pos1, Point pos2, int& BBSize)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	pos1.y = glview->getFrameSize().height - pos1.y;
	pos2.y = glview->getFrameSize().height - pos2.y;
	if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
		pos2.y += getOldWindowSize().height() - getNewWindowSize().height();
		pos1.y += getOldWindowSize().height() - getNewWindowSize().height();
	}
	int x1 = pos1.x / grid_W;
	int x2 = pos2.x / grid_W;
	int y1 = pos1.y / grid_H;
	int y2 = pos2.y / grid_H;
	int sizeX = abs(x1 - x2) + 1;
	int sizeY = abs(y1 - y2) + 1;

	drawData* data;
	if (pos1.y > pos2.y){
		if (!testDirstance(pos1, pos2))
			return;
		data = new drawData[sizeX + sizeY];
		
		int num;
		if (pos1.x > pos2.x)
			num = getPoints(pos2, pos1, data, Italic_Right2Left_Up);
		else
			num = getPoints(pos2, pos1, data, Italic_Left2Right_Up);
		for (int i = 0; i < num; i++){
			if (data[i].pos.x < 0 || data[i].pos.y < 0 || data[i].pos.x > this->col - 1 || data[i].pos.y > this->row - 1)
				continue;
			if (vec && vec[(int)data[i].pos.y][(int)data[i].pos.x] == NULL){
				auto block = ItalicBody::create("gridBg.png", grid_W, grid_H);
				addChild(block, 1);
				vec[(int)data[i].pos.y][(int)data[i].pos.x] = block;
				BBSize++;
				auto pos = Point((int)data[i].pos.x * grid_W + grid_W / 2, glview->getFrameSize().height - (int)data[i].pos.y*grid_H - grid_H / 2);
				if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
					pos.y += getOldWindowSize().height() - getNewWindowSize().height();
				}
				block->setPosition(pos);
				block->setTag((int)data[i].pos.y*col + (int)data[i].pos.x);
				block->setOpacity(80);
				block->setType(Type_Italic);
			
				//一定要保证setTag在DrawNode之前
				block->addDrawNode(data[i].start, data[i].end, data[i].type);
			}
		}
	}
	else{
		if (!testDirstance(pos2, pos1))
			return;
		data = new drawData[sizeX + sizeY];
		int num;
		if (pos1.x > pos2.x)
			num = getPoints(pos1, pos2, data, Italic_Right2Left_Down);
		else
			num = getPoints(pos1, pos2, data, Italic_Left2Right_Down);
		for (int i = 0; i < num; i++){
			if (data[i].pos.x < 0 || data[i].pos.y < 0 || data[i].pos.x > this->col - 1 || data[i].pos.y > this->row - 1)
				continue;
			if (vec && vec[(int)data[i].pos.y][(int)data[i].pos.x] == NULL){
				auto block = ItalicBody::create("gridBg.png", grid_W, grid_H);
				addChild(block);
				vec[(int)data[i].pos.y][(int)data[i].pos.x] = block;
				BBSize++;
				auto pos = Point((int)data[i].pos.x * grid_W + grid_W / 2, glview->getFrameSize().height - (int)data[i].pos.y*grid_H - grid_H / 2);
				if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
					pos.y += getOldWindowSize().height() - getNewWindowSize().height();
				}
				block->setPosition(pos);
				block->setTag((int)data[i].pos.y*col + (int)data[i].pos.x);
				block->setOpacity(80);
				block->setType(Type_Italic);
				
				block->addDrawNode(data[i].start, data[i].end, data[i].type);
			}
		}
	}
	delete[]data;
}

void GridSpriteLayer::CorrectionRange(int &x1, int &y1, int &x2, int &y2, int type, Point pos1, Point pos2)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	Point end = Point(endedPos.x, glview->getFrameSize().height - endedPos.y);
	Point start = Point(startPos.x, glview->getFrameSize().height - startPos.y);
	if (g_CocosWindowInitSize.height() == getNewWindowSize().height()){
		switch (type){
		case Italic_Left2Right_Down:
			if (end.y != pos2.y)
				y2--;
			if (end.x != pos2.x)
				x2--;
			break;
		case Italic_Left2Right_Up:
			//end 上 start 下 pos1 右 pos2 左
			//x1 y1 上 x2 y2 下
			if (start.y != pos2.y)
				y2--;
			if (end.x != pos1.x)
				x1--;
			break;
		case Italic_Right2Left_Down:
			if (start.x != pos1.x)
				x1--;
			if (end.y != pos2.y)
				y2--;
			break;
		case Italic_Right2Left_Up:
			if (start.x != pos2.x)
				x2--;
			if (start.y != pos2.y)
				y2--;
			break;
		}
	}
}

bool GridSpriteLayer::testDirstance(Point pos1, Point pos2)
{
	float distance = pos1.getDistance(pos2);
	if (distance < Point(grid_W, grid_H).getLength() / 5){
		return false;
	}
	if (pos1.y - pos2.y <= 5){
		return false;
	}
	if (pos1.x - pos2.x <= 5 && pos1.x - pos2.x >= -5){
		return false;
	}
	if (pos2.x - pos1.x <= 5 && pos2.x - pos1.x >= -5){
		return false;
	}
	return true;
}

void GridSpriteLayer::setStartPos(Point pos)
{
	startPos = pos;
}

void GridSpriteLayer::setEndedPos(Point pos)
{
	endedPos = pos;
}

void GridSpriteLayer::updateGrid()
{
	_bufferCount = 0;
	_bufferCountGLLine = 0;
	char str[256];
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (vec[i][j]){
				switch (vec[i][j]->getType()){
				case Type_Box:
				{
								 auto block = dynamic_cast<BoxBody*>(vec[i][j]);
								 //block->deleteTarget_Index();
								 block->DrawGrid();
				}
					break;
				case Type_Italic:
				{
									auto block = dynamic_cast<ItalicBody*>(vec[i][j]);
									//block->deleteTarget_Index();
									block->addDrawNode(block->getPos1(), block->getPos2(), block->getDrawType());
				}
					break;
				case Type_Fluid:
					break;
				}
			}
		}
	}
	this->setDirty(true);
}

int GridSpriteLayer::getMinX()
{
	for (int j = 0; j < col; j++){
		for (int i = 0; i < row; i++){
			if (vec[i][j]){
				return j;
			}
		}
	}
	return col;
}

int GridSpriteLayer::getMinY()
{
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (vec[i][j]){
				return i;
			}
		}
	}
	return row;
}

int GridSpriteLayer::getMaxX()
{
	for (int j = col - 1; j> 0; j--){
		for (int i = row - 1; i > 0; i--){
			if (vec[i][j]){
				return j;
			}
		}
	}
	return 0;
}

int GridSpriteLayer::getMaxY()
{
	for (int i = row - 1; i > 0; i--){
		for (int j = col - 1; j > 0; j--){
			if (vec[i][j]){
				return i;
			}
		}
	}
	return 0;
}

int GridSpriteLayer::getGridCount()
{
	int num = 0;
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (vec[i][j]){
				num++;
			}
		}
	}
	return num;
}