#pragma once
#include "ControlBaseDockWidget.h"
#include <qlineedit.h>
#include "ImageSprite.h"
#include <qcombobox.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QSelectedCheckBox.h"

class MoveProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	MoveProperties(char* title, QSize size);
	~MoveProperties();
	QVariantMap getVariantMap();
	void setValueFromFile(QString file, bool clearTarget = true);
	virtual void setDefaultValue();
	//重新设置sprite的值为当前的值
	void changeSpriteValue();

protected:
	void _setLayout();
	void _connect();
	void wheelEvent(QWheelEvent *e);
	void targetSpriteDrawLine();
	void addSelectedCheckBox();
	virtual void setWidgetValue();

	void resetTemplateValue(int index);
	void resetTemplateTranslateX(QJsonObject& obj);
	void resetTemplateTranslateY(QJsonObject& obj);
	void resetTemplateRunTime(QJsonObject& obj);
	void resetTemplateDelayTime(QJsonObject& obj);
	void resetTemplateRunType(QJsonObject& obj);
	void resetTemplateBackDelayTime(QJsonObject& obj);
	void resetTemplateStartRunTime(QJsonObject& obj);
private slots:
	void translatXEditFinished();
	void translatYEditFinished();
	void runTimeEditFinished();
	void delayTimeEditFinished();
	void backDelayTimeEditFinished();
	void startRunTImeEditFinished();
	void SpriterunAction(int index);
private:
	QLineEdit* translatXEdit;
	QLineEdit* translatYEdit;
	QLineEdit* runTime;
	QLineEdit* delayTime;
	QLineEdit* backDelayTime;
	QComboBox* runType;
	QLineEdit* startRunTime;

	QSelectedCheckBox* translatXSelectedBox;
	QSelectedCheckBox* translatYSelectedBox;
	QSelectedCheckBox* runTimeSelectedBox;
	QSelectedCheckBox* delayTimeSelectedBox;
	QSelectedCheckBox* runTypeSelectedBox;
	QSelectedCheckBox* backDelayTimeSelectedBox;
	QSelectedCheckBox* startRunTimeSelectedBox;

	enum resetType{
		resetTranslateX,
		resetTranslateY,
		resetRunTime,
		resetDelayTime,
		resetRunType,
		resetBackDelayTime,
		resetStartRunTime,
	};
};

