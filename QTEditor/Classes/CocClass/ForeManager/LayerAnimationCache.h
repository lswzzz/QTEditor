#pragma once
#include "cocos2d.h"

USING_NS_CC;

class LayerAnimationCache
{
public:
	LayerAnimationCache();
	~LayerAnimationCache();
	Animation* getAnimation(std::string animationname);
	void addAnimation(std::string animationname, std::vector<int> animationList);
	void addAnimation(std::string animationname, std::vector<Texture2D*> textureVec, std::vector<Rect> rectVec);
	void clearAnimation();
	void removeAnimation(std::string animationname);
private:
	std::map<std::string, Animation*> animationCache;
};

