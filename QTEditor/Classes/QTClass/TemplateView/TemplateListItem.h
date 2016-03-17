#pragma once
#include <qlistwidget.h>
#include "cocos2d.h"
#include "ImageSprite.h"

using namespace cocos2d;

class TemplateListItem : public QListWidgetItem
{
public:
	TemplateListItem();
	~TemplateListItem();
	QString getType();
	void setType(QString type);
	QString getFile();
	void setFile(QString file);
	void addNode(Node* node);
	void clearVec();
	void removeNode(Node* node);
	void setMD5(QString md5);
	QString getMD5(){ return md5; };
	void updateAllSpriteValue();
	void updateAllSpriteTemplateNameAndMd5();
	void setProperties(ImageSprite* sprite, QJsonObject& obj);
	Vector<Node*> getVec(){ return _vec; };

protected:
	void setBaseProperties(ImageSprite* sprite, QJsonObject& obj);
	void setBoxProperties(ImageSprite* sprite, QJsonObject& obj);
	void setCircleProperties(ImageSprite* sprite, QJsonObject& obj);
	void setEntityProperties(ImageSprite* sprite, QJsonObject& obj);
	void setMonsterProperties(ImageSprite* sprite, QJsonObject& obj);
	void setMonsterDropProperties(ImageSprite* sprite, QJsonObject& obj);
	void setAIProperties(ImageSprite* sprite, QJsonObject& obj);
	void setMoveProperties(ImageSprite* sprite, QJsonObject& obj);
	void setAnimationProperties(ImageSprite* sprite, QJsonObject& obj);
	void setSimplerTriggerProperties(ImageSprite* sprite, QJsonObject& obj);
	void setFilterProperties(ImageSprite* sprite, QJsonObject& obj);
private:
	QString m_type;
	QString md5;
	QString m_file;
	Vector<Node*> _vec;
};

