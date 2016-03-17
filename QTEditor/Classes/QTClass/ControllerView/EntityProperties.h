#pragma once
#include "ControlBaseDockWidget.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "ImageSprite.h"
#include "qlineedit.h"
#include "QComboBox.h"
#include "QSelectedCheckBox.h"

class EntityProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	EntityProperties(char* title, QSize size);
	~EntityProperties();
	QVariantMap getVariantMap();
	virtual void setDefaultValue();
	void setValueFromFile(QString file, bool clearTarget = true);
	//重新设置sprite的值为当前的值
	void changeSpriteValue();
	virtual void addController();
	virtual void deleteController();

protected:
	virtual void setWidgetValue();
	void _connect();
	void _setLayout();
	void addSelectedCheckBox();

	void resetTemplateValue(int index);
	void resetTemplateNumGenerate(QJsonObject& obj);
	void resetTemplateGenerateDelay(QJsonObject& obj);
	void resetTemplateSleepTime(QJsonObject& obj);
	void resetTemplateEntityType(QJsonObject& obj);

private slots:
	void numGenerateRedo();
	void generateDelayRedo();
	void sleepTimeRedo();
	void entityTypeChange(int index); 
private:
	QLineEdit* numGenerate;
	QLineEdit* generateDelay;
	QLineEdit* sleepTime;
	QComboBox* entity_Type;
	
	QSelectedCheckBox* numGenerateSelectedBox;
	QSelectedCheckBox* generateDelaySelectedBox;
	QSelectedCheckBox* sleepTimeSelectedBox;
	QSelectedCheckBox* entity_TypeSelectedBox;

	enum resetType{
		resetNumGenerate,
		resetGenerateDelay,
		resetSleepTime,
		resetEntityType
	};
};

