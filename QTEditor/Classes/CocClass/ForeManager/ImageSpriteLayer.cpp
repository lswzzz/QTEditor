#include "ImageSpriteLayer.h"
#include "Global.h"
#include "TexturePackerView.h"
#include <QDomDocument>
#include <QDir>
#include "MyLogger.h"
#include <exception> 
#include "HelloWorldScene.h"
#include "ResourceView.h"
#include "ImgTransPath.h"
#include "ExteralView.h"
#include "TemplateView.h"
#include "LayerManagerWidget.h"
#include "AnimationView.h"

//std::vector<OutputSpriteData*> ImageSpriteLayer::vecOutputData;
//std::mutex ImageSpriteLayer::m_Outputmutex;

ImageSpriteLayer::ImageSpriteLayer()
{
	updateAnchr = false;
	targetSprite = nullptr;
	sport_state = true;
	updateSprite = false;
	__lock = false;
	isdontoutput = false;
	texturecount = 0;
	static_index = -1;
	moveScaleX = 1.0f;
	moveScaleY = 1.0f;
	sceneScaleFluence = 1.0f;
	filterType = 0;
	inOpenState = false;
	textureManager = nullptr;
	animationCache = new(std::nothrow) LayerAnimationCache;
	inPackerAsyn = false;
	needOutputUpdate = false;
	topTag = 0;
	currentTag = 0;
	layerIndex = 0;
}

ImageSpriteLayer::~ImageSpriteLayer()
{
	delete textureManager;
	delete animationCache;
	removeAllSpriteAndUpdateChange();
	for (auto itr = resMap.begin(); itr != resMap.end(); itr++) {
		delete itr->second;
	}
	resMap.clear();
	for (auto itr = roleMap.begin(); itr != roleMap.end(); itr++) {
		delete itr->second;
	}
	roleMap.clear();
	for (auto itr = animationResMap.begin(); itr != animationResMap.end(); itr++) {
		delete itr->second;
	}
	animationResMap.clear();
}

ImageSpriteLayer* ImageSpriteLayer::create(std::string name, bool inOpenState)
{
	ImageSpriteLayer* ret = new ImageSpriteLayer();
	if (ret){
		ret->init();
		ret->setOpenState(inOpenState);
		ret->setTagName(name);
		ret->setOpacity(80);
	}
	return ret;
}

bool ImageSpriteLayer::init()
{
	Layer::init();
	scheduleUpdate();
	outputDir = "../Resources/dynamic_texture";
	QDir* dir = new QDir();
	if (!dir->exists(outputDir.c_str())){
		dir->mkdir(outputDir.c_str());
	}
	return true;
}

void ImageSpriteLayer::removeSprite(Point pos)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	pos.y = glview->getFrameSize().height - pos.y;

	Vector<Node*> node = this->getChildren();
	int size = node.size();
	for (int i = 0; i < size; i++){
		auto box = node.at(i)->getBoundingBox();
		if (box.containsPoint(pos)){
			ImageSprite* sprite = dynamic_cast<ImageSprite*>(node.at(i));
			char str[256];
			sprintf(str, "删除了一张背景图片");
			addConsoleInfo(str);
			sprite->removeFromParent();
			sprite->release();
			return;
		}
	}
}

void ImageSpriteLayer::NeedUpdateSprite(unsigned int* data)
{
	std::unique_lock<std::recursive_mutex> lk(tagmutex);
	MyLogger::getInstance()->addInfo("the ImageSpriteLayer::NeedUpdateSprite be called");
	texturecount = data[0];
	setCurrentTag(data[1]);
	if (currentTag == topTag){
		updateSprite = true;
	}
}

void ImageSpriteLayer::setTagName(std::string name)
{
	this->tagname = name;
	if (!inOpenState){
		texturePackerCommand(true);
	}
}

std::string ImageSpriteLayer::getTagName()
{
	return tagname;
}

void ImageSpriteLayer::setTargetSprite(std::string tagname)
{
	auto sprite = getSpriteFromTagName(tagname);
	targetSprite = sprite;
}

void ImageSpriteLayer::update(float dt)
{
	if (updateSprite){
		changeAllSpriteToBatch(texturecount);
		updateSprite = false;
	}
}

ImageSprite* ImageSpriteLayer::addImageSprite(Point pos, std::string listFile, std::string tagname, Image_Type type_)
{
	bool isRepeat = false;
	QString file = outputDir.c_str();
	file += "/";
	file += this->tagname.c_str();
 	std::string name;
	int imageIndex;
	imageIndex = ImgTransPath::getIndexByListImage(listFile.c_str());
	name = ImgTransPath::getListPngName(imageIndex).toStdString();
	map<std::string, int>::iterator it = imageIndexMap.find(name);
	if (it != imageIndexMap.end()){
		isRepeat = true;
	}
	if (isRepeat){
		int index = 0;
		index = it->second;
		file += "_";
		file += QString::number(index);
		EntityImageSprite* sprite = AddImageSpriteAddToMapDefaultInfo(imageIndex, pos, tagname, type_);
		bool result = sprite->initWithText(textureManager->getTexture(name), textureManager->getRect(name), textureManager->getPng(name));
		targetSprite = sprite;
		return sprite;
	}
	else if (containsInUnLoadVec(imageIndex))
	{
		EntityImageSprite* sprite = AddImageSpriteAddToMapDefaultInfo(imageIndex, pos, tagname, type_);
		targetSprite = sprite;
		return sprite;
	}
	else{
		EntityImageSprite* sprite = AddImageSpriteAddToMapDefaultInfo(imageIndex, pos, tagname, type_);
		targetSprite = sprite;
		addToUnLoadVec(imageIndex);
		texturePackerCommand(true);
		return sprite;
	}
}

ImageSprite* ImageSpriteLayer::addSpriteInOpenFile(Point pos, std::string realFileName, std::string tagname, Image_Type type__, std::string animationname)
{
	int index = ImgTransPath::getListImageIndex(realFileName.c_str());
	EntityImageSprite* sprite = AddImageSpriteAddToMapDefaultInfo(index, pos, tagname, type__, animationname);
	return sprite;
}

bool ImageSpriteLayer::resetImageSpriteFrameInfo(ImageSprite* sprite)
{
	AnimationView* view = dynamic_cast<AnimationView*>(g_AnimationView);
	std::string animationname = g_animationName;
	std::vector<int> animationlist = view->getTreeWidget()->getSinpleFrameNameList(animationname.c_str());;
	animationCache->addAnimation(animationname, animationlist);
	sprite->setAnimationName(animationname);
	sprite->resetAnimation(animationCache->getAnimation(animationname));
	return addAnimationToMap(animationname, animationlist);
}

bool ImageSpriteLayer::resetImageSpriteFrameInfoInOpen(ImageSprite* sprite, std::string animation)
{
	AnimationView* view = dynamic_cast<AnimationView*>(g_AnimationView);
	std::string animationname;
	std::vector<int> animationlist;
	animationname = animation;
	animationlist = view->getTreeWidget()->getSinpleFrameNameList(animation.c_str());
	animationCache->addAnimation(animationname, animationlist);
	sprite->setAnimationName(animationname);
	sprite->resetAnimation(animationCache->getAnimation(animationname));
	return addAnimationToMap(animationname, animationlist);
}

EntityImageSprite* ImageSpriteLayer::AddImageSpriteAddToMapDefaultInfo(int index, Point pos, std::string tagname, Image_Type type_, std::string animationname)
{
	EntityImageSprite* sprite = EntityImageSprite::create(index);
	resetImageSpriteDefaultBaseInfo(sprite, pos, tagname, type_);
	if (Image_Type::AnimationBlock == type_){
		if (animationname == ""){
			
			resetImageSpriteFrameInfo(sprite);
		}
		else{
			//打开的时候创建的
			resetImageSpriteFrameInfoInOpen(sprite, animationname);
		}
		
	}
	else{
		std::string listpng = ImgTransPath::getListImagePathByIndex(index).toStdString();
		if (Image_Type::Entity == type_){
			addToMap(listpng, true);
		}
		else{
			addToMap(listpng, false);
		}
	}
	sprite->setParentTag(this->getTagName());
	this->addChild(sprite);
	return sprite;
}

void ImageSpriteLayer::resetImageSpriteDefaultBaseInfo(ImageSprite* sprite, Point pos, std::string tagname, Image_Type type_)
{
	if (type_ == Image_Type::NormalBlock){
		sprite->setType(SpriteType::NORMAL_BLOCK);
		sprite->setCanAutoResetResNameInfo(true);
		sprite->setHasEntityProperties(false);
	}
	else if (type_ == Image_Type::AnimationBlock){
		sprite->setType(SpriteType::DYNAMIC_BLOCK);
		sprite->setCanAutoResetResNameInfo(true);
		sprite->setHasEntityProperties(true);
		sprite->setHasAnimation(true);
	}
	else{
		sprite->setType(SpriteType::MONSTER);
		sprite->setCanAutoResetResNameInfo(false);
		sprite->setHasEntityProperties(true);
	}
	sprite->setNewId();
	sprite->setAnchorPoint(ccp(0.5, 0.5));
	sprite->setPosition(pos);
	sprite->setTagName(tagname);
}

bool ImageSpriteLayer::resetImageSpriteBatchInfo(ImageSprite* sprite)
{
	if (sprite->isHasAnimation()){
		return resetImageSpriteSpriteFrame(sprite);
	}
	return resetImageSpriteTexture(sprite);
}

bool ImageSpriteLayer::resetImageSpriteTexture(ImageSprite* sprite)
{
	std::string str = ImgTransPath::getListPngName(sprite).toStdString();
	bool result = sprite->initWithText(textureManager->getTexture(str), textureManager->getRect(str), textureManager->getPng(str));
	sprite->setFlippedX(sprite->flipx);
	sprite->setFlippedY(sprite->flipy);
	sprite->setAnchorPoint(sprite->anchr);
	return result;
}

bool ImageSpriteLayer::resetImageSpriteSpriteFrame(ImageSprite* sprite)
{
	bool result = resetImageSpriteTexture(sprite);
	sprite->resetAnimation(animationCache->getAnimation(sprite->getAnimationName()));
	return result;
}

void ImageSpriteLayer::texturePackerCommand(bool showPackerLabel)
{
	TexturePackerView* view = dynamic_cast<TexturePackerView*>(g_TextureView);
	QString outname = this->tagname.c_str();
	outname += "_";
	std::vector<std::string> vec = this->getAllResourceImageContainsExteral();
	QStringList list;
	for (int i = 0; i < vec.size(); i++){
		list << vec.at(i).c_str();
	}
	if (list.size() > 0){
		needOutputUpdate = true;
		view->PackerAsyn(list, outname, outputDir.c_str(), this, showPackerLabel);
	}
}

void ImageSpriteLayer::addOtherLayerSpriteToSpriteMap(std::map<int, std::vector<ImageSprite*>*>& spriteMap, ImageSprite* sprite)
{
	int index = sprite->getImageIndex();
	auto iter = spriteMap.find(index);
	if (iter == spriteMap.end()){
		std::vector<ImageSprite*>* vec = new std::vector < ImageSprite* > ;
		vec->push_back(sprite);
		spriteMap.insert(std::pair<int, std::vector<ImageSprite*>*>(index, vec));
	}
	else{
		iter->second->push_back(sprite);
	}
}

bool ImageSpriteLayer::addOtherLayerSpriteToLayerMap(std::map<int, std::vector<ImageSprite*>*>& spriteMap, Image_Type type_)
{
	bool hasNewImage = false;
	std::map<int, std::vector<ImageSprite*>*>::iterator iter;
	for (iter = spriteMap.begin(); iter != spriteMap.end(); iter++){
		int index = iter->first;
		auto image = ImgTransPath::getListImagePathByIndex(index).toStdString();
		switch (type_){
		case Image_Type::NormalBlock:
			if (addToMap(image, false)){
				hasNewImage = true;
			}
			break;
		case Image_Type::Entity:
			if (addToMap(image, false)){
				hasNewImage = true;
			}
			break;
		case Image_Type::AnimationBlock:
		{
			auto sprite = iter->second->at(0);
			if (addAnimationToMap(sprite->getAnimationName(), sprite->getAnimationList())){
				hasNewImage = true;
			}
		}
			break;
		}
	}
	return hasNewImage;
}

void ImageSpriteLayer::deleteSpriteMap(std::map<int, std::vector<ImageSprite*>*>& spriteMap)
{
	std::map<int, std::vector<ImageSprite*>*>::iterator iter;
	for (iter = spriteMap.begin(); iter != spriteMap.end(); iter++){
		auto vec = iter->second;
		delete vec;
	}
	spriteMap.clear();
}

//添加从其他层复制过来的精灵
void ImageSpriteLayer::addOtherLayerCopySprites(std::vector<ImageSprite*> spriteVec, bool addToTemplate)
{
	//第一个是资源下标因为first要存入这个layer的map中
	std::map<int, std::vector<ImageSprite*>*> spriteImageMap;
	std::map<int, std::vector<ImageSprite*>*> spriteRoleMap;
	std::map<int, std::vector<ImageSprite*>*> animationSpriteMap;

	for (int i = 0; i < spriteVec.size(); i++){
		auto sprite = spriteVec.at(i);
		if (sprite->getType() == SpriteType::MONSTER || sprite->getType() == SpriteType::ROLE){
			addOtherLayerSpriteToSpriteMap(spriteRoleMap, sprite);
		}
		else if (sprite->isHasAnimation()){
			addOtherLayerSpriteToSpriteMap(animationSpriteMap, sprite);
		}
		else{
			addOtherLayerSpriteToSpriteMap(spriteImageMap, sprite);
		}
	}
	bool hasNewImage = false;
	if (spriteImageMap.size()>0)
	{
		if (addOtherLayerSpriteToLayerMap(spriteImageMap, Image_Type::NormalBlock)){
			hasNewImage = true;
		}
	}
	if (spriteRoleMap.size() > 0)
	{
		if (addOtherLayerSpriteToLayerMap(spriteRoleMap, Image_Type::Entity)){
			hasNewImage = true;
		}
	}
	if (animationSpriteMap.size() > 0)
	{
		if (addOtherLayerSpriteToLayerMap(animationSpriteMap, Image_Type::AnimationBlock)){
			hasNewImage = true;
		}
	}

	deleteSpriteMap(spriteImageMap);
	deleteSpriteMap(spriteRoleMap);
	deleteSpriteMap(animationSpriteMap);

	for (int i = 0; i < spriteVec.size(); i++){
		auto sprite = spriteVec.at(i);
		sprite->setParentTag(this->getTagName());
		this->addChild(sprite);
		if (addToTemplate){
			auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
			templateView->itemAddSprite(sprite);
		}
	}
	
	if (hasNewImage){
		texturePackerCommand(true);
	}
	else{
		for (int i = 0; i < spriteVec.size(); i++){
			auto sprite = spriteVec.at(i);
			resetImageSpriteBatchInfo(sprite);
		}
	}
}

void ImageSpriteLayer::AsynPackerImageInOpenFile()
{
	texturePackerCommand(false);
}

void ImageSpriteLayer::setImageIndex()
{
	imageIndexMap.clear();
	QString filename = outputDir.c_str();
	filename += "/";
	filename += tagname.c_str();
	filename += "_";
	for (int i = 0; i < texturecount; i++){
		QString name = filename;
		name += QString::number(i);
		name += ".plist";
		QFile sFile(name);
		if (filename == "" || !sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
			MyLogger::getInstance()->addError("Error setImageIndex function texture " + name + " not found layername is " + QString(getTagName().c_str()));
			return;
		}
		QDomDocument document;
		if (!document.setContent(&sFile)){
			MyLogger::getInstance()->addError("Error setImageIndex error can't read xml file layername is " + QString(getTagName().c_str()));
			return;
		}
		QDomElement root = document.documentElement();
		QDomElement dict = root.firstChildElement();
		QDomElement dictChild = dict.firstChildElement().nextSiblingElement();
		QDomNodeList child_list = dictChild.childNodes();
		int childCount = child_list.size();
		for (int k = 0; k < childCount; k += 2){
			QDomNode child_Node = child_list.item(k);
			QDomElement ele = child_Node.toElement();
			QString key = ele.text();
			imageIndexMap.insert(std::pair<std::string, int>(key.toStdString(), i));
		}
	}
}

//这里还要修改 支持多张贴图
void ImageSpriteLayer::changeAllSpriteToBatch(int count)
{
	MyLogger::getInstance()->addInfo("the ImageSpriteLayer::changeAllSpriteToBatch be called, the count is " + QString::number(count) + " this layer name is " + QString(this->tagname.c_str()));

	texturecount = count;
	m_filename = outputDir.c_str();
	m_filename += "/";
	m_filename += tagname.c_str();
	m_filename += "_";
	setImageIndex();
	MyLogger::getInstance()->addInfo("in changeAllSpriteToBatch setImageIndex OK");
	for (int i = 0; i < texturecount; i++){
		QString png = m_filename;
		png += QString::number(i);
		png += ".png";
		TextureCache::getInstance()->removeTextureForKey(png.toLocal8Bit().data());
		TextureCache::getInstance()->addImageAsync(png.toLocal8Bit().data(), CC_CALLBACK_1(ImageSpriteLayer::loadingTextureCallback, this));
	}	
	clearLoadedImageIndex();
}

void ImageSpriteLayer::loadingTextureCallback(Texture2D *texture)
{
	static int count = 0;
	count++;
	if (count < texturecount)return;
	count = 0;
	createTextureManager(m_filename, texturecount);
	resetAnimationCache();
	MyLogger::getInstance()->addInfo("in changeAllSpriteToBatch createTextureManager OK");
	Vector<Node*> spriteVec = getAllSpriteNode();
	MyLogger::getInstance()->addInfo("In changeAllSpriteToBatch, start replace sprite texture, the sprite count is " + QString::number(spriteVec.size()));
	int size = spriteVec.size();
	for (int i = 0; i < size; i++){
		try{
			ImageSprite* sprite = dynamic_cast<ImageSprite*>(spriteVec.at(i));
			bool result = resetImageSpriteBatchInfo(sprite);
			if (result == false){
				addConsoleInfo("BatchSprite失败");
			}
		}
		catch (exception& e){
			MyLogger::getInstance()->addError("Error find error sprite , error sprite index " + QString::number(i) + " the layer name is " + QString(this->tagname.c_str()));
			MyLogger::getInstance()->addError("Error  texture name is " + m_filename);
		}
	}
	auto scene = static_cast<HelloWorld*>(g_scene);
	scene->setPackerLabelVisible(false);
}

void ImageSpriteLayer::createTextureManager(QString filename, int count)
{
	delete textureManager;
	textureManager = new ImageTextureManager;
	textureManager->layername = getTagName();
	textureManager->setPackerImage(filename.toStdString(), count);
	textureManager->setImageIndexMap(imageIndexMap);
	if (isNotContainsAnyRes()){
		MyLogger::getInstance()->addError("Error ImageSpriteLayer::createTextureManager imageMap size is 0, layername is " + getTagName());
	}
	for (auto it = resMap.begin(); it != resMap.end(); it++){
		textureManager->addTextureToMap(it->first);
	}
	for (auto it = roleMap.begin(); it != roleMap.end(); it++){
		textureManager->addTextureToMap(it->first);
	}
	for (auto it = animationResMap.begin(); it != animationResMap.end(); it++){
		auto vec = it->second->animationList;
		for (auto iter = vec.begin(); iter != vec.end(); iter++){
			int index = *iter;
			std::string image = ImgTransPath::getListPngName(index).toStdString();
			textureManager->addTextureToMap(image);
		}
	}
}

void ImageSpriteLayer::resetAnimationCache()
{
	animationCache->clearAnimation();
	for (auto iter = animationResMap.begin(); iter != animationResMap.end(); iter++){
		auto animationIndexList = iter->second->animationList;
		auto animatoinname = iter->first;
		int size = animationIndexList.size();
		std::vector<Texture2D*> textureVec;
		std::vector<Rect> rectVec;
		for (auto index : animationIndexList){
			auto image = ImgTransPath::getListPngName(index).toStdString();
			auto texture = textureManager->getTexture(image);
			auto rect = textureManager->getRect(image);
			textureVec.push_back(texture);
			rectVec.push_back(rect);
		}
		animationCache->addAnimation(animatoinname, textureVec, rectVec);
		textureVec.clear();
		rectVec.clear();
	}
}

void ImageSpriteLayer::setOpenState(bool state)
{
	inOpenState = state;
}

void ImageSpriteLayer::setStartPos(Point pos)
{
	start = pos;
}

void ImageSpriteLayer::setTargetSpriteByPoint(Point pos)
{
	Vector<Node*> node = this->getChildren();
	int size = node.size();
	for (int i = 0; i < size; i++){
		auto box = node.at(i)->getBoundingBox();
		if (box.containsPoint(pos)){
			start = pos;
			targetSprite = dynamic_cast<ImageSprite*>(node.at(i));
			return;
		}
	}
}

ImageSprite* ImageSpriteLayer::getSpriteFromTagName(std::string tagname)
{
	Vector<Node*> node = this->getChildren();
	int size = node.size();
	for (int i = 0; i < size; i++){
		auto sprite = dynamic_cast<ImageSprite*>(node.at(i));
		if (sprite->getTagName() == tagname)
			return sprite;
	}
	return NULL;
}

void ImageSpriteLayer::removeSpriteFromTagName(std::string tagname)
{
	auto sprite = getSpriteFromTagName(tagname);
	if (sprite){
		removeSpriteRemoveFromMap(sprite);
	}
}

void ImageSpriteLayer::removeSpriteRemoveFromMap(ImageSprite* sprite)
{
	if (sprite->isHasAnimation()){
		removeFromAnimatiomMap(sprite->getAnimationName());
	}
	else{
		removeFromMap(ImgTransPath::getListPngName(sprite).toStdString());
	}
	sprite->removeFromParent();
	sprite->release();
}

void ImageSpriteLayer::removeAllSpriteAndUpdateChange()
{
	auto vec = getAllSpriteNode();
	auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	for (int i = 0; i < vec.size(); i++){
		auto sprite = static_cast<ImageSprite*>(vec.at(i));
		templateView->itemDeleteSprite(sprite);
		removeSpriteRemoveFromMap(sprite);
	}
	updateChanged();
}

void ImageSpriteLayer::subResRefFromSprite(ImageSprite* sprite)
{
	removeFromMap(ImgTransPath::getListPngName(sprite).toStdString());
	removeChild(sprite);
}

void ImageSpriteLayer::changeSpriteTag(std::string oldtag, std::string newtag)
{
	auto sprite = getSpriteFromTagName(oldtag);
	if (sprite){
		sprite->setTagName(newtag);
	}
}

void ImageSpriteLayer::changeSpriteZOrderFromImageLayer(std::string tagname, int z_order)
{
	auto sprite = getSpriteFromTagName(tagname);
	if (sprite){
		sprite->setZOrder(z_order);
	}
}

void ImageSpriteLayer::MoveSprite(Point pos)
{
	end = pos;
	if (targetSprite){
		Point oldPos = targetSprite->getPosition();
		targetSprite->setPosition(oldPos - start + end);
	}
	start = end;
}

Vector<Node*> ImageSpriteLayer::getAllSpriteNode()
{
	Vector<Node*> node = this->getChildren();
	return node;
}

//这里注意可能有错
std::vector<std::string> ImageSpriteLayer::getAllResourceImage()
{
	std::vector<std::string> vec;
	for (auto iter = resMap.begin(); iter != resMap.end(); iter++){
		vec.push_back(iter->second->path);
	}
	for (auto iter = animationResMap.begin(); iter != animationResMap.end(); iter++){
		auto vec__ = iter->second->animationList;
		for (auto it = vec__.begin(); it != vec__.end(); it++){
			vec.push_back(ImgTransPath::getListImagePathByIndex(*it).toStdString());
		}
	}
	return vec;
}

std::vector<std::string> ImageSpriteLayer::getAllResourceImageContainsExteral()
{
	if (isNotContainsAnyRes() && getAllSpriteNode().size() > 0){
		MyLogger::getInstance()->addError("Error ImageSpriteLayer::getAllImageContainsRole imageMap size == 0 layername is " + getTagName());
	}
	std::vector<std::string> vec;
	for (auto iter = resMap.begin(); iter != resMap.end(); iter++){
		vec.push_back(iter->second->path);
	}
	for (auto iter = roleMap.begin(); iter != roleMap.end(); iter++){
		vec.push_back(iter->second->path);
	}
	for (auto iter = animationResMap.begin(); iter != animationResMap.end(); iter++){
		auto animationList = iter->second->animationList;
		for (auto it = animationList.begin(); it != animationList.end(); it++){
			int index = *it;
			std::string image = ImgTransPath::getListImagePathByIndex(index).toStdString();
			vec.push_back(image);
		}
	}
	return vec;
}

std::vector<animateData> ImageSpriteLayer::getAllAnimateData()
{
	std::vector<animateData> vec;
	int size = this->getChildrenCount();
	Vector<Node*> child = this->getChildren();
	for (int i = 0; i < size; i++)
	{
		auto sprite = dynamic_cast<ImageSprite*>(child.at(i));
		if (sprite->getAnimation() == NULL){
			continue;
		}
		std::string animName = sprite->getAnimationName();
		float delay = sprite->getPerTime();
		unsigned char animType = 0;
		unsigned char wrapMode = 1;
		bool repeat = false;
		if (vec.size() == 0){
			animateData data;
			std::vector<int> framevec = sprite->getAnimationList();
			data.setData(animName, animType, wrapMode, delay, framevec.size(), framevec);
			vec.push_back(data);
		}
		else{
			for (int k = 0; k < vec.size(); k++){
				animateData data = vec.at(k);
				if (data.animName == animName){
					repeat = true;
					break;
				}
			}
			if (repeat == false){
				animateData data;
				std::vector<int> framevec = sprite->getAnimationList();
				data.setData(animName, animType, wrapMode, delay, framevec.size(), framevec);
				vec.push_back(data);
			}
		}
	}
	return vec;
}

std::vector<OutputSpriteData*> ImageSpriteLayer::getOutputSpriteData()
{
	std::vector<ImageSpriteLayer*> vec;
	vec.push_back(this);
	return getOutputSpriteData(vec);
}

std::vector<OutputSpriteData*> ImageSpriteLayer::getOutputSpriteData(std::vector<ImageSpriteLayer*> layerVec)
{
	std::unique_lock<std::mutex> lk(m_Outputmutex);
	releaseOutputSpriteData();
	Vector<Node*> childVec;
	for (int i = 0; i < layerVec.size(); i++){
		auto vec = layerVec.at(i)->getAllSpriteNode();
		for (int k = 0; k < vec.size(); k++){
			childVec.pushBack(vec.at(k));
		}
	}
	for (int i = 0; i < childVec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(childVec.at(i));
		auto layer = dynamic_cast<ImageSpriteLayer*>(sprite->getParent());
		std::string tagname = sprite->getTagName();
		unsigned char resType = sprite->getResType();
		OutputSpriteData* data = new OutputSpriteData;
		data->layerIndex = layer->getLayerIndex();
		data->numController = 0;
		data->resType = resType;
		data->dontMerge = sprite->isDontMerge();
		if (sprite->isHasBoxProperties()){
			data->rigidData.hasProperties = true;
			data->circleData.hasProperties = false;
			data->rigidData.way = sprite->getBoxWay();
			auto size = sprite->getContentSize();
			auto spriteAnchr = sprite->getAnchorPoint();
			Point centerPos;
			auto anchr = sprite->getBoxAnchrPos();
			auto half = sprite->getHalf();
			centerPos.x = size.width*(anchr.x - spriteAnchr.x);
			centerPos.y = size.height*(anchr.y - spriteAnchr.y);
			data->rigidData.centerX = centerPos.x;
			data->rigidData.centerY = centerPos.y;
			data->rigidData.halfWidth = half.width;
			data->rigidData.halfHeight = half.height;
			data->numController++;
		}
		else if (sprite->isHasCircleProperties()){
			data->rigidData.hasProperties = false;
			data->circleData.hasProperties = true;
			auto size = sprite->getContentSize();
			float radius = sprite->getRadius();
			auto anchr = sprite->getCircleAnchrPos();
			auto spriteAnchr = sprite->getAnchorPoint();
			Point centerPos;
			centerPos.x = size.width*(anchr.x - spriteAnchr.x);
			centerPos.y = size.height*(anchr.y - spriteAnchr.y);
			data->circleData.centerX = centerPos.x;
			data->circleData.centerY = centerPos.y;
			data->circleData.radius = radius;
			data->numController++;
		}
		else{
			data->rigidData.hasProperties = false;
			data->circleData.hasProperties = false;
		}
		if (sprite->isHasMoveProperties()){
			data->line_moveData.hasProperties = true;
			data->line_moveData.delay = sprite->getMoveDelayTime();
			data->line_moveData.backDelay = sprite->getMoveBackDelayTime();
			data->line_moveData.startRunTime = sprite->getStartRunTime();
			Point translate = sprite->getTranslatDis();
			float velocityX = translate.x / sprite->getMoveTime();
			float velocityY = translate.y / sprite->getMoveTime();
			float distance = Point().distance(translate);
			data->line_moveData.velocityX = velocityX;
			data->line_moveData.velocityY = velocityY;
			data->line_moveData.distance = distance;
			data->numController++;
		}
		else{
			data->line_moveData.hasProperties = false;
		}
		if (sprite->getType() != SpriteType::NORMAL_BLOCK){
			data->en_Data.hasProperties = true;
			switch (sprite->getType()){
			case SpriteType::DYNAMIC_BLOCK:
				data->en_Data.entityType = 0;
				data->isAtlas = true;
				break;
			case SpriteType::MONSTER:
				data->en_Data.entityType = 1;
				data->isAtlas = false;
				break;
			case SpriteType::ROLE:
				data->en_Data.entityType = 2;
				data->isAtlas = false;
				break;
			}
			data->en_Data.id = sprite->getId();
			data->en_Data.sleepTime = sprite->getSleepTime();
			data->en_Data.numGenerate = sprite->getNumGenerate();
			data->en_Data.generateDelay = sprite->getGenerateDelay();
			data->numController++;
		}
		else{
			data->en_Data.hasProperties = false;
			data->isAtlas = true;
		}

		if (sprite->isHasMonsterProperties()){
			auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
			data->monster.hasProperties = true;
			data->monster.hp = entitySprite->getHp();
			data->monster.mp = entitySprite->getMp();
			data->monster.atk = entitySprite->getAttack();
			data->monster.def = entitySprite->getDefense();
			data->monster.mat = entitySprite->getMagicAttack();
			data->monster.mdf = entitySprite->getMagicDefense();
			data->monster.spd = entitySprite->getSpeed();
			data->monster.hardFactor = entitySprite->getHardFactor();
			data->numController++;
		}
		else{
			data->monster.hasProperties = false;
		}

		if (sprite->isHasMonsterDropProperties()){
			auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
			data->monsterDrop.hasProperties = true;
			data->monsterDrop.gold = entitySprite->getDropGold();
			for (int i = 0; i < entitySprite->getDropList().size(); i++){
				monsterDropData::Drop_Data* data__ = new monsterDropData::Drop_Data;
				data__->id = entitySprite->getDropList().at(i)->id.toStdString();
				data__->odds = entitySprite->getDropList().at(i)->odds;
				data__->difficulty = entitySprite->getDropList().at(i)->difficulty;
				data->monsterDrop._vec.push_back(data__);
			}
			data->numController++;
		}
		else{
			data->monsterDrop.hasProperties = false;
		}

		if (sprite->isHasAIProperties()){
			auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
			data->aiData.hasProperties = true;
			data->aiData.rangeMinX = entitySprite->getRangeMinX();
			data->aiData.rangeMaxX = entitySprite->getRangeMaxX();
			data->aiData.rangeMinY = entitySprite->getRangeMinY();
			data->aiData.rangeMaxY = entitySprite->getRangeMaxY();
			data->aiData.aiFile = entitySprite->getAIFile();
			int count = entitySprite->getUserAIList().size();
			for (int i = 0; i < entitySprite->getUserAIList().size(); i++){
				monsterAIData::key_v* data__ = new monsterAIData::key_v;
				data__->key = entitySprite->getUserAIList().at(i)->key.toStdString();
				data__->value = entitySprite->getUserAIList().at(i)->value.toStdString();
				data->aiData.keyV.push_back(data__);
			}
			data->numController++;
		}
		else{
			data->aiData.hasProperties = false;
		}

		if (sprite->isHasSimplerTrigger()){
			auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
			data->triData.hasProperties = true;
			for (int i = 0; i < entitySprite->getSimpleTriggerList().size(); i++){
				triggerData::tri_v* data__ = new triggerData::tri_v;
				data__->tiggerType = entitySprite->getSimpleTriggerList().at(i)->type;
				data__->targetID = entitySprite->getSimpleTriggerList().at(i)->id;
				data->triData.tri_V.push_back(data__);
			}
			data->numController++;
		}
		else{
			data->triData.hasProperties = false;
		}
		
		if (sprite->isHasFilterProperties()){
			data->filData.hasProperties = true;
			data->filData.filterType = sprite->getFilterType();
			data->numController++;
		}
		else{
			data->filData.hasProperties = false;
		}

		//singleFrame
		if (resType == 0){
			auto layer = static_cast<ImageSpriteLayer*>(sprite->getParent());
			if (layer->isSportState()){
				data->resName = ImgTransPath::getOutputListPngName(sprite, "dynamic").toStdString();
			}
			else{
				data->resName = ImgTransPath::getOutputListPngName(sprite, "static" + QString::number(layer->getStaticIndex())).toStdString();
			}
		}//spriteFrames
		else if (resType == 1){
			data->animName = sprite->getAnimationName();
		}
		else if (resType == 2){
			if (sprite->getType() == SpriteType::ROLE){
				data->playerNumber = "0";
			}
			else{
				//这里有问题，导出的名字有前缀
				data->resName = ImgTransPath::getOutputRelativeImageName(sprite).toStdString();
				//data->resName = ImgTransPath::getRealImageName(sprite).toStdString();
			}
		}
		data->contentSize = sprite->getContentSize();
		data->numInstances = 1;
		data->posvec.push_back(sprite->getStartPos());
		data->flipXvec.push_back(sprite->isFlippedX());
		data->flipYvec.push_back(sprite->isFlippedY());
		data->scaleVec.push_back(Vec2(sprite->getScaleX(), sprite->getScaleY()));
		data->anchrVec.push_back(sprite->getAnchorPoint());
		data->rotationVec.push_back(sprite->getRotation());
		int size = vecOutputData.size();
		if (size == 0){
			vecOutputData.push_back(data);
		}
		else{
			bool repeat = false;
			for (int k = 0; k < vecOutputData.size(); k++){
				OutputSpriteData* vecdata = vecOutputData.at(k);
				if (vecdata->isEqual(data)){
					repeat = true;
					vecdata->posvec.push_back(sprite->getStartPos());
					bool flipX = sprite->isFlippedX();
					bool flipY = sprite->isFlippedY();
					vecdata->flipXvec.push_back(flipX);
					vecdata->flipYvec.push_back(flipY);
					vecdata->anchrVec.push_back(Vec2(sprite->getAnchorPoint()));
					vecdata->scaleVec.push_back(Vec2(sprite->getScaleX(), sprite->getScaleY()));
					vecdata->rotationVec.push_back(sprite->getRotation());
					vecdata->numInstances++;
					delete data;
					break;
				}
			}
			if (repeat == false){
				vecOutputData.push_back(data);
			}
		}
	}
	for (int i = 0; i < vecOutputData.size(); i++){
		auto data = vecOutputData.at(i);
		if (data->resType == 2 && data->monster.hasProperties == true){
			QString str = data->resName.c_str();
			data->resName = str.section(".", 0, -2).toStdString();
		}
	}
	return vecOutputData;
}

void ImageSpriteLayer::releaseOutputSpriteData()
{
	int size = vecOutputData.size();
	for (int i = 0; i < size; i++){
		auto data = vecOutputData.at(i);
		delete data;
	}
	vecOutputData.clear();
}

void ImageSpriteLayer::setisSportState(bool state)
{
	sport_state = state;
}

bool ImageSpriteLayer::isSportState()
{
	return sport_state;
}

bool ImageSpriteLayer::hasRange()
{
	auto foreManager = (ForeManager*)this->getParent();
	auto rangeManage = foreManager->getLayerMoveRangeManage();
	auto data1 = rangeManage->getLayerData(this->getTagName());
	if (data1->col1 >= 0 && data1->row1 >= 0 && data1->col2 >= 0 && data1->row2 >= 0){
		return true;
	}
	return false;
}

float ImageSpriteLayer::getRangeLeftTopX()
{
	auto foreManager = (ForeManager*)this->getParent();
	auto rangeManage = foreManager->getLayerMoveRangeManage();
	auto data = rangeManage->getLayerData(this->getTagName());
	return data->col1 * rangeManage->getGridWidth() + 5000;
}

float ImageSpriteLayer::getRangeLeftTopY()
{
	auto foreManager = (ForeManager*)this->getParent();
	auto rangeManage = foreManager->getLayerMoveRangeManage();
	auto data = rangeManage->getLayerData(this->getTagName());
	int height = data->row1 * rangeManage->getGridHeight() + 5000;
	return height;
}

float ImageSpriteLayer::getRangeRightBottomX()
{
	auto foreManager = (ForeManager*)this->getParent();
	auto rangeManage = foreManager->getLayerMoveRangeManage();
	auto data = rangeManage->getLayerData(this->getTagName());
	return (data->col2) * rangeManage->getGridWidth() + 5000;
}

float ImageSpriteLayer::getRangeRightBottomY()
{
	auto foreManager = (ForeManager*)this->getParent();
	auto rangeManage = foreManager->getLayerMoveRangeManage();
	auto data = rangeManage->getLayerData(this->getTagName());
	int height = data->row2 * rangeManage->getGridHeight() + 5000;
	return height;
}

Point* ImageSpriteLayer::getLayerRange()
{
	Vector<Node*> vec = getAllSpriteNode();
	int vecsize = vec.size();
	Point minPos, maxPos;
	for (int i = 0; i < vecsize; i++){
		auto sprite = dynamic_cast<ImageSprite*>(vec.at(i));
		auto size = Size(sprite->getContentSize().width*sprite->anchr.x*sprite->getScaleX(),
			sprite->getContentSize().height*sprite->anchr.y*sprite->getScaleY());
		auto pos = sprite->getPosition() - size;
		size = Size(sprite->getContentSize().width*sprite->getScaleX(),
			sprite->getContentSize().height*sprite->getScaleY());
		if (i == 0){ 
			minPos = Point(pos.x, pos.y);
			maxPos = Point(pos.x + size.width, pos.y + size.height);
		}
		else{
			if (minPos.x > pos.x){
				minPos.x = pos.x;
			}
			if (minPos.y > pos.y){
				minPos.y = pos.y;
			}
			if (maxPos.x < pos.x + size.width){
				maxPos.x = pos.x + size.width;
			}
			if (maxPos.y < pos.y + size.height){
				maxPos.y = pos.y + size.height;
			}
		}
	}
	Point* ss = new Point[2];
	ss[0] = minPos + this->getPosition();
	ss[1] = maxPos + this->getPosition();
	return ss;
}

void ImageSpriteLayer::setLock(bool lock)
{
	__lock = lock;
}

bool ImageSpriteLayer::isLock()
{
	return __lock;
}

bool ImageSpriteLayer::isDontOutput()
{
	return isdontoutput;
}

void ImageSpriteLayer::setDontOutputState(bool dontoutput)
{
	isdontoutput = dontoutput;
}

int ImageSpriteLayer::getStaticIndex()
{
	if (!sport_state){
		return static_index;
	}
	return -1;
}

void ImageSpriteLayer::setStaticIndex(int index)
{
	if (!sport_state){
		static_index = index;
	}
}

void ImageSpriteLayer::setMoveScaleX(float scale)
{
	moveScaleX = scale;
}

void ImageSpriteLayer::setMoveScaleY(float scale)
{
	moveScaleY = scale;
}

void ImageSpriteLayer::setSceneScaleFluence(float scale)
{
	sceneScaleFluence = scale;
}

float ImageSpriteLayer::getMoveScaleX()
{
	return moveScaleX;
}

float ImageSpriteLayer::getMoveScaleY()
{
	return moveScaleY;
}

float ImageSpriteLayer::getSceneScaleFluence()
{
	return sceneScaleFluence;
}

int ImageSpriteLayer::getFilterType()
{
	return filterType;
}

void ImageSpriteLayer::setFilterType(int type_)
{
	filterType = type_;
}

bool ImageSpriteLayer::addToMap(std::string imagePath, bool isExteral)
{
	if (!isExteral){
		return addImageToMap(imagePath);
	}
	else{
		return addExteralToMap(imagePath);
	}
}

bool ImageSpriteLayer::addImageToMap(std::string imagePath)
{
	std::string key = imagePath.substr(imagePath.find_last_of("/") + 1, imagePath.length());
	ResInfo* info = nullptr;
	auto iter = resMap.find(key);
	if (iter == resMap.end()){
		info = new ResInfo();
		info->path = imagePath;
		info->isExteral = false;
		resMap.insert(pair<std::string, ResInfo*>(key, info));
	}
	else{
		info = iter->second;
	}
	info->refCount++;
	if (!info->isChanged) {
		info->isChanged = true;
		_changedResInfos.push_back(info);
	}

	return info->refCount == 1;
}

bool ImageSpriteLayer::addExteralToMap(std::string imagePath)
{
	std::string key = imagePath.substr(imagePath.find_last_of("/") + 1, imagePath.length());
	ResInfo* info = nullptr;
	auto iter = roleMap.find(key);
	if (iter == roleMap.end()){
		info = new ResInfo();
		info->path = imagePath;
		info->isExteral = true;
		roleMap.insert(pair<std::string, ResInfo*>(key, info));
	}
	else{
		info = iter->second;
	}
	info->refCount++;
	if (!info->isChanged) {
		info->isChanged = true;
		_changedResInfos.push_back(info);
	}
	return info->refCount == 1;
}

bool ImageSpriteLayer::addAnimationToMap(std::string animationname, std::vector<int> animationList)
{
	AnimationInfo* info = nullptr;
	auto itr = animationResMap.find(animationname);
	if (itr == animationResMap.end()) {
		info = new AnimationInfo();
		info->animationname = animationname;
		info->animationList = animationList;
		animationResMap.insert(pair<std::string, AnimationInfo*>(animationname, info));
	}
	else {
		info = itr->second;
	}
	info->refCount++;
	if (!info->isChanged) {
		info->isChanged = true;
		_changeAnimationInfos.push_back(info);
	}
	return info->refCount == 1;
}

//这里可能有错误
void ImageSpriteLayer::addCopySpriteCurLayer(ImageSprite* sprite)
{
	auto key = ImgTransPath::getListPngName(sprite).toStdString();
	switch (sprite->getType()){
	case SpriteType::ROLE:
	case SpriteType::MONSTER:
		addToMap(key, true);
		break;
	case SpriteType::DYNAMIC_BLOCK:
		if (sprite->isHasAnimation()){
			addAnimationToMap(sprite->getAnimationName(), sprite->getAnimationList());
		}
		else{
			addToMap(key, false);
		}
		break;
	case SpriteType::NORMAL_BLOCK:
		addToMap(key, false);
		break;
	}
	sprite->setParentTag(this->getTagName());
	addChild(sprite, sprite->getZOrder());
}

void ImageSpriteLayer::removeFromMap(std::string imagePath) {
	std::string key = imagePath.substr(imagePath.find_last_of("/") + 1);
	auto itr = resMap.find(key);
	if (itr != resMap.end()) {
		ResInfo* info = itr->second;
		if (info->refCount > 0) {
			info->refCount--;
			if (!info->isChanged) {
				info->isChanged = true;
				_changedResInfos.push_back(info);
			}
		}
	}
	itr = roleMap.find(key);
	if (itr != roleMap.end()){
		ResInfo* info = itr->second;
		if (info->refCount > 0) {
			info->refCount--;
			if (!info->isChanged) {
				info->isChanged = true;
				_changedResInfos.push_back(info);
			}
		}
	}
}

void ImageSpriteLayer::removeFromAnimatiomMap(std::string animation)
{
	auto itr = animationResMap.find(animation);
	if (itr != animationResMap.end()){
		AnimationInfo* info = itr->second;
		if (info->refCount > 0){
			info->refCount--;
			if (!info->isChanged){
				info->isChanged = true;
				_changeAnimationInfos.push_back(info);
			}
		}
	}
}

//只查看ResMap
bool ImageSpriteLayer::containsInImageMap(const std::string& listImagePath)
{
	auto iter = resMap.find(listImagePath);
	if (iter != resMap.end()){
		return true;
	}
	iter = roleMap.find(listImagePath);
	if (iter != roleMap.end()){
		return true;
	}
	return false;
}

bool ImageSpriteLayer::containsInAnimationMap(const std::string& animatonname)
{
	auto iter = animationResMap.find(animatonname);
	if (iter != animationResMap.end()){
		return true;
	}
	return false;
}

void ImageSpriteLayer::updateChanged() {
	if (_changedResInfos.size() > 0) {
		ResourceView* resView = dynamic_cast<ResourceView*>(g_resourceView);
		ExteralView* extView = dynamic_cast<ExteralView*>(g_exteralView);
		for (unsigned int i = 0; i < _changedResInfos.size(); i++) {
			ResInfo* info = _changedResInfos.at(i);
			info->isChanged = false;

			int n = info->refCount - info->lastRefCount;
			info->lastRefCount = info->refCount;
			if (info->isExteral){
				extView->changedResRef(info->path, n);
			}
			else{
				resView->changedResRef(info->path, n);
			}

			if (info->refCount == 0) {
				std::string key = info->path.substr(info->path.find_last_of("/") + 1);
				auto itr = resMap.find(key);
				if (itr != resMap.end()) {
					resMap.erase(itr);
				}
				if (info->isExteral){
					auto itr = roleMap.find(key);
					if (itr != roleMap.end()) {
						roleMap.erase(itr);
					}
				}
				delete info;
			}
		}

		_changedResInfos.clear();
	}

	if (_changeAnimationInfos.size() > 0){
		AnimationView* animationView = dynamic_cast<AnimationView*>(g_AnimationView);
		for (unsigned int i = 0; i < _changeAnimationInfos.size(); i++) {
			AnimationInfo* info = _changeAnimationInfos.at(i);
			info->isChanged = false;
			int n = info->refCount - info->lastRefCount;
			info->lastRefCount = info->refCount;
			animationView->changedResRef(info->animationname, n);
			if (info->refCount == 0) {
				std::string key = info->animationname;
				auto itr = animationResMap.find(key);
				if (itr != animationResMap.end()) {
					animationResMap.erase(itr);
				}
				delete info;
			}
		}

		_changeAnimationInfos.clear();
	}
}

void ImageSpriteLayer::addSpriteOver()
{
	updateChanged();
}

void ImageSpriteLayer::removeSpriteOver()
{
	updateChanged();
}

bool ImageSpriteLayer::isSameProperties(ImageSpriteLayer* layer)
{
	auto foreManager = (ForeManager*)this->getParent();
	auto rangeManage = foreManager->getLayerMoveRangeManage();
	auto data1 = rangeManage->getLayerData(this->getTagName());
	auto data2 = rangeManage->getLayerData(layer->getTagName());
	if (data1->col1 != data2->col1 || data1->row1 != data2->row1 || data1->col2 != data2->col2 || data1->row2 != data2->row2){
		return false;
	}
	if (this->moveScaleX != layer->moveScaleX)return false;
	if (this->moveScaleY != layer->moveScaleY)return false;
	if (this->sceneScaleFluence != layer->sceneScaleFluence)return false;
	if (this->filterType != layer->filterType)return false;
	return true;
}

unsigned int ImageSpriteLayer::generateThreadTag()
{
	std::unique_lock<std::recursive_mutex> lk(tagmutex);
	return ++topTag;
}

void ImageSpriteLayer::setCurrentTag(unsigned int tag)
{
	std::unique_lock<std::recursive_mutex> lk(tagmutex);
	if (tag > currentTag)currentTag = tag;
}

bool ImageSpriteLayer::containsInUnLoadVec(int index)
{
	for (size_t i = 0; i < unAddImageVec.size(); i++){
		auto __imageIndex = unAddImageVec.at(i);
		if (index == __imageIndex)return true;
	}
	return false;
}

void ImageSpriteLayer::addToUnLoadVec(int index)
{
	unAddImageVec.push_back(index);
}

void ImageSpriteLayer::clearLoadedImageIndex()
{
	std::map<std::string, int>::iterator iter;
	for (iter = imageIndexMap.begin(); iter != imageIndexMap.end(); iter++){
		int index = atoi(iter->first.substr(0, iter->first.find_first_of(".")).c_str());
		std::vector<int>::iterator iter2;
		for (iter2 = unAddImageVec.begin(); iter2 != unAddImageVec.end(); iter2++){
			if (*iter2 == index){
				unAddImageVec.erase(iter2);
				break;
			}
		}
	}
}

void ImageSpriteLayer::resetLayerIndex()
{
	auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	layerIndex = layerManager->getLayerIndex(this->getTagName());
}

bool ImageSpriteLayer::isNotContainsAnyRes()
{
	if (resMap.size() == 0 && roleMap.size() == 0 && animationResMap.size() == 0){
		return true;
	}
	return false;
}

std::vector<int> ImageSpriteLayer::getAnimationSpriteFrameList(std::string animationname)
{
	auto iter = animationResMap.find(animationname);
	if (iter != animationResMap.end()){
		return iter->second->animationList;
	}
	std::vector <int> vec;
	return vec;
}