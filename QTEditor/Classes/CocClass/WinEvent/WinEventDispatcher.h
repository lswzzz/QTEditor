#pragma once
#include "cocos2d.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
class WinEventDispatcher : public Sprite
{
public:
    static WinEventDispatcher* getInstance();
    virtual bool init();
    virtual void update(float dt);
    bool inEditorHWND(HWND hwnd);
    void initEditorHWND();
    void initHook();
    HWND oldHwnd;
    HWND newHwnd;
    HWND curHwnd;
    std::vector<HWND> editorHwndVec;
    
public:
    //LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam);
    //LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    void sceneTouchBegan();
    void sceneTouchMoved();
    void sceneTouchEnded();
    Point getMousePosInCocos();
    bool hasBegin;
    Point startPos;
private:
    WinEventDispatcher();
    ~WinEventDispatcher();
    
};
#endif
