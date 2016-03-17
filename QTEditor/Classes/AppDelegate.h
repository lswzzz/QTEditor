#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_
#include "cocos2d.h"
#include "CCQApplication.h"

class QTEditor;
/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : public cocos2d::CCQApplication//private cocos2d::Application
{
public:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    AppDelegate(int argc, char *argv[]) : cocos2d::CCQApplication(argc, argv) {};
#else
    AppDelegate(int argc, char *argv[]) : cocos2d::CCQApplication(argc, argv) {
        m_ParentHwnd = NULL;
        m_ParentRect.left = 0;
        m_ParentRect.bottom = 640;
        m_ParentRect.right = 960;
        m_ParentRect.top = 0;
    };
    
#endif
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();

    virtual void applicationDidEnterBackground();

    virtual void applicationWillEnterForeground();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    void setParent(HWND hwnd, RECT rect);
    
private:
    HWND m_ParentHwnd;
    RECT m_ParentRect;
#else
    void setEditor(QTEditor* editor);
private:
    QTEditor* _mainWindow;
#endif
};

#endif // _APP_DELEGATE_H_
