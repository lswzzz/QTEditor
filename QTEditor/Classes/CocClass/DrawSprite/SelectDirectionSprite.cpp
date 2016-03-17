#include "SelectDirectionSprite.h"
#include "BoxBody.h"
#include "Global.h"

SelectDirectionSprite::SelectDirectionSprite()
{
	drawRect = false;
	m_type = NONE;
}


SelectDirectionSprite::~SelectDirectionSprite()
{
}

SelectDirectionSprite* SelectDirectionSprite::create()
{
	auto ret = new SelectDirectionSprite();
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

void SelectDirectionSprite::setStartPos(Point pos)
{
	startPos = pos;
	drawRect = false;
}

void SelectDirectionSprite::setMovedPos(Point pos)
{
	movedPos = pos;
	drawRect = true;
	/*char str[256];
	sprintf(str, "moved %f %f %d", pos.x, pos.y, drawRect);
	addConsoleInfo(str);*/
}

void SelectDirectionSprite::setEndedPos(Point pos)
{
	endedPos = pos;
	drawRect = false;
}

void SelectDirectionSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(SelectDirectionSprite::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void SelectDirectionSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	if (drawRect){
		Director* director = Director::getInstance();
		director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
		CHECK_GL_ERROR_DEBUG();
		glLineWidth(1.0f);
		DrawPrimitives::setDrawColor4B(130, 220, 188, 255);
		Point pos1, pos2, pos3, pos4;
		pos1 = startPos;
		pos2 = Point(startPos.x, movedPos.y);
		pos3 = movedPos;
		pos4 = Point(movedPos.x, startPos.y);
		DrawPrimitives::drawLine(pos1, pos2);
		DrawPrimitives::drawLine(pos2, pos3);
		DrawPrimitives::drawLine(pos3, pos4);
		DrawPrimitives::drawLine(pos4, pos1);
		CHECK_GL_ERROR_DEBUG();
		director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	}
}

std::vector<Point> SelectDirectionSprite::getRange(int gridW, int gridH)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	float width1 = startPos.x;
	float height1 = glview->getFrameSize().height - startPos.y;
	float width2 = endedPos.x;
	float height2 = glview->getFrameSize().height - endedPos.y;
	if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
		height1 += getOldWindowSize().height() - getNewWindowSize().height();
		height2 += getOldWindowSize().height() - getNewWindowSize().height();
	}
	int pos1x = width1 / gridW;
	int pos1y = height1 / gridH;
	int pos2x = width2 / gridW;
	int pos2y = height2 / gridH;
	int temp;
	if (pos1x > pos2x){
		temp = pos1x;
		pos1x = pos2x;
		pos2x = temp;
	}
	if (pos1y > pos2y){
		temp = pos1y;
		pos1y = pos2y;
		pos2y = temp;
	}
	std::vector<Point> poss;
	for (int i = pos1y; i <= pos2y; i++){
		for (int j = pos1x; j <= pos2x; j++){
			poss.push_back(Point(j, i));
		}
	}
	return poss;
}

void SelectDirectionSprite::setType(int type)
{
	m_type = type;
}

int SelectDirectionSprite::getType()
{
	return m_type;
}