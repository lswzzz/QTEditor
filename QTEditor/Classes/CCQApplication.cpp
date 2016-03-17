#include "CCQApplication.h"
#include <QTimer>
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import <Cocoa/Cocoa.h>
#include <algorithm>
#import "platform/CCApplication.h"
#include "platform/CCFileUtils.h"
#include "math/CCGeometry.h"
#include "base/CCDirector.h"
#endif

NS_CC_BEGIN

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
static long getCurrentMillSecond()
{
    long lLastTime = 0;
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
    return lLastTime;
}
#endif

/* sharedApplication pointer */
CCQApplication * CCQApplication::sm_pSharedApplication = nullptr;

CCQApplication::CCQApplication(int argc, char *argv[])
    : QApplication(argc, argv)
    , _timer(nullptr)
{
    _qAnimationInterval = 1.0f / 60.0f * 1000.0f;
    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
    isRun = true;
}

CCQApplication::~CCQApplication()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = nullptr;
}

int CCQApplication::run()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (! applicationDidFinishLaunching())
    {
        return 0;
    }
    
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    _timer->start(_qAnimationInterval);
    
    return cocos2d::CCQApplication::getInstance()->exec();
#else
    initGLContextAttrs();
    if(!applicationDidFinishLaunching())
    {
        return 1;
    }
    
    long lastTime = 0L;
    long curTime = 0L;
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    glview->retain();
    
    while (true)
    {
        lastTime = getCurrentMillSecond();
        QApplication::processEvents();
        director->mainLoop();
        glview->pollEvents();
        
        curTime = getCurrentMillSecond();
        if (curTime - lastTime < _animationInterval)
        {
            usleep(static_cast<useconds_t>((_animationInterval - curTime + lastTime)*1000));
        }
        if(!isRun)break;
    }
    
    if (glview->isOpenGLReady())
    {
        director->end();
        director->mainLoop();
    }
    
    return 0;
#endif
}

void CCQApplication::setAnimationInterval(float interval)
{
    _qAnimationInterval = interval * 1000.0f;
    if (_timer)
    {
        _timer->start(_qAnimationInterval);
    }
}

/* static member function */
CCQApplication* CCQApplication::getInstance()
{
    CC_ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

void CCQApplication::timerUpdate()
{;
    Director::getInstance()->mainLoop();
}

void CCQApplication::endRun()
{
    isRun = false;
    Director::getInstance()->end();
    qApp->quit();
}

NS_CC_END

