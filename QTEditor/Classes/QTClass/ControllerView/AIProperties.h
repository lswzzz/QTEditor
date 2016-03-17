#pragma once
#include "ControlBaseDockWidget.h"
#include "EntityImageSprite.h"
#include "QLineEdit"
#include "QPushButton"
#include "QLabel"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QSelectedCheckBox.h"

class AIProperties : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	AIProperties(char* title, QSize size);
	~AIProperties();
	bool checkSprite();
	QVariantMap getVariantMap();
	virtual void setDefaultValue();
	void setValueFromFile(QString file, bool clearTarget = true);
	void changeSpriteValue();
	
private slots:
	void minXRedo();
	void maxXRedo();
	void minYRedo();
	void maxYRedo();
	void aiFileRedo();
	void on_addAI_clicked();
	void on_addUserDefine_clicked();
protected:
	virtual void addListData(QString key, QString value, QString extra = "");
	virtual void resetSpriteListKey(int index, QString key);
	virtual void resetSpriteListValue(int index, QString value);
	virtual void resetTemplateListKey(int index, QWidget* w);
	virtual void resetTemplateListValue(int index, QWidget* w);
	virtual void subSpriteListItem(int index);
	virtual void subTemplateListItem(int index, QWidget* w);

	void resetTemplateValue(int item, QWidget* edit = NULL, int index = 0);
	void resetTemplateMinX(QJsonObject& obj);
	void resetTemplateMaxX(QJsonObject& obj);
	void resetTemplateMinY(QJsonObject& obj);
	void resetTemplateMaxY(QJsonObject& obj);
	void resetTemplateAIFile(QJsonObject& obj);
	void resetTemplateListKey(QJsonObject& obj, QWidget* edit, int index);
	void resetTemplateListValue(QJsonObject& obj, QWidget* edit, int index);
	void subTemplateListItem(QJsonObject& obj, QWidget* edit, int index);
	virtual void setWidgetValue();

	void addSelectedCheckBox();

protected:
	void __connect();
	void setLayout();


private:
	QLineEdit* minX;
	QLineEdit* maxX;
	QLineEdit* minY;
	QLineEdit* maxY;
	QPushButton* addAi;
	QLineEdit* aiFile;
	QPushButton* addUserDefine;

	QSelectedCheckBox* minXSelectedBox;
	QSelectedCheckBox* maxXSelectedBox;
	QSelectedCheckBox* minYSelectedBox;
	QSelectedCheckBox* maxYSelectedBox;
	QSelectedCheckBox* aiFileSelectedBox;

	enum resetType{
		resetRangeMinX,
		resetRangeMaxX,
		resetRangeMinY,
		resetRangeMaxY,
		resetAIFile,
		resetListItemKey,
		resetListItemValue,
		subListItem,
	};
	
};


