#pragma once
#include "cocos2d.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "ImageSprite.h"
#include "EntityImageSprite.h"

using namespace cocos2d;

class DragImageSprite : public Sprite
{
public:
	static DragImageSprite* create(std::string file);
	void initOtherData();
	void initSpriteTemplateData();
	void initFrameTemplateData();
	void setTemplateToImageSprite(ImageSprite *sprite);
	void getStateProperties();
	static cocos2d::Point t_anchr;
	static bool t_flipX;
	static bool t_flipY;
	static float t_scaleX;
	static float t_scaleY;
	static float t_rotation;
protected:
	DragImageSprite();
	virtual ~DragImageSprite();
	void setPropertiesForImageSprite(ImageSprite* sprite);
	void setBasePropertiesFromImageSprite(ImageSprite* sprite, QJsonObject obj);
	void setBoxPropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj);
	void setCirclePropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj);
	void setMovePropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj);
	void setEntityPropertiesFroImageSprite(ImageSprite* sprite, QJsonObject obj);
	void setActinoForImageSprite(ImageSprite* sprite);
	void setMonsterPropertiesForEntityImageSprite(EntityImageSprite* sprite, QJsonObject obj);
	void setMonsterDropProperitesForEntityImageSprite(EntityImageSprite* sprite, QJsonObject obj);
	void setAIPropertiesForEntityImageSprite(EntityImageSprite* sprite, QJsonObject obj);
	void setSimplerTriggerPropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj);
	void setFilterPropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj);
private:
	QString animationname;
	int playType;
	bool hasBaseProperties;
	bool hasBoxProperties;
	bool hasMoveProperties;
	bool hasEntityProperties;
	bool hasCircleProperties;
	bool hasMonsterProperties;
	bool hasMonsterDropProperties;
	bool hasAIProperties;
	bool hasAnimation;
	bool hasSimplerTriggerProperties;
	bool hasFilterProperties;
	double perFrameTime;
	std::string templateMd5;
};

