#pragma once
#include "ControlBaseDockWidget.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "ImageSprite.h"
#include "QPushButton"
#include "TriggerWidget.h"

class TriggerView : public ControlBaseDockWidget
{
	Q_OBJECT
public:
	TriggerView(char* title, QSize size);
	~TriggerView();
	QVariantMap getVariantMap();
	void addController();
	void deleteController();
	bool checkSprite();
	void setValueFromFile(QString file, bool clearTarget = true);
	void addItem(QMap<int, QMap<int, std::vector<float>>> map);
private slots:
	void on_addButton_clicked();
	void on_deleteButton_clicked();
protected:
	virtual void setWidgetValue();
	void _connect();
	void _setLayout();
	virtual void setDefaultValue();

protected:
	QPushButton* addTrigger;
	QPushButton* deleteTrigger;
	TriggerWidget* triggerList;
};

