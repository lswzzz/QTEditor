#include "CocEventCenter.h"
#include "ImageSpriteLayer.h"

CocEventCenter* CocEventCenter::__instance = NULL;

CocEventCenter::CocEventCenter()
{
}


CocEventCenter::~CocEventCenter()
{
}

CocEventCenter* CocEventCenter::getInstance()
{
	if (__instance == NULL){
		__instance = new CocEventCenter();
	}
	return __instance;
}

//void CocEventCenter::postImageLayerEvent(Node* layer, void* data)
//{
//	std::unique_lock<std::mutex> lk(mute);
//	ImageSpriteLayer* layer_ = dynamic_cast<ImageSpriteLayer*>(layer);
//	std::string tagname = layer_->getTagName();
//	NotificationCenter::getInstance()->postNotification(tagname, (Ref*)data);
//}