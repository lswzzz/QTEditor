#pragma once
#include "ControlBaseDockWidget.h"
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include "ImageSprite.h"
#include <qcombobox.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QSelectedCheckBox.h"

class ActionView : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	ActionView(char* title, QSize size);
	~ActionView();
	
	void setValueFromFile(QString file, bool clearTarget = true);
	QVariantMap getVariantMap();
	int getFrameCount();
	virtual void setDefaultValue();
	void changeSpriteValue();
	
private slots:
	void setPerFrame();
	void playTypeChanged(int index);
	void lookPerSpriteFrame(int index);
protected:
	void addSelectedCheckBox();
	QStringList transIndexToRealImageName(std::vector<int> list);
	virtual void setWidgetValue();
	void setPerSpriteFrameList(QStringList list);
	void __connect();
	void _setLayout();

	void resetTemplateValue(int index);
	void resetTemplatePerFrame(QJsonObject& obj);
	void resetTemplatePlayType(QJsonObject& obj);

private:
	QLineEdit* perFrameTime;
	QLabel*		frameFilename;
	QComboBox* playType;
	QComboBox* perSpriteFrame;

	QSelectedCheckBox*  playTypeSelectedBox;
	QSelectedCheckBox*  perFrameTimeSelectedBox;
	enum resetType{
		resetPerFrame,
		resetPlayType
	};
};

