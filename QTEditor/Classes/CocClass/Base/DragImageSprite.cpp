#include "DragImageSprite.h"
#include "Global.h"
#include <qtextstream.h>
#include "ControlView.h"
#include "ActionView.h"
#include "TemplateValueGenerator.h"

bool DragImageSprite::t_flipX = false;
bool DragImageSprite::t_flipY = false;
float DragImageSprite::t_scaleX = 1;
float DragImageSprite::t_scaleY = 1;
float DragImageSprite::t_rotation = 0;
Point DragImageSprite::t_anchr = Point(0.5, 0.5);

DragImageSprite::DragImageSprite()
{
	hasBaseProperties = false;
	hasBoxProperties = false;
	hasMoveProperties = false;
	hasEntityProperties = false;
	hasMonsterProperties = false;
	hasMonsterDropProperties = false;
	hasCircleProperties = false;
	hasAIProperties = false;
	hasSimplerTriggerProperties = false;
	hasFilterProperties = false;
}

DragImageSprite::~DragImageSprite()
{
}

DragImageSprite* DragImageSprite::create(std::string file)
{
	DragImageSprite* ret = new DragImageSprite();
	if (ret && ret->initWithFile(file)){
		ret->initOtherData();
		return ret;
	}
	return NULL;
}

void DragImageSprite::initOtherData()
{
	if (g_TemplateFile != ""){
		initSpriteTemplateData();
		initFrameTemplateData();
	}
}

void DragImageSprite::initSpriteTemplateData()
{
	std::string file = g_TemplateFile;
	QFile sFile(file.c_str());
	if (file == "" || !sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		hasBoxProperties = false;
		hasBaseProperties = true;
		hasMoveProperties = false;
		hasCircleProperties = false;
		hasFilterProperties = false;
		return;
	}
	QTextStream in(&sFile);
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toLatin1(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		templateMd5 = obj["md5"].toString().toStdString();
		if (obj["hasBaseProperties"].toBool()){
			hasBaseProperties = true;
			QJsonObject properties = obj["baseProperties"].toObject();
			if (properties.contains("scaleX")){
				float scaleX = properties["scaleX"].toDouble();
				float scaleY = properties["scaleY"].toDouble();
				this->setScaleX(scaleX);
				this->setScaleY(scaleY);
			}
			else{
				float scaleX = properties["scale"].toDouble();
				float scaleY = properties["scale"].toDouble();
				this->setScaleX(scaleX);
				this->setScaleY(scaleY);
			}
			float rotation = properties["rotation"].toInt();
			this->setRotation(rotation);
		}
		if (obj["hasBoxProperties"].toBool()){
			hasBoxProperties = true;
			hasCircleProperties = false;
		}
		else if (obj["hasCircleProperties"].toBool()){
			hasCircleProperties = true;
			hasBoxProperties = false;
		}
		else{
			hasBoxProperties = false;
			hasCircleProperties = false;
		}
		if (obj["hasMoveProperties"].toBool()){
			hasMoveProperties = true;
		}
		if (obj["hasFilterProperties"].toBool()){
			hasFilterProperties = true;
		}
		if (obj["hasEntityProperties"].toBool()){
			hasEntityProperties = true;
		}
		if (obj["hasMonsterProperties"].toBool()){
			hasMonsterProperties = true;
		}
		if (obj["hasMonsterDropProperties"].toBool()){
			hasMonsterDropProperties = true;
		}
		if (obj["hasAIProperties"].toBool()){
			hasAIProperties = true;
		}
		if (obj["hasSimpleTriggerProperties"].toBool()){
			hasSimplerTriggerProperties = true;
		}
	}
	sFile.close();
}

void DragImageSprite::initFrameTemplateData()
{
	std::string file = g_TemplateFile;
	QFile sFile(file.c_str());
	hasAnimation = true;
	if (file == "" || !sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		hasAnimation = false;
		return;
	}
	QTextStream in(&sFile);
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toLatin1(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		hasAnimation = obj["hasAnimation"].toBool();
		if (hasAnimation){
			QJsonObject properties = obj["actionProperties"].toObject();
			animationname = properties["animationName"].toString();
			playType = properties["playType"].toInt();
			perFrameTime = properties["perFrameTime"].toDouble();
		}
	}
	sFile.close();
}

void DragImageSprite::setPropertiesForImageSprite(ImageSprite* sprite)
{
	sprite->setScaleX(this->getScaleX());
	sprite->setScaleY(this->getScaleY());
	sprite->setRotation(this->getRotation());
	std::string file = g_TemplateFile;
	QFile sFile(file.c_str());
	if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		sprite->setHasBaseProperties(hasBaseProperties);
		return;
	}
	QTextStream in(&sFile);
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toLatin1(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		setBasePropertiesFromImageSprite(sprite, obj);
		setMovePropertiesForImageSprite(sprite, obj);
		setFilterPropertiesForImageSprite(sprite, obj);
		if (hasBoxProperties){
			setBoxPropertiesForImageSprite(sprite, obj);
		}
		else if (hasCircleProperties){
			setCirclePropertiesForImageSprite(sprite, obj);
		}
		setEntityPropertiesFroImageSprite(sprite, obj);
		if (hasMonsterProperties){
			auto sprite2 = dynamic_cast<EntityImageSprite*>(sprite);
			setMonsterPropertiesForEntityImageSprite(sprite2, obj);
		}
		if (hasMonsterDropProperties){
			auto sprite2 = dynamic_cast<EntityImageSprite*>(sprite);
			setMonsterDropProperitesForEntityImageSprite(sprite2, obj);
		}
		if (hasAIProperties){
			auto sprite2 = dynamic_cast<EntityImageSprite*>(sprite);
			setAIPropertiesForEntityImageSprite(sprite2, obj);
		}
		if (hasSimplerTriggerProperties){
			setSimplerTriggerPropertiesForImageSprite(sprite, obj);
		}
	}
	sFile.close();
}

void DragImageSprite::setBasePropertiesFromImageSprite(ImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasBaseProperties(hasBaseProperties);
	if (hasBaseProperties){
		TemplateValueGenerator::setSpriteBaseProperties(sprite, obj, true);
	}
}

void DragImageSprite::setBoxPropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasBoxProperties(hasBoxProperties);
	if (hasBoxProperties){
		sprite->setHasCircleProperties(false);
		TemplateValueGenerator::setSpriteBoxProperties(sprite, obj, true);
	}
}

void DragImageSprite::setCirclePropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasCircleProperties(hasCircleProperties);
	if (hasCircleProperties){
		sprite->setHasBoxProperties(false);
		TemplateValueGenerator::setSpriteCircleProperties(sprite, obj, true);
	}
}

void DragImageSprite::setMovePropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasMoveProperties(hasMoveProperties);
	if (hasMoveProperties){
		TemplateValueGenerator::setSpriteMoveProperties(sprite, obj, true);
	}
}

void DragImageSprite::setActinoForImageSprite(ImageSprite* sprite)
{
	sprite->setHasAnimation(hasAnimation);
	if (g_TemplateFile != "" && hasAnimation){
		sprite->setAnimationName(animationname.toStdString());
		//sprite->resetAnimation();
		sprite->setPerTime(perFrameTime);
		sprite->playerAnimation(playType);
	}
}

void DragImageSprite::setEntityPropertiesFroImageSprite(ImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasEntityProperties(hasEntityProperties);
	if (obj["image_Type"].toInt() == 0){
		sprite->setType(SpriteType::NORMAL_BLOCK);
	}
	if (hasEntityProperties){
		TemplateValueGenerator::setSpriteEntityProperties(sprite, obj, true);
	}
}

void DragImageSprite::setMonsterPropertiesForEntityImageSprite(EntityImageSprite* sprite, QJsonObject obj)
{
	TemplateValueGenerator::setSpriteMonsterProperties(sprite, obj, true);
}

void DragImageSprite::setMonsterDropProperitesForEntityImageSprite(EntityImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasMonsterDropProperties(hasMonsterDropProperties);
	if (hasMonsterDropProperties){
		TemplateValueGenerator::setSpriteMonsterDropProperties(sprite, obj, true);
	}
}

void DragImageSprite::setAIPropertiesForEntityImageSprite(EntityImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasAIProperties(hasAIProperties);
	if (hasAIProperties){
		TemplateValueGenerator::setSpriteAIProperties(sprite, obj, true);
	}
}

void DragImageSprite::setSimplerTriggerPropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasSimplerTrigger(hasSimplerTriggerProperties);
	if (hasSimplerTriggerProperties){
		TemplateValueGenerator::setSpriteSimplerTriggerProperties(sprite, obj, true);
	}
}

void DragImageSprite::setFilterPropertiesForImageSprite(ImageSprite* sprite, QJsonObject obj)
{
	sprite->setHasFilterProperties(hasFilterProperties);
	if (hasFilterProperties){
		TemplateValueGenerator::setSpriteFilterProperties(sprite, obj, true);
	}
}

void DragImageSprite::setTemplateToImageSprite(ImageSprite *sprite)
{
	if (g_TemplateFile != ""){
		setPropertiesForImageSprite(sprite);
		setActinoForImageSprite(sprite);
		sprite->setTemplateFile(g_TemplateFile.c_str(), templateMd5);
	}
}

void DragImageSprite::getStateProperties()
{
	this->setAnchorPoint(t_anchr);
	this->setFlippedX(t_flipX);
	this->setFlippedY(t_flipY);
	this->setScaleX(t_scaleX);
	this->setScaleY(t_scaleY);
	this->setRotation(t_rotation);
}