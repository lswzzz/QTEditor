#include "HelloWorldScene.h"
#include "Global.h"
#include "BaseBlock.h"
#include "BoxBody.h"
#include "ItalicBody.h"
#include "FluidBody.h"
#include "qteditor.h"
#include "LayerManagerWidget.h"
#include "FontChina.h"
#include "QTOutput.h"
#include "ControlView.h"
#include <QDir>
#include "MyLogger.h"
#include "MySqlLiteManager.h"
#include "WinEventDispatcher.h"
#include "MacTransViewPos.h"
#include "GlobalConstant.h"
#include "ImgTransPath.h"

static LayerColor* layerColor;

enum LayerType{
	smallbgLayer = 0,
	gridLayer = 1,
	blcokLayer = 2,
	touchSpriteLayer = 3,
	touchManLayer = 4,
	uiLayer = 10,
	dragLayer = 100,
};

static int gridW = 32;
static int gridH = 32;
static int static_type = Type_Box;
static Point cursorPos;
static bool isInit = false;
static float autoSaveTime = 0.0f;

HelloWorld::HelloWorld()
{
	autoSaveLabel = NULL;
	drawNodeSprite = NULL;
	adsorbentLayer = NULL;
	static_type = Type_None;
	imgSprite = NULL;
	foreManager = NULL;
	is_Shift_Key = false;
	is_Space_Key = false;
	is_Alt_Key = false;
	is_P_Key = false;
	is_O_Key = false;
	is_Ctrl_Key = false;
	touchManagerLayer = NULL;
	gridManagerLayer = NULL;
	isRightMouseDown = false;
	edgeSprite = NULL;
	minScale = 0.1f;
	maxScale = 1.0f;
	showHeight = 640;
	showWidth = 960;
	cancelSaveData = CancelSaveData::create();
	cancelSaveData->setMaxSize(100);
}

HelloWorld::~HelloWorld()
{
	if (gridManagerLayer){
		gridManagerLayer->removeFromParent();
		gridManagerLayer->release();
	}
	if (foreManager){
		foreManager->removeFromParent();
		foreManager->release();
	}
	if (imgSprite){
		imgSprite->removeFromParent();
		imgSprite->release();
	}
	if (adsorbentLayer){
		adsorbentLayer->removeFromParent();
		adsorbentLayer->release();
	}
	if (edgeSprite){
		edgeSprite->removeFromParent();
		edgeSprite->release();
	}
	if (drawNodeSprite){
		drawNodeSprite->removeFromParent();
		drawNodeSprite->release();
	}
	delete cancelSaveData;
}

//http://bbs.csdn.net/topics/50048744
Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
	layerColor = LayerColor::create(Color4B(0, 0, 0, 255), 9999, 9999);

	scene->addChild(layerColor, -1);
    auto layer = HelloWorld::create();
	g_scene = layer;
    scene->addChild(layer);
    
    auto layerUI = Layer::create();
    scene->addChild(layerUI, 1);
    layer->autoSaveLabel = Label::createWithSystemFont(FontChina::G2U("自动保存中!"), "Arial", 12);
    layerUI->addChild(layer->autoSaveLabel, uiLayer);
    layer->packerLabel = Label::createWithSystemFont(FontChina::G2U("打包图片中!"), "Arial", 12);
    layerUI->addChild(layer->packerLabel, uiLayer);
    layer->packerLabel->setVisible(false);
    layer->autoSaveLabel->setVisible(false);
    
    return scene;
}

void HelloWorld::setPackerLabelVisible(bool isVisible)
{
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	packerLabel->setVisible(isVisible);
	packerLabel->setPosition(frameSize.width - 50, 25);
}

void HelloWorld::setAutoSaveLabelVisible(bool isVisible)
{
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	autoSaveLabel->setPosition(frameSize.width - 50, 50);
	autoSaveLabel->setVisible(isVisible);
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	gridManagerLayer = GridManagerLayer::create();
	addChild(gridManagerLayer, gridLayer);
	foreManager = ForeManager::create();
	this->addChild(foreManager, smallbgLayer);
    this->scheduleUpdate();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	addChild(WinEventDispatcher::getInstance());
#endif
	this->addKeyEvent();
	this->addTouchEvent();
	this->addMouseEvent();

#if (_DEBUG && (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32))
	autoSaveTime = 60.0f;
	this->schedule(schedule_selector(HelloWorld::saveConf), autoSaveTime);
#else
	autoSaveTime = 300.0f;
#endif
	

	//这里开始会把层的纹理删除掉
	DeleteDirectory((FileUtils::getInstance()->getSearchPaths().at(0) + "dynamic_texture").c_str());
    return true;
}

void HelloWorld::saveConf(float dt)
{
	if (!isInit || QTOutput::inOutputConf || g_openConfile){
		return;
	}
	QTOutput* output = new QTOutput();
	QString backdir = "../Resources/backup";
	QDir dir;
	if (!dir.exists(backdir)){
		dir.mkdir(backdir);
	}
	time_t timep;
	time(&timep);
	struct tm *tm = localtime(&timep);
	char currentTime[256];
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int sec = tm->tm_sec;
	QDir dir2(backdir);
	dir2.setFilter(QDir::Files | QDir::NoSymLinks);
	QFileInfoList list = dir2.entryInfoList();
	int file_count = list.count();
	QStringList string_list;
	for (int i = 0; i < list.size(); i++)
	{
		QFileInfo file_info = list.at(i);
		QString suffix = file_info.suffix();
		if (QString::compare(suffix, QString("QConf"), Qt::CaseInsensitive) == 0)
		{
			QString absolute_file_path = file_info.absoluteFilePath();
			string_list.append(absolute_file_path);
		}
	}
	sprintf(currentTime, "-%d-%d-%d-%d-%d-%d-", year, month, day, hour, min, sec);
	QString fileName = "../Resources/backup/Default";
	fileName += currentTime;
	fileName += ".QConf";
	MyLogger::getInstance()->addInfo("the HelloWorld::saveConf be called, the conf name is " + fileName);
	if (string_list.size() >= 10){
		int minYear;
		int minMonth;
		int minDay;
		int minHour;
		int minMin;
		int minSec;
		int minindex = 0;
		
		int maxYear;
		int maxMonth;
		int maxDay;
		int maxHour;
		int maxSec;
		int maxMin;
		int maxindex1 = 0, maxindex2 = -1;
		for (int i = 0; i < string_list.size(); i++){
			QString str = string_list.at(i).section("/", -1).section(".", 0, -2);
			int year = str.section("-", 1, 1).toInt();
			int month = str.section("-", 2, 2).toInt();
			int day = str.section("-", 3, 3).toInt();
			int hour = str.section("-", 4, 4).toInt();
			int min = str.section("-", 5, 5).toInt();
			int sec = str.section("-", 6, 6).toInt();
			if (i == 0){
				minYear = maxYear = year;
				minMonth = maxMonth = month;
				minDay = maxDay = day;
				minHour = maxHour = hour;
				minMin = maxMin = min;
				minSec = maxSec = sec;
				minindex = 0;
				maxindex1 = 0;
			}
			else{
				int findmin = false;
				if (year < minYear){
					findmin = true;
					minindex = i;
				}
				else if (year == minYear && month < minMonth){
					findmin = true;
					minindex = i;
				}
				else if (year == minYear && month == minMonth && day < minDay){
					findmin = true;
					minindex = i;
				}
				else if (year == minYear && month == minMonth && day == minDay && hour < minHour){
					findmin = true;
					minindex = i;
				}
				else if (year == minYear && month == minMonth && day == minDay && hour == minHour && min < minMin){
					findmin = true;
					minindex = i;
				}
				else if (year == minYear && month == minMonth && day == minDay && hour == minHour && min == minMin && sec < minSec){
					findmin = true;
					minindex = i;
				}
				if (findmin){minYear = year;minMonth = month;minDay = day;minHour = hour;minMin = min;minSec = sec;}

			}
		}
		QFile::remove(string_list.at(minindex));
	}
	std::thread t1(&QTOutput::OutputJsonConf, output, fileName, false);
	t1.detach();
	setAutoSaveLabelVisible(true);
}

void HelloWorld::addMouseEvent()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	EventListenerMouse* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	mouseListener->onMouseScroll = CC_CALLBACK_1(HelloWorld::onMouseScroll, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void HelloWorld::onMouseDown(Event* event)
{
	EventMouse* ee = (EventMouse*)event;
	int type = ee->getMouseButton();
	if (type == GLFW_MOUSE_BUTTON_RIGHT){
		isRightMouseDown = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		prePos = ee->getLocation();
#else
		prePos = MacTransViewPos::getViewTouchPos();
#endif
	}
}

void HelloWorld::onMouseUp(Event* event)
{
	EventMouse* ee = (EventMouse*)event;
	int type = ee->getMouseButton();
	//鼠标右键
	if (type == GLFW_MOUSE_BUTTON_RIGHT){
		if (static_type != Type_Multi_Select_Sprite){
			setType(Type_None);
			g_TemplateFile = "";
			if (imgSprite){
				imgSprite->removeFromParent();
				imgSprite->release();
				imgSprite = nullptr;
				MyLogger::getInstance()->addInfo("the HelloWorld::onMouseUp be called, and the user hold on left mouse, and touch right mouse, clear imgSprite");
			}
			else{
				MyLogger::getInstance()->addInfo("the HelloWorld::onMouseUp be called, user cancel operate");
			}
			
		}
		isRightMouseDown = false;
	}
}
 
//这里相当于调用了一个线程当主进程发生改变的时候线程是不知道的所有有些数值的改变线程不能发现
void HelloWorld::onMouseCallback(float dt)
{
	QTEditor* qe = dynamic_cast<QTEditor*>(g_editor);
	QPoint p;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	p.setX(cursorPos.x);
	p.setY(glview->getFrameSize().height - cursorPos.y);
}

void HelloWorld::onStopMouseCallback()
{
	unschedule(schedule_selector(HelloWorld::onMouseCallback));
}

void HelloWorld::onMouseMove(Event* event)
{
	EventMouse* e = (EventMouse*)event;
	cursorPos = Point(e->getCursorX(), e->getCursorY());
	unschedule(schedule_selector(HelloWorld::onMouseCallback));
	scheduleOnce(schedule_selector(HelloWorld::onMouseCallback), 2.0f);
	if (static_type == Type_Multi_Select_Sprite && isRightMouseDown){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		Point curPos = e->getLocation();
#else
		Point curPos = MacTransViewPos::getViewTouchPos();
#endif
		Point size = curPos - prePos;
		size.y = -size.y;
		foreManager->moveMultiSelectSprite(size);
		prePos = curPos;
	}
}

void HelloWorld::onMouseScroll(Event* event)
{
 	EventMouse* ee = (EventMouse*)event;
}

void HelloWorld::addTouchEvent()
{
	touchManagerLayer = TouchManager::create();
	this->addChild(touchManagerLayer, touchManLayer);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void HelloWorld::update(float dt)
{
	//addLog(dt);
}

bool HelloWorld::setOperatorMoveIfNeed(Point pos)
{
	if ((static_type == Type_Move_Foce_Image_Child || static_type == Type_None || (static_type == Type_Multi_Select_Sprite&& !is_Alt_Key)
		|| static_type == Type_Move_Multi_Select_Sprite) && foreManager)
	{
		auto size = Director::getInstance()->getOpenGLView()->getFrameSize();
		static_type = Type_Move_Foce_Image_Child;
		bool multiResult = foreManager->isTouchMultiSelect(pos);
		bool result = foreManager->ChooseSpriteChildFromPosition(pos);
		if (multiResult){
			setCocCursor(Cursor_Type::Move);
			setType(Type_Move_Multi_Select_Sprite);
			if (foreManager->getMultiSelect().size() > 1){
				if (edgeSprite)edgeSprite->endDraw();
			}
		}
		else if(foreManager->getTargetSpriteChild() && result){
			if (foreManager)foreManager->clearDraw();
			if (gridManagerLayer)gridManagerLayer->clearDraw();
			auto sprite = foreManager->getTargetSpriteChild();
			auto layer = dynamic_cast<ImageSpriteLayer*>(sprite->getParent());
			LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
			layerManager->setCurrentSpriteChild(layer->getTagName(), sprite->getTagName());
			showNodeEdge(layer->getTagName(), sprite->getTagName());
			auto controlView = dynamic_cast<ControlView*>(g_ControlView);
			controlView->setValueFromChildSprite(sprite);
			setCocCursor(Cursor_Type::Move);
		}
		else{
			if (foreManager)foreManager->clearDraw();
			if (gridManagerLayer)gridManagerLayer->clearDraw();
			if (edgeSprite)edgeSprite->endDraw();
			setType(Type_None);
		}
		return true;
	}
	return false;
}

GridManagerLayer* HelloWorld::getGridManagerLayer()
{
	return gridManagerLayer;
}

ForeManager* HelloWorld::getForeManager()
{
	return foreManager;
}

Point HelloWorld::getSceneScalePos(Point pos)
{
	Point relPos;
	auto frameSize = Size(g_CocosWindowInitSize.width(), g_CocosWindowInitSize.height());//Size(960,640);
	auto screenSize = frameSize * this->getScale();
	relPos = pos - (frameSize / 2 - screenSize/2) - this->getPosition();
	auto point = relPos / this->getScale();
	return point;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (is_Space_Key){
		return true;
	}
	MyLogger::getInstance()->addInfo("the HelloWorld::onTouchBegan be called, the type is " + QString::number(static_type));
	canMoveSprite = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	startPos = getSceneScalePos(touch->getLocation());
#else
	startPos = getSceneScalePos(MacTransViewPos::getViewTouchPos());
#endif
	
	unusualOnTouchBegn();
	preMovedPos = startPos;
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	float width = startPos.x;
	float height = glview->getFrameSize().height - startPos.y;
	if (g_CocosWindowInitSize.height() != getNewWindowSize().height()){
		height += getOldWindowSize().height() - getNewWindowSize().height();
	}
	gridManagerLayer->setStartPos(startPos);
	setOperatorMoveIfNeed(startPos);
	switch (static_type){
	case Type_Italic:
		//创建斜体时设置起始触摸点type = 1
		gridManagerLayer->setItalicPosAndTouchPos(1);
		break;
	case Type_Foce_Image_Layer:
		break;
	case Type_Move_Foce_Image_Layer:
		if (foreManager){
			foreManager->setStartPos(startPos);
		}
		edgeSprite->startPos(startPos);
		break;
	case Type_Delete_Block:
		gridManagerLayer->setDeleteSpriteStartPos(startPos);
		break;
	case Type_Box:
		gridManagerLayer->setAddSpriteStartPos(startPos);
		break;
	case Type_Select_Direction:
		gridManagerLayer->setSelectDirectionSpriteStartPos(startPos);
		break;
	case Type_Foce_Image_Child:
		onTouchBeganImageSprite(startPos);
		break;
	case Type_Move_Foce_Image_Child:
		if (foreManager){
			foreManager->setStartPos(startPos);
		}
		if (adsorbentLayer == NULL && gridManagerLayer->getDrawNode()){
			adsorbentLayer = AdsorbentLayer::create();
			this->addChild(adsorbentLayer, touchSpriteLayer);
			adsorbentLayer->setGrid(row, col, gridH, gridW);
		}
		if (is_Shift_Key && adsorbentLayer){
			adsorbentLayer->setShowLine(foreManager->getTargetSpriteChild()->getPosition() + foreManager->getTargetLayer()->getPosition(), 
				foreManager->getTargetSpriteChild());
		}
		//用于数据回滚
		cancelSaveData->addSaveData(foreManager->getTargetSpriteChild());
		edgeSprite->startPos(startPos);
		break;
	case Type_Delete_Foce_Image_Child:
		break;
	case Type_Multi_Select_Block:
		gridManagerLayer->setMultiSelectBlockStartPos(startPos);
		break;
	case Type_Multi_Select_Sprite:
		foreManager->setMultiSelectSpriteStartPos(startPos);
		break;
	case Type_Move_Multi_Select_Sprite:
		cancelSaveData->addSaveData(foreManager->getMultiSelect());
		if (foreManager->getMultiSelect().size() == 1){
			edgeSprite->startPos(startPos);
		}
		break;
	}
	int col_cur = width / gridW;
	int row_cur = height / gridH;
	char str[256];
	sprintf(str, "触摸点位置:(%f %f)\n网格位置:(row:%d, col:%d)\n当前网格的tag为%d", width, height, row_cur, col_cur, row_cur*this->col + col_cur);
	setAttributeInfo(str);
	return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{ 
	if (is_Space_Key){
		if (imgSprite){
			imgSprite->removeFromParent();
			imgSprite->release();
			imgSprite = NULL;
		}
		return;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	movedPos = getSceneScalePos(touch->getLocation());
#else
	movedPos = getSceneScalePos(MacTransViewPos::getViewTouchPos());
#endif
	unusualOnTouchMoved();
	
	gridManagerLayer->setMovedPos(movedPos);
	switch (static_type){
	case Type_Italic:
		gridManagerLayer->setTouchSpriteMovedPos(movedPos);
		break;
	case Type_Foce_Image_Layer:
		break;
	case Type_Move_Foce_Image_Layer:
		if (foreManager){
			foreManager->MoveLayer(movedPos);
		}
		edgeSprite->move(movedPos);
		break;
	case Type_Delete_Block:
		gridManagerLayer->setDeleteSpriteMovedPos(movedPos);
		break;
	case Type_Box:
		gridManagerLayer->setAddSpriteMovedPos(movedPos);
		break;
	case Type_Select_Direction:
		gridManagerLayer->setSelectDirectionSpriteMovedPos(movedPos);
		break;
	case Type_Foce_Image_Child:
		onTouchMovedImageSprite(movedPos);
		break;
	case Type_Move_Foce_Image_Child:
		if (movedPos.distance(startPos) > 10){
			canMoveSprite = true;
		}
		if (canMoveSprite){
			if (foreManager){
				foreManager->MoveSpriteChildFromLayer(movedPos);
			}
			edgeSprite->move(movedPos);
		}
		if (is_Shift_Key && adsorbentLayer){
			adsorbentLayer->setShowLine(foreManager->getTargetSpriteChild()->getPosition() + foreManager->getTargetLayer()->getPosition(), foreManager->getTargetSpriteChild());
		}
		break;
	case Type_Delete_Foce_Image_Child:
		break;
	case Type_Multi_Select_Block:
		gridManagerLayer->setMultiSelectBlockMovedPos(movedPos);
		break;
	case Type_Multi_Select_Sprite:
		foreManager->setMultiSelectSpriteMovedPos(movedPos);
		break;
	case Type_Move_Multi_Select_Sprite:
		if (movedPos.distance(startPos) > 10){
			canMoveSprite = true;
		}
		if (canMoveSprite){
			foreManager->moveMultiSelectSprite(movedPos - preMovedPos);
			if (foreManager->getMultiSelect().size() == 1){
				edgeSprite->move(movedPos);
			}
		}
		break;
	}
	preMovedPos = movedPos;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{ 
	if (is_Space_Key){
		if (imgSprite){
			imgSprite->removeFromParent();
			imgSprite->release();
			imgSprite = NULL;
		}
		return;
	}
	MyLogger::getInstance()->addInfo("the HelloWorld::onTouchEnded be called, the type is " + QString::number(static_type));
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	endedPos = getSceneScalePos(touch->getLocation());
#else
	endedPos = getSceneScalePos(MacTransViewPos::getViewTouchPos());
#endif
	unusualOnTouchEnded();
	gridManagerLayer->setEndedPos(endedPos);
	switch (static_type){
	case Type_Italic:
		gridManagerLayer->setTouchSpriteEndedPos(endedPos);
		break;
	case Type_Foce_Image_Layer:
		break; 
	case Type_Move_Foce_Image_Layer:
		break;
	case Type_Delete_Block:
		gridManagerLayer->setDeleteSpriteEndedPos(endedPos);
		break;
	case Type_Box:
		gridManagerLayer->setAddSpriteEndedPos(endedPos);
		break;
	case Type_Select_Direction:
		gridManagerLayer->setSelectDirectionSpriteEndedPos(endedPos);
		break;
	case Type_Foce_Image_Child:
		onTouchEndedImageSprite(startPos, endedPos);
		break;
	case Type_Move_Foce_Image_Child:
		if (endedPos.distance(startPos) > 10){
			canMoveSprite = true;
		}
		if (foreManager && canMoveSprite){
			Point pos = endedPos;
			if (is_Shift_Key && adsorbentLayer && adsorbentLayer->getIsDraw()){
				pos = adsorbentLayer->getTargetPos();
				foreManager->setSpriteChildPositionFromLayer(pos - foreManager->getTargetLayer()->getPosition());
			}
			else{
				foreManager->MoveSpriteChildFromLayer(pos);
			}
			foreManager->setMultiSelectSpriteSingleSprite(foreManager->getTargetSpriteChild());
		}
		if (adsorbentLayer){
			adsorbentLayer->setIsDraw(false);
		}
		break;
	case Type_Delete_Foce_Image_Child:
		break;
	case Type_Multi_Select_Block:
		gridManagerLayer->setMultiSelectBlockEndedPos(endedPos);
		break;
	case Type_Multi_Select_Sprite:
		if (edgeSprite)edgeSprite->endDraw();
		foreManager->setMultiSelectSpriteEndedPos(endedPos);
		break;
	case Type_Move_Multi_Select_Sprite:
		break;
	}
}

void HelloWorld::unusualOnTouchBegn()
{
	copyOperToQT();
}

void HelloWorld::unusualOnTouchMoved()
{
	showWindowsRange(movedPos);
}

void HelloWorld::unusualOnTouchEnded()
{

}

void HelloWorld::copyOperToQT()
{
	if (!is_O_Key)return;
	else static_type = Type_None;
}

void HelloWorld::onTouchBeganImageSprite(Point pos)
{
	if (imgSprite == NULL){
		imgSprite = DragImageSprite::create(g_filePath);
		this->addChild(imgSprite, dragLayer);
		if (g_TemplateFile != ""){
			imgSprite->getStateProperties();
		}
	}
	if (adsorbentLayer == NULL && gridManagerLayer->getDrawNode()){
		adsorbentLayer = AdsorbentLayer::create();
		this->addChild(adsorbentLayer, touchSpriteLayer);
		adsorbentLayer->setGrid(row, col, gridH, gridW);
	}
	imgSprite->setPosition(pos);
	if (is_Shift_Key && adsorbentLayer){
		adsorbentLayer->setShowLine(imgSprite->getPosition(), imgSprite);
	}
}

void HelloWorld::onTouchMovedImageSprite(Point pos)
{
	if (!imgSprite)return;
	imgSprite->setPosition(pos);
	if (is_Shift_Key && adsorbentLayer){
		adsorbentLayer->setShowLine(imgSprite->getPosition(), imgSprite);
	}
}

void HelloWorld::onTouchEndedImageSprite(Point stPos, Point enPos)
{
	if (!imgSprite)return;
	imgSprite->setPosition(enPos);
	if (stPos.distance(enPos) > 5){
		LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
		QString spritename;
		if (g_ImageType == Image_Type::NormalBlock){
			spritename = ImageSprite::imagePrefixQString;
		}
		else if (g_ImageType == Image_Type::AnimationBlock){
			spritename = ImageSprite::animationPrefixQString;
		}
		else{
			spritename = ImgTransPath::getRealPathByIndex(ImgTransPath::getIndexByListImage(g_filePath.c_str()));
			spritename = spritename.section("/", -1).section(".", 0, -2);
			spritename += "_";
		}
		spritename += QString::number(ImageSprite::getMaxId());
		Point pos = imgSprite->getPosition();
		if (is_Shift_Key && adsorbentLayer && adsorbentLayer->getIsDraw()){
			pos = adsorbentLayer->getTargetPos();
		}
		int  sprite_Type = 0;
		if (g_ImageType == Image_Type::Entity){
			sprite_Type = 1;
		}
		else if (g_ImageType == Image_Type::AnimationBlock){
			sprite_Type = 2;
		}
		MyLogger::getInstance()->addInfo("add Sprite to layer: info layer name is %s, sprite tag is %s, filename is %s, type is " + foreManager->getData().toString() + spritename
			+ QString(g_filePath.c_str()) + QString::number(sprite_Type));
		foreManager->addSpriteChildFromLayer(pos - foreManager->getTargetLayer()->getPosition(), foreManager->getData().toString().toStdString(), g_filePath, spritename.toUtf8().data(), layerManager->getCurrentRootChildIndex(), g_ImageType);
		ImageSprite* sprite = foreManager->getImageChildFromTag(foreManager->getData().toString().toStdString(), spritename.toUtf8().data());
		//模板的赋值
		imgSprite->setTemplateToImageSprite(sprite);
		QString layername = foreManager->getTargetLayer()->getTagName().c_str();
		layerManager->addItemSpriteByNode(layername, sprite, sprite->getZOrder(), false);
		ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
		controlView->setValueFromChildSprite(sprite);
		auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
		//把sprite保存到template中
		templateView->itemAddSprite(sprite);
		if (sprite->getTemplateFile() == ""){
			controlView->setInitState();
		}
	}
	imgSprite->removeFromParent();
	imgSprite->release();
	imgSprite = NULL;
	if (adsorbentLayer){
		adsorbentLayer->setIsDraw(false);
	}
}

std::vector<ImageSpriteLayer*> HelloWorld::getAllImageLayer()
{
	if (foreManager){
		//用于在qteditor中生成配置文件的时候
		return foreManager->getALLImageLayer();
	}
	else{
		std::vector<ImageSpriteLayer*> s;
		return s;
	}
}

void HelloWorld::changeSpriteChildTemplate_All(std::string md5, std::string new_templateFile)
{
	if (foreManager){
		MyLogger::getInstance()->addInfo("the HelloWorld::changeSpriteChildTemplate_All be called, the args is " + md5 + " " + new_templateFile);
		foreManager->changeSpriteChildTemplate_All(md5, new_templateFile);
	}
}

//http://www.eoeandroid.com/thread-548140-1-1.html
void HelloWorld::addKeyEvent()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	EventListenerKeyboard* keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode){
	case EventKeyboard::KeyCode::KEY_TAB:
	{
		float oldScale = this->getScale();
		if (oldScale > minScale){
			this->setScale(minScale);
			this->setPosition(this->getPosition()*(this->getScale() / oldScale));
		}
		else{
			this->setScale(maxScale);
			this->setPosition(this->getPosition()*(this->getScale() / oldScale));
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_TAB");
	}
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		setShiftKeyStates(true);
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_SHIFT");
		break;
	case EventKeyboard::KeyCode::KEY_ALT:
		setType(Type_Multi_Select_Sprite);
		setAltKeyStates(true);
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_ALT");
		break;
	case EventKeyboard::KeyCode::KEY_CTRL:
		setType(Type_Multi_Select_Block);
		is_Ctrl_Key = true;
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_CTRL");
		break;
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
		resetState();
		g_TemplateFile = "";
		setCocCursor(Cursor_Type::Normal);
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_BACKSPACE");
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		is_Space_Key = true;
		setTouchManagerState(true);
		setCocCursor(Cursor_Type::Move_Scene);
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_SPACE");
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->pressUp();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			cancelSaveData->addSaveData(foreManager->getMultiSelect());
			foreManager->pressedUp(is_Alt_Key, gridH);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_UP_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->pressDown();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			cancelSaveData->addSaveData(foreManager->getMultiSelect());
			foreManager->pressedDown(is_Alt_Key, gridH);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_DOWN_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->pressLeft();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			cancelSaveData->addSaveData(foreManager->getMultiSelect());
			foreManager->pressedLeft(is_Alt_Key, gridW);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_LEFT_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->pressRight();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			cancelSaveData->addSaveData(foreManager->getMultiSelect());
			foreManager->pressedRight(is_Alt_Key, gridW);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_RIGHT_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_C:
		if (is_Alt_Key && foreManager && static_type == Type_Multi_Select_Sprite){
			foreManager->copySpriteFromMultiSelect();
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_C");
		break;
	case EventKeyboard::KeyCode::KEY_P:
		is_P_Key = true;
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_P");
		break;
	case EventKeyboard::KeyCode::KEY_O:
		is_O_Key = true;
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_O");
		break;
	case EventKeyboard::KeyCode::KEY_Z:
		if (is_Alt_Key){
			cancelSaveData->pressAltZ();
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_Z");
		break;
	case EventKeyboard::KeyCode::KEY_S:
		if (is_Ctrl_Key){
			auto editor = dynamic_cast<QTEditor*>(g_editor);
			editor->saveConf(MySqlLiteManager::getInstance()->getConfigFile());
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_S");
		break;
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode){
	case EventKeyboard::KeyCode::KEY_SHIFT:
		setShiftKeyStates(false);
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_SHIFT");
		break;
	case EventKeyboard::KeyCode::KEY_ALT:
    case EventKeyboard::KeyCode::KEY_COMMA:
		setAltKeyStates(false);
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_ALT");
		break;
	case EventKeyboard::KeyCode::KEY_CTRL:
		setType(Type_None);
		is_Ctrl_Key = false;
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_CTRL");
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		is_Space_Key = false;
		setTouchManagerState(false);
		setCocCursor(getOldCursorType());
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_SPACE");
		break;
	case EventKeyboard::KeyCode::KEY_DELETE:
		if (static_type == Type_Multi_Select_Sprite){
			cancelSaveData->addDeleteData(foreManager->getMultiSelect());
			foreManager->deleteMultiSelect();
		}
		else if (foreManager->getTargetSpriteChild()){
			auto sprite = foreManager->getTargetSpriteChild();
			cancelSaveData->addDeleteData(sprite);
			deleteSpriteFromForeManagerFromLayerManagerFromTemplateView(foreManager->getTargetLayer()->getTagName(), foreManager->getTargetSpriteChild()->getTagName(), sprite);
		}
		else if (static_type == Type_Move_Foce_Image_Layer){
			auto layer = foreManager->getTargetLayer();
			deleteLayerFromForeManagerFromLayerManagerFromTemplateView(layer->getTagName());
		}
		if (edgeSprite)edgeSprite->clearDraw();
		setCocCursor(Cursor_Type::Normal);
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_DELETE");
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->releaseUp();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			foreManager->releaseUp(is_Alt_Key, gridH);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_UP_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->releaseDown();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			foreManager->releaseDown(is_Alt_Key, gridH);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_DOWN_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->releaseLeft();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			foreManager->releaseLeft(is_Alt_Key, gridW);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_LEFT_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (static_type == Type_Multi_Select_Block){
			gridManagerLayer->releaseRight();
		}
		else if (static_type == Type_Multi_Select_Sprite && foreManager){
			foreManager->releaseRight(is_Alt_Key, gridW);
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_RIGHT_ARROW");
		break;
	case EventKeyboard::KeyCode::KEY_P:
		is_P_Key = false;
		deleteWindowsRangeSprite();
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_P");
		break;
	case EventKeyboard::KeyCode::KEY_O:
		is_O_Key = false;
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyReleased be called , the KeyCode is KEY_O");
		break;
	case EventKeyboard::KeyCode::KEY_V:
		if (is_Alt_Key && foreManager){
			foreManager->PasteSpriteFromMultiSelect();
		}
		MyLogger::getInstance()->addInfo("the HelloWorld::onKeyPressed be called , the KeyCode is KEY_V");
		break;
	default:
	{
		auto editor = dynamic_cast<QTEditor*>(g_editor);
		editor->getKeyBoardMappingDialog()->catchedKeyEvent(keyCode);
	}
		break;
	}
}

void HelloWorld::createGrid(int row, int col, int height, int width, bool byWinSize)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::createGrid be called, the args is " + QString::number(row) + " " 
		+ QString::number(col) + " " + QString::number(height) + " " + QString::number(width) + " " + QString::number(byWinSize));
	this->row = row;
	this->col = col;
	this->gridWidth = width;
	this->gridHeight = height;
	gridW = width;
	gridH = height;
	gridManagerLayer->createGrid(row, col, height, width, byWinSize);
	if (adsorbentLayer){
		adsorbentLayer->removeFromParent();
		adsorbentLayer->release();
		adsorbentLayer = NULL;
	}
	foreManager->getLayerMoveRangeManage()->setGridData(width, height);
	isInit = true;
}

bool HelloWorld::addGrid(int count, int direction)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::addGrid be called, the args is " + QString::number(count) + " " + QString::number(direction));
	gridManagerLayer->row = this->row;
	gridManagerLayer->col = this->col;
	gridManagerLayer->addGrid(count, direction);
	switch (direction){
	case 0:
		if (foreManager)foreManager->addGridResetPos(count, direction, gridH);
		break;
	case 1:
		if (foreManager)foreManager->addGridResetPos(count, direction, gridH);
		break;
	case 2:
		if (foreManager)foreManager->addGridResetPos(count, direction, gridW);
		break;
	case 3:
		if (foreManager)foreManager->addGridResetPos(count, direction, gridW);
		break;
	}
	return true;
}

void HelloWorld::deleteLayerFromForeManager(std::string tagname)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::deleteLayerFromForeManager be called, the args is " + tagname);
	foreManager->removeLayer(tagname);
	if (edgeSprite)edgeSprite->clearDraw();
}

void HelloWorld::deleteSprtteChildFromForeManager(std::string parent, std::string tagname)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::deleteSprtteChildFromForeManager be called, the args is " + parent + " " + tagname);
	foreManager->removeSpriteChild(parent, tagname);
	if (edgeSprite)edgeSprite->clearDraw();
	foreManager->clearDraw();
}

void HelloWorld::changeSpriteChildTagFromForeManager(std::string parent, std::string oldtag, std::string newtag)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::changeSpriteChildTagFromForeManager be called, the args is " + parent + " " + oldtag + " " + newtag);
	foreManager->changeSpriteChildTagName(parent, oldtag, newtag);
}

void HelloWorld::changeLayerTagNameFromForeManager(std::string oldtag, std::string newtag)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::changeLayerTagNameFromForeManager be called, the args is " + oldtag + " " + newtag);
	foreManager->changeLayerTagName(oldtag, newtag);
}

void HelloWorld::changeSpriteChildZOrderFromForeManager(std::string parent, std::string tagname, int z_order)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::changeSpriteChildZOrderFromForeManager be called, the args is " + parent + " " + tagname + " " + QString::number(z_order).toStdString());
	foreManager->changeSpriteChildZOrder(parent, tagname, z_order);
}

void HelloWorld::changeLayerZOrderFromForeManager(std::string tagname, int z_order)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::changeLayerZOrderFromForeManager be called, the args is " + tagname + " " + QString::number(z_order).toStdString());
	foreManager->changeLayerZOrder(tagname, z_order);
}

ImageSpriteLayer* HelloWorld::getLayerFromForeManager(std::string tagname)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::getLayerFromForeManager be called, the args is " + tagname);
	return foreManager->getImageLayerFromTag(tagname);
}

ImageSprite* HelloWorld::getSpriteChildFromForeManager(std::string parent, std::string tagname)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::getSpriteChildFromForeManager be called, the args is " + parent + " " + tagname);
	return foreManager->getImageChildFromTag(parent, tagname);
}

void HelloWorld::addForeImageLayer(std::string tagname, int zorder)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::addForeImageLayer be called, the args is " + tagname + " " + QString::number(zorder).toStdString());
	foreManager->addImageLayer(tagname, zorder);
}

void HelloWorld::deleteSpriteFromForeManagerFromLayerManagerFromTemplateView(std::string parent, std::string tagname, ImageSprite* sprite)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::deleteSpriteFromForeManagerFromLayerManagerFromTemplateView be called, the args is " + parent + " " + tagname);
	LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	layerManager->deleteSpriteChildItem(parent, tagname);
	auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	templateView->itemDeleteSprite(sprite);
	deleteSprtteChildFromForeManager(parent, tagname);
	auto layer = getLayerFromForeManager(parent);
	layer->removeSpriteOver();
}

void HelloWorld::deleteLayerFromForeManagerFromLayerManagerFromTemplateView(std::string tagname)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::deleteLayerFromForeManagerFromLayerManagerFromTemplateView be called, the args is " + tagname);
	auto layer = foreManager->getImageLayerFromTag(tagname);
	if (layer){
		LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
		layerManager->deleteSpriteLayerItem(layer->getTagName());
		foreManager->removeLayer(layer->getTagName());
	}
}

void HelloWorld::setLayerVisibleFromForeManager(std::string name, bool isVisible)
{
	foreManager->setLayerVisible(name, isVisible);
}

void HelloWorld::setSpriteChildVisibleFromForeManager(std::string parent, std::string name, bool isVisible)
{
	foreManager->setSpriteChildVisible(parent, name, isVisible);
}

bool HelloWorld::setLayerStateFromForeManager(std::string name, bool state)
{
	auto layer = foreManager->getImageLayerFromTag(name);
	if (layer){
		layer->setisSportState(state);
		return true;
	}
	else{
		return false;
	}
	
}

std::vector<std::string> HelloWorld::getAllImage()
{
	MyLogger::getInstance()->addInfo("the HelloWorld::getAllImage be called");
	std::vector<std::string> vec;
	if (foreManager){
		vec = foreManager->getAllImage();
	}
	return vec;
}

std::vector<std::string> HelloWorld::getAllImageNoStatic()
{
	MyLogger::getInstance()->addInfo("the HelloWorld::getAllImageNoStatic() be called");
	return foreManager->getAllImageNoStatic();
}

std::vector<std::string> HelloWorld::getAllStaticLayerImage()
{
	MyLogger::getInstance()->addInfo("the HelloWorld::getAllStaticLayerImage() be called");
	return foreManager->getAllStaticLayerImage();
}

std::vector<std::vector<std::string>> HelloWorld::getAllStaticImageLayerByLayer()
{
	MyLogger::getInstance()->addInfo("the HelloWorld::getAllStaticImageLayerByLayer() be called");
	return foreManager->getALLStaticImageLayerByLayer();
}

std::vector<animateData> HelloWorld::getAllAnimateData()
{
	MyLogger::getInstance()->addInfo("the HelloWorld::getAllAnimateData() be called");
	return foreManager->getAllAnimateData();
}

bool HelloWorld::hasAnimationSprite()
{
	MyLogger::getInstance()->addInfo("the HelloWorld::hasAnimationSprite() be called");
	return foreManager->hasAnimationSprite();
}
void HelloWorld::setShiftKeyStates(bool isShift)
{
	is_Shift_Key = isShift;
}

void HelloWorld::setSpaceKeyStates(bool isSpace)
{
	is_Space_Key = isSpace;
	setTouchManagerState(isSpace);
}

void HelloWorld::setAltKeyStates(bool isAlt)
{
	is_Alt_Key = isAlt;
}

void HelloWorld::setCtrlKeyStates(bool isCtrl)
{
	is_Ctrl_Key = isCtrl;
}

void HelloWorld::setPKeyStates(bool state)
{
	is_P_Key = state;
}

void HelloWorld::setOKeyStates(bool state)
{
	is_O_Key = state;
}

void HelloWorld::moveSceneToPos(Point pos)
{
	touchManagerLayer->sceneMoveToScenePos(pos);
}

void HelloWorld::deleteForeManager()
{
	if (foreManager){
		foreManager->removeFromParent();
		foreManager->release();
		foreManager = NULL;
	}
}

void HelloWorld::createForeManager()
{
	if (foreManager == NULL){
		foreManager = ForeManager::create();
		this->addChild(foreManager, smallbgLayer);
	}
}

void HelloWorld::showNodeEdge(std::string parent, std::string name, bool isParent)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::showNodeEdge be called, the args is " + parent + name);
	if (!edgeSprite){
		edgeSprite = EdgeSprite::create();
		this->addChild(edgeSprite, touchSpriteLayer);
	}
	if (!isParent){
		auto sprite = foreManager->getImageChildFromTag(parent, name);
		auto layer = foreManager->getImageLayerFromTag(parent);
		Size size;
		size.width = sprite->getContentSize().width * sprite->anchr.x * sprite->getScaleX();
		size.height = sprite->getContentSize().height * sprite->anchr.y * sprite->getScaleY();
		edgeSprite->startDraw(sprite->getPosition() + layer->getPosition() - size,
			Size(sprite->getContentSize().width* sprite->getScaleX(), sprite->getContentSize().height*sprite->getScaleY()));
	}
	else{
		auto layer = foreManager->getImageLayerFromTag(parent);
		Point* data = layer->getLayerRange();
		Size size = Size(data[1].x - data[0].x, data[1].y - data[0].y);
		edgeSprite->startDraw(data[0], size);
		delete[]data;
	}
}

void HelloWorld::resetState()
{
	static_type = Type_None;
}

void HelloWorld::setTouchManagerState(bool state)
{
	if (state == true){
		touchManagerLayer->startTouchManage();
	}
	else{
		touchManagerLayer->endTouchManage();
	}
}

void HelloWorld::setType(int Type, QVariant data)
{
	MyLogger::getInstance()->addInfo("the HelloWorld::setType be called, the args is " + QString::number(Type));
	static_type = Type;
	switch (Type){
		//对于Type_Select_Direction来说他还需要一个参数指定方向
	case Type_Select_Direction:
		setCocCursor(Cursor_Type::Normal);
	{
			int type = data.toInt();
			gridManagerLayer->setSelectDirectionType(type);
	}
		break;
	case Type_Foce_Image_Child:
		setCocCursor(Cursor_Type::addSprite);
		foreManager->setData(data);
		break;
	case Type_Move_Foce_Image_Child:
		setCocCursor(Cursor_Type::Move);
	{
		QStringList list = data.toStringList();
		foreManager->setData(data);
		foreManager->setChooseSpriteChild(list.at(0).toStdString(), list.at(1).toStdString());
		break;
	}
	case Type_Move_Foce_Image_Layer:
		setCocCursor(Cursor_Type::Move);
		foreManager->setChooseLayer(data.toString().toStdString());
		foreManager->setData(data);
		break;
	case Type_Box:
		setCocCursor(Cursor_Type::Block);
		break;
	case Type_Italic:
		setCocCursor(Cursor_Type::Block);
		break;
	case Type_Fluid:
		setCocCursor(Cursor_Type::Block);
		break;
	case Type_Multi_Select_Block:
		setCocCursor(Cursor_Type::Ctrl);
		break;
	case Type_Multi_Select_Sprite:
		setCocCursor(Cursor_Type::Alt);
		break;
	default:
		setCocCursor(Cursor_Type::Normal);
		break;
	}
}

void HelloWorld::setOptionShowOrHide(int type)
{
	switch (type){
	case 0:
		//显示图片
		if (foreManager){
			foreManager->setVisible(true);
		}
		break;
	case 1:
		//显示网格
		if (gridManagerLayer){
			gridManagerLayer->setVisible(true);
		}
		break;
	case 2:
		//隐藏图片
		if (foreManager){
			foreManager->setVisible(false);
		}
		break;
	case 3:
		//隐藏网格
		if (gridManagerLayer){
			gridManagerLayer->setVisible(false);
		}
	case 4:
		if (edgeSprite)edgeSprite->setVisible(true);
		foreManager->getMultiSelectSprite()->setVisible(true);
		break;
	case 5:
		if (edgeSprite)edgeSprite->setVisible(false);
		foreManager->getMultiSelectSprite()->setVisible(false);
		break;
		

	}
}

int HelloWorld::getType()
{
	return static_type;
}

void HelloWorld::setInit(bool isIni)
{
	isInit = isIni;
}

bool HelloWorld::getIsInit()
{
	return isInit;
}

void HelloWorld::setLayerColor(Color3B b)
{
	layerColor->setColor(b);
}

Color3B HelloWorld::getLayerColor()
{
	return layerColor->getColor();
}

void HelloWorld::setGridColor(Color4B b)
{
	if (gridManagerLayer){
		gridManagerLayer->setDrawColor(b);
	}
}

Color4B HelloWorld::getGridColor()
{
	if (gridManagerLayer)return gridManagerLayer->getDrawColor();
	return Color4B();
}

void HelloWorld::showWindowsRange(Point pos)
{
	if (!is_P_Key)return;
	else static_type = Type_None;
	if (!drawNodeSprite){
		drawNodeSprite = DrawNodeSprite::create(showWidth, showHeight);
		this->addChild(drawNodeSprite, dragLayer);
	}
	drawNodeSprite->setPosition(pos);
}

void HelloWorld::deleteWindowsRangeSprite()
{
	if (drawNodeSprite){
		drawNodeSprite->Delete();
		drawNodeSprite = NULL;
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
