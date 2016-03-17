#include "TouchSprite.h"


TouchSprite::TouchSprite()
{
	//startSprite = NULL;
	//endedSprite = NULL;
	drawLine = false;
	m_type = 1;
}


TouchSprite::~TouchSprite()
{

}

TouchSprite* TouchSprite::create()
{
	auto ret = new TouchSprite();
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

void TouchSprite::setStartPos(Point pos)
{
	/*if (startSprite == NULL){
		startSprite = Sprite::create();
		startSprite->setScale(0.03);
		this->addChild(startSprite);
		auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.3, 0.02), ScaleTo::create(0.3, 0.04));
		startSprite->runAction(RepeatForever::create(seq));
	}
	startSprite->setPosition(pos);*/
	startPos = pos;
	drawLine = false;
}

void TouchSprite::setMovedPos(Point pos)
{
	movedPos = pos;
	drawLine = true;
}

void TouchSprite::setType(int type)
{
	m_type = type;
}

void TouchSprite::setEndedPos(Point pos)
{
	/*if (endedSprite == NULL){
		endedSprite = Sprite::create();
		endedSprite->setScale(0.03);
		this->addChild(endedSprite);
		auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.3, 0.02), ScaleTo::create(0.3, 0.04));
		endedSprite->runAction(RepeatForever::create(seq));
	}
	endedSprite->setPosition(pos);*/
	endedPos = pos;
	drawLine = false;
	//endedSprite->stopAllActions();
	
}

void TouchSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(TouchSprite::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void TouchSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	if (drawLine){
		Director* director = Director::getInstance();
		director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
		CHECK_GL_ERROR_DEBUG();
		glLineWidth(1.0f);
		if (m_type)
			DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
		else
			DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
		DrawPrimitives::drawLine(startPos, movedPos);
		CHECK_GL_ERROR_DEBUG();
		director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	}
}

int TouchSprite::getType()
{
	return m_type;
}