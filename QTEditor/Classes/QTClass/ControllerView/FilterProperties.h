#pragma once
#include "ControlBaseDockWidget.h"
#include "cocos2d.h"
#include "QSelectedCheckBox.h"
#include <QComboBox>

USING_NS_CC;

class FilterProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	FilterProperties(char* title, QSize size);
	~FilterProperties();

	QVariantMap getVariantMap();
	virtual void setDefaultValue();
	void changeSpriteValue();
	void setValueFromFile(QString file, bool clearTarget = true);
protected:
	void __setLayout();
	void __connect();
	virtual void setWidgetValue();
	void addSelectedCheckBox();

	void resetTemplateValue(int index);
	void resetTemplateFilterType(QJsonObject& obj);

private slots:
	void filterTypeChange(int type_);

private:
	QSelectedCheckBox* filterTypeSelectedBox;
	QComboBox* filterType;
	
	enum resetType{
		resetFilterType,
	};
};

