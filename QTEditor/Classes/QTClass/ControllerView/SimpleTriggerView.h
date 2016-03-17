#pragma once
#include "ControlBaseDockWidget.h"
#include "EntityImageSprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QSelectedCheckBox.h"

class SimpleTriggerView : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	SimpleTriggerView(char* title, QSize size);
	~SimpleTriggerView();
	bool checkSprite();
	QVariantMap getVariantMap();
	virtual void setDefaultValue();
	void setValueFromFile(QString file, bool clearTarget = true);
	void changeSpriteValue();

private slots:
	void addTriggerItem();

protected:
	virtual void addListData(QString key, QString value, QString extra = "");
	virtual void resetSpriteListKey(int index, QString key);
	virtual void resetSpriteListValue(int index, QString value);
	virtual void resetTemplateListKey(int index, QWidget* w);
	virtual void resetTemplateListValue(int index, QWidget* w);
	virtual void subSpriteListItem(int index);
	virtual void subTemplateListItem(int index, QWidget* w);

	void resetTemplateValue(int item, QWidget* edit = NULL, int index = 0);
	void resetTemplateListType(QJsonObject& obj, QWidget* edit, int index);
	void resetTemplateListId(QJsonObject& obj, QWidget* edit, int index);
	void subTemplateListItem(QJsonObject& obj, QWidget* edit, int index);
	virtual void setWidgetValue();

	void addSelectedCheckBox();
	
protected:
	void __connect();
	void setLayout();
	void contextMenuEvent(QContextMenuEvent *event);
private:
	enum resetType{
		resetListItemType,
		resetListItemId,
		subListItem,
	};
};

