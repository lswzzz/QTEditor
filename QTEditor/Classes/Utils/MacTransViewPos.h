//
//  MacTransViewPos.h
//  QTEditor
//
//  Created by lsw on 15/11/19.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)


USING_NS_CC;

class MacTransViewPos{
    
public:
    static cocos2d::Vec2 getViewTouchPos();
    static cocos2d::Vec2 getViewPreTouchPos();
    static void setCocView(id cocview);
private:
    static Vec2 touchPos;
    static Vec2 preTouchPos;
};
#endif

