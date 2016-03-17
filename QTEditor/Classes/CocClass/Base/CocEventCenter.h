#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class CocEventCenter
{
public:
	CocEventCenter();
	~CocEventCenter();
	static CocEventCenter* getInstance();
private:
	static CocEventCenter* __instance;
};

