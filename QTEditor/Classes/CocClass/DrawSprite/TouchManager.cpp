#include "TouchManager.h"
#include "Global.h"
#include "HelloWorldScene.h"
#include "ConfigView.h"
#include "MacTransViewPos.h"

TouchManager::TouchManager()
{
	isDrawEdge = false;
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
}


TouchManager::~TouchManager()
{
}

TouchManager* TouchManager::create()
{
	auto ret = new TouchManager;
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

bool TouchManager::init()
{
	if (!Layer::init())
	{
		return false;
	}
	manage = false;
	addTouchEvent();
	addMouseEvent();
	return true;
}

void TouchManager::addTouchEvent()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(TouchManager::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(TouchManager::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(TouchManager::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void TouchManager::addMouseEvent()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	EventListenerMouse* mouseListener = EventListenerMouse::create();
	//mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	mouseListener->onMouseScroll = CC_CALLBACK_1(TouchManager::onMouseScroll, this);
	//mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

bool TouchManager::onTouchBegan(Touch *touch, Event *unused_event)
{
	isDrawEdge = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    MacTransViewPos::getViewTouchPos();
#endif
	return true;
}

void TouchManager::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (manage){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		movedPos = touch->getLocation();
		Point pre = touch->getPreviousLocation();
#else
		movedPos = MacTransViewPos::getViewTouchPos();
		Point pre = MacTransViewPos::getViewPreTouchPos();
#endif
		Point dis = movedPos - pre;
		Node* parent = this->getParent();
		Point parentPos = parent->getPosition();
		parent->setPosition(parentPos + dis);
		direction = getEdgeDirection();
		if (direction != 0 && parent->getScale() == 1.0f){
			isDrawEdge = true;
		}
		else{
			isDrawEdge = false;
		}
	}
}

void TouchManager::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (manage){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		endedPos = touch->getLocation();
#else
		endedPos = MacTransViewPos::getViewTouchPos();
#endif
		if (isDrawEdge){
			auto parent = dynamic_cast<HelloWorld*>(this->getParent());
			parent->setPosition(scenePosition);
		}
	}
	isDrawEdge = false;
}

void TouchManager::sceneMoveToScenePos(Point pos)
{
	//根据场景中某个点将场景移动到这个点上
	auto parent = static_cast<HelloWorld*>(this->getParent()); 
	auto scenePos = parent->getPosition();
	auto viewsize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto screenCenterPos = Point(viewsize.width / 2, viewsize.height / 2);
	screenCenterPos = parent->getSceneScalePos(screenCenterPos);
	auto dis = screenCenterPos - pos;
	parent->runAction(MoveBy::create(0.2, dis*parent->getScale()));
}

void TouchManager::onMouseScroll(Event* event)
{
	if (manage){
		EventMouse* ee = (EventMouse*)event;
		int y = ee->getScrollY();
		Node* parent = this->getParent();
		float scale = parent->getScale();
		ConfigView* cv = dynamic_cast<ConfigView*>(g_configView);
		float oldscale = parent->getScale();
		switch (y){
			//上
		case -1:
			if (scale - 0.05f > 0.0f){
				parent->setScale(scale - 0.05f);
				cv->setScaleTextValue(scale - 0.05f);
				parent->setPosition(parent->getPosition()*(parent->getScale() / oldscale));
			}
			break;
			//下
		case 1:
			parent->setScale(scale + 0.05f);
			cv->setScaleTextValue(scale + 0.05f);
			parent->setPosition(parent->getPosition()*(parent->getScale() / oldscale));
			break;
		}	
	}
}

int TouchManager::getEdgeDirection()
{
	auto viewsize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto parent = dynamic_cast<HelloWorld*>(this->getParent());
	int row = parent->row;
	int col = parent->col;
	int gridHeight = parent->gridHeight;
	int gridWidth = parent->gridWidth;
	int height = row * gridHeight;
	int width = col * gridWidth;
	Point scenePos = parent->getPosition();
	Point pos1, pos2, pos3, pos4;
	pos1 = Point(scenePos.x, scenePos.y+viewsize.height);
	pos4 = Point(scenePos.x + width, scenePos.y + viewsize.height);
	pos2.x = scenePos.x;
	pos3.x = scenePos.x + width;
	pos2.y = scenePos.y - (height - viewsize.height);
	pos3.y = scenePos.y - (height - viewsize.height);
	if ((pos1).distance(Point(0, viewsize.height)) <= 30){
		//上左
		scenePosition = Point(0, 0);
		return 1;
	}
	else if ((pos2).distance(Point(0, 0)) <= 30){
		//下左
		scenePosition.x = 0;
		scenePosition.y = height - viewsize.height;
		return 2;
	}
	else if ((pos3).distance(Point(viewsize.width, 0)) <= 30){
		//下右
		scenePosition.x = viewsize.width - width;
		scenePosition.y = height - viewsize.height;
		return 3;
	}
	else if ((pos4).distance(Point(viewsize.width, viewsize.height)) <= 30){
		//上右
		scenePosition.x = viewsize.width - width;
		scenePosition.y = 0;
		return 4;
	}
	return 0;
}

void TouchManager::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(TouchManager::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void TouchManager::onDraw(const Mat4 &transform, uint32_t flags)
{
	if (manage && isDrawEdge){
		Director* director = Director::getInstance();
		auto viewsize = Director::getInstance()->getOpenGLView()->getFrameSize();
		auto parent = dynamic_cast<HelloWorld*>(this->getParent());
		CHECK_GL_ERROR_DEBUG();
		glLineWidth(6.0f);
		DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
		Point pos1(0, 0);
		Point pos2(0, viewsize.height);
		Point pos3(viewsize.width, 0);
		Point pos4(viewsize.width, viewsize.height);
		switch (direction){
		case 1:
			pos1.y = viewsize.height / 2;
			pos4.x = viewsize.width / 2;
			DrawPrimitives::drawLine(pos1, pos2);
			DrawPrimitives::drawLine(pos2, pos4);
			break;
		case 2:
			pos2.y = viewsize.height / 2;
			pos3.x = viewsize.width / 2;
			DrawPrimitives::drawLine(pos1, pos2);
			DrawPrimitives::drawLine(pos1, pos3);
			break;
		case 3:
			pos1.x = viewsize.width / 2;
			pos4.y = viewsize.height / 2;
			DrawPrimitives::drawLine(pos1, pos3);
			DrawPrimitives::drawLine(pos3, pos4);
			break;
		case 4:
			pos2.x = viewsize.width / 2;
			pos3.y = viewsize.height / 2;
			DrawPrimitives::drawLine(pos2, pos4);
			DrawPrimitives::drawLine(pos3, pos4);
			break;
		}
		CHECK_GL_ERROR_DEBUG();
	}
}

void TouchManager::startTouchManage()
{
	manage = true;
}

void TouchManager::endTouchManage()
{
	manage = false;
}