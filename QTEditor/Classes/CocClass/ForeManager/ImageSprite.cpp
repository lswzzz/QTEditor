#include "ImageSprite.h"
#include <qstring.h>
#include "Global.h"
#include "HelloWorldScene.h"
#include "ResMap.h"
#include "LayerManagerWidget.h"
#include "ImgTransPath.h"

static unsigned int maxid = 0;
std::string ImageSprite::imagePrefix = "image_";
std::string ImageSprite::animationPrefix = "animation_";
QString ImageSprite::imagePrefixQString = "image_";
QString ImageSprite::animationPrefixQString = "animation_";

unsigned int ImageSprite::getMaxId()
{
	return maxid;
}

ImageSprite::ImageSprite()
{
	hasMoveProperties = false;
	runAnimate = NULL;
	translateDis = Point(0, 0);
	moveType = 0;
	moveDelay = 0.0f;
	moveAction = NULL;
	moveTime = 0;
	halfPos = Point(-999, -999);
	direction = 1;

	hasAnimation = false;
	perTime = 0.5;
	playerType = 0;
	animation = NULL;

	hasBoxProperties = false;
	drawnode = nullptr;
	drawNodeParent = nullptr;
	boxway = 0;
	radius = 0.0f;
	hasCircleProperties = false;

	copyCount = 0;
	isUserOutput = false;
	m_type = SpriteType::NORMAL_BLOCK;

	hasBaseProperties = true;
	flipx = false;
	flipy = false;
	anchr = Point(0.5, 0.5);

	numGenerate = 0;
	generateDelay = 0.0f;
	sleepTime = 0.0f;
	hasEntityProperties = false;

	id = 0;

	hasMonsterDropProperties = false;
	hasAIProperties = false;
	hasSimpleTriggerProperties = false;

	hasFilterProperties = false;
	filterType = 0;

	canAutoResetResName = true;
	//禁止合并 在导出的时候是否合并
	dontMerge = false;
}

ImageSprite::~ImageSprite()
{
	if (drawnode){
		drawnode->removeFromParent();
		//drawnode->release();
		drawNodeParent->removeFromParent();
		//drawNodeParent->release();
	}
	deleteSimpleTriList();
}

void ImageSprite::showRigid(int type)
{
	switch (type){
	case 0:
		if (drawNodeParent){
			drawNodeParent->setVisible(true);
		}
		break;
	case 1:
		if (drawNodeParent){
			drawNodeParent->setVisible(false);
		}
	
		break;
	}
}

ImageSprite* ImageSprite::create()
{
	auto ret = new ImageSprite();
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

ImageSprite* ImageSprite::create(int index)
{
	ImageSprite* ret = new ImageSprite();
	if (ret){
		ret->initWithTexture(ResMap::getSinglePngTexture(index));
		ret->imageIndex = index;
		return ret;
	}
	else{
		return NULL;
	}
}

bool ImageSprite::initWithText(Texture2D* texture, Rect rect, std::string png)
{
	this->rect = rect;
	this->texturePng = png;
	return this->initWithTexture(texture, rect);
}

void ImageSprite::setParentTag(std::string parentTag)
{
	this->parentTag = parentTag;
}

std::string ImageSprite::getParentTag()
{
	return parentTag;
}

std::string ImageSprite::getTemplateFile()
{
	return templateFile;
}

std::string ImageSprite::getTemplateMD5()
{
	return templatemd5;
}

void ImageSprite::setTemplateFile(std::string str, std::string md5)
{
	templateFile = str;
	templatemd5 = md5;
}

void ImageSprite::translatLine(Point translate)
{
	translateDis = translate;
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
}

//drawNode设置锚点无效
void ImageSprite::drawRigid(Point anchr, Size half, int way)
{
	auto spritesize = this->getContentSize();
	BoxAnchrPos = anchr;
	this->half = half;
	auto spriteAnchr = this->getAnchorPoint();
	Point drawNodePos1, drawNodePos2;
	drawNodePos1.x = spritesize.width*BoxAnchrPos.x - half.width;
	drawNodePos1.y = spritesize.height*BoxAnchrPos.y - half.height;
	drawNodePos2.x = spritesize.width*BoxAnchrPos.x + half.width;
	drawNodePos2.y = spritesize.height*BoxAnchrPos.y + half.height;
	if (!drawNodeParent){
		drawnode = DrawNode::create();
		drawnode->setAnchorPoint(Point(0.5, 0.5));
		drawNodeParent = Sprite::create();
		drawNodeParent->addChild(drawnode);
		drawNodeParent->setAnchorPoint(BoxAnchrPos);
		this->addChild(drawNodeParent, 3);
	}
	drawnode->clear();
	drawnode->drawSolidRect(drawNodePos1, drawNodePos2, Color4F(0.015, 1.0, 0.015, 1.0));
	this->boxway = way;
	switch (way){
	case 0:
		break;
	case 1:
		drawnode->drawSolidRect(Point(drawNodePos1.x + 2, drawNodePos2.y - 2), Point(drawNodePos2.x - 2, drawNodePos2.y - 8), Color4F(Color4B(0x00, 0x10, 0xE0, 0xFF)));
		break;
	case 2:
		drawnode->drawSolidRect(Point(drawNodePos1.x + 2, drawNodePos1.y + 2), Point(drawNodePos2.x - 2, drawNodePos1.y + 8), Color4F(Color4B(0x00, 0x10, 0xE0, 0xFF)));
		break;
	case 3:
		drawnode->drawSolidRect(Point(drawNodePos1.x + 2, drawNodePos1.y + 2), Point(drawNodePos1.x + 8, drawNodePos2.y - 2), Color4F(Color4B(0x00, 0x10, 0xE0, 0xFF)));
		break;
	case 4:
		drawnode->drawSolidRect(Point(drawNodePos2.x - 2, drawNodePos1.y + 2), Point(drawNodePos2.x - 8, drawNodePos2.y - 2), Color4F(Color4B(0x00, 0x10, 0xE0, 0xFF)));
		break;
	}
}

void ImageSprite::drawRigid(Point anchr, float radius)
{
	auto spritesize = this->getContentSize();
	CircleAnchrPos = anchr;
	Point pos1;
	pos1.x = spritesize.width * anchr.x;
	pos1.y = spritesize.height * anchr.y;
	this->radius = radius;
	if (!drawNodeParent){
		drawnode = DrawNode::create();
		drawnode->setAnchorPoint(Point(0.5, 0.5));
		drawNodeParent = Sprite::create();
		drawNodeParent->setAnchorPoint(CircleAnchrPos);
		drawNodeParent->addChild(drawnode);
		this->addChild(drawNodeParent, 3);
	}
	drawnode->clear();
	drawnode->drawSolidCircle(pos1, radius, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(Color4B(0x00, 0x00, 0xFF, 0xFF)));
}

void ImageSprite::setMoveTime(float time)
{
	moveTime = time;
}

void ImageSprite::setMoveDelayTime(float time)
{
	moveDelay = time;
}

void ImageSprite::setMoveBackDelayTime(float time)
{
	moveBackDelay = time;
}

void ImageSprite::setStartRunTime(float time)
{
	startRunTime = time;
}

void ImageSprite::runMove(float time, float delay, float backDelay)
{
	moveType = 2;
	if (moveAction){
		this->stopAction(moveAction);
		moveAction = NULL;
	}
	moveTime = time;
	moveDelay = delay;
	moveBackDelay = backDelay;
	if (halfPos == Point(-999, -999)){
		if (moveAction){
			return;
		}
		moveAction = RepeatForever::create(Sequence::create(CallFunc::create([this]{direction = 1; }), MoveBy::create(time, translateDis), DelayTime::create(moveBackDelay),
			CallFunc::create([this]{direction = 0; }), MoveBy::create(time, -translateDis), DelayTime::create(moveDelay), NULL));
		this->runAction(moveAction);
	}
	else{
		switch (direction){
		//for startPos
		case 0:
		{
			auto endPos = startPos + translateDis;
			auto movehalf = MoveBy::create(time*(startPos.distance(halfPos) / startPos.distance(startPos-translateDis)), startPos-halfPos);
			moveAction = Sequence::create(movehalf, DelayTime::create(moveDelay), CallFunc::create([this, time]{
				this->stopAction(moveAction);
				moveAction = NULL;
				moveAction = RepeatForever::create(Sequence::create(CallFunc::create([this]{direction = 1; }), MoveBy::create(time, translateDis), DelayTime::create(moveDelay),
					CallFunc::create([this]{direction = 0; }), MoveBy::create(time, -translateDis), DelayTime::create(moveBackDelay), NULL));
				this->runAction(moveAction);
			}), NULL);
			this->runAction(moveAction);
		}
			break;
		//for destPos
		case 1:
		{
			auto endPos = startPos + translateDis;
			auto movehalf = MoveBy::create(time*(endPos.distance(halfPos) / startPos.distance(startPos - translateDis)), endPos - halfPos);
			moveAction = Sequence::create(movehalf, DelayTime::create(moveBackDelay), CallFunc::create([this, time]{
				this->stopAction(moveAction);
				moveAction = NULL;
				moveAction = RepeatForever::create(Sequence::create(CallFunc::create([this]{direction = 0; }), MoveBy::create(time, -translateDis), DelayTime::create(moveDelay),
					CallFunc::create([this]{direction = 1; }), MoveBy::create(time, translateDis), DelayTime::create(moveDelay), NULL));
				this->runAction(moveAction);
			}), NULL);
			this->runAction(moveAction);
		}
			break;
		}
	}   
}

void ImageSprite::setPosition(Point pos)
{
	//pos = Point((int)pos.x, (int)pos.y);
	startPos = pos;
	Sprite::setPosition(startPos);
}
 
Point ImageSprite::getTranslatDis()
{
	return translateDis;
}

Point ImageSprite::getBoxAnchrPos()
{
	return BoxAnchrPos;
}

Size ImageSprite::getHalf()
{
	return half;
}

bool ImageSprite::getDrawNodeVisible()
{
	if (drawNodeParent){
		return drawNodeParent->isVisible();
	}
	return false;
}

double ImageSprite::getMoveTime()
{
	return moveTime;
}

int ImageSprite::getMoveType()
{
	return moveType;
}

int ImageSprite::getBoxWay()
{
	return boxway;
}

Point ImageSprite::getStartPos()
{
	return startPos;
}

float ImageSprite::getMoveDelayTime()
{
	return moveDelay;
}

float ImageSprite::getMoveBackDelayTime()
{
	return moveBackDelay;
}

float ImageSprite::getStartRunTime()
{
	return startRunTime;
}

void ImageSprite::stopMove()
{
	moveType = 0;
	if (moveAction){
		this->stopAction(moveAction);
		moveAction = NULL;
	}
	this->setPosition(startPos);
	direction = 1;
	halfPos = Point(-999, -999);
}

void ImageSprite::pauseMove()
{
	moveType = 1;
	if (moveAction){
		this->stopAction(moveAction);
		moveAction = NULL;
	}
	if(this->getPosition() != startPos && this->getPosition() != (startPos + translateDis)){
		halfPos = this->getPosition();
	}
}

void ImageSprite::setAnimationName(std::string animationname)
{
	this->animationname = animationname;
}

void ImageSprite::resetAnimation(Animation* animatoin)
{
	this->animation = animatoin;
}

void ImageSprite::resetAnimation()
{
	auto layer = dynamic_cast<ImageSpriteLayer*>(this->getParent());
	this->animation = layer->getAnimationCache()->getAnimation(animationname);
}

void ImageSprite::setPerTime(float perTime)
{
	this->perTime = perTime;
	playerAnimation(playerType);
}

void ImageSprite::playerAnimation(int type)
{
	if (animation){
		Animate* animate;
		switch (type){
			//不播放
		case 0:
			if (runAnimate){
				this->stopAction(runAnimate);
				runAnimate = NULL;
			}
			playerType = type;
			break;
			//循环播放
		case 1:
			if (runAnimate){
				this->stopAction(runAnimate);
				runAnimate = NULL;
			}
			playerType = type;
			animation->setDelayPerUnit(perTime);
			animation->setRestoreOriginalFrame(true);
			animate = Animate::create(animation);
			runAnimate = RepeatForever::create(animate);
			this->runAction(runAnimate);
			break;
			//循环往复播放
		case 2:
			if (runAnimate){
				this->stopAction(runAnimate);
				runAnimate = NULL;
			}
			playerType = type;
			animation->setDelayPerUnit(perTime);
			animation->setRestoreOriginalFrame(true);
			animate = Animate::create(animation);
			runAnimate = RepeatForever::create(Sequence::create(animate, animate->reverse(), NULL));
			this->runAction(runAnimate);
			break;
			//播放一次
		case 3:
			if (runAnimate){
				this->stopAction(runAnimate);
				runAnimate = NULL;
			}
			playerType = type;
			animate = Animate::create(animation);
			runAnimate = Sequence::create(animate, CallFunc::create([this](){runAnimate = NULL; }), NULL);
			this->runAction(runAnimate);
			break;
		}
	}
}

void ImageSprite::pauseInPerSpriteFrame(int index)
{
	if (runAnimate){
		this->stopAction(runAnimate);
		runAnimate = NULL;
	}
	Vector<AnimationFrame*> frames = animation->getFrames();
	AnimationFrame* frame = frames.at(index);
	this->initWithSpriteFrame(frame->getSpriteFrame());
}

unsigned char ImageSprite::getResType()
{
	if (isHasAnimation()){
		return 1;
	}
	if (m_type == SpriteType::MONSTER || m_type == SpriteType::ROLE){
		return 2;
	}
	return 0;
}

void ImageSprite::setHasAnimation(bool has)
{
	hasAnimation = has;
	if (!hasAnimation){
		this->stopAction(runAnimate);
	}
}

bool ImageSprite::isHasAnimation()
{
	return hasAnimation;
}

void ImageSprite::setHasBaseProperties(bool has)
{
	hasBaseProperties = has;
}

void ImageSprite::setRotation(double rotation)
{
	Sprite::setRotation(rotation);
}

void ImageSprite::setHasBoxProperties(bool has)
{
	hasBoxProperties = has;
	if (has){
		hasCircleProperties = false;
	}
}

void ImageSprite::setHasMoveProperties(bool has)
{
	hasMoveProperties = has;
}

void ImageSprite::setHasEntityProperties(bool has)
{
	hasEntityProperties = has;
}

bool ImageSprite::isHasEntityProperties()
{
	return hasEntityProperties;
}

bool ImageSprite::isHasBaseProperties()
{
	return hasBaseProperties;
}

bool ImageSprite::isHasMoveProperties()
{
	return hasMoveProperties;
}

bool ImageSprite::isHasBoxProperties()
{
	return hasBoxProperties;
}

void ImageSprite::setHasCircleProperties(bool has)
{
	hasCircleProperties = has;
	if (has){
		hasBoxProperties = false;
	}
}

bool ImageSprite::isHasCircleProperties()
{
	return hasCircleProperties;
}

Point ImageSprite::getCircleAnchrPos()
{
	return CircleAnchrPos;
}

float ImageSprite::getRadius()
{
	return radius;
}

Animation* ImageSprite::getAnimation()
{
	return animation;
}

std::string ImageSprite::getAnimationName()
{
	return animationname;
}

std::vector<int> ImageSprite::getAnimationList()
{
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto layer = dynamic_cast<ImageSpriteLayer*>(scene->getLayerFromForeManager(parentTag));
	return layer->getAnimationSpriteFrameList(this->animationname);
}

float ImageSprite::getPerTime()
{
	return perTime;
}

int ImageSprite::getPlayerType()
{
	return playerType;
}

void ImageSprite::setTagName(std::string tagname)
{
	this->tagname = tagname;
}

std::string ImageSprite::getTagName()
{
	return this->tagname;
}

int ImageSprite::getImageIndex()
{
	return imageIndex;
}

void ImageSprite::copySprite(ImageSprite* sprite)
{
	Texture2D* texture = TextureCache::getInstance()->getTextureForKey(this->texturePng);
	sprite->initWithTexture(texture, rect);
	{
		sprite->flipx = this->flipx;
		sprite->flipy = this->flipy;
		sprite->anchr = this->anchr;
		sprite->setAnchorPoint(this->getAnchorPoint());
		sprite->setFlippedX(sprite->flipx);
		sprite->setFlippedY(sprite->flipy);
		sprite->setPosition(this->getStartPos());
		sprite->setScaleX(this->getScaleX());
		sprite->setScaleY(this->getScaleY());
		sprite->setRotation(this->getRotation());
		sprite->setHasBaseProperties(this->isHasBaseProperties());
		sprite->texturePng = this->texturePng;
		sprite->rect = this->rect;
		sprite->parentTag = this->parentTag;
	}

	{
		sprite->setHasMoveProperties(this->isHasMoveProperties());
		if (sprite->isHasMoveProperties()){
			auto size = Director::getInstance()->getOpenGLView()->getFrameSize();
			sprite->translatLine(this->translateDis);
			sprite->setMoveDelayTime(this->moveDelay);
			sprite->setMoveBackDelayTime(this->moveBackDelay);
			sprite->setStartRunTime(this->startRunTime);
			sprite->setMoveTime(this->moveTime);
			switch (this->moveType){
			case 0:
				sprite->stopMove();
				break;
			case 1:
				sprite->pauseMove();
				break;
			case 2:
				sprite->runMove(this->moveTime, this->moveDelay, this->moveBackDelay);
				break;
			}
		}
	}

	{
		sprite->setHasBoxProperties(this->isHasBoxProperties());
		if (this->isHasBoxProperties()){
			sprite->drawRigid(this->BoxAnchrPos, half, boxway);
			if (this->drawNodeParent){
				sprite->showRigid(!this->drawNodeParent->isVisible());
			}
		}
	}

	{
		sprite->setHasCircleProperties(this->isHasCircleProperties());
		if (this->isHasCircleProperties()){
			sprite->drawRigid(CircleAnchrPos, radius);
			if (this->drawNodeParent){
				sprite->showRigid(!this->drawNodeParent->isVisible());
			}
		}
	}

	{
		sprite->setHasEntityProperties(this->isHasEntityProperties());
		if (sprite->isHasEntityProperties()){
			sprite->setNumGenerate(this->getNumGenerate());
			sprite->setSleepTime(this->getSleepTime());
			sprite->setGenerateDelay(this->getGenerateDelay());
		}
	}

	{
		sprite->setHasSimplerTrigger(this->isHasSimplerTrigger());
		if (sprite->isHasSimplerTrigger()){
			for (int i = 0; i < _simTri_Vec.size(); i++){
				SimpleTri* da = _simTri_Vec.at(i);
				sprite->addSimpleTri(da->type, da->id);
			}
		}
	}

	{
		sprite->setHasAnimation(this->isHasAnimation());
		if (this->isHasAnimation()){
			sprite->setAnimationName(this->getAnimationName());
			sprite->resetAnimation(this->getAnimation());
			sprite->setPerTime(this->getPerTime());
			sprite->playerAnimation(this->playerType);
		}
	}

	{
		//这里在加了动画之后要改
		sprite->setType(this->getType());
		sprite->autoResetCanAutoResetResName();
		if (sprite->getCanAutoResetResNameInfo()){
			QString tag;
			switch (sprite->m_type){
			case SpriteType::NORMAL_BLOCK:
				tag += imagePrefixQString;
				break;
			case SpriteType::DYNAMIC_BLOCK:
				if (this->isHasAnimation()){
					tag += animationPrefixQString;
				}
				else{
					tag += imagePrefixQString;
				}
				break;
			case SpriteType::MONSTER:
				break;
			case SpriteType::ROLE:
				break;
			}
			tag += QString::number(maxid);
			sprite->setTagName(tag.toStdString());
		}
		else{
			QString tag = this->getTagName().c_str();
			tag += "_";
			tag += QString::number(++copyCount);
			sprite->setTagName(tag.toStdString());
		}
		sprite->imageIndex = this->imageIndex;
		sprite->setNewId();
	}

	{
		sprite->setTemplateFile(this->getTemplateFile(), templatemd5);
	}
}

int ImageSprite::getCopyCount()
{
	return copyCount;
}

void ImageSprite::setCopyCount(int count)
{
	copyCount = count;
}

bool ImageSprite::isHasMonsterProperties()
{
	if (m_type == SpriteType::MONSTER){
		return true;
	}
	return false;
}

bool ImageSprite::isHasMonsterDropProperties()
{
	if (hasMonsterDropProperties){
		return true;
	}
	else{
		return false;
	}
}

bool ImageSprite::isHasAIProperties()
{
	if (hasAIProperties){
		return true;
	}
	else{
		return false;
	}
}

bool ImageSprite::isHasSimplerTrigger()
{
	if (this->getType() == SpriteType::MONSTER || getType() == SpriteType::ROLE || this->getType() == SpriteType::DYNAMIC_BLOCK){
		if (hasSimpleTriggerProperties){
			return true;
		}
	}
	return false;
}

void ImageSprite::setHasMonsterDropProperties(bool state)
{
	if (isHasEntityProperties()){
		hasMonsterDropProperties = state;
	}
}

void ImageSprite::setHasAIProperties(bool state)
{
	if (isHasEntityProperties()){
		hasAIProperties = state;
	}
}

void ImageSprite::setHasSimplerTrigger(bool state)
{
	if (isHasEntityProperties()){
		hasSimpleTriggerProperties = state;
	}
}

void ImageSprite::addSimpleTri(unsigned char type, unsigned int id)
{
	SimpleTri* data = new SimpleTri;
	data->type = type;
	data->id = id;
	_simTri_Vec.push_back(data);
}

void ImageSprite::resetSimpleTriType(int index, unsigned char type)
{
	auto data = _simTri_Vec.at(index);
	data->type = type;
}

void ImageSprite::resetSimpleTriId(int index, unsigned int id)
{
	auto data = _simTri_Vec.at(index);
	data->id = id;
}

void ImageSprite::deleteSimpleTri(unsigned char type, unsigned int id)
{
	std::vector<SimpleTri*>::iterator iter;
	for (iter = _simTri_Vec.begin(); iter != _simTri_Vec.end(); iter++){
		auto data = *iter;
		if (data->type == type && id == data->id){
			_simTri_Vec.erase(iter);
			delete data;
			return;
		}
	}
}

void ImageSprite::deleteSimpleTri(int index)
{
	std::vector<SimpleTri*>::iterator iter = _simTri_Vec.begin();
	auto data = _simTri_Vec.at(index);
	_simTri_Vec.erase(iter + index);
	delete data;
}

void ImageSprite::deleteSimpleTriList()
{
	for (int i = 0; i < _simTri_Vec.size(); i++){
		auto data = _simTri_Vec.at(i);
		delete data;
	}
	_simTri_Vec.clear();
}

//这个一般出现在打开的时候
void ImageSprite::setId(unsigned int id)
{
	if (maxid < id){
		maxid = id;
	}
	this->id = id;
}

//当在场景中创建精灵的时候使用的是这个
void ImageSprite::setNewId()
{
	++maxid;
	this->id = maxid;
}

void ImageSprite::autoResetCanAutoResetResName()
{
	switch (this->m_type){
	case SpriteType::NORMAL_BLOCK:
	case SpriteType::DYNAMIC_BLOCK:
		setCanAutoResetResNameInfo(true);
		break;
	case SpriteType::MONSTER:
	case SpriteType::ROLE:
		setCanAutoResetResNameInfo(false);
		break;
	}
}