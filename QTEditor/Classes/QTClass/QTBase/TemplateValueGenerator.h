#pragma once
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "ImageSprite.h"
#include "EntityImageSprite.h"

class TemplateValueGenerator
{
public:
	TemplateValueGenerator();
	~TemplateValueGenerator();


	static void setSpriteBaseProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteBoxProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteCircleProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteEntityProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteMonsterProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteMonsterDropProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteAIProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteMoveProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteAnimationProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteSimplerTriggerProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);
	static void setSpriteFilterProperties(ImageSprite* sprite, QJsonObject& obj, bool isTemplate);

	static QVariantMap setBasePropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setBoxPropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setCirclePropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setEntityPropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setMonsterPropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setMonsterDropPropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setAIPropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setMovePropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setAnimationPropertiesFromSprite(ImageSprite* sprite, bool isTemplate, bool outputimage, QString relaDir = "");
	static QVariantMap setSimplerTriggerPropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
	static QVariantMap setFilterPropertiesFromSprite(ImageSprite* sprite, bool isTemplate);
};

