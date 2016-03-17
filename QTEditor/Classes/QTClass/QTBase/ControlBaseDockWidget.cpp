#include "ControlBaseDockWidget.h"
#include "QFile"
#include <qtextstream.h>
#include "FontChina.h"
#include "Global.h"
#include "MyLogger.h"
#include "convertMapToJsonFile.h"

ControlBaseDockWidget::ControlBaseDockWidget(char* title, QSize size, bool isNeed)
	:BaseDockWidget(title, size, isNeed)
{
	targetSprite = NULL;
	InternalOperation = false;
	connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(resetTargetSpriteState(bool)));
	rowListDataCount = 2;
	check = true;
}

ControlBaseDockWidget::~ControlBaseDockWidget()
{
}

void ControlBaseDockWidget::addCheckBoxConnect()
{
	for (int i = 0; i < checkVec.size(); i++){
		auto data = checkVec.at(i);
		connect(data, &QCheckBox::stateChanged, [=](int state){
			if (targetSprite == NULL && templateFile != ""){
				QJsonObject obj = readFileGetQJsonObject(templateFile);
				QJsonObject properties;
				properties = obj[getViewPropertiesName()].toObject();
				switch (state){
				case Qt::CheckState::Checked:
					properties[selectNameVec.at(i)] = true;
					break;
				case Qt::CheckState::Unchecked:
					properties[selectNameVec.at(i)] = false;
					break;
				}
				obj[getViewPropertiesName()] = properties;
				if (check){
					writeToFile(templateFile, obj);
				}
			}
		});
	}
}

void ControlBaseDockWidget::setSelectedBoxState(bool state)
{
	for (int i = 0; i < checkVec.size(); i++){
		if (state){
			checkVec.at(i)->setCheckState(Qt::CheckState::Checked);
		}
		else{
			checkVec.at(i)->setCheckState(Qt::CheckState::Unchecked);
		}
	}
	setListSelectedState(state);
}

void ControlBaseDockWidget::setListSelectedState(bool state)
{
	for (int i = 0; i < list.size(); i++){
		auto data = list.at(i);
		if (state){
			data->labelKey->setCheckState(Qt::CheckState::Checked);
			data->labelValue->setCheckState(Qt::CheckState::Checked);
			if (data->labelExtra){
				data->labelExtra->setCheckState(Qt::CheckState::Checked);
			}
		}
		else{
			data->labelKey->setCheckState(Qt::CheckState::Unchecked);
			data->labelValue->setCheckState(Qt::CheckState::Unchecked);
			if (data->labelExtra){
				data->labelExtra->setCheckState(Qt::CheckState::Unchecked);
			}
		}
	}
}

void ControlBaseDockWidget::setTargetSprite(ImageSprite* sprite)
{
	targetSprite = sprite;
	QString str = targetSprite->getTemplateFile().c_str();
	if (str != NULL && str != ""){
		templateFile = str;
	}
	setWidgetValue();
}

void ControlBaseDockWidget::clearTarget()
{
	InternalOperation = true;
	targetSprite = NULL;
	templateFile = "";
	setDefaultValue();
	InternalOperation = false;
}

QJsonObject ControlBaseDockWidget::readFileGetQJsonObject(QString file)
{
	QJsonObject obj;
	QFile sFile(file);
	if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		fileError = true;
		return obj;
	}
	QTextStream in(&sFile);
	in.setCodec("UTF-8");
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		fileError = false;
		obj = document.object();
	}
	else{
		fileError = true;
	}
	sFile.close();
	return obj;
}

bool ControlBaseDockWidget::writeToFile(QString file, QJsonObject& obj)
{
	QFile dFile(file);
	if (!dFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		fileError = true;
		return false;
	}
	QTextStream out(&dFile);
	QJsonDocument doc = QJsonDocument::fromVariant(obj.toVariantMap());
	out << doc.toJson();
	fileError = false;
	dFile.close();
	convertMapToJsonFile::changeFileToUtf8(file);
	return true;
}

void ControlBaseDockWidget::setCheckState(QStringList list, QJsonObject& obj)
{
	check = false;
	std::vector<bool> vec;
	for (int i = 0; i < list.size(); i++){
		if (obj[list.at(i)].toBool()){
			vec.push_back(true);
		}
		else{
			vec.push_back(false);
		}
	}
	setCheckState(vec);
	check = true;
}

void ControlBaseDockWidget::setCheckState(std::vector<bool> vec)
{
	for (int i = 0; i < checkVec.size(); i++){
		checkVec.at(i)->setState(vec.at(i));
	}
}

QString _______hasVisible_____(bool vis){
	if (vis){
		return "true";
	}
	else{
		return "false";
	}
}

void ControlBaseDockWidget::resetTargetSpriteState(bool visible)
{
	switch (m_ControlView_Type){
	case Type_ActionView:
		break;
	case Type_AIProperties:
		if (targetSprite && !visible){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite AIProperties is false");
			targetSprite->setHasAIProperties(false);
		}
		break;
	case Type_BaseProperties:
		break;
	case Type_BoxProperties:
		if (targetSprite && !visible){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite BoxPropertie is false");
			targetSprite->setHasBoxProperties(false);
		}
		break;
	case Type_CircleProperties:
		if (targetSprite && !visible){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite BoxPropertie is false");
			targetSprite->setHasCircleProperties(false);
		}
		break;
	case Type_EntityProperties:
		if (targetSprite && (targetSprite->getType() == SpriteType::NORMAL_BLOCK) && !visible){
			targetSprite->setType(SpriteType::NORMAL_BLOCK);
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite EntityPropertie is false");
			targetSprite->setHasEntityProperties(false);
		}
		break;
	case Type_MonsterDropProperties:
		if (targetSprite && !visible){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite MonsterDropPropertie is false");
			targetSprite->setHasMonsterDropProperties(false);
		}
		break;
	case Type_MonsterProperties:
		break;
	case Type_MoveProperties:
		if (targetSprite && !visible){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite MovePropertie is false");
			targetSprite->setHasMoveProperties(false);
		}
		break;
	case Type_SimpleTriggerProperties:
		if (targetSprite && !visible){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite SimpleTriggerPropertie is false");
			targetSprite->setHasSimplerTrigger(false);
		}
		break;
	case Type_FilterProperties:
		if (targetSprite && !visible){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::resetTargetSpriteState be called, the sprite FilterProperties is false");
			targetSprite->setHasFilterProperties(false);
		}
		break;
	}
}

void ControlBaseDockWidget::addController()
{
	switch (m_ControlView_Type){
	case Type_ActionView:
		break;
	case Type_AIProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::addController be called, the sprite AIProperties is true");
			targetSprite->setHasAIProperties(true);
		}
		break;
	case Type_BaseProperties:
		break;
	case Type_BoxProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::addController be called, the sprite BoxProperties is true");
			targetSprite->setHasBoxProperties(true);
		}
		break;
	case Type_CircleProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::addController be called, the sprite CircleProperties is true");
			targetSprite->setHasCircleProperties(true);
		}
		break;
	case Type_EntityProperties:
		break;
	case Type_MonsterDropProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::addController be called, the sprite MonsterDropProperties is true");
			targetSprite->setHasMonsterDropProperties(true);
		}
		break;
	case Type_MonsterProperties:
		break;
	case Type_MoveProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::addController be called, the sprite MoveProperties is true");
			targetSprite->setHasMoveProperties(true);
		}
		break;
	case Type_SimpleTriggerProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::addController be called, the sprite SimpleTriggerProperties is true");
			targetSprite->setHasSimplerTrigger(true);
		}
		break;
	case Type_FilterProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::addController be called, the sprite FilterProperties is true");
			targetSprite->setHasFilterProperties(true);
		}
		break;
	}
}

void ControlBaseDockWidget::deleteController()
{
	switch (m_ControlView_Type){
	case Type_ActionView:
		break;
	case Type_AIProperties:
		break;
	case Type_BaseProperties:
		break;
	case Type_BoxProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::deleteController be called, the sprite BoxProperties is false");
			targetSprite->setHasBoxProperties(false);
			targetSprite->showRigid(1);
		}
		break;
	case Type_CircleProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::deleteController be called, the sprite CirclePropertie is false");
			targetSprite->setHasCircleProperties(false);
			targetSprite->showRigid(1);
		}
		break;
	case Type_EntityProperties:
		break;
	case Type_MonsterDropProperties:
		break;
	case Type_MonsterProperties:
		break;
	case Type_MoveProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::deleteController be called, the sprite MoveProperties is false");
			targetSprite->setHasMoveProperties(false);
		}
		break;
	case Type_SimpleTriggerProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::deleteController be called, the sprite SimpleTriggerProperties is false");
			targetSprite->setHasSimplerTrigger(false);
		}
		break;
	case Type_FilterProperties:
		if (targetSprite){
			MyLogger::getInstance()->addInfo("ControlBaseDockWidget::deleteController be called, the sprite FilterProperties is false");
			targetSprite->setHasFilterProperties(false);
		}
		break;
	}
}


void ControlBaseDockWidget::createListItem(ListData* data, bool addtoSprite, QString key, QString value, QString extra)
{
	size = this->minimumSize();
	this->setMinimumSize(QSize(size.width(), size.height() + 30));
	this->setMaximumSize(QSize(size.width(), size.height() + 30));
	size = QSize(size.width(), size.height() + 30);
	data->editKey = new QLineEdit;
	data->editValue = new QLineEdit;
	data->labelKey = new QSelectedCheckBox(FontChina::G2U(listKey.c_str()));
	data->labelValue = new QSelectedCheckBox(FontChina::G2U(listValue.c_str()));
	data->sub = new QPushButton;
	data->sub->setIcon(QIcon(":/icon/Resources/sub.ico"));
	data->sub->setStyleSheet("border:none");
	data->editKey->setText(key);
	data->editValue->setText(value);
	if (rowListDataCount > 2){
		data->labelExtra = new QSelectedCheckBox(FontChina::G2U(listExtra.c_str()));
		data->extra = new QLineEdit;
		data->extra->setText(extra);
	}
	if (addtoSprite && targetSprite)
		addListData(key, value);
	list.push_back(data);
	addListValueToLayout();
	connect(data->editKey, &QLineEdit::editingFinished, [this, data]
	{
		if (!InternalOperation && targetSprite){
			for (int i = 0; i < list.size(); i++){
				auto edit = list.at(i)->editKey;
				if (data->editKey == edit){
					resetSpriteListKey(i, data->editKey->text());
				}
			}
		}
		else if (!InternalOperation && !targetSprite && templateFile != ""){
			for (int i = 0; i < list.size(); i++){
				auto edit = list.at(i)->editKey;
				if (data->editKey == edit){
					resetTemplateListKey(i, data->editKey);
				}
			}

		}
	});
	connect(data->editValue, &QLineEdit::editingFinished, [this, data]
	{
		if (!InternalOperation && targetSprite){
			for (int i = 0; i < list.size(); i++){
				auto edit = list.at(i)->editValue;
				if (data->editValue == edit){
					resetSpriteListValue(i, data->editValue->text());
				}
			}
		}
		else if (!InternalOperation && !targetSprite && templateFile != ""){
			for (int i = 0; i < list.size(); i++){
				auto edit = list.at(i)->editValue;
				if (data->editValue == edit){
					resetTemplateListValue(i, data->editValue);
				}
			}
		}
	});
	connect(data->sub, &QPushButton::clicked, [this, data]{
		if (!InternalOperation && targetSprite){
			for (int i = 0; i < list.size(); i++){
				auto sub = list.at(i)->sub;
				if (data->sub == sub){
					subSpriteListItem(i);
					deleteListItem(i);
					return;
				}
			}
		}
		else if (!InternalOperation && !targetSprite && templateFile != ""){
			for (int i = 0; i < list.size(); i++){
				auto sub = list.at(i)->sub;
				if (data->sub == sub){
					subTemplateListItem(i, data->sub);
					deleteListItem(i);
					return;
				}
			}
		}
	});
	connect(data->labelKey, &QCheckBox::stateChanged, [this, data](int state){
		if (targetSprite == NULL && templateFile != ""){
			resetTemplateListLabelSelectState(state, 0, data);
		}
	});
	connect(data->labelValue, &QCheckBox::stateChanged, [=](int state){
		if (targetSprite == NULL && templateFile != ""){
			resetTemplateListLabelSelectState(state, 1, data);
		}
	});
	if (rowListDataCount > 2){
		connect(data->extra, &QLineEdit::editingFinished, [this, data]
		{
			if (!InternalOperation && targetSprite){
				for (int i = 0; i < list.size(); i++){
					auto edit = list.at(i)->extra;
					if (data->extra == edit){
						resetSpriteListExtra(i, data->extra->text());
					}
				}
			}
			else if (!InternalOperation && !targetSprite && templateFile != ""){
				for (int i = 0; i < list.size(); i++){
					auto edit = list.at(i)->extra;
					if (data->extra == edit){
						resetTemplateListExtra(i, data->extra);
					}
				}
			}
		});
		connect(data->labelExtra, &QCheckBox::stateChanged, [=](int state){
			if (targetSprite == NULL && templateFile != ""){
				resetTemplateListLabelSelectState(state, 2, data);
			}
		});
	}
}

void ControlBaseDockWidget::addListValueToLayout()
{
	for (int i = 0; i < list.size(); i++){
		auto data = list.at(i);
		gListlayout->addWidget(data->labelKey, i, 0);
		gListlayout->addWidget(data->editKey, i, 1);
		gListlayout->addWidget(data->labelValue, i, 2);
		gListlayout->addWidget(data->editValue, i, 3);
		if (rowListDataCount > 2){
			gListlayout->addWidget(data->labelExtra, i, 4);
			gListlayout->addWidget(data->extra, i, 5);
			gListlayout->addWidget(data->sub, i, 6);
		}
		else{
			gListlayout->addWidget(data->sub, i, 4);
		}
	}
	if (rowListDataCount > 2){
		gListlayout->setColumnStretch(0, 1);
		gListlayout->setColumnStretch(1, 4);
		gListlayout->setColumnStretch(2, 1);
		gListlayout->setColumnStretch(3, 2);
		gListlayout->setColumnStretch(4, 1);
		gListlayout->setColumnStretch(5, 2);
		gListlayout->setColumnStretch(6, 1);
	}
	else{
		gListlayout->setColumnStretch(0, 2);
		gListlayout->setColumnStretch(1, 6);
		gListlayout->setColumnStretch(2, 2);
		gListlayout->setColumnStretch(3, 6);
		gListlayout->setColumnStretch(4, 1);
	}
}

void ControlBaseDockWidget::setCheckLabelState(QJsonObject& jo, ListData* data)
{
	bool keyState;
	bool valueState;
	if (jo.contains(selectListKey)){
		keyState = jo[selectListKey].toBool();
	}
	else{
		keyState = true;
	}
	if (jo.contains(selectListValue)){
		valueState = jo[selectListValue].toBool();
	}
	else{
		valueState = true;
	}
	if (keyState)
		data->labelKey->setCheckState(Qt::CheckState::Checked);
	else
		data->labelKey->setCheckState(Qt::CheckState::Unchecked);
	if (valueState)
		data->labelValue->setCheckState(Qt::CheckState::Checked);
	else
		data->labelValue->setCheckState(Qt::CheckState::Unchecked);
	if (rowListDataCount > 2){
		bool extraState;
		if (jo.contains(selectListExtra)){
			extraState = jo[selectListExtra].toBool();
		}
		else{
			extraState = true;
		}
		if (extraState)
			data->labelExtra->setCheckState(Qt::CheckState::Checked);
		else
			data->labelExtra->setCheckState(Qt::CheckState::Unchecked);
	}
}

void ControlBaseDockWidget::resetTemplateListLabelSelectState(int state, int labelIndex, ListData* data)
{
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties;
	QJsonArray jList;
	QString str = getViewPropertiesName();
	properties = obj[str].toObject();
	jList = properties[listname].toArray();
	for (int i = 0; i < list.size(); i++){
		auto label = list.at(i)->labelKey;
		if (data->labelKey == label){
			QJsonObject value = jList.at(i).toObject();
			switch (state){
			case Qt::CheckState::Checked:
				switch (labelIndex){
				case 0:
					if (value.contains(selectListKey)){
						value[selectListKey] = true;
					}
					else{
						value.insert(selectListKey, true);
					}
					break;
				case 1:
					if (value.contains(selectListValue)){
						value[selectListValue] = true;
					}
					else{
						value.insert(selectListValue, true);
					}
					break;
				case 2:
					if (value.contains(selectListExtra)){
						value[selectListExtra] = true;
					}
					else{
						value.insert(selectListExtra, true);
					}
					break;
				}
				break;
			case Qt::CheckState::Unchecked:
				switch (labelIndex){
				case 0:
					if (value.contains(selectListKey)){
						value[selectListKey] = false;
					}
					else{
						value.insert(selectListKey, false);
					}
					break;
				case 1:
					if (value.contains(selectListValue)){
						value[selectListValue] = false;
					}
					else{
						value.insert(selectListValue, false);
					}
					break;
				case 2:
					if (value.contains(selectListExtra)){
						value[selectListExtra] = false;
					}
					else{
						value.insert(selectListExtra, false);
					}
					break;
				}
				break;
			}
			jList[i] = value;
			properties[listname] = jList;
			obj[getViewPropertiesName()] = properties;
			writeToFile(templateFile, obj);
		}
	}
}

QString ControlBaseDockWidget::getViewPropertiesName()
{
	QString str;
	switch (m_ControlView_Type){
	case Type_ActionView:
		str = "actionProperties";
		break;
	case Type_AIProperties:
		str = "aiProperties";
		break;
	case Type_BaseProperties:
		str = "baseProperties";
		break;
	case Type_BoxProperties:
		str = "boxProperties";
		break;
	case Type_CircleProperties:
		str = "circleProperties";
		break;
	case Type_EntityProperties:
		str = "entityProperties";
		break;
	case Type_MonsterDropProperties:
		str = "monsterDropProperties";
		break;
	case Type_MonsterProperties:
		str = "monsterProperties";
		break;
	case Type_MoveProperties:
		str = "moveProperties";
		break;
	case Type_SimpleTriggerProperties:
		str = "simpleTriggerProperties";
		break;
	case Type_FilterProperties:
		str = "filterProperties";
		break;
	}
	return str;
}

void ControlBaseDockWidget::deleteListItem(int i)
{
	auto data = list.at(i);
	gListlayout->removeWidget(data->labelKey);
	data->labelKey->deleteLater();
	gListlayout->removeWidget(data->editKey);
	data->editKey->deleteLater();
	gListlayout->removeWidget(data->labelValue);
	data->labelValue->deleteLater();
	gListlayout->removeWidget(data->editValue);
	data->editValue->deleteLater();
	gListlayout->removeWidget(data->sub);
	data->sub->deleteLater();
	if (rowListDataCount > 2){
		gListlayout->removeWidget(data->labelExtra);
		data->labelExtra->deleteLater();
		gListlayout->removeWidget(data->extra);
		data->extra->deleteLater();
	}
	delete data->labelKey;
	delete data->labelValue;
	delete data->editKey;
	delete data->editValue;
	delete data->sub;
	delete data->labelExtra;
	delete data->extra;
	QVector<ListData*>::iterator iter = list.begin();
	list.erase(iter + i);
	delete data;
	size = this->minimumSize();
	this->setMinimumSize(QSize(size.width(), size.height() - 30));
	this->setMaximumSize(QSize(size.width(), size.height() - 30));
	size = QSize(size.width(), size.height() - 30);
}

void ControlBaseDockWidget::deleteAllList()
{
	for (int i = list.size() - 1; i >= 0; i--){
		deleteListItem(i);
	}
}

void ControlBaseDockWidget::addListData(QString key, QString value, QString extra)
{

}

void ControlBaseDockWidget::resetSpriteListKey(int index, QString key)
{

}

void ControlBaseDockWidget::resetSpriteListValue(int index, QString value)
{

}

void ControlBaseDockWidget::resetSpriteListExtra(int index, QString extra)
{

}

void ControlBaseDockWidget::resetTemplateListKey(int index, QWidget* w)
{

}

void ControlBaseDockWidget::resetTemplateListValue(int index, QWidget* w)
{

}

void ControlBaseDockWidget::resetTemplateListExtra(int index, QWidget* w)
{

}

void ControlBaseDockWidget::subSpriteListItem(int index)
{

}

void ControlBaseDockWidget::subTemplateListItem(int index, QWidget* w)
{

}