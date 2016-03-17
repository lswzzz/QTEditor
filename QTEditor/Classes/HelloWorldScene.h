#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <qwidget.h>
#include <qobject.h>
#include "ImageSpriteLayer.h"
#include "ForeManager.h"
#include "AdsorbentLayer.h"
#include "DragImageSprite.h"
#include "TouchManager.h"
#include "GridManagerLayer.h"
#include "EdgeSprite.h"
#include "DrawNodeSprite.h"
#include "CancelSaveData.h"


using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
	HelloWorld();

	~HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance cocos2d::Pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	void setPackerLabelVisible(bool isVisible);
	void setAutoSaveLabelVisible(bool isVisible);

	void addTouchEvent();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void onTouchBeganImageSprite(cocos2d::Point pos);
	void onTouchMovedImageSprite(cocos2d::Point pos);
	void onTouchEndedImageSprite(cocos2d::Point stPos, cocos2d::Point enPos);
	
	void unusualOnTouchBegn();
	void unusualOnTouchMoved();
	void unusualOnTouchEnded();
	void copyOperToQT();

	void addMouseEvent();
	void onMouseScroll(Event* event);
	void onMouseUp(Event* event);
	//用于暂停鼠标停止时的提示
	void onMouseCallback(float dt);
	//鼠标停止时的回调方法
	void onStopMouseCallback();
	void onMouseMove(Event* event);
	void onMouseDown(Event* event);

	void addKeyEvent();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	
	virtual void update(float dt);

	//显示或隐藏某些元素
	void setOptionShowOrHide(int type);
	
	std::vector<ImageSpriteLayer*> getAllImageLayer();
	void changeSpriteChildTemplate_All(std::string md5, std::string new_templateFile);
	void deleteLayerFromForeManager(std::string tagname);
	void deleteSprtteChildFromForeManager(std::string parent, std::string tagname);
	void changeSpriteChildTagFromForeManager(std::string parent, std::string oldtag, std::string newtag);
	void changeLayerTagNameFromForeManager(std::string oldtag, std::string newtag);
	void changeSpriteChildZOrderFromForeManager(std::string parent, std::string tagname, int z_order);
	void changeLayerZOrderFromForeManager(std::string tagname, int z_order);
	ImageSpriteLayer* getLayerFromForeManager(std::string tagname);
	ImageSprite* getSpriteChildFromForeManager(std::string parent, std::string tagname);
	void addForeImageLayer(std::string tagname, int zorder);
	void deleteSpriteFromForeManagerFromLayerManagerFromTemplateView(std::string parent, std::string tagname, ImageSprite* sprite);
	void deleteLayerFromForeManagerFromLayerManagerFromTemplateView(std::string tagname);

	//设置Layer是否可见
	void setLayerVisibleFromForeManager(std::string name, bool isVisible);
	//设置子Sprite是否可见
	void setSpriteChildVisibleFromForeManager(std::string parent, std::string name, bool isVisible);
	//设置layer的状态 有静态false 动态true
	bool setLayerStateFromForeManager(std::string name, bool state);
	//返回ForeManager中使用到的所有图片这个方法会遍历一边所有的精灵最后检索出所有使用的图片
	std::vector<std::string> getAllImage();
	std::vector<std::string> getAllImageNoStatic();
	std::vector<std::string> getAllStaticLayerImage();
	std::vector<std::vector<std::string>> getAllStaticImageLayerByLayer();
	bool hasAnimationSprite();
	std::vector<animateData> getAllAnimateData();
	//用于外部的qteditor使用设置ctrl键
	void setShiftKeyStates(bool isShift);
	void setSpaceKeyStates(bool isSpace);
	void setAltKeyStates(bool isAlt);
	void setCtrlKeyStates(bool isCtrl);
	void setPKeyStates(bool state);
	void setOKeyStates(bool state);
	bool getCtrlKeyStates(){ return is_Ctrl_Key; };

	void moveSceneToPos(cocos2d::Point pos);

	//当重新打开一个配置文件的时候需要将ForeManager删除
	void deleteForeManager();
	void createForeManager();
	//检测边缘用于点击treeWidget的时候在cocos中能友好地显示出点击的精灵位置
	void showNodeEdge(std::string parent, std::string name, bool isParent = false);
	//将当前的状态设置为Type_None
	void resetState();
	void setTouchManagerState(bool state);
	bool setOperatorMoveIfNeed(cocos2d::Point pos);
	GridManagerLayer* getGridManagerLayer();
	ForeManager* getForeManager();
	void createGrid(int row, int col, int height, int width, bool byWinSize = true);
	bool addGrid(int count, int direction);
	cocos2d::Point getSceneScalePos(cocos2d::Point pos);

	int getType();

	void setInit(bool isIni);
	bool getIsInit();
	float minScale;
	float maxScale;
	int showHeight;
	int showWidth;
	void setLayerColor(Color3B b);
	Color3B getLayerColor();
	void setGridColor(Color4B b);
	Color4B getGridColor();

	Label* autoSaveLabel;
	Label* packerLabel;

public:

	void saveConf(float dt);
    CREATE_FUNC(HelloWorld);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void setType(int Type, QVariant data = NULL);
#else
	void setType(int Type, QVariant data = 0);
#endif
	
protected:
	void showWindowsRange(cocos2d::Point pos);
	void deleteWindowsRangeSprite();
private:
	GridManagerLayer* gridManagerLayer;
	ForeManager* foreManager;
	DragImageSprite* imgSprite;
	AdsorbentLayer* adsorbentLayer;
	TouchManager* touchManagerLayer;
	EdgeSprite* edgeSprite;
	DrawNodeSprite* drawNodeSprite;
	CancelSaveData* cancelSaveData;
	
	
	int col;
	int row;
	int gridWidth;
	int gridHeight;
	cocos2d::Point startPos;
	cocos2d::Point endedPos;
	cocos2d::Point preMovedPos;
	cocos2d::Point movedPos;
	bool isRightMouseDown;
	cocos2d::Point prePos;
	//是否有创建过网格
	
	bool is_Shift_Key;
	bool is_Space_Key;
	bool is_Ctrl_Key;
	bool is_Alt_Key;
	bool is_P_Key;
	bool is_O_Key;
	bool canMoveSprite;
	friend class QTEditor;
	friend class LayerManagerWidget;
	friend class QTOutput;
	friend class QTInput;
	friend class TouchManager;
	friend class ConfigView;
};

#endif // __HELLOWORLD_SCENE_H__
