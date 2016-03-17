#pragma once
#include "cocos2d.h"
#include "ImageSprite.h"
#include "EntityImageSprite.h"
#include "ByteArray.h"

using namespace cocos2d;

struct entityData{
	unsigned char entityType;
	float sleepTime;
	unsigned short numGenerate;
	float generateDelay;
	bool hasProperties;
	unsigned int id;
	bool isEqual(entityData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (this->entityType != data.entityType){
			return false;
		}
		if (this->sleepTime != data.sleepTime){
			return false;
		}
		if (this->numGenerate != data.numGenerate){
			return false;
		}
		if (this->generateDelay != data.generateDelay){
			return false;
		}
		return true;
	}
	unsigned short size(){
		if (numGenerate != 1)
			return sizeof(unsigned char) + sizeof(unsigned int) + sizeof(float) + sizeof(unsigned short) + sizeof(float);
		else
			return sizeof(unsigned char) + sizeof(unsigned int) + sizeof(float) + sizeof(unsigned short);
	}
};

struct rigidAABBData{
	unsigned char way;
	float centerX;
	float centerY;
	float halfWidth;
	float halfHeight;
	bool hasProperties;
	bool isEqual(rigidAABBData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (this->way != data.way){
			return false;
		}
		if (this->centerX != data.centerX){
			return false;
		}
		if (this->centerY != data.centerY){
			return false;
		}
		if (this->halfWidth != data.halfWidth){
			return false;
		}
		if (this->halfHeight != data.halfHeight){
			return false;
		}
		return true;
	}
	unsigned short size(){
		return sizeof(unsigned char) + sizeof(float) * 4;
	}
};

struct monsterData{
	int hp;
	int mp;
	int atk;
	int def;
	int mat;
	int mdf;
	float spd;
	float hardFactor;
	bool hasProperties;
	bool isEquals(monsterData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (this->hp != data.hp){
			return false;
		}
		if (this->mp != data.mp){
			return false;
		}
		if (this->atk != data.atk){
			return false;
		}
		if (this->def != data.def){
			return false;
		}
		if (this->mat != data.mat){
			return false;
		}
		if (this->mdf != data.mdf){
			return false;
		}
		if (this->spd != data.spd){
			return false;
		}
		if (this->hardFactor != data.hardFactor){
			return false;
		}
		return true;
	}
	unsigned short size(){
		return sizeof(int) * 6 + sizeof(float) * 2;
	}
};

struct monsterDropData{
	struct Drop_Data{
		std::string id;
		float odds;
		unsigned char difficulty;
		Drop_Data()
		{
			odds = 0.0f;
			difficulty = 0;
		}
	};
	int gold;
	bool hasProperties;
	std::vector< Drop_Data*> _vec;
	bool isEquals(monsterDropData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (_vec.size() != data._vec.size()){
			return false;
		}
		for (int i = 0; i < _vec.size(); i++){
			if (_vec.at(i)->id != data._vec.at(i)->id || _vec.at(i)->odds != data._vec.at(i)->odds || _vec.at(i)->difficulty != data._vec.at(i)->difficulty){
				return false;
			}
		}
		return true;
	}
	unsigned short size(){
		ByteArray* ba = new ByteArray(false);
		ba->writeUnsignedChar(_vec.size());
		for (int i = 0; i < _vec.size(); i++){
			ba->writeString(_vec.at(i)->id);
			ba->writeFloat(_vec.at(i)->odds);
			ba->writeUnsignedChar(_vec.at(i)->difficulty);
		}
		unsigned short si = ba->getPosition();
		delete ba;
		return si;
	}
};

//小心内存泄漏
struct monsterAIData{
	struct key_v{
		std::string key;
		std::string value;
	};
	float rangeMinX;
	float rangeMaxX;
	float rangeMinY;
	float rangeMaxY;
	std::string aiFile;
	bool hasProperties;
	std::vector<key_v*> keyV;
	bool isEquals(monsterAIData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (rangeMinX != data.rangeMinX || rangeMaxX != data.rangeMaxX || rangeMinY != data.rangeMinY || rangeMaxY != data.rangeMaxY){
			return false;
		}
		if (aiFile != data.aiFile){
			return false;
		}
		if (keyV.size() != data.keyV.size()){
			return false;
		}
		for (int i = 0; i < keyV.size(); i++){
			if (keyV.at(i)->key != data.keyV.at(i)->key || keyV.at(i)->value != data.keyV.at(i)->value){
				return false;
			}
		}
		return true;
	}
	unsigned short size(){
		ByteArray* ba = new ByteArray(false);
		ba->writeFloat(rangeMinX);
		ba->writeFloat(rangeMinY);
		ba->writeFloat(rangeMaxX);
		ba->writeFloat(rangeMaxY);
		ba->writeString(aiFile);
		ba->writeUnsignedChar(keyV.size());
		for (int i = 0; i < keyV.size(); i++){
			ba->writeString(keyV.at(i)->key);
			ba->writeString(keyV.at(i)->value);
		}
		unsigned short si = ba->getPosition();
		delete ba;
		return si;
	}
};

struct rigidCircleData{
	float centerX;
	float centerY;
	float radius;
	bool hasProperties;
	bool isEquals(rigidCircleData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (centerX != data.centerX || centerY != data.centerY){
			return false;
		}
		if (radius != data.radius){
			return false;
		}
		return true;
	}
	unsigned short size(){
		return sizeof(float)*3;
	}
};

struct lineMoveData
{
	float delay;
	float backDelay;
	float startRunTime;
	float velocityX;
	float velocityY;
	float distance;
	bool hasProperties;
	bool isEqual(lineMoveData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (this->delay != data.delay){
			return false;
		}
		if (this->backDelay != data.backDelay){
			return false;
		}
		if (this->startRunTime != data.startRunTime){
			return false;
		}
		if (this->velocityX != data.velocityX){
			return false;
		}
		if (this->velocityY != data.velocityY){
			return false;
		}
		if (this->distance != data.distance){
			return false;
		}
		return true;
	};
	unsigned short size(){
		return sizeof(float) * 6;
	}
};

struct triggerData{
	struct tri_v{
		unsigned char tiggerType;
		unsigned int targetID;
	};
	std::vector<tri_v*> tri_V;
	bool hasProperties;
	bool isEqual(triggerData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (tri_V.size() != data.tri_V.size()){
			return false;
		}
		for (int i = 0; i < tri_V.size(); i++){
			if (tri_V.at(i)->tiggerType != data.tri_V.at(i)->tiggerType || tri_V.at(i)->targetID != data.tri_V.at(i)->targetID){
				return false;
			}
		}
		return true;
	}
	unsigned short size(){
		ByteArray* ba = new ByteArray(false);
		ba->writeUnsignedChar(tri_V.size());
		for (int i = 0; i < tri_V.size(); i++){
			ba->writeUnsignedChar(tri_V.at(i)->tiggerType);
			ba->writeUnsignedInt(tri_V.at(i)->targetID);
		}
		unsigned short si = ba->getPosition();
		delete ba;
		return si;
	}
};

struct filterData
{
	unsigned char filterType;
	bool hasProperties;
	bool isEqual(filterData data){
		if (!this->hasProperties && !data.hasProperties){
			return true;
		}
		if (this->hasProperties != data.hasProperties){
			return false;
		}
		if (this->filterType != data.filterType){
			return false;
		}
		return true;
	}
	unsigned short size(){
		return sizeof(unsigned char);
	}
};

class OutputSpriteData
{
public:
	OutputSpriteData();
	~OutputSpriteData();

	bool isEqual(OutputSpriteData* otherdata);
public:
	unsigned char resType;
	bool isAtlas;
	std::string resName;
	std::string animName;
	std::string playerNumber;
	unsigned short numInstances;
	std::vector<Vec2> posvec;
	std::vector<bool> flipXvec;
	std::vector<bool> flipYvec;
	std::vector<Vec2> anchrVec;
	std::vector<Vec2> scaleVec;
	std::vector<float> rotationVec;
	cocos2d::Size contentSize;
	unsigned char numController;
	entityData en_Data;
	lineMoveData line_moveData;
	rigidAABBData rigidData;
	monsterData monster;
	rigidCircleData circleData;
	monsterDropData monsterDrop;
	monsterAIData aiData;
	triggerData triData;
	filterData filData;
	bool dontMerge;
	int layerIndex;
};

