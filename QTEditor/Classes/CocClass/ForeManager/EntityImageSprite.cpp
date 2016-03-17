#include "EntityImageSprite.h"
#include "QString"
#include "Global.h"
#include "ResMap.h"

EntityImageSprite::EntityImageSprite()
{
	__hp = 1;
	__mp = 0;
	__attack = 0;
	__defense = 0;
	__magic_attack = 0;
	__magic_defense = 0;
	__speed = 0.0f;
	__hard_factor = 0.0f;

	gold = 0;

	rangeMinX = 0;
	rangeMaxX = 0;
	rangeMinY = 0;
	rangeMaxY = 0;
	alertRange = DrawNode::create();
	addChild(alertRange, 2);
	aiFile = "";
}


EntityImageSprite::~EntityImageSprite()
{
	if (alertRange){
		alertRange->removeFromParent();
	}
	deleteUserAIList();
	deleteDropList();
}

EntityImageSprite* EntityImageSprite::create()
{
	auto ret = new EntityImageSprite();
	if (ret && ret->init()){
		return ret;
	}
	return nullptr;
}

EntityImageSprite* EntityImageSprite::create(int index)
{
	EntityImageSprite* ret = new EntityImageSprite();
	if (ret){
		ret->initWithTexture(ResMap::getSinglePngTexture(index));
		ret->imageIndex = index;
		return ret;
	}
	else{
		return NULL;
	}
}

void EntityImageSprite::setHp(int hp)
{
	__hp = hp;
}

void EntityImageSprite::setMp(int mp)
{
	__mp = mp;
}

void EntityImageSprite::setAttack(int attack)
{
	__attack = attack;
}

void EntityImageSprite::setDefense(int defense)
{
	__defense = defense;
}

void EntityImageSprite::setMagic_Attack(int attack)
{
	__magic_attack = attack;
}

void EntityImageSprite::setMagic_Defense(int defense)
{
	__magic_defense = defense;
}

void EntityImageSprite::setSpeed(float speed)
{
	__speed = speed;
}

void EntityImageSprite::setHardFactor(float factor)
{
	__hard_factor = factor;
}

void EntityImageSprite::copySprite(EntityImageSprite* sprite)
{
	ImageSprite::copySprite(sprite);
	if (sprite->isHasMonsterProperties()){
		sprite->setHp(this->__hp);
		sprite->setMp(this->__mp);
		sprite->setAttack(this->__attack);
		sprite->setDefense(this->__defense);
		sprite->setMagic_Attack(this->__magic_attack);
		sprite->setMagic_Defense(this->__magic_defense);
		sprite->setSpeed(this->__speed);
		sprite->setHardFactor(this->__hard_factor);
	}

	{
		sprite->setHasAIProperties(this->isHasAIProperties());
		if (sprite->isHasAIProperties()){
			sprite->setRangeMinX(this->rangeMinX, false);
			sprite->setRangeMaxX(this->rangeMaxX, false);
			sprite->setRangeMinY(this->rangeMinY, false);
			sprite->setRangeMaxY(this->rangeMaxY, false);
			sprite->setAIFile(this->aiFile);
			for (int i = 0; i < _AI_Vec.size(); i++){
				UserAI* da = _AI_Vec.at(i);
				sprite->addUserAI(da->key, da->value);
			}
		}
	}
	
	{
		sprite->setHasMonsterDropProperties(this->isHasMonsterDropProperties());
		if (sprite->isHasMonsterDropProperties()){
			sprite->setDropGold(this->gold);
			for (int i = 0; i < _dropVec.size(); i++){
				dropData* da = _dropVec.at(i);
				sprite->addDrop(da->id, da->odds, da->difficulty);
			}
		}
	}
}

void EntityImageSprite::addDrop(QString id, float odd, int difficulty)
{
	dropData* data = new dropData;
	data->id = id;
	data->odds = odd;
	data->difficulty = difficulty;
	_dropVec.push_back(data);
}

void EntityImageSprite::resetDropId(int index, QString id)
{
	dropData* data = _dropVec.at(index);
	data->id = id;
}

void EntityImageSprite::resetDropOdd(int index, float odd)
{
	dropData* data = _dropVec.at(index);
	data->odds = odd;
}

void EntityImageSprite::resetDropDifficulty(int index, int difficulty)
{
	dropData* data = _dropVec.at(index);
	data->difficulty = difficulty;
}

void EntityImageSprite::deleteDrop(QString id, float odd)
{
	std::vector<dropData*>::iterator iter;
	for (iter = _dropVec.begin(); iter != _dropVec.end(); iter++){
		auto data = *iter;
		if (data->odds = odd && id == data->id){
			_dropVec.erase(iter);
			delete data;
			return;
		}
	}
}

void EntityImageSprite::deleteDrop(int index)
{
	std::vector<dropData*>::iterator iter = _dropVec.begin();
	auto data = _dropVec.at(index);
	_dropVec.erase(iter+index);
	delete data;
}

void EntityImageSprite::setDropGold(int gold)
{
	this->gold = gold;
}

void EntityImageSprite::deleteDropList()
{
	for (int i = 0; i < _dropVec.size(); i++){
		auto data = _dropVec.at(i);
		delete data;
	}
	_dropVec.clear();
}

void EntityImageSprite::addUserAI(QString key, QString value)
{
	UserAI* data = new UserAI;
	data->key = key;
	data->value = value;
	_AI_Vec.push_back(data);
}

void EntityImageSprite::resetUserAIKey(int index, QString key)
{
	auto data = _AI_Vec.at(index);
	data->key = key;
}

void EntityImageSprite::resetUserAIValue(int index, QString value)
{
	auto data = _AI_Vec.at(index);
	data->value = value;
}

void EntityImageSprite::deleteUserAI(QString key, QString value)
{
	std::vector<UserAI*>::iterator iter;
	for (iter = _AI_Vec.begin(); iter != _AI_Vec.end(); iter++){
		auto data = *iter;
		if (data->key == key && value == data->value){
			_AI_Vec.erase(iter);
			delete data;
			return;
		}
	}
}

void EntityImageSprite::deleteUserAI(int index)
{
	std::vector<UserAI*>::iterator iter = _AI_Vec.begin();
	auto data = _AI_Vec.at(index);
	_AI_Vec.erase(iter + index);
	delete data;
}

void EntityImageSprite::deleteUserAIList()
{
	for (int i = 0; i < _AI_Vec.size(); i++){
		auto data = _AI_Vec.at(i);
		delete data;
	}
	_AI_Vec.clear();
}

void EntityImageSprite::setRangeMinX(int x, bool draw)
{
	rangeMinX = x;
	if (draw){
		drawAlertRange();
	}
}

void EntityImageSprite::setRangeMaxX(int x, bool draw)
{
	rangeMaxX = x;
	if (draw){
		drawAlertRange();
	}
}

void EntityImageSprite::setRangeMinY(int y, bool draw)
{
	rangeMinY = y;
	if (draw){
		drawAlertRange();
	}
}

void EntityImageSprite::setRangeMaxY(int y, bool draw)
{
	rangeMaxY = y;
	if (draw){
		drawAlertRange();
	}
}

void EntityImageSprite::drawAlertRange()
{
	alertRange->clear();
	alertRange->setVisible(true);
	auto spritesize = this->getContentSize();
	if (flipx == false){
		Point pos1;
		pos1.x = spritesize.width*0.5 - rangeMinX;
		pos1.y = spritesize.height*0.5 - rangeMinY;
		Point pos2;
		pos2.x = spritesize.width*0.5 + rangeMaxX;
		pos2.y = spritesize.height*0.5 + rangeMaxY;
		alertRange->drawRect(pos1, pos2, Color4F(Color4B(0xDA, 0x00, 0xD6, 0xFF)));
	}
	else{
		Point pos1;
		pos1.x = spritesize.width*0.5 - rangeMaxX;
		pos1.y = spritesize.height*0.5 - rangeMaxY;
		Point pos2;
		pos2.x = spritesize.width*0.5 + rangeMinX;
		pos2.y = spritesize.height*0.5 + rangeMinY;
		alertRange->drawRect(pos1, pos2, Color4F(Color4B(0xDA, 0x00, 0xD6, 0xFF)));
	}
	this->schedule(schedule_selector(EntityImageSprite::setDrawNoVisible), 5.0f);
}

void EntityImageSprite::setDrawNoVisible(float dt)
{
	alertRange->setVisible(false);
}

void EntityImageSprite::setAIFile(std::string file)
{
	aiFile = file;
}

std::string EntityImageSprite::getAIFile()
{
	return aiFile;
}

int EntityImageSprite::getRangeMinX()
{
	return rangeMinX;
}

int EntityImageSprite::getRangeMaxX()
{
	return rangeMaxX;
}

int EntityImageSprite::getRangeMinY()
{
	return rangeMinY;
}

int EntityImageSprite::getRangeMaxY()
{
	return rangeMaxY;
}