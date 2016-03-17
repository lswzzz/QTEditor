#include "ForeManager.h"
#include "Global.h"
#include "LayerManagerWidget.h"
#include "TemplateView.h"
#include "EntityImageSprite.h"
#include "MyLogger.h"

ForeManager::ForeManager()
{
	targetLayer = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	__data = 0;
#else
    __data = NULL;
#endif
	targetSpriteChild = NULL;
	multiSelectSprite = MultiselectSprite::create();
	addChild(multiSelectSprite, 9999);
	layerMoveRangeManager = LayerMoveRangeManage::create();
	addChild(layerMoveRangeManager, 10);
	inOpenState = false;
	clear = true;
}

ForeManager::~ForeManager()
{
	multiSelectSprite->removeFromParent();
	for (int i = 0; i < _Imagevec.size(); i++){
		auto layer = _Imagevec.at(i);
		layerMoveRangeManager->deleteDrawRect(layer->getTagName());
		layer->removeFromParent();
		layer->release();
	}
}

ForeManager* ForeManager::create()
{
	auto ret = new ForeManager();
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

bool ForeManager::addImageLayer(std::string tagname, int zorder)
{
	auto layer = ImageSpriteLayer::create(tagname, this->inOpenState);
	if (layer){
		this->addChild(layer, zorder);
		layer->setPosition(0, 0);
		layer->setAnchorPoint(Point(0, 0));
		_Imagevec.push_back(layer);
		targetLayer = layer;
		layerMoveRangeManager->addDrawData(tagname);
		return true;
	}
	return false;
}

void ForeManager::changeSpriteChildTemplate_All(std::string md5, std::string new_templateFile)
{
	std::vector<ImageSpriteLayer*> vec = getALLImageLayer();
	for (int i = 0; i < vec.size(); i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(vec.at(i));
		Vector<Node*> node_vec = layer->getAllSpriteNode();
		for (int k = 0; k < node_vec.size(); k++){
			auto sprite = dynamic_cast<ImageSprite*>(node_vec.at(k));
			if (sprite->getTemplateMD5() == md5 && md5 != ""){
				sprite->setTemplateFile(new_templateFile, md5);
			}
			if (new_templateFile == ""){
				auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
				auto item = layerManager->getSpriteTreeItem(layer->getTagName(), sprite->getTagName());
				if (item){ item->setIcon(1, QIcon(":/icon/Resources/template.ico")); }
			}
		}
	}
}

ImageSprite* ForeManager::addSpriteChildFromLayer(Point pos, std::string parent, std::string filename, std::string tag, int zorder, Image_Type type_)
{	
	auto layer = getImageLayerFromTag(parent);
	if (layer){
		ImageSprite* sprite;
		if (g_openConfile){
			sprite = layer->addSpriteInOpenFile(pos, filename, tag, type_);
		}
		else{
			sprite = layer->addImageSprite(pos, filename, tag, type_);
		}
		layer->changeSpriteZOrderFromImageLayer(filename, zorder);
		layer->addSpriteOver();
		return sprite;
	}
	return NULL;
}

bool ForeManager::setChooseLayer(std::string name)
{
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = _Imagevec.at(i);
		if (layer->getTagName() == name){
			targetLayer = layer;
			return true;
		}
	}
	return false;
}

bool ForeManager::setChooseSpriteChild(std::string parent, std::string child)
{
	auto sprite = getImageChildFromTag(parent, child);
	auto layer = getImageLayerFromTag(parent);
	if (sprite)
	{
		targetSpriteChild = sprite;
		targetLayer = layer;
		return true;
	}
	return false;
}

ImageSpriteLayer* ForeManager::getImageLayerFromTag(std::string tagname)
{
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = _Imagevec.at(i);
		if (layer->getTagName() == tagname){
			return layer;
		}
	}
	return NULL;
}

void ForeManager::setStartPos(Point pos)
{
	startPos = pos;
}

bool ForeManager::MoveLayer(Point pos)
{
	movedPos = pos;
	if (targetLayer){
		Point oldPos = targetLayer->getPosition();
		targetLayer->setPosition(oldPos - startPos + movedPos);
	}
	startPos = movedPos;
	return true;
}

bool ForeManager::MoveSpriteChildFromLayer(Point pos)
{
	movedPos = pos;
	if (targetSpriteChild){
		Point oldPos = targetSpriteChild->getPosition();
		targetSpriteChild->setPosition(oldPos - startPos + movedPos);
	}
	startPos = movedPos;
	return true;
}

void ForeManager::setSpriteChildPositionFromLayer(Point pos)
{
	targetSpriteChild->setPosition(pos);
}

//如果层被锁定的话就不能选中了
bool ForeManager::ChooseSpriteChildFromPosition(Point pos)
{
	LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	std::string name = layerManager->getCurrentLayerName();
	auto layer = getImageLayerFromTag(name);
	if (!layer){
		targetSpriteChild = NULL;
		return false;
	}
	auto vec = layer->getAllSpriteNode();
	if (!layer->isLock()){
		for (int k = 0; k < vec.size(); k++){
			auto sprite = dynamic_cast<ImageSprite*>(vec.at(k));
			Rect rect = Rect(pos.x - 1 - layer->getPositionX(), pos.y - 1 - layer->getPositionY(), 2, 2);
			if (sprite->getBoundingBox().intersectsRect(rect)){
				targetLayer = dynamic_cast<ImageSpriteLayer*>(sprite->getParent());
				targetSpriteChild = sprite;
				layerManager->setCurrentSpriteChild(name, targetSpriteChild->getTagName());
				return true;
			}
		}
	}
	
	for (int i = 0; i < _Imagevec.size(); i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(_Imagevec.at(i));
		if (!layer->isLock()){
			auto vec = layer->getAllSpriteNode();
			for (int k = 0; k < vec.size(); k++){
				auto sprite = dynamic_cast<ImageSprite*>(vec.at(k));
				Rect rect = Rect(pos.x - 1 - layer->getPositionX(), pos.y - 1 - layer->getPositionY(), 2, 2);
				if (sprite->getBoundingBox().intersectsRect(rect)){
					targetLayer = dynamic_cast<ImageSpriteLayer*>(sprite->getParent());
					targetSpriteChild = sprite;
					layerManager->setCurrentSpriteChild(layer->getTagName(), targetSpriteChild->getTagName());
					return true;
				}
			}
		}
	}
	targetSpriteChild = NULL;
	return false;
}

void ForeManager::setMultiSelectSpriteStartPos(Point pos)
{
	multiSelectSprite->setStartPos(pos);
}

void ForeManager::setMultiSelectSpriteMovedPos(Point pos)
{
	multiSelectSprite->setMovedPos(pos);
}

void ForeManager::setMultiSelectSpriteEndedPos(Point pos)
{
	multiSelectSprite->setEndedPos(pos);
	if (targetLayer && !targetLayer->isLock()){
		if (multiSelectSprite->getVec().size() == 1){
			auto sprite = multiSelectSprite->getVec().at(0);
			if (sprite == targetSpriteChild){
				multiSelectSprite->clearDrawNode();
			}
		}
		clear = multiSelectSprite->MultiSelect(targetLayer->getAllSpriteNode());
	}
	else{
		clear = true;
	}
}

void ForeManager::setMultiSelectSpriteSingleSprite(ImageSprite* sprite)
{
	clear = multiSelectSprite->addSimpleNode(sprite);
}

bool ForeManager::isTouchMultiSelect(Point pos)
{
	return multiSelectSprite->isTouchMultiSelectSprite(pos);
}

void ForeManager::clearDraw()
{
	clear = true;
	multiSelectSprite->clearDrawNode();
}

void ForeManager::moveMultiSelectSprite(Point size)
{
	multiSelectSprite->moveSize(size);
}

void ForeManager::addGridResetPos(int count, int direction, int gridSize)
{
	std::vector<ImageSpriteLayer*> vec = getALLImageLayer();
	for (int i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		Vector<Node*> spritevec = layer->getAllSpriteNode();
		for (int j = 0; j < spritevec.size(); j++){
			auto sprite = dynamic_cast<ImageSprite*>(spritevec.at(j));
			switch (direction){
			case 0:
				sprite->setPosition(Point(sprite->getStartPos().x, sprite->getStartPos().y - gridSize * count));
				break;
			case 1:
				break;
			case 2:
				sprite->setPosition(Point(sprite->getStartPos().x + gridSize * count, sprite->getStartPos().y));
				break;
			case 3:
				break;
			}
		}
	}
	layerMoveRangeManager->addGrid(count, direction);
}

void ForeManager::copySpriteFromMultiSelect()
{
	_copyVec.clear();
	_copyVec = multiSelectSprite->getVec();
}

void ForeManager::PasteSpriteFromMultiSelect()
{
	Vector<Node*> vec;
	std::vector<ImageSprite*> spriteVec;
	bool isOtherLayer = false;
	auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	std::string layername = layerManager->getCurrentLayerName();
	QString qLayername = QString(layername.c_str());
	auto layer = dynamic_cast<ImageSpriteLayer*>(this->getImageLayerFromTag(layername));
	for (int i = 0; i < _copyVec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(_copyVec.at(i));
		ImageSprite* othSprite;
		auto entitySprite = EntityImageSprite::create();
		auto entitySpriteSource = dynamic_cast<EntityImageSprite*>(sprite);
		entitySpriteSource->copySprite(entitySprite);
		othSprite = entitySprite;
		if (isOtherLayer == false){
			auto oldSpriteParent = dynamic_cast<ImageSpriteLayer*>(sprite->getParent());
			auto oldSpriteParentName = oldSpriteParent->getTagName();
			if (oldSpriteParentName != layername) isOtherLayer = true;
		}
		layerManager->addItemSpriteByNode(qLayername, othSprite, othSprite->getZOrder(), true);
		vec.pushBack(othSprite);
		spriteVec.push_back(othSprite);
	}
	if (isOtherLayer){
		layer->addOtherLayerCopySprites(spriteVec, true);
	}
	else{
		for (int i = 0; i < spriteVec.size(); i++){
			auto sprite = spriteVec.at(i);
			layer->addCopySpriteCurLayer(sprite);
			sprite->setAnchorPoint(sprite->anchr);
		}
	}
	layer->addSpriteOver();

	//切换复制的对象
	multiSelectSprite->setVec(vec);
	_copyVec = vec;
}

Vector<Node*> ForeManager::getMultiSelect()
{
	return multiSelectSprite->getVec();
}

void ForeManager::deleteMultiSelect()
{
	LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	ImageSpriteLayer* targetLayer = nullptr;
	Vector<Node*> vec = multiSelectSprite->getVec();
	for (int i = 0; i < vec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(vec.at(i));
		if (i == 0){
			targetLayer = dynamic_cast<ImageSpriteLayer*>(sprite->getParent());
		}
		layerManager->deleteSpriteChildItem(targetLayer->getTagName(), sprite->getTagName());
		templateView->itemDeleteSprite(sprite);
		removeSpriteChild(targetLayer->getTagName(), sprite->getTagName());
	}
	targetLayer->removeSpriteOver();
	clearDraw();
}

bool ForeManager::setZOrderFromImageLayerTagName(std::string tagname, int zorder)
{
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = _Imagevec.at(i);
		if (layer->getTagName() == tagname){
			layer->setZOrder(zorder);
			return true;
		}
	}
	return false;
}

bool ForeManager::setZOrderFromImageChildTagName(std::string parenttag, std::string tagname, int zorder)
{
	auto layer = getImageLayerFromTag(parenttag);
	if (layer){
		layer->changeSpriteZOrderFromImageLayer(tagname, zorder);
		return true;
	}
	return false;
}

ImageSprite* ForeManager::getImageChildFromTag(std::string parenttag, std::string tagname)
{
	auto layer = getImageLayerFromTag(parenttag);
	if (layer){
		auto sprite = layer->getSpriteFromTagName(tagname);
		return sprite;
	}
	return NULL;
}

ImageSprite* ForeManager::getImageChildFromTag(std::string taname)
{
	auto layer = getTargetLayer();
	std::string parentName = layer->getTagName();
	if (layer){
		parentName = layer->getTagName();
		auto vec = layer->getAllSpriteNode();
		for (int i = 0; i < vec.size(); i++){
			auto sprite = dynamic_cast<ImageSprite*>(vec.at(i));
			if (sprite->getTagName() == taname){
				return sprite;
			}
		}
	}
	auto layerVec = getALLImageLayer();
	for (int i = 0; i < layerVec.size(); i++){
		auto layer = layerVec.at(i);
		if (layer->getTagName() != parentName){
			auto vec = layer->getAllSpriteNode();
			for (int i = 0; i < vec.size(); i++){
				auto sprite = dynamic_cast<ImageSprite*>(vec.at(i));
				if (sprite->getTagName() == taname){
					return sprite;
				}
			}
		}
	}
	return NULL;
}

ImageSprite* ForeManager::getImageChildFromId(unsigned int id)
{
	auto layer = getTargetLayer();
	std::string parentName = layer->getTagName();
	if (layer){
		parentName = layer->getTagName();
		auto vec = layer->getAllSpriteNode();
		for (int i = 0; i < vec.size(); i++){
			auto sprite = dynamic_cast<ImageSprite*>(vec.at(i));
			if (sprite->getId() == id){
				return sprite;
			}
		}
	}
	auto layerVec = getALLImageLayer();
	for (int i = 0; i < layerVec.size(); i++){
		auto layer = layerVec.at(i);
		if (layer->getTagName() != parentName){
			auto vec = layer->getAllSpriteNode();
			for (int i = 0; i < vec.size(); i++){
				auto sprite = dynamic_cast<ImageSprite*>(vec.at(i));
				if (sprite->getId() == id){
					return sprite;
				}
			}
		}
	}
	return 0;
}

bool ForeManager::removeLayer(std::string name)
{
	auto layer = getImageLayerFromTag(name);
	if (!layer){
		return false;
	}
	layerMoveRangeManager->deleteDrawRect(layer->getTagName());
	layer->removeFromParent();
	layer->release();
	for (std::vector<ImageSpriteLayer*>::iterator itr = _Imagevec.begin(); itr != _Imagevec.end(); itr++)
	{
		if (*itr == layer)
		{
			_Imagevec.erase(itr);
			return true;
		}
	}
	return false;
}

bool ForeManager::removeSpriteChild(std::string parent, std::string name)
{
	auto layer = getImageLayerFromTag(parent);
	if (layer){
		layer->removeSpriteFromTagName(name);
		return true;
	}
	return false;
}

bool ForeManager::changeLayerTagName(std::string oldtag, std::string newtag)
{
	auto layer = getImageLayerFromTag(oldtag);
	if (layer){
		layer->setTagName(newtag);
		layerMoveRangeManager->resetDrawData(oldtag, newtag);
		return true;
	}
	return false;
}

bool ForeManager::changeSpriteChildTagName(std::string parent, std::string oldtag, std::string newtag)
{
	auto layer = getImageLayerFromTag(parent);
	if (layer){
		layer->changeSpriteTag(oldtag, newtag);
		return true;
	}
	return false;
}

bool ForeManager::changeLayerZOrder(std::string name, int zorder)
{
	auto layer = getImageLayerFromTag(name);
	if (layer){
		layer->setZOrder(zorder);
		return true;
	}
	return false;
}

bool ForeManager::changeSpriteChildZOrder(std::string parent, std::string name, int zorder)
{
	auto layer = getImageLayerFromTag(parent);
	if (layer){
		layer->changeSpriteZOrderFromImageLayer(name, zorder);
		return true;
	}
	return false;
}

std::vector<ImageSpriteLayer*> ForeManager::getALLImageLayer()
{
	std::vector<ImageSpriteLayer*> vec;
	int size = _Imagevec.size();
	auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	for (int i = 0; i < size; i++){
		auto layername = layerManager->getLayerNameByIndex(i);
		auto layer = getImageLayerFromTag(layername);
		vec.push_back(layer);
	}
	return vec;
}

void ForeManager::setLayerVisible(std::string name, bool isVisible)
{
	auto layer = getImageLayerFromTag(name);
	if (layer){
		layer->setVisible(isVisible);
	}
}

void ForeManager::setSpriteChildVisible(std::string parent, std::string name, bool isVisible)
{
	auto sprite = getImageChildFromTag(parent, name);
	if (sprite){
		sprite->setVisible(isVisible);
	}
}

QVariant ForeManager::getData()
{
	return __data;
}

void ForeManager::setData(QVariant data)
{
	__data = data;
}

std::vector<std::string> ForeManager::getImage(int type)
{
	std::vector<std::string> vec;
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(_Imagevec.at(i));
		if (type == 1){
			if (layer->isSportState() == true)
				continue;
		}
		else if (type == 2){
			if (layer->isSportState() == false)
				continue;
			if (layer->isDontOutput())continue;
		}
		std::vector<std::string> layervec = layer->getAllResourceImage();
		for (int j = 0; j < layervec.size(); j++){
			std::string str = layervec.at(j);
			int vecsize = vec.size();
			bool repeat = false;
			if (vecsize == 0){
				vec.push_back(str);
			}
			else{
				for (int k = 0; k < vec.size(); k++){
					if (strcmp(vec.at(k).c_str(), str.c_str()) == 0){
						repeat = true;
						break;
					}
				}
				if (repeat == false){
					vec.push_back(str);
				}
			}
		}
	}
	return vec;
}

bool ForeManager::hasAnimationSprite()
{
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(_Imagevec.at(i));
		if (layer->isHasAnimationSprite())return true;
	}
	return false;
}

std::vector<animateData> ForeManager::getAllAnimateData()
{
	std::vector<animateData> vec;
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(_Imagevec.at(i));
		std::vector<animateData> animatevec = layer->getAllAnimateData();
		for (int j = 0; j < animatevec.size(); j++){
			animateData data = animatevec.at(j);
			std::string animName = data.animName;
			int vecsize = vec.size();
			bool repeat = false;
			if (vecsize == 0){
				vec.push_back(data);
			}
			else{
				for (int k = 0; k < vec.size(); k++){
					animateData vecdata = vec.at(k);
					std::string vecanimName = vecdata.animName;
					if (animName == vecanimName){
						repeat = true;
						break;
					}
				}
				if (repeat == false){
					vec.push_back(data);
				}
			}
		}
	}
	return vec;
}

std::vector<std::vector<ImageSpriteLayer*>*> ForeManager::getAllStaticMergeLayerVec()
{
	std::vector<std::vector<ImageSpriteLayer*>*> vec;
	int static_index = 0;
	auto layerVec = this->getALLImageLayer();
	bool isBreak = false;
	for (int i = 0; i < layerVec.size(); i++){
		auto layer = layerVec.at(i);
		auto curChilds = layer->getAllResourceImage();
		if (layer->isSportState() || curChilds.size() == 0 || layer->isDontOutput()){
			layer->setStaticIndex(-1);
			if (vec.size() > 0)isBreak = true;
			continue;
		}
		layer->setStaticIndex(static_index++);
		if (vec.size() == 0){
			auto layerChild = new std::vector<ImageSpriteLayer*>();
			layerChild->push_back(layer);
			vec.push_back(layerChild);
			isBreak = false;
		}
		else if(isBreak == false){
			auto othLayer = vec.at(vec.size() - 1)->at(0);
			if (layer->isSameProperties(othLayer)){
				vec.at(vec.size() - 1)->push_back(layer);
			}
			else{
				auto layerChild = new std::vector<ImageSpriteLayer*>();
				layerChild->push_back(layer);
				vec.push_back(layerChild);
				isBreak = false;
			}
		}
		else{
			auto layerChild = new std::vector<ImageSpriteLayer*>();
			layerChild->push_back(layer);
			vec.push_back(layerChild);
			isBreak = false;
		}
	}
	return vec;
}

void ForeManager::deleteStaticMergeLayerVecData(std::vector<std::vector<ImageSpriteLayer*>*> vec)
{
	for (int i = 0; i < vec.size(); i++){
		auto child = vec.at(i);
		delete child;
	}
}

std::vector<std::string> ForeManager::getAllImageNoStatic()
{
	return getImage(2);
}

std::vector<std::string> ForeManager::getAllStaticLayerImage()
{
	return getImage(1);
}

std::vector<std::vector<std::string>> ForeManager::getALLStaticImageLayerByLayer()
{
	std::vector<std::vector<std::string>> vec;
	int size = _Imagevec.size();
	int static_index = 0;
	for (int i = 0; i < size; i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(_Imagevec.at(i));
		if (layer->isSportState() == true)
			continue;
		layer->setStaticIndex(static_index++);
		std::vector<std::string> layervec = layer->getAllResourceImage();
		vec.push_back(layervec);
	}
	return vec;
}

std::vector<std::string> ForeManager::getAllStaticImageLayerTagName()
{
	std::vector<std::string> vec;
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(_Imagevec.at(i));
		if (layer->isSportState() == true)
			continue;
		vec.push_back(layer->getTagName());
	}
	return vec;
}

std::vector<std::vector<std::string>> ForeManager::getAllDynamicImageLayerByLayer()
{
	std::vector<std::vector<std::string>> vec;
	int size = _Imagevec.size();
	for (int i = 0; i < size; i++){
		auto layer = dynamic_cast<ImageSpriteLayer*>(_Imagevec.at(i));
		if (layer->isSportState() == false)
			continue;
		std::vector<std::string> layervec = layer->getAllResourceImage();
		vec.push_back(layervec);
	}
	return vec;
}

std::vector<std::string> ForeManager::getAllImage()
{
	return getImage(0);
}

ImageSprite* ForeManager::getTargetSpriteChild()
{
	return targetSpriteChild;
}

ImageSpriteLayer* ForeManager::getTargetLayer()
{
	return targetLayer;
}

void ForeManager::setLayerLockState(std::string layername, bool lock)
{
	auto layer = getImageLayerFromTag(layername);
	layer->setLock(lock);
}

void ForeManager::pressedUp(bool is_Alt_Key, int size)
{
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	if (is_Alt_Key){
		this->moveMultiSelectSprite(Size(0, size));
	}else{
		this->moveMultiSelectSprite(Size(0, 1));
	}
	key_type = Up;
	this->is_Alt_Key = is_Alt_Key;
	move_size = size;
	this->scheduleOnce(schedule_selector(ForeManager::keyPressedCallBack), 1.0f);
}

void ForeManager::pressedDown(bool is_Alt_Key, int size)
{
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	if (is_Alt_Key){
		this->moveMultiSelectSprite(Size(0, -size));
	}
	else{
		this->moveMultiSelectSprite(Size(0, -1));
	}
	key_type = Down;
	this->is_Alt_Key = is_Alt_Key;
	move_size = size;
	this->scheduleOnce(schedule_selector(ForeManager::keyPressedCallBack), 1.0f);
}

void ForeManager::pressedLeft(bool is_Alt_Key, int size)
{
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	if (is_Alt_Key){
		this->moveMultiSelectSprite(Size(-size, 0));
	}
	else{
		this->moveMultiSelectSprite(Size(-1, 0));
	}
	key_type = Left;
	this->is_Alt_Key = is_Alt_Key;
	move_size = size;
	this->scheduleOnce(schedule_selector(ForeManager::keyPressedCallBack), 1.0f);
}

void ForeManager::pressedRight(bool is_Alt_Key, int size)
{
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	if (is_Alt_Key){
		this->moveMultiSelectSprite(Size(size, 0));
	}
	else{
		this->moveMultiSelectSprite(Size(1, 0));
	}
	key_type = Right;
	this->is_Alt_Key = is_Alt_Key;
	move_size = size;
	this->scheduleOnce(schedule_selector(ForeManager::keyPressedCallBack), 1.0f);
}

void ForeManager::releaseUp(bool is_Alt_Key, int size)
{
	key_type = None_type;
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	this->unschedule(schedule_selector(ForeManager::keyCallBack));
}

void ForeManager::releaseDown(bool is_Alt_Key, int size)
{
	key_type = None_type;
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	this->unschedule(schedule_selector(ForeManager::keyCallBack));
}

void ForeManager::releaseLeft(bool is_Alt_Key, int size)
{
	key_type = None_type;
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	this->unschedule(schedule_selector(ForeManager::keyCallBack));
}

void ForeManager::releaseRight(bool is_Alt_Key, int size)
{
	key_type = None_type;
	this->unschedule(schedule_selector(ForeManager::keyPressedCallBack));
	this->unschedule(schedule_selector(ForeManager::keyCallBack));
}

void ForeManager::keyPressedCallBack(float dt)
{
	this->schedule(schedule_selector(ForeManager::keyCallBack), 0.2f/3.0f);
}

void ForeManager::keyCallBack(float dt)
{
	switch (key_type){
	case Up:
		if (is_Alt_Key){
			this->moveMultiSelectSprite(Size(0, move_size));
		}
		else{
			this->moveMultiSelectSprite(Size(0, 1));
		}
		break;
	case Down:
		if (is_Alt_Key){
			this->moveMultiSelectSprite(Size(0, -move_size));
		}
		else{
			this->moveMultiSelectSprite(Size(0, -1));
		}
		break;
	case Left:
		if (is_Alt_Key){
			this->moveMultiSelectSprite(Size(-move_size, 0));
		}
		else{
			this->moveMultiSelectSprite(Size(-1, 0));
		}
		break;
	case Right:
		if (is_Alt_Key){
			this->moveMultiSelectSprite(Size(move_size, 0));
		}
		else{
			this->moveMultiSelectSprite(Size(1, 0));
		}
		break;
	}
}

void ForeManager::setOpenState(bool state)
{
	MyLogger::getInstance()->addInfo("the ForeManager::setOpenState be called, the alllayer openState is " + QString::number(state));
	inOpenState = state;
	auto vec = getALLImageLayer();
	for (int i = 0; i < vec.size(); i++){
		auto layer = vec.at(i);
		layer->setOpenState(state);
	}
}

void ForeManager::updateImageVecOrder(std::vector<std::string> layerVec)
{
	if (layerVec.size() != _Imagevec.size()){
		addConsoleInfo("错误！！！！！！！,显示层的数量与实际层的数量不同");
		return;
	}
	std::map<std::string, ImageSpriteLayer*> map;
	for (int i = 0; i < _Imagevec.size(); i++){
		auto layername = _Imagevec.at(i)->getTagName();
		map.insert(std::pair<std::string, ImageSpriteLayer*>(layername, _Imagevec.at(i)));
	}
	_Imagevec.clear();
	for (int i = 0; i < layerVec.size(); i++){
		_Imagevec.push_back(map[layerVec.at(i)]);
	}
}

bool ForeManager::judgeNeeedUpdateOutputTexture()
{
	auto size = _Imagevec.size();
	bool need = false;
	for (size_t i = 0; i < size; i++){
		auto layer = _Imagevec.at(i);
		if (layer->getNeedOutputUpdateState()){
			need = true;
		}
	}
	return need;
}

void ForeManager::setAllyLayerNeedUpdateOutputState(bool state)
{
	auto size = _Imagevec.size();
	for (size_t i = 0; i < size; i++){
		auto layer = _Imagevec.at(i);
		layer->setNeedOutputUpdate(state);
	}
}