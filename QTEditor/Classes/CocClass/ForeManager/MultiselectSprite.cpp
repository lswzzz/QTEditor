#include "MultiselectSprite.h"
#include "ImageSprite.h"
#include "Global.h"
#include "LayerManagerWidget.h"
#include "ControlView.h"

MultiselectSprite::MultiselectSprite()
{
	drawnode = NULL;
	isSimple = true;
}

MultiselectSprite::~MultiselectSprite()
{
	if (drawnode){
		drawnode->removeFromParent();
		drawnode = NULL;
	}
}

MultiselectSprite* MultiselectSprite::create()
{
	MultiselectSprite* ret = new MultiselectSprite();
	if (ret && ret->init()){
		ret->autorelease();
		return ret;
	}
	return nullptr;
}

bool MultiselectSprite::init()
{
	if (!Sprite::init()){
		return false;
	}
	drawRect = false;
	cur_static = 0;
	return true;
}

void MultiselectSprite::setStartPos(Point pos)
{
	startPos = pos;
	drawRect = false;
	cur_static = 1;
}

void MultiselectSprite::setMovedPos(Point pos)
{
	movedPos = pos;
	drawRect = true;
	if (cur_static == 1)
		cur_static = 2;
}

void MultiselectSprite::setEndedPos(Point pos)
{
	endedPos = pos;
	drawRect = false;
	cur_static = 0;
}

bool MultiselectSprite::MultiSelect(Vector<Node*> vec)
{
	isSimple = false;
	Rect rect = getRect();
	for (int i = 0; i < vec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(vec.at(i));
		Rect sprRect = sprite->getBoundingBox();
		auto size = sprite->getContentSize();
		auto layer = sprite->getParent();
		sprRect.setRect(sprRect.getMidX() + layer->getPositionX()-size.width*sprite->getScaleX()/2, sprRect.getMidY() + layer->getPositionY()-size.height*sprite->getScaleY()/2, sprRect.size.width, sprRect.size.height);
		if (rect.intersectsRect(sprRect)){
			bool repeat = false;
			int index = 0;
			for (int j = 0; j < _vec.size(); j++){
				auto othSprite = dynamic_cast<ImageSprite*>(_vec.at(j));
				if (othSprite == sprite){
					repeat = true;
					index = j;
					break;
				}
			}
			if (!repeat){
				_vec.pushBack(sprite);
			}
			else{
				_vec.erase(index);
			}
		}
	}
	addDrawNode();
	if (_vec.size() == 1){
		auto sprite = dynamic_cast<ImageSprite*>(_vec.at(0));
		auto layer = dynamic_cast<ImageSpriteLayer*>(sprite->getParent());
		auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
		layerManager->setCurrentSpriteChild(layer->getTagName(), sprite->getTagName());
		auto controlView = dynamic_cast<ControlView*>(g_ControlView);
		controlView->setValueFromChildSprite(sprite);
	}
	else{
		auto controlView = dynamic_cast<ControlView*>(g_ControlView);
		controlView->clearProperties();
	}
	if (_vec.size() == 0){
		return false;
	}
	else{
		return true;
	}
}

bool MultiselectSprite::addSimpleNode(ImageSprite* sprite)
{
	_vec.pushBack(sprite);
	isSimple = true;
	//addDrawNode();
	if (_vec.size() > 1){
		return false;
	}
	return true;
}

bool MultiselectSprite::isTouchMultiSelectSprite(Point pos)
{
	for (int i = 0; i < _vec.size(); i++){
		auto sprite = _vec.at(i);

		Rect sprRect = sprite->getBoundingBox();
		auto size = sprite->getContentSize();
		auto layer = sprite->getParent();
		sprRect.setRect(sprRect.getMidX() + layer->getPositionX() - size.width*sprite->getScaleX() / 2, sprRect.getMidY() + layer->getPositionY() - size.height *sprite->getScaleY()/ 2, sprRect.size.width, sprRect.size.height);
		if (sprRect.containsPoint(pos)){
			return true;
		}
	}
	return false;
}

void MultiselectSprite::addDrawNode()
{
	if (isSimple)return;
	if (!drawnode){
		drawnode = DrawNode::create();
		addChild(drawnode, 9999);
	}
	drawnode->clear();
	for (int i = 0; i < _vec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(_vec.at(i));
		auto layer = sprite->getParent();
		Rect rect = sprite->getBoundingBox();
		drawnode->drawRect(Point(rect.origin.x+2+layer->getPositionX(), rect.origin.y+2+layer->getPositionY()),
			Vec2(rect.getMaxX() - 2 + layer->getPositionX(), rect.getMaxY() - 2 + layer->getPositionY()), Color4F(Color4B(0xFF, 0x00, 0xFF, 0xff)));
	}
}

void MultiselectSprite::clearDrawNode()
{
	if (drawnode){
		drawnode->clear();
	}
	_vec.clear();
}

void MultiselectSprite::moveSize(Point size)
{
	for (int i = 0; i < _vec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(_vec.at(i));
		sprite->setPosition(sprite->getStartPos() + size);
	}
	addDrawNode();
}

Vector<Node*> MultiselectSprite::getVec()
{
	return _vec;
}

void MultiselectSprite::setVec(Vector<Node*> vec)
{
	_vec = vec;
}

Rect MultiselectSprite::getRect()
{
	float minx, miny, width, height;
	if (startPos.x > endedPos.x){
		minx = endedPos.x;
		width = startPos.x - endedPos.x;
	}
	else{
		minx = startPos.x;
		width = endedPos.x - startPos.x;
	}
	if (startPos.y < endedPos.y){
		miny = startPos.y;
		height = endedPos.y - startPos.y;
	}
	else{
		miny = endedPos.y;
		height = startPos.y - endedPos.y;
	}
	return Rect(minx, miny, width, height);
}

void MultiselectSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(MultiselectSprite::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void MultiselectSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	if (drawRect && cur_static > 0){
		Director* director = Director::getInstance();
		director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
		CHECK_GL_ERROR_DEBUG();
		glLineWidth(2.0f);
		DrawPrimitives::setDrawColor4B(0x00, 0xBF, 0xFF, 255);
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