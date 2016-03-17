#include "TemplateListItem.h"
#include "Global.h"
#include "EntityImageSprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "TemplateValueGenerator.h"

TemplateListItem::TemplateListItem()
:QListWidgetItem()
{
}

TemplateListItem::~TemplateListItem()
{
}

QString TemplateListItem::getType()
{
	return m_type;
}

void TemplateListItem::setType(QString type)
{
	m_type = type;
}

QString TemplateListItem::getFile()
{
	return m_file;
}

void TemplateListItem::setFile(QString file)
{
	m_file = file;
}

void TemplateListItem::addNode(Node* node)
{
	_vec.pushBack(node);
}

void TemplateListItem::clearVec()
{
	_vec.clear();
}

void TemplateListItem::removeNode(Node* node)
{
	_vec.eraseObject(node);
	for (int i = 0; i < _vec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(_vec.at(i));
		addConsoleInfo(sprite->getTagName());
	}
}

void TemplateListItem::setMD5(QString md5)
{
	this->md5 = md5;
}

void TemplateListItem::updateAllSpriteValue()
{
	QFile sFile(m_file);
	if ( !sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		return ;
	}
	QTextStream in(&sFile);
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toLatin1(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		for (int i = 0; i < _vec.size(); i++){
			auto sprite = dynamic_cast<ImageSprite*>(_vec.at(i));
			setProperties(sprite, obj);
		}
	}
	sFile.close();
}

void TemplateListItem::updateAllSpriteTemplateNameAndMd5()
{
	for (int i = 0; i < _vec.size(); i++){
		auto sprite = dynamic_cast<ImageSprite*>(_vec.at(i));
		sprite->setTemplateFile(m_file.toLocal8Bit().data(), md5.toLocal8Bit().data());
	}
}

void TemplateListItem::setProperties(ImageSprite* sprite, QJsonObject& obj)
{
	sprite->setHasAnimation(obj["hasAnimation"].toBool());
	sprite->setHasBaseProperties(obj["hasBaseProperties"].toBool());
	sprite->setHasMoveProperties(obj["hasMoveProperties"].toBool());
	sprite->setHasBoxProperties(obj["hasBoxProperties"].toBool());
	sprite->setHasCircleProperties(obj["hasCircleProperties"].toBool());
	sprite->setHasEntityProperties(obj["hasEntityProperties"].toBool());
	sprite->setHasAIProperties(obj["hasAIProperties"].toBool());
	sprite->setHasSimplerTrigger(obj["hasSimpleTriggerProperties"].toBool());
	sprite->setHasMonsterDropProperties(obj["hasMonsterDropProperties"].toBool());
	sprite->setHasSimplerTrigger(obj["hasSimpleTriggerProperties"].toBool());
	sprite->setHasFilterProperties(obj["hasFilterProperties"].toBool());

	if (obj["hasBaseProperties"].toBool()){
		setBaseProperties(sprite, obj);
	}
	if (obj["hasBoxProperties"].toBool()){
		setBoxProperties(sprite, obj);
	}
	else if (obj["hasCircleProperties"].toBool()){
		setCircleProperties(sprite, obj);
	}

	if (obj["hasEntityProperties"].toBool()){
		setEntityProperties(sprite, obj);
	}

	if (obj["hasMonsterProperties"].toBool()){
		setMonsterProperties(sprite, obj);
	}

	if (obj["hasMonsterDropProperties"].toBool()){
		setMonsterDropProperties(sprite, obj);
	}

	if (obj["hasAIProperties"].toBool()){
		setAIProperties(sprite, obj);
	}

	if (obj["hasMoveProperties"].toBool()){
		setMoveProperties(sprite, obj);
	}
	if (obj["hasAnimation"].toBool()){
		setAnimationProperties(sprite, obj);
	}

	if (obj["hasSimpleTriggerProperties"].toBool()){
		setSimplerTriggerProperties(sprite, obj);
	}

	if (obj["hasFilterProperties"].toBool()){
		setFilterProperties(sprite, obj);
	}
}

void TemplateListItem::setBaseProperties(ImageSprite* sprite, QJsonObject& obj)
{
	//set BaseProperties
	TemplateValueGenerator::setSpriteBaseProperties(sprite, obj, true);
}

void TemplateListItem::setBoxProperties(ImageSprite* sprite, QJsonObject& obj)
{
	//set BoxProperties
	TemplateValueGenerator::setSpriteBoxProperties(sprite, obj, true);
}

void TemplateListItem::setCircleProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteCircleProperties(sprite, obj, true);
}

void TemplateListItem::setEntityProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteEntityProperties(sprite, obj, true);
}

void TemplateListItem::setMonsterProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteMonsterProperties(sprite, obj, true);
}

void TemplateListItem::setMonsterDropProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteMonsterDropProperties(sprite, obj, true);
}

void TemplateListItem::setAIProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteAIProperties(sprite, obj, true);
}

void TemplateListItem::setMoveProperties(ImageSprite* sprite, QJsonObject& obj)
{
	//set MoveProperties
	TemplateValueGenerator::setSpriteMoveProperties(sprite, obj, true);
}

void TemplateListItem::setAnimationProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteAnimationProperties(sprite, obj, true);
}

void TemplateListItem::setSimplerTriggerProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteSimplerTriggerProperties(sprite, obj, true);
}

void TemplateListItem::setFilterProperties(ImageSprite* sprite, QJsonObject& obj)
{
	TemplateValueGenerator::setSpriteFilterProperties(sprite, obj, true);
}