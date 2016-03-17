//
//  MacTransViewPos.cpp
//  QTEditor
//
//  Created by lsw on 15/11/19.
//
//

#include "MacTransViewPos.h"
#include "Global.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
NSView* cocView = nullptr;
NSWindow* q_Window = nullptr;
Vec2 MacTransViewPos::touchPos = Vec2();
Vec2 MacTransViewPos::preTouchPos = Vec2();

void MacTransViewPos::setCocView(id cocview)
{
    cocView = cocview;
}

Vec2 MacTransViewPos::getViewTouchPos()
{
    NSWindow* window = [cocView window];
    NSPoint mousePosition = [window mouseLocationOutsideOfEventStream];
    NSPoint mousePos = [cocView convertPoint:mousePosition fromView:nil];
    float mX = mousePos.x;
    float mY = mousePos.y;
    preTouchPos = touchPos;
    touchPos = Vec2(mX, mY);
    return touchPos;
}

Vec2 MacTransViewPos::getViewPreTouchPos()
{
    return preTouchPos;
}

#endif