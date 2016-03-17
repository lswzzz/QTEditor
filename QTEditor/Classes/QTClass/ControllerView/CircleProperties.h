#pragma once
#include "ControlBaseDockWidget.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "ImageSprite.h"
#include "QLineEdit"
#include "QComboBox"
#include "QSelectedCheckBox.h"

class CircleProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	CircleProperties(char* title, QSize size);
	~CircleProperties();

	QVariantMap getVariantMap();
	virtual void setDefaultValue();
	void setValueFromFile(QString file, bool clearTarget = true);
	void changeSpriteValue();

protected:
	virtual void setWidgetValue();
	void _connect();
	void _setLayout();
	void targetSpriteDrawCircle();
	void addSelectedCheckBox();

	void resetTemplateValue(int index);
	void resetTemplateAnchrX(QJsonObject& obj);
	void resetTemplateAnchrY(QJsonObject& obj);
	void resetTemplateRadius(QJsonObject& obj);
	void resetTemplateShow(QJsonObject& obj);
private slots:
	void AnchrXRedo();
	void AnchrYRedo();
	void RadiusRedo();
	void showRigidChange(int index);

private:
	QLineEdit* anchrX;
	QLineEdit* anchrY;
	QLineEdit* radius;
	QComboBox* showRigid;

	QSelectedCheckBox* anchorXSelectedBox;
	QSelectedCheckBox* anchorYSelectedBox;
	QSelectedCheckBox* radiusSelectedBox;
	QSelectedCheckBox* showRigidSelectedBox;

	enum resetType{
		resetAnchrX,
		resetAnchrY,
		resetRadius,
		resetShowType,
	};
};

