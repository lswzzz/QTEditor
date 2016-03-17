#include "SimpleTriggerView.h"
#include "QFile"
#include "QLabel"
#include "QMenu"
#include "FontChina.h"
#include "MyLogger.h"

SimpleTriggerView::SimpleTriggerView(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_SimpleTriggerProperties;
	setLayout();
	__connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
	listKey = "Type";
	listValue = "Id";
	selectListKey = "selectType";
	selectListValue = "selectId";
	listname = "list";
}

void SimpleTriggerView::addSelectedCheckBox()
{
	
}

SimpleTriggerView::~SimpleTriggerView()
{
}

void SimpleTriggerView::__connect()
{
}

void SimpleTriggerView::setLayout()
{
	gListlayout = new QGridLayout;
	QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addLayout(gListlayout);
	widget->setLayout(vlayout);
}

void SimpleTriggerView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* menu = new QMenu(this);
	QAction* action = new QAction(FontChina::G2U("添加一个触发器"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(addTriggerItem()));
	menu->addAction(action);
	menu->exec(QCursor::pos());
}

bool SimpleTriggerView::checkSprite()
{
	if (targetSprite && targetSprite->isHasEntityProperties()){
		return true;
	}
	return false;
}

QVariantMap SimpleTriggerView::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["simpleTriggerProperties"].toObject();
	QVariantList Vlist;
	for (int i = 0; i < list.size(); i++){
		if (list.at(i)->labelKey->checkState() == Qt::CheckState::Checked && list.at(i)->labelValue->checkState() == Qt::CheckState::Checked){
			QVariantMap data;
			data.insert("type", list.at(i)->editKey->text().toInt());
			data.insert("id", list.at(i)->editValue->text().toInt());
			Vlist.push_back(data);
		}
	}
	map.insert("list", Vlist);

	return map;
}

void SimpleTriggerView::setDefaultValue()
{
	deleteAllList();
}

void SimpleTriggerView::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("SimpleTriggerView::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["simpleTriggerProperties"].toObject();

	setCheckState(selectNameVec, properties);

	bool hasSimpleTriggerProperties = obj["hasSimpleTriggerProperties"].toBool();
	if (targetSprite){
		targetSprite->setHasSimplerTrigger(hasSimpleTriggerProperties);
	}
	if (!hasSimpleTriggerProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		deleteAllList();
		auto __list = properties["list"].toArray();
		for (int i = 0; i < __list.size(); i++){
			ListData* data = new ListData;
			QJsonObject jo = __list.at(i).toObject();
			createListItem(data, false, QString::number(jo["type"].toInt()), QString::number(jo["id"].toInt()));
			setCheckLabelState(jo, data);
		}
	}
	InternalOperation = false;
}

void SimpleTriggerView::changeSpriteValue()
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	if (targetSprite && this->isVisible() && targetSprite->isHasSimplerTrigger()){
		if (list.size() > 0){
			sprite->deleteSimpleTriList();
			for (int i = 0; i < list.size(); i++){
				auto data = list.at(i);
				sprite->addSimpleTri(data->editKey->text().toInt(), data->editValue->text().toInt());
			}
		}
	}
}

void SimpleTriggerView::addTriggerItem()
{
	ListData* data = new ListData;
	createListItem(data, true);
}

void SimpleTriggerView::resetTemplateValue(int item, QWidget* edit, int index)
{
	QFile sFile(templateFile);
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (item){
	case resetListItemType:
		resetTemplateListType(obj, edit, index);
		break;
	case resetListItemId:
		resetTemplateListId(obj, edit, index);
		break;
	case subListItem:
		subTemplateListItem(obj, edit, index);
		break;
	}
	writeToFile(templateFile, obj);
}

void SimpleTriggerView::resetTemplateListType(QJsonObject& obj, QWidget* edit, int index)
{
	auto editor = dynamic_cast<QLineEdit*>(edit);
	QJsonObject simpleTriggerProperties = obj["simpleTriggerProperties"].toObject();
	QJsonArray listdata = simpleTriggerProperties["list"].toArray();
	if (index >= listdata.size()){
		QJsonObject data;
		data["type"] = editor->text().toInt();
		data["id"] = 0;
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			listother.push_back(listdata.at(i).toObject());
		}
		listother.push_back(data);
		simpleTriggerProperties["list"] = listother;
	}
	else{
		auto data = listdata.at(index).toObject();
		data["type"] = editor->text().toInt();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			if (i != index){
				listother.push_back(listdata.at(i).toObject());
			}
			else{
				listother.push_back(data);
			}
		}
		simpleTriggerProperties["list"] = listother;
	}
	obj["simpleTriggerProperties"] = simpleTriggerProperties;
}

void SimpleTriggerView::resetTemplateListId(QJsonObject& obj, QWidget* edit, int index)
{
	auto editor = dynamic_cast<QLineEdit*>(edit);
	QJsonObject simpleTriggerProperties = obj["simpleTriggerProperties"].toObject();
	QJsonArray listdata = simpleTriggerProperties["list"].toArray();
	if (index >= listdata.size()){
		QJsonObject data;
		data["type"] = 0;
		data["id"] = editor->text().toInt();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			listother.push_back(listdata.at(i).toObject());
		}
		listother.push_back(data);
		simpleTriggerProperties["list"] = listother;
	}
	else{
		auto data = listdata.at(index).toObject();
		data["id"] = editor->text().toInt();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			if (i != index){
				listother.push_back(listdata.at(i).toObject());
			}
			else{
				listother.push_back(data);
			}
		}
		simpleTriggerProperties["list"] = listother;
	}
	obj["simpleTriggerProperties"] = simpleTriggerProperties;
}

void SimpleTriggerView::subTemplateListItem(QJsonObject& obj, QWidget* edit, int index)
{
	auto sub = dynamic_cast<QPushButton*>(edit);
	QJsonObject simpleTriggerProperties = obj["simpleTriggerProperties"].toObject();
	QJsonArray listdata = simpleTriggerProperties["list"].toArray();
	QJsonArray listother;
	for (int i = 0; i < listdata.size(); i++){
		if (i != index){
			listother.push_back(listdata.at(i).toObject());
		}
	}
	simpleTriggerProperties["list"] = listother;
	obj["simpleTriggerProperties"] = simpleTriggerProperties;
}

void SimpleTriggerView::setWidgetValue()
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	InternalOperation = true;
	if (targetSprite->isHasSimplerTrigger()){
		this->setVisible(true);
		deleteAllList();
		auto vec = sprite->getSimpleTriggerList();
		for (int i = 0; i < vec.size(); i++){
			ListData* data = new ListData;
			createListItem(data, false, QString::number(vec.at(i)->type), QString::number(vec.at(i)->id));
		}
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
	InternalOperation = false;
}

void SimpleTriggerView::addListData(QString key, QString value, QString extra)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->addSimpleTri(key.toInt(), value.toInt());
}

void SimpleTriggerView::resetSpriteListKey(int index, QString key)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->resetSimpleTriType(index, key.toInt());
}

void SimpleTriggerView::resetSpriteListValue(int index, QString value)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->resetSimpleTriId(index, value.toInt());
}

void SimpleTriggerView::resetTemplateListKey(int index, QWidget* w)
{
	resetTemplateValue(resetListItemType, w, index);
}

void SimpleTriggerView::resetTemplateListValue(int index, QWidget* w)
{
	resetTemplateValue(resetListItemId, w, index);
}

void SimpleTriggerView::subSpriteListItem(int index)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->deleteSimpleTri(index);
}

void SimpleTriggerView::subTemplateListItem(int index, QWidget* w)
{
	resetTemplateValue(subListItem, w, index);
}
