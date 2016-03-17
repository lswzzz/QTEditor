#include "LayerAnimationCache.h"
#include "ImgTransPath.h"

LayerAnimationCache::LayerAnimationCache()
{
}


LayerAnimationCache::~LayerAnimationCache()
{
	
}

Animation* LayerAnimationCache::getAnimation(std::string animationname)
{
	auto iter = animationCache.find(animationname);
	if (iter != animationCache.end()){
		return iter->second;
	}
	return nullptr;
}

void LayerAnimationCache::addAnimation(std::string animationname, std::vector<int> animationList)
{
	Animation* animation = getAnimation(animationname);
	if (animation == nullptr){
		animation = new(std::nothrow) Animation();
		animation->init();
		for (size_t i = 0; i < animationList.size(); i++){
			auto image = ImgTransPath::getListImagePathByIndex(animationList.at(i)).toStdString();
			animation->addSpriteFrameWithFile(image);
		}
		animationCache.insert(std::pair<std::string, Animation*>(animationname, animation));
	}
}

void LayerAnimationCache::addAnimation(std::string animationname, std::vector<Texture2D*> textureVec, std::vector<Rect> rectVec)
{
	Animation* animation = getAnimation(animationname);
	if (animation == nullptr){
		animation = new(std::nothrow) Animation();
		animation->init();
		for (size_t i = 0; i < textureVec.size(); i++){
			auto texture = textureVec.at(i);
			auto rect = rectVec.at(i);
			animation->addSpriteFrameWithTexture(texture, rect);
		}
		animationCache.insert(std::pair<std::string, Animation*>(animationname, animation));
	}
}

void LayerAnimationCache::clearAnimation()
{
	for (auto iter = animationCache.begin(); iter != animationCache.end(); iter++){
		auto animation = iter->second;
		animation->release();
	}
	animationCache.clear();
}

void LayerAnimationCache::removeAnimation(std::string animationname)
{
	auto iter = animationCache.find(animationname);
	if (iter != animationCache.end()){
		animationCache.erase(iter);
	}
}
