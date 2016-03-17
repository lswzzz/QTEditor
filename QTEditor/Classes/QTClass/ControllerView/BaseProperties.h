#pragma once
#include "ControlBaseDockWidget.h"
#include <QCheckBox>
#include <qlineedit.h>
#include "ImageSprite.h"
#include "cocos2d.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QSelectedCheckBox.h"
#include "QLabel"

using namespace cocos2d;

class BaseProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	BaseProperties(char* title, QSize size);
	~BaseProperties();

	QVariantMap getVariantMap();
	void setValueFromFile(QString file, bool clearTarget = true);
	virtual void setDefaultValue();
	void changeSpriteValue();
	
	void switchFlipXState();
	void switchFlipYState();
protected:
	void _setLayout();
	void __connect();
	virtual void setWidgetValue();
	void addSelectedCheckBox();

	void resetTemplateValue(int index);
	void resetTemplateScaleX(QJsonObject &obj);
	void resetTemplateScaleY(QJsonObject &obj);
	void resetTemplateRotation(QJsonObject &obj);
	void resetTemplateFlipX(QJsonObject &obj);
	void resetTemplateFlipY(QJsonObject &obj);
	void resetTemplateAnchrX(QJsonObject &obj);
	void resetTemplateAnchrY(QJsonObject &obj);
	void resetTemplateDontMerge(QJsonObject& obj);
private slots:
	void ScaleXTextRedo();
	void ScaleYTextRedo();
	void RotationTextRedo();
	void flipXSelected(int state);
	void flipYSelected(int state);
	void anchrXRedo();
	void anchrYRedo();
	void spriteDontMergeState(int state);
private:
	QLineEdit* scaleXEdit;
	QLineEdit* scaleYEdit;
	QLineEdit* rotateEdit;
	QLineEdit* anchrXEdit;
	QLineEdit* anchrYEdit;
	QCheckBox* flipXButton;
	QCheckBox* flipYButton;
	QLineEdit* idLabel;
	QLineEdit* wLabel;
	QLineEdit* hLabel;
	QCheckBox* spriteMergeBox;

	QSelectedCheckBox* scaleXSelectedBox;
	QSelectedCheckBox* scaleYSelectedBox;
	QSelectedCheckBox* rotateSelectedBox;
	QSelectedCheckBox* anchrXSelectedBox;
	QSelectedCheckBox* anchrYSelectedBox;
	QSelectedCheckBox* flipXSelectedBox;
	QSelectedCheckBox* flipYSelectedBox;
	QSelectedCheckBox* spriteMergeSelectBox;
	
	enum resetType{
		resetScaleX,
		resetScaleY,
		resetRotation,
		resetFlipX,
		resetFlipY,
		resetDontMerge,
		resetAnchrX,
		resetAnchrY
	};

	friend class InvincibleFriendClass;
};

