#include "MonsterDropProperties.h"
#include "FontChina.h"
#include "Global.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "MyLogger.h"

MonsterDropProperties::MonsterDropProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_MonsterDropProperties;
	_setLayout();
	_connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
	listKey = "Id";
	listValue = "Odds";
	listExtra = "diff";
	selectListKey = "selectId";
	selectListValue = "selectOdds";
	selectListExtra = "selectDiff";
	listname = "list";
	rowListDataCount = 3;
}

void MonsterDropProperties::_setLayout()
{
	addDrop = new QPushButton;
	addDrop->setIcon(QIcon(":/icon/Resources/add2.ico"));
	addDrop->setMaximumSize(30, 30);
	dropGold = new QLineEdit;
	gListlayout = new QGridLayout;
	QGridLayout* glayout = new QGridLayout;
	gListlayout = new QGridLayout;
	dropGoldSelectedBox = new QSelectedCheckBox(FontChina::G2U("掉落金币"));
	glayout->addWidget(dropGoldSelectedBox, 0, 0);
	glayout->addWidget(dropGold, 0, 1);
	glayout->addWidget(new QLabel(FontChina::G2U("添加掉落")), 0, 2);
	glayout->addWidget(new QLabel, 0, 3);
	glayout->addWidget(addDrop, 0, 4);
	glayout->setColumnStretch(0, 2);
	glayout->setColumnStretch(1, 5);
	glayout->setColumnStretch(2, 2);
	glayout->setColumnStretch(3, 2);
	glayout->setColumnStretch(4, 1);
	glayout->addLayout(gListlayout, 1, 0, 1, 5);
	widget->setLayout(glayout);
}

MonsterDropProperties::~MonsterDropProperties()
{ 
}

void MonsterDropProperties::addSelectedCheckBox()
{
	checkVec.push_back(dropGoldSelectedBox);
	selectNameVec << "selectGold";
}

void MonsterDropProperties::setWidgetValue()
{
	InternalOperation = true;
	if (targetSprite->isHasMonsterDropProperties()){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		InternalOperation = true;
		this->setVisible(true);
		dropGold->setText(QString::number(sprite->getDropGold()));
		deleteAllList();
		auto vec = sprite->getDropList();
		for (int i = 0; i < vec.size(); i++){
			ListData* data = new ListData;
			createListItem(data, false, vec.at(i)->id, QString::number(vec.at(i)->odds), QString::number(vec.at(i)->difficulty));
		}
		InternalOperation = false;
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
	InternalOperation = false;
}

void MonsterDropProperties::_connect()
{
	connect(dropGold, SIGNAL(editingFinished()), this, SLOT(GoldRedo()));
	connect(addDrop, SIGNAL(clicked()), this, SLOT(ListRedo()));
}

void MonsterDropProperties::resetTemplateValue(int item, QWidget* edit, int index)
{
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (item){
	case resetGold:
		resetTemplateGold(obj);
		break;
	case resetListItemId:
		resetTemplateListId(obj, edit, index);
		break;
	case resetListItemOdd:
		resetTemplateListOdd(obj, edit, index);
		break;
	case subListItem:
		subTemplateListItem(obj, edit, index);
		break;
	case resetListItemDifficuty:
		resetTemplateListExtra(obj, edit, index);
		break;
	}
	writeToFile(templateFile, obj);
}

void MonsterDropProperties::resetTemplateGold(QJsonObject& obj)
{
	QJsonObject monsterDropProperties = obj["monsterDropProperties"].toObject();
	int gold = dropGold->text().toInt();
	monsterDropProperties["gold"] = gold;
	obj["monsterDropProperties"] = monsterDropProperties;
}

void MonsterDropProperties::resetTemplateListId(QJsonObject& obj, QWidget* edit, int index)
{
	auto editor = dynamic_cast<QLineEdit*>(edit);
	QJsonObject monsterDropProperties = obj["monsterDropProperties"].toObject();
	QJsonArray listdata = monsterDropProperties["list"].toArray();
	if (index >= listdata.size()){
		QJsonObject data;
		data["id"] = editor->text();
		data["odd"] = 0.0f;
		data["difficulty"] = 0;
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			listother.push_back(listdata.at(i).toObject());
		}
		listother.push_back(data);
		monsterDropProperties["list"] = listother;
	}
	else{
		auto data = listdata.at(index).toObject();
		data["id"] = editor->text();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			if (i != index){
				listother.push_back(listdata.at(i).toObject());
			}
			else{
				listother.push_back(data);
			}
		}
		monsterDropProperties["list"] = listother;
	}
	obj["monsterDropProperties"] = monsterDropProperties;
}

void MonsterDropProperties::resetTemplateListOdd(QJsonObject& obj, QWidget* edit, int index)
{
	auto editor = dynamic_cast<QLineEdit*>(edit);
	QJsonObject monsterDropProperties = obj["monsterDropProperties"].toObject();
	QJsonArray listdata = monsterDropProperties["list"].toArray();
	int size = listdata.size();
	if (index >= listdata.size()){
		QJsonObject data;
		data["id"] = "";
		data["odd"] = editor->text().toFloat();
		data["difficulty"] = 0;
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			listother.push_back(listdata.at(i).toObject());
		}
		listother.push_back(data);
		monsterDropProperties["list"] = listother;
	}
	else{
		auto data = listdata.at(index).toObject();
		data["odd"] = editor->text().toFloat();
		QString id_ = data["id"].toString();
		float odd_ = data["odd"].toDouble();
		float difficulty_ = data["difficulty"].toInt();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			if (i != index){
				listother.push_back(listdata.at(i).toObject());
			}
			else{
				listother.push_back(data);
			}
		}
		monsterDropProperties["list"] = listother;
	}
	obj["monsterDropProperties"] = monsterDropProperties;
}

void MonsterDropProperties::resetTemplateListExtra(QJsonObject& obj, QWidget* edit, int index)
{
	auto editor = dynamic_cast<QLineEdit*>(edit);
	QJsonObject monsterDropProperties = obj["monsterDropProperties"].toObject();
	QJsonArray listdata = monsterDropProperties["list"].toArray();
	if (index >= listdata.size()){
		QJsonObject data;
		data["id"] = "";
		data["odd"] = 0.0f;
		data["difficulty"] = editor->text().toInt();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			listother.push_back(listdata.at(i).toObject());
		}
		listother.push_back(data);
		monsterDropProperties["list"] = listother;
	}
	else{
		auto data = listdata.at(index).toObject();
		data["difficulty"] = editor->text().toInt();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			if (i != index){
				listother.push_back(listdata.at(i).toObject());
			}
			else{
				listother.push_back(data);
			}
		}
		monsterDropProperties["list"] = listother;
	}
	obj["monsterDropProperties"] = monsterDropProperties;
}

void MonsterDropProperties::subTemplateListItem(QJsonObject& obj, QWidget* edit, int index)
{
	auto sub = dynamic_cast<QPushButton*>(edit);
	QJsonObject monsterDropProperties = obj["monsterDropProperties"].toObject();
	QJsonArray listdata = monsterDropProperties["list"].toArray();
	QJsonArray listother;
	for (int i = 0; i < listdata.size(); i++){
		if (i != index){
			listother.push_back(listdata.at(i).toObject());
		}
	}
	monsterDropProperties["list"] = listother;
	obj["monsterDropProperties"] = monsterDropProperties;
}

void MonsterDropProperties::GoldRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setDropGold(dropGold->text().toInt());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetGold);
	}
}

void MonsterDropProperties::ListRedo()
{
	ListData* data = new ListData;
	createListItem(data, true);
}

void MonsterDropProperties::setDefaultValue()
{
	deleteAllList();
	list.clear();
	dropGold->setText("");
}

void MonsterDropProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("MonsterDropProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["monsterDropProperties"].toObject();
	
	setCheckState(selectNameVec, properties);
	bool hasMonsterDropProperties = obj["hasMonsterDropProperties"].toBool();
	if (targetSprite){
		targetSprite->setHasMonsterDropProperties(hasMonsterDropProperties);
	}
	if (!hasMonsterDropProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		dropGold->setText(QString::number(properties["gold"].toInt()));
		deleteAllList();
		auto __list = properties["list"].toArray();
		for (int i = 0; i < __list.size(); i++){
			ListData* data = new ListData;
			QJsonObject jo = __list.at(i).toObject();
			createListItem(data, false, jo["id"].toString(), QString::number(jo["odd"].toDouble()), QString::number(jo["difficulty"].toInt()));
			setCheckLabelState(jo, data);
		}
	}
	InternalOperation = false;
}

void MonsterDropProperties::changeSpriteValue()
{
	if (targetSprite && targetSprite->isHasMonsterDropProperties()){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setDropGold(dropGold->text().toInt());
		if (list.size() > 0){
			sprite->deleteDropList();
			for (int i = 0; i < list.size(); i++){
				auto data = list.at(i);
				sprite->addDrop(data->editKey->text(), data->editValue->text().toFloat(), data->extra->text().toInt());
			}
		}
	}
}

bool MonsterDropProperties::checkSprite()
{
	if (targetSprite && targetSprite->isHasMonsterProperties()){
		return true;
	}
	return false;
}

QVariantMap MonsterDropProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["monsterDropProperties"].toObject();
	if (dropGoldSelectedBox->getState()){
		map.insert("gold", dropGold->text().toInt());
		map.insert("selectGold", true);
	}
	else{
		map.insert("gold", properties["gold"].toInt());
		map.insert("selectGold", false);
	}
	QVariantList Vlist;
	for (int i = 0; i < list.size(); i++){
		if (list.at(i)->labelKey->checkState() == Qt::CheckState::Checked && list.at(i)->labelValue->checkState() == Qt::CheckState::Checked &&
			list.at(i)->labelExtra->checkState() == Qt::CheckState::Checked){
			QVariantMap data;
			data.insert("id", list.at(i)->editKey->text());
			data.insert("odd", list.at(i)->editValue->text().toFloat());
			data.insert("difficulty", list.at(i)->extra->text().toInt());
			Vlist.push_back(data);
		}
	}
	map.insert("list", Vlist);
	return map;
}

void MonsterDropProperties::addListData(QString key, QString value, QString difficulty)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->addDrop(key, value.toDouble(), difficulty.toInt());
}

void MonsterDropProperties::resetSpriteListKey(int index, QString key)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->resetDropId(index, key);
}

void MonsterDropProperties::resetSpriteListValue(int index, QString value)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->resetDropOdd(index, value.toDouble());
}

void MonsterDropProperties::resetSpriteListExtra(int index, QString extra)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->resetDropDifficulty(index, extra.toInt());
}

void MonsterDropProperties::resetTemplateListKey(int index, QWidget* w)
{
	resetTemplateValue(resetListItemId, w, index);
}

void MonsterDropProperties::resetTemplateListValue(int index, QWidget* w)
{
	resetTemplateValue(resetListItemOdd, w, index);
}

void MonsterDropProperties::resetTemplateListExtra(int index, QWidget* w)
{
	resetTemplateValue(resetListItemDifficuty, w, index);
}

void MonsterDropProperties::subSpriteListItem(int index)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->deleteDrop(index);
}

void MonsterDropProperties::subTemplateListItem(int index, QWidget* w)
{
	resetTemplateValue(subListItem, w, index);
}