#include "TemplateValueGenerator.h"
#include "FontChina.h"
#include "Global.h"
#include "ImgTransPath.h"

TemplateValueGenerator::TemplateValueGenerator()
{
}

TemplateValueGenerator::~TemplateValueGenerator()
{
}

void TemplateValueGenerator::setSpriteBaseProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	if (isTemplate){
		if (baseProperties.contains("scaleX")){
			if (baseProperties["selectScaleX"].toBool()){
				sprite->setScaleX(baseProperties["scaleX"].toDouble());
			}
			if (baseProperties["selectScaleY"].toBool()){
				sprite->setScaleY(baseProperties["scaleY"].toDouble());
			}
		}
		else{
			sprite->setScaleX(baseProperties["scale"].toDouble());
			sprite->setScaleY(baseProperties["scale"].toDouble());
		}

		if (baseProperties["selectRotation"].toBool()){
			sprite->setRotation(baseProperties["rotation"].toDouble());
		}

		if (baseProperties["selectFlipx"].toBool()){
			bool x = baseProperties["flipx"].toBool();
			sprite->setFlippedX(x);
			sprite->flipx = x;
		}
		if (baseProperties["selectFlipy"].toBool()){
			bool y = baseProperties["flipy"].toBool();
			sprite->setFlippedY(y);
			sprite->flipy = y;
		}
		if (baseProperties["selectDontMerge"].toBool()){
			bool y = baseProperties["dontMerge"].toBool();
			sprite->setDontMerge(y);
		}
		if (baseProperties["selectAnchrx"].toBool()){
			Point anchr;
			anchr.x = baseProperties["anchrx"].toDouble();
			anchr.y = sprite->anchr.y;
			sprite->anchr = anchr;
			sprite->setAnchorPoint(anchr);
		}
		if (baseProperties["selectAnchry"].toBool()){
			Point anchr;
			anchr.y = baseProperties["anchry"].toDouble();
			anchr.x = sprite->anchr.x;
			sprite->anchr = anchr;
			sprite->setAnchorPoint(anchr);
		}
	}
	else{
		if (baseProperties.contains("scaleX")){
			sprite->setScaleX(baseProperties["scaleX"].toDouble());
			sprite->setScaleY(baseProperties["scaleY"].toDouble());
		}
		else{
			sprite->setScaleX(baseProperties["scale"].toDouble());
			sprite->setScaleY(baseProperties["scale"].toDouble());
		}
		sprite->setRotation(baseProperties["rotation"].toDouble());
		sprite->setFlippedX(baseProperties["flipx"].toBool());
		sprite->setFlippedY(baseProperties["flipy"].toBool());
		sprite->setDontMerge(baseProperties["dontMerge"].toBool());
		sprite->flipx = baseProperties["flipx"].toBool();
		sprite->flipy = baseProperties["flipy"].toBool();
		sprite->anchr.x = baseProperties["anchrx"].toDouble();
		sprite->anchr.y = baseProperties["anchry"].toDouble();
		sprite->setAnchorPoint(sprite->anchr);
	}
	
}

void TemplateValueGenerator::setSpriteBoxProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject boxProperties = obj["boxProperties"].toObject();
	if (isTemplate){
		Point anchr = sprite->getBoxAnchrPos();
		if (boxProperties["selectAnchrx"].toBool()){
			anchr.x = boxProperties["anchrx"].toDouble();
		}
		if (boxProperties["selectAnchry"].toBool()){
			anchr.y = boxProperties["anchry"].toDouble();
		}
		Size half = sprite->getHalf();
		if (boxProperties["selectHalfWidth"].toBool()){
			half.width = boxProperties["halfWidth"].toDouble();
		}
		if (boxProperties["selectHalfHeight"].toBool()){
			half.height = boxProperties["halfHeight"].toDouble();
		}
		int way = sprite->getBoxWay();
		if (boxProperties["selectWay"].toBool()){
			way = boxProperties["way"].toInt();
		}
		sprite->drawRigid(anchr, half, way);
		//这个不做处理
		if (boxProperties["showRigid"].toBool()){
			sprite->showRigid(0);
		}
		else{
			sprite->showRigid(1);
		}
	}
	else{
		Point anchr = Point(boxProperties["anchrx"].toDouble(), boxProperties["anchry"].toDouble());
		Size half = Size(boxProperties["halfWidth"].toDouble(), boxProperties["halfHeight"].toDouble());
		float way = boxProperties["way"].toInt();
		sprite->drawRigid(anchr, half, way);
		sprite->showRigid(boxProperties["showRigid"].toInt());
	}
}

void TemplateValueGenerator::setSpriteCircleProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject circleProperties = obj["circleProperties"].toObject();
	if (isTemplate){
		Point anchr = sprite->getCircleAnchrPos();
		if (circleProperties["selectAnchrx"].toBool()){
			anchr.x = circleProperties["anchrx"].toDouble();
		}
		if (circleProperties["selectAnchry"].toBool()){
			anchr.y = circleProperties["anchry"].toDouble();
		}

		float radius = sprite->getRadius();
		if (circleProperties["selectRadius"].toBool()){
			radius = circleProperties["radius"].toDouble();
		}
		sprite->drawRigid(anchr, radius);
		//不做处理
		if (circleProperties["showRigid"].toBool()){
			sprite->showRigid(0);
		}
		else{
			sprite->showRigid(1);
		}
	}
	else{
		Point anchr = Point(circleProperties["anchrx"].toDouble(), circleProperties["anchry"].toDouble());
		float radius = circleProperties["radius"].toDouble();
		sprite->drawRigid(anchr, radius);
		sprite->showRigid(circleProperties["showRigid"].toInt());
	}
}

void TemplateValueGenerator::setSpriteEntityProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject entityProperties = obj["entityProperties"].toObject();
	if (isTemplate){
		if (entityProperties["selectNumGenerate"].toBool()){
			sprite->setNumGenerate(entityProperties["numGenerate"].toInt());
		}
		if (entityProperties["selectGenerateDelay"].toBool()){
			sprite->setGenerateDelay(entityProperties["generateDelay"].toDouble());
		}
		if (entityProperties["selectSleepTime"].toBool()){
			sprite->setSleepTime(entityProperties["sleepTime"].toDouble());
		}
		if (entityProperties["selectSpriteType"].toBool()){
			switch (entityProperties["spriteType"].toInt()){
			case 0:
				sprite->setType(SpriteType::NORMAL_BLOCK);
				break;
			case 1:
				sprite->setType(SpriteType::DYNAMIC_BLOCK);
				break;
			case 2:
				sprite->setType(SpriteType::MONSTER);
				break;
			case 3:
				sprite->setType(SpriteType::ROLE);
				break;
			}
		}
	}
	else{
		switch (entityProperties["spriteType"].toInt()){
		case 1:
			sprite->setType(SpriteType::DYNAMIC_BLOCK);
			break;
		case 2:
			sprite->setType(SpriteType::MONSTER);
			break;
		case 3:
			sprite->setType(SpriteType::ROLE);
			break;
		}
		sprite->setNumGenerate(entityProperties["numGenerate"].toInt());
		sprite->setGenerateDelay(entityProperties["generateDelay"].toDouble());
		sprite->setSleepTime(entityProperties["sleepTime"].toDouble());
	}
}

void TemplateValueGenerator::setSpriteMonsterProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	if (isTemplate){
		if (monsterProperties["selectHp"].toBool()){
			entitySprite->setHp(monsterProperties["hp"].toInt());
		}
		if (monsterProperties["selectMp"].toBool()){
			entitySprite->setMp(monsterProperties["mp"].toInt());
		}
		if (monsterProperties["selectAttack"].toBool()){
			entitySprite->setAttack(monsterProperties["attack"].toInt());
		}
		if (monsterProperties["selectDefense"].toBool()){
			entitySprite->setDefense(monsterProperties["defense"].toInt());
		}
		if (monsterProperties["selectMagic_Attack"].toBool()){
			entitySprite->setMagic_Attack(monsterProperties["magic_attack"].toInt());
		}
		if (monsterProperties["selectMagic_Defense"].toBool()){
			entitySprite->setMagic_Defense(monsterProperties["magic_defense"].toInt());
		}
		if (monsterProperties["selectSpeed"].toBool()){
			entitySprite->setSpeed(monsterProperties["speed"].toDouble());
		}
		if (monsterProperties["selectHardFactor"].toBool()){
			entitySprite->setHardFactor(monsterProperties["hardFactor"].toDouble());
		}
	}
	else{
		entitySprite->setHp(monsterProperties["hp"].toInt());
		entitySprite->setMp(monsterProperties["mp"].toInt());
		entitySprite->setAttack(monsterProperties["attack"].toInt());
		entitySprite->setDefense(monsterProperties["defense"].toInt());
		entitySprite->setMagic_Attack(monsterProperties["magic_attack"].toInt());
		entitySprite->setMagic_Defense(monsterProperties["magic_defense"].toInt());
		entitySprite->setSpeed(monsterProperties["speed"].toDouble());
		entitySprite->setHardFactor(monsterProperties["hardFactor"].toDouble());
	}
}

void TemplateValueGenerator::setSpriteMonsterDropProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject monsterDropProperties = obj["monsterDropProperties"].toObject();
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	if (isTemplate){
		if (monsterDropProperties["selectGold"].toBool()){
			entitySprite->setDropGold(monsterDropProperties["gold"].toInt());
		}
		entitySprite->deleteDropList();
		auto dropList = monsterDropProperties["list"].toArray();
		for (int i = 0; i < dropList.size(); i++){
			auto data_ = dropList.at(i).toObject();
			entitySprite->addDrop(data_["id"].toString(), data_["odd"].toDouble(), data_["difficulty"].toInt());
		}
	}
	else{
		entitySprite->setDropGold(monsterDropProperties["gold"].toInt());
		entitySprite->deleteDropList();
		QJsonArray list = monsterDropProperties["list"].toArray();
		for (int i = 0; i < list.size(); i++){
			auto data = list.at(i).toObject();
			entitySprite->addDrop(data["id"].toString(), data["odd"].toDouble(), data["difficulty"].toInt());
		}
	}
}
	
void TemplateValueGenerator::setSpriteAIProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	if (isTemplate){
		if (aiProperties["selectMinX"].toBool()){
			entitySprite->setRangeMinX(aiProperties["minX"].toInt(), false);
		}
		if (aiProperties["selectMaxX"].toBool()){
			entitySprite->setRangeMaxX(aiProperties["maxX"].toInt(), false);
		}
		if (aiProperties["selectMinY"].toBool()){
			entitySprite->setRangeMinY(aiProperties["minY"].toInt(), false);
		}
		if (aiProperties["selectMaxY"].toBool()){
			entitySprite->setRangeMaxY(aiProperties["maxY"].toInt(), false);
		}
		if (aiProperties["selectAiFile"].toBool()){
			entitySprite->setAIFile(aiProperties["aiFile"].toString().toStdString());
		}
		auto vec = entitySprite->getUserAIList();
		auto list = aiProperties["list"].toArray();
		for (int i = 0; i < list.size(); i++){
			auto data_ = list.at(i).toObject();
			bool hasKey = false;
			for (int n = 0; n < vec.size(); n++){
				if (data_["key"].toString() == vec.at(n)->key){
					hasKey = true;
					vec.at(n)->value = data_["value"].toString();
				}
			}
			if (!hasKey){
				entitySprite->addUserAI(data_["key"].toString(), data_["value"].toString());
			}
		}
	}
	else{
		entitySprite->setAIFile(aiProperties["aiFile"].toString().toStdString());
		entitySprite->setRangeMinX(aiProperties["minX"].toInt(), false);
		entitySprite->setRangeMaxX(aiProperties["maxX"].toInt(), false);
		entitySprite->setRangeMinY(aiProperties["minY"].toInt(), false);
		entitySprite->setRangeMaxY(aiProperties["maxY"].toInt(), false);
		entitySprite->deleteUserAIList();
		auto list = aiProperties["list"].toArray();
		for (int i = 0; i < list.size(); i++){
			auto data_ = list.at(i).toObject();
			entitySprite->addUserAI(data_["key"].toString(), data_["value"].toString());
		}
	}
}

void TemplateValueGenerator::setSpriteMoveProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	if (isTemplate){
		Point translate = sprite->getTranslatDis();
		if (moveProperties["selectTranslatex"].toBool()){
			translate.x = moveProperties["translatex"].toDouble();
		}
		if (moveProperties["selectTranslatey"].toBool()){
			translate.y = moveProperties["translatey"].toDouble();
		}
		sprite->translatLine(translate);
		if (moveProperties["selectRunTime"].toBool()){
			sprite->setMoveTime(moveProperties["runTime"].toDouble());
		}
		if (moveProperties["selectDelay"].toBool()){
			sprite->setMoveDelayTime(moveProperties["delay"].toDouble());
		}
		if (!moveProperties.contains("selectBackDelay")){
			sprite->setMoveBackDelayTime(sprite->getMoveDelayTime());
		}
		else{
			if (moveProperties["selectBackDelay"].toBool()){
				sprite->setMoveBackDelayTime(moveProperties["backDelay"].toDouble());
			}
		}

		if (moveProperties["selectStartRunTime"].toBool()){
			sprite->setStartRunTime(moveProperties["startRunTime"].toDouble());
		}
		if (moveProperties["selectRunType"].toBool()){
			switch (moveProperties["runType"].toInt()){
			case 0:
				sprite->stopMove();
				break;
			case 1:
				sprite->pauseMove();
				break;
			case 2:
				sprite->runMove(sprite->getMoveTime(), sprite->getMoveDelayTime(), sprite->getMoveBackDelayTime());
				break;
			}
		}
	}
	else{
		sprite->translatLine(Point(moveProperties["translatex"].toDouble(), moveProperties["translatey"].toDouble()));
		sprite->setMoveDelayTime(moveProperties["delay"].toDouble());
		sprite->setMoveTime(moveProperties["runTime"].toDouble());
		if (!moveProperties.contains("backDelay")){
			sprite->setMoveBackDelayTime(sprite->getMoveDelayTime());
		}
		else{
			sprite->setMoveBackDelayTime(moveProperties["backDelay"].toDouble());
		}
		sprite->setStartRunTime(moveProperties["startRunTime"].toDouble());
		switch (moveProperties["runType"].toInt()){
		case 0:
			sprite->stopMove();
			break;
		case 1:
			sprite->pauseMove();
			break;
		case 2:
			sprite->runMove(moveProperties["runTime"].toDouble(), sprite->getMoveDelayTime(), sprite->getMoveBackDelayTime());
			break;
		}
	}
}

void TemplateValueGenerator::setSpriteAnimationProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject actions = obj["actionProperties"].toObject();
	if (isTemplate){
		sprite->setAnimationName(actions["animationName"].toString().toStdString());
		sprite->resetAnimation();
		if (actions["selectPerFrameTime"].toBool()){
			sprite->setPerTime(actions["perFrameTime"].toDouble());
		}
		if (actions["selectPlayType"].toBool()){
			sprite->playerAnimation(actions["playType"].toInt());
		}
	}
	else{
		sprite->setAnimationName(actions["animationName"].toString().toStdString());
		sprite->resetAnimation();
		sprite->setPerTime(actions["perFrameTime"].toDouble());
		sprite->playerAnimation(actions["playType"].toInt());
	}
}

void TemplateValueGenerator::setSpriteSimplerTriggerProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject simplerTriggerProperties = obj["simpleTriggerProperties"].toObject();
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	if (isTemplate){
		sprite->deleteSimpleTriList();
		auto list = simplerTriggerProperties["list"].toArray();
		for (int i = 0; i < list.size(); i++){
			auto data_ = list.at(i).toObject();
			entitySprite->addSimpleTri(data_["id"].toInt(), data_["type"].toInt());
		}
	}
	else{
		entitySprite->deleteSimpleTriList();
		auto list = simplerTriggerProperties["list"].toArray();
		for (int i = 0; i < list.size(); i++){
			auto data_ = list.at(i).toObject();
			entitySprite->addSimpleTri(data_["id"].toInt(), data_["type"].toInt());
		}
	}
}

void TemplateValueGenerator::setSpriteFilterProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate)
{
	QJsonObject filterProperties = obj["filterProperties"].toObject();
	if (isTemplate){
		if (filterProperties["selectFilterType"].toBool()){
			sprite->setFilterType(filterProperties["filterType"].toInt());
		}
	}
	else{
		sprite->setFilterType(filterProperties["filterType"].toInt());
	}
}

QVariantMap TemplateValueGenerator::setBasePropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	QVariantMap baseProperties;
	if (isTemplate){
		baseProperties.insert("selectScaleX", true);
		baseProperties.insert("selectScaleY", true);
		baseProperties.insert("selectRotation", true);
		baseProperties.insert("selectFlipx", true);
		baseProperties.insert("selectFlipy", true);
		baseProperties.insert("selectDontMerge", true);
		baseProperties.insert("selectAnchrx", true);
		baseProperties.insert("selectAnchry", true);
	}
	baseProperties.insert("scaleX", sprite->getScaleX());
	baseProperties.insert("scaleY", sprite->getScaleY());
	baseProperties.insert("rotation", sprite->getRotation());
	baseProperties.insert("flipx", sprite->isFlippedX());
	baseProperties.insert("flipy", sprite->isFlippedY());
	baseProperties.insert("dontMerge", sprite->isDontMerge());
	//这个是精灵的锚点
	baseProperties.insert("anchrx", sprite->anchr.x);
	baseProperties.insert("anchry", sprite->anchr.y);
	return baseProperties;
}

QVariantMap TemplateValueGenerator::setBoxPropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	QVariantMap boxProperties;
	if (isTemplate){
		boxProperties.insert("selectAnchrx", true);
		boxProperties.insert("selectAnchry", true);
		boxProperties.insert("selectHalfWidth", true);
		boxProperties.insert("selectHalfHeight", true);
		boxProperties.insert("selectShowRigid", true);
		boxProperties.insert("selectWay", true);
	}
	boxProperties.insert("anchrx", sprite->getBoxAnchrPos().x);
	boxProperties.insert("anchry", sprite->getBoxAnchrPos().y);
	boxProperties.insert("halfWidth", sprite->getHalf().width);
	boxProperties.insert("halfHeight", sprite->getHalf().height);
	if (sprite->getDrawNodeVisible()){
		boxProperties.insert("showRigid", 0);
	}
	else{
		boxProperties.insert("showRigid", 1);
	}
	boxProperties.insert("way", sprite->getBoxWay());
	return boxProperties;
}

QVariantMap TemplateValueGenerator::setCirclePropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	QVariantMap circleProperties;
	if (isTemplate){
		circleProperties.insert("selectAnchrx", true);
		circleProperties.insert("selectAnchry", true);
		circleProperties.insert("selectRadius", true);
		circleProperties.insert("selectShowRagid", true);
	}
	circleProperties.insert("anchrx", sprite->getCircleAnchrPos().x);
	circleProperties.insert("anchry", sprite->getCircleAnchrPos().y);
	circleProperties.insert("radius", sprite->getRadius());
	if (sprite->getDrawNodeVisible()){
		circleProperties.insert("showRigid", 0);
	}
	else{
		circleProperties.insert("showRigid", 1);
	}
	return circleProperties;
}

QVariantMap TemplateValueGenerator::setEntityPropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	QVariantMap entityProperties;
	if (isTemplate){
		entityProperties.insert("selectGenerateDelay", true);
		entityProperties.insert("selectNumGenerate", true);
		entityProperties.insert("selectSleepTime", true);
		entityProperties.insert("selectSpriteType", true);
	}
	entityProperties.insert("generateDelay", sprite->getGenerateDelay());
	entityProperties.insert("numGenerate", sprite->getNumGenerate());
	entityProperties.insert("sleepTime", sprite->getSleepTime());
	switch (sprite->getType()){
	case SpriteType::DYNAMIC_BLOCK:
		entityProperties.insert("spriteType", 1);
		break;
	case SpriteType::MONSTER:
		entityProperties.insert("spriteType", 2);
		break;
	case SpriteType::ROLE:
		entityProperties.insert("spriteType", 3);
		break;
	case SpriteType::NORMAL_BLOCK:
		entityProperties.insert("spriteType", 0);
		break;
	}
	return entityProperties;
}

QVariantMap TemplateValueGenerator::setMonsterPropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	QVariantMap monsterProperties;
	if (isTemplate){
		monsterProperties.insert("selectAttack", true);
		monsterProperties.insert("selectDefense", true);
		monsterProperties.insert("selectHp", true);
		monsterProperties.insert("selectMp", true);
		monsterProperties.insert("selectMagic_Attack", true);
		monsterProperties.insert("selectMagic_Defense", true);
		monsterProperties.insert("selectSpeed", true);
		monsterProperties.insert("selectHardFactor", true);
	}
	monsterProperties.insert("hp", entitySprite->getHp());
	monsterProperties.insert("mp", entitySprite->getMp());
	monsterProperties.insert("attack", entitySprite->getAttack());
	monsterProperties.insert("defense", entitySprite->getDefense());
	monsterProperties.insert("magic_attack", entitySprite->getMagicAttack());
	monsterProperties.insert("magic_defense", entitySprite->getMagicDefense());
	monsterProperties.insert("speed", entitySprite->getSpeed());
	monsterProperties.insert("hardFactor", entitySprite->getHardFactor());
	return monsterProperties;
}

QVariantMap TemplateValueGenerator::setMonsterDropPropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	QVariantMap monsterDropProperties;
	if (isTemplate){
		monsterDropProperties.insert("selectGold", true);
	}
	monsterDropProperties.insert("gold", entitySprite->getDropGold());
	QVariantList dropList;
	for (int i = 0; i < entitySprite->getDropList().size(); i++){
		auto data = entitySprite->getDropList().at(i);
		QVariantMap drop_Data;
		drop_Data.insert("id", data->id);
		drop_Data.insert("odd", data->odds);
		drop_Data.insert("difficulty", data->difficulty);
		dropList.push_back(drop_Data);
	}
	monsterDropProperties.insert("list", dropList);
	return monsterDropProperties;
}

QVariantMap TemplateValueGenerator::setAIPropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	QVariantMap aiProperties;
	if (isTemplate){
		aiProperties.insert("selectAiFile", true);
		aiProperties.insert("selectMaxX", true);
		aiProperties.insert("selectMaxY", true);
		aiProperties.insert("selectMinX", true);
		aiProperties.insert("selectMinY", true);
	}
	aiProperties.insert("minX", entitySprite->getRangeMinX());
	aiProperties.insert("maxX", entitySprite->getRangeMaxX());
	aiProperties.insert("minY", entitySprite->getRangeMinY());
	aiProperties.insert("maxY", entitySprite->getRangeMaxY());
	aiProperties.insert("aiFile", entitySprite->getAIFile().c_str());
	QVariantList List;
	for (int i = 0; i < entitySprite->getUserAIList().size(); i++){
		auto data = entitySprite->getUserAIList().at(i);
		QVariantMap AI_Data;
		AI_Data.insert("key", data->key);
		AI_Data.insert("value", data->value);
		List.push_back(AI_Data);
	}
	aiProperties.insert("list", List);
	return aiProperties;
}

QVariantMap TemplateValueGenerator::setMovePropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	QVariantMap moveProperties;
	if (isTemplate){
		moveProperties.insert("selectDelay", true);
		moveProperties.insert("selectRunTime", true);
		moveProperties.insert("selectRunType", true);
		moveProperties.insert("selectTranslatex", true);
		moveProperties.insert("selectTranslatey", true);
		moveProperties.insert("selecBackDelay", true);
		moveProperties.insert("selecStartRunTime", true);
	}
	moveProperties.insert("translatex", sprite->getTranslatDis().x);
	moveProperties.insert("translatey", sprite->getTranslatDis().y);
	moveProperties.insert("runTime", sprite->getMoveTime());
	moveProperties.insert("runType", sprite->getMoveType());
	moveProperties.insert("delay", sprite->getMoveDelayTime());
	moveProperties.insert("backDelay", sprite->getMoveBackDelayTime());
	moveProperties.insert("startRunTime", sprite->getStartRunTime());
	return moveProperties;
}

QVariantMap TemplateValueGenerator::setAnimationPropertiesFromSprite(ImageSprite* sprite, bool isTemplate, bool outputimage, QString relaDir)
{
	QVariantMap actions;
	if (isTemplate){
		actions.insert("selectPerFrameTime", true);
		actions.insert("selectPlayType", true);
	}
	actions.insert("animationName", sprite->getAnimationName().c_str());
	actions.insert("perFrameTime", sprite->getPerTime());
	actions.insert("playType", sprite->getPlayerType());
	return actions;
}

QVariantMap TemplateValueGenerator::setSimplerTriggerPropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
	QVariantMap simpleTriggerProperties;
	QVariantList List;
	for (int i = 0; i < entitySprite->getSimpleTriggerList().size(); i++){
		auto data = entitySprite->getSimpleTriggerList().at(i);
		QVariantMap SimTri_Data;
		SimTri_Data.insert("type", data->type);
		SimTri_Data.insert("id", data->id);
		List.push_back(SimTri_Data);
	}
	simpleTriggerProperties.insert("list", List);
	return simpleTriggerProperties;
}

QVariantMap TemplateValueGenerator::setFilterPropertiesFromSprite(ImageSprite* sprite, bool isTemplate)
{
	QVariantMap filterProperties;
	if (isTemplate){
		filterProperties.insert("selectFilterType", true);
	}
	filterProperties.insert("filterType", sprite->getFilterType());
	return filterProperties;
}