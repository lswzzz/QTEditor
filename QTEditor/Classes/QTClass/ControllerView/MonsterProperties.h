#pragma once
#include "ControlBaseDockWidget.h"
#include "QLineEdit"
#include "EntityImageSprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QSelectedCheckBox.h"

class MonsterProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	MonsterProperties(char* title, QSize size);
	~MonsterProperties();
	QVariantMap getVariantMap();
	virtual void setDefaultValue();
	void setValueFromFile(QString file, bool clearTarget = true);
	void changeSpriteValue();
	bool checkSprite();
	

private slots:
	void hpRedo();
	void mpRedo();
	void attackRedo();
	void defenseRedo();
	void magic_attackRedo();
	void magic_defenseRedo();
	void speedRedo();
	void hardFactorRedo();

protected:
	void resetTemplateValue(int index);
	void resetTemplateHp(QJsonObject& obj);
	void resetTemplateMp(QJsonObject& obj);
	void resetTemplateAttack(QJsonObject& obj);
	void resetTemplateDefense(QJsonObject& obj);
	void resetTemplateMagic_Attack(QJsonObject& obj);
	void resetTemplateMagic_Defense(QJsonObject& obj);
	void resetTemplateSpeed(QJsonObject& obj);
	void resetTemplateHardFactor(QJsonObject& obj);
	
	virtual void setWidgetValue();
	void _connect();
	void _setLayout();
	void addSelectedCheckBox();


	QLineEdit* hp;
	QLineEdit* mp;
	QLineEdit* attack;
	QLineEdit* defense;
	QLineEdit* magic_attack;
	QLineEdit* magic_defense;
	QLineEdit* speed;
	QLineEdit* hardFactor;

	enum resetType{
		resetHp,
		resetMp,
		resetAttack,
		resetDefense,
		resetMagic_Attack,
		resetMagic_Defense,
		resetSpeed,
		resetHard_Factor,
	};

	QSelectedCheckBox* hpSelectedBox;
	QSelectedCheckBox* mpSelectedBox;
	QSelectedCheckBox* attackSelectedBox;
	QSelectedCheckBox* defenseSelectedBox;
	QSelectedCheckBox* magic_attackSelectedBox;
	QSelectedCheckBox* magic_defenseSelectedBox;
	QSelectedCheckBox* speedSelectedBox;
	QSelectedCheckBox* hardFactorSelectedBox;
	
};

