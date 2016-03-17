#pragma once
#include "ControlBaseDockWidget.h"
#include "EntityImageSprite.h"
#include "QPushButton"
#include "QLineEdit"
#include "QLayout"
#include "QLabel"
#include "QSelectedCheckBox.h"

class MonsterDropProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	MonsterDropProperties(char* title, QSize size);
	~MonsterDropProperties();
	bool checkSprite();
	virtual void setDefaultValue();
	void setValueFromFile(QString file, bool clearTarget = true);
	void changeSpriteValue();
	QVariantMap getVariantMap();
	
protected:
	virtual void addListData(QString key, QString value, QString difficulty = "");
	virtual void resetSpriteListKey(int index, QString key);
	virtual void resetSpriteListValue(int index, QString value);
	virtual void resetSpriteListExtra(int index, QString extra);
	virtual void resetTemplateListKey(int index, QWidget* w);
	virtual void resetTemplateListValue(int index, QWidget* w);
	virtual void resetTemplateListExtra(int index, QWidget* w);
	virtual void subSpriteListItem(int index);
	virtual void subTemplateListItem(int index, QWidget* w);
protected:
	virtual void setWidgetValue();
	void _connect();
	void _setLayout();
	void addSelectedCheckBox();

	void resetTemplateValue(int item, QWidget* edit = NULL, int index = 0);
	void resetTemplateGold(QJsonObject& obj);
	void resetTemplateListId(QJsonObject& obj, QWidget* edit, int index);
	void resetTemplateListOdd(QJsonObject& obj, QWidget* edit, int index);
	void resetTemplateListExtra(QJsonObject& obj, QWidget* edit, int index);
	void subTemplateListItem(QJsonObject& obj, QWidget* edit, int index);
private slots:
	void GoldRedo();
	void ListRedo();
protected:

	enum resetType{
		resetGold,
		resetListItemId,
		resetListItemOdd,
		resetListItemDifficuty,
		subListItem,
	};
	QPushButton* addDrop;
	QLineEdit* dropGold;

	QSelectedCheckBox* dropGoldSelectedBox;

};

