#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Global.h"
#include "qteditor.h"
#include "QDir"
#include "MacTransViewPos.h"

USING_NS_CC;

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    auto director = Director::getInstance();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    auto glview = GLViewImpl::create("My Game", m_ParentRect.right - m_ParentRect.left, m_ParentRect.bottom - m_ParentRect.top, m_ParentHwnd);
    g_cocos_HWND = dynamic_cast<GLViewImpl*>(glview)->getHwnd();
    director->setOpenGLView(glview);
#else
    auto glview = GLViewImpl::create("Game");
    director->setOpenGLView(glview);
    NSWindow* _NSWindow = glfwGetCocoaWindow(glview->getWindow());
    NSView* view = [_NSWindow contentView];
    NSView* widgetView = (NSView*)_mainWindow->getUI().MainWidget->winId();
    [_NSWindow close];
    [widgetView addSubview:view];// positioned:NSWindowAbove relativeTo:nil];
	MacTransViewPos::setCocView(view);
    g_cocView = view;
#endif
    
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
    
    // run
    director->runWithScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ((GLViewImpl*)glview)->setVisible(true);
    
    auto frameSize = glview->getFrameSize();
    glview->setFrameSize(frameSize.width, frameSize.height);
    glview->setDesignResolutionSize(frameSize.width, frameSize.height, ResolutionPolicy::NO_BORDER);
    ::SetWindowPos(((GLViewImpl*)glview)->getHwnd(), HWND_TOP, 0, 0, frameSize.width, frameSize.height, SWP_NOCOPYBITS);
	
#endif
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void AppDelegate::setParent(HWND hwnd, RECT rect)
{
	m_ParentHwnd = hwnd;
	m_ParentRect.left = rect.left;
	m_ParentRect.top = rect.top;
	m_ParentRect.right = rect.right;
	m_ParentRect.bottom = rect.bottom;
}
#else
void AppDelegate::setEditor(QTEditor* editor)
{
    _mainWindow = editor;
}

#endif