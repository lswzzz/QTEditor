#pragma once
#include "BaseDockWidget.h"
#include "QSelectedCheckBox.h"
#include "ImageSprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "QGridLayout"
#include "QLineEdit"
#include "QPushButton"

class ControlBaseDockWidget : public BaseDockWidget
{
	Q_OBJECT
public:
	ControlBaseDockWidget(char* title, QSize size, bool isNeed = false);
	~ControlBaseDockWidget();

	void setSelectedBoxState(bool state);
	void setTargetSprite(ImageSprite* sprite);
	void clearTarget();
	virtual void setListSelectedState(bool state);

	virtual void setDefaultValue() = 0;
	virtual void setWidgetValue() = 0;

	virtual void addController();
	virtual void deleteController();

	struct ListData{
		QSelectedCheckBox* labelKey;
		QLineEdit* editKey;
		QSelectedCheckBox* labelValue;
		QLineEdit* editValue;
		QPushButton* sub;
		QSelectedCheckBox* labelExtra;
		QLineEdit* extra;
		ListData(){
			labelKey = NULL;
			editKey = NULL;
			labelValue = NULL;
			sub = NULL;
			editValue = NULL;
			labelExtra = NULL;
			extra = NULL;
		}
	};
	void createListItem(ListData* data, bool addtoSprite = false, QString key = "", QString value = "", QString extra = "");
	void deleteAllList();
	void deleteListItem(int i);

protected slots:
	void resetTargetSpriteState(bool visible);
protected:

	virtual void addListData(QString key, QString value, QString extra = "");
	virtual void resetSpriteListKey(int index, QString key);
	virtual void resetSpriteListValue(int index, QString value);
	virtual void resetSpriteListExtra(int index, QString extra);
	virtual void resetTemplateListKey(int index, QWidget* w);
	virtual void resetTemplateListValue(int index, QWidget* w);
	virtual void resetTemplateListExtra(int index, QWidget* w);
	virtual void subSpriteListItem(int index);
	virtual void subTemplateListItem(int index, QWidget* w);
	QJsonObject readFileGetQJsonObject(QString file);
	bool writeToFile(QString file, QJsonObject& obj);
	void setCheckState(QStringList list, QJsonObject& obj);
	void setCheckState(std::vector<bool> vec);

	void addCheckBoxConnect();
protected:
	QString getViewPropertiesName();
	void addListValueToLayout();
	void setCheckLabelState(QJsonObject& jo, ListData* data);
	void resetTemplateListLabelSelectState(int state, int labelIndex, ListData* date);
	ImageSprite* targetSprite;
	bool InternalOperation;
	QString templateFile;
	QVector<QSelectedCheckBox*> checkVec;
	QStringList selectNameVec;
	enum ControlView_Type{
		Type_ActionView,
		Type_AIProperties,
		Type_BaseProperties,
		Type_BoxProperties,
		Type_CircleProperties,
		Type_EntityProperties,
		Type_MonsterDropProperties,
		Type_MonsterProperties,
		Type_MoveProperties,
		Type_SimpleTriggerProperties,
		Type_FilterProperties,
	};
	ControlView_Type m_ControlView_Type;
	QGridLayout* gListlayout;
	QVector<ListData*> list;
	//label的显示
	std::string listKey;
	std::string listValue;
	std::string listExtra;
	//label在template中的值
	QString selectListKey;
	QString selectListValue;
	QString selectListExtra;
	QString listname;
	bool fileError;
	int rowListDataCount;
	bool check;
};

