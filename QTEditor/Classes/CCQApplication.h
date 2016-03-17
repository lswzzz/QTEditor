#pragma once

#include "cocos2d.h"
#include <QApplication>

NS_CC_BEGIN

class CCQApplication : public QApplication, public cocos2d::Application
{
    Q_OBJECT
public:
    CCQApplication(int argc, char *argv[]);
    virtual ~CCQApplication();

    /* Run the message loop. */
    int run();

    /* Get current application instance. */
    static CCQApplication* getInstance();

    /* override functions */
	virtual void setAnimationInterval(float interval) override;
    
    void endRun();
public slots:
    void timerUpdate();

protected:
	float _qAnimationInterval;
    static CCQApplication * sm_pSharedApplication;

private:
    QTimer *_timer;
    bool isRun;
};

NS_CC_END

