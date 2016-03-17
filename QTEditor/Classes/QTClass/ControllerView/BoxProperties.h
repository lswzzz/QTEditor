#pragma once
#include "ControlBaseDockWidget.h"
#include <qlineedit.h>
#include <QComboBox>
#include "ImageSprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QSelectedCheckBox.h"

class BoxProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	BoxProperties(char* title, QSize size);
	~BoxProperties();

	QVariantMap getVariantMap();
	void setValueFromFile(QString file, bool clearTarget = true);
	virtual void setDefaultValue();
	//重新设置sprite的值为当前的值
	void changeSpriteValue();

private slots:
	void AnchorXEditFinished();
	void AnchorYEditFinished();
	void halfWidthEditFinished();
	void halfHeightEditFinished();
	void showOrHideRigid(int index);
	void setRigidWay(int index);
	//void leftExtrusionSelected(int state);
	//void rightExtrusionSelected(int state);
	//void upExtrusionSelected(int state);
	//void downExtrusionSelected(int state);
protected:
	void targetSpriteDrawRect();
	void _setLayout();
	void _connect();
	void wheelEvent(QWheelEvent *e);
	virtual void setWidgetValue();
	void addSelectedCheckBox();

	void resetTemplateValue(int index);
	void resetTemplateAnchrX(QJsonObject& obj);
	void resetTemplateAnchrY(QJsonObject& obj);
	void resetTemplateHalfW(QJsonObject& obj);
	void resetTemplateHalfH(QJsonObject& obj);
	void resetTemplateWay(QJsonObject& obj);
	void resetTemplateShow(QJsonObject& obj);
	//void resetTemplateleftExtrusion(QJsonObject& obj);
	//void resetTemplaterightExtrusion(QJsonObject& obj);
	//void resetTemplateupExtrusion(QJsonObject& obj);
	//void resetTemplatedownExtrusion(QJsonObject& obj);
private:
	QLineEdit* AnchorX;
	QLineEdit* AnchorY;
	QLineEdit* halfWidth;
	QLineEdit* halfHeight;
	//QCheckBox* leftExtrusion;
	//QCheckBox* rightExtrusion;
	//QCheckBox* upExtrusion;
	//QCheckBox* downExtrusion;

	QSelectedCheckBox* anchorXSelectedBox;
	QSelectedCheckBox* anchorYSelectedBox;
	QSelectedCheckBox* halfWidthSelectedBox;
	QSelectedCheckBox* halfHeightSelectedBox;
	QSelectedCheckBox* waySelectedBox;
	QSelectedCheckBox* showRigidSelectedBox;
	//QSelectedCheckBox* leftExtrusionSelectedBox;
	//QSelectedCheckBox* rightExtrusionSelectedBox;
	//QSelectedCheckBox* upExtrusionSelectedBox;
	//QSelectedCheckBox* downExtrusionSelectedBox;

	QComboBox* way;
	QComboBox* showRigid;
	enum resetType{
		resetAnchrX,
		resetAnchrY,
		resetHalfW,
		resetHalfH,
		resetWay,
		resetShow
	};


};

