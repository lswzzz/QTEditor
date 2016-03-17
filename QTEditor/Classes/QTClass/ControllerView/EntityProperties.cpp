#include "EntityProperties.h"
#include "ControlView.h"
#include <QLayout>
#include "FontChina.h"
#include "QLabel"
#include "QFile"
#include "MyLogger.h"

EntityProperties::EntityProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_EntityProperties;
	_setLayout();
	_connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}

EntityProperties::~EntityProperties()
{
}

void EntityProperties::_setLayout()
{
	QVBoxLayout* vlayout = new QVBoxLayout();
	QGridLayout* gridLayout = new QGridLayout();
	numGenerateSelectedBox = new QSelectedCheckBox(FontChina::G2U("最大生成数"));
	generateDelaySelectedBox = new QSelectedCheckBox(FontChina::G2U("出生间隔时间"));
	sleepTimeSelectedBox = new QSelectedCheckBox(FontChina::G2U("睡眠时间"));
	entity_TypeSelectedBox = new QSelectedCheckBox(FontChina::G2U("实体类型"));
	InternalOperation = false;
	numGenerate = new QLineEdit();
	generateDelay = new QLineEdit();
	sleepTime = new QLineEdit();
	entity_Type = new QComboBox();
	gridLayout->addWidget(numGenerateSelectedBox, 0, 0);
	gridLayout->addWidget(numGenerate, 0, 1);
	gridLayout->addWidget(generateDelaySelectedBox, 0, 2);
	gridLayout->addWidget(generateDelay, 0, 3);
	gridLayout->addWidget(sleepTimeSelectedBox, 1, 0);
	gridLayout->addWidget(sleepTime, 1, 1);
	gridLayout->addWidget(entity_TypeSelectedBox, 1, 2);
	gridLayout->addWidget(entity_Type, 1, 3);
	vlayout->addLayout(gridLayout);
	widget->setLayout(vlayout);
}

void EntityProperties::addSelectedCheckBox()
{
	checkVec.push_back(numGenerateSelectedBox);
	checkVec.push_back(generateDelaySelectedBox);
	checkVec.push_back(sleepTimeSelectedBox);
	checkVec.push_back(entity_TypeSelectedBox);

	selectNameVec << "selectNumGenerate" << "selectGenerateDelay" << "selectSleepTime" << "selectSpriteType";
}

void EntityProperties::_connect()
{
	connect(generateDelay, SIGNAL(editingFinished()), this, SLOT(generateDelayRedo()));
	connect(numGenerate, SIGNAL(editingFinished()), this, SLOT(numGenerateRedo()));
	connect(sleepTime, SIGNAL(editingFinished()), this, SLOT(sleepTimeRedo()));
	connect(entity_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(entityTypeChange(int)));
}

QVariantMap EntityProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["entityProperties"].toObject();
	if (numGenerateSelectedBox->getState()){
		map.insert("numGenerate", numGenerate->text().toInt());
		map.insert("selectNumGenerate", true);
	}
	else{
		map.insert("numGenerate", properties["numGenerate"].toInt());
		map.insert("selectNumGenerate", false);
	}

	if (generateDelaySelectedBox->getState()){
		map.insert("generateDelay", generateDelay->text().toDouble());
		map.insert("selectGenerateDelay", true);
	}
	else{
		map.insert("generateDelay", properties["generateDelay"].toDouble());
		map.insert("selectGenerateDelay", false);
	}

	if (sleepTimeSelectedBox->getState()){
		map.insert("sleepTime", sleepTime->text().toDouble());
		map.insert("selectSleepTime", true);
	}
	else{
		map.insert("sleepTime", properties["sleepTime"].toDouble());
		map.insert("selectSleepTime", false);
	}
	
	if (entity_TypeSelectedBox->getState()){
		if (entity_Type->count() == 0){
			map.insert("spriteType", 0);
		}
		else if (entity_Type->count() == 1){
			map.insert("spriteType", 1);
		}
		else if (entity_Type->count() == 2){
			if (entity_Type->currentIndex() == 0){
				map.insert("spriteType", 2);
			}
			else{
				map.insert("spriteType", 3);
			}
		}
		map.insert("selectSpriteType", true);
	}
	else{
		map.insert("spriteType", properties["spriteType"].toInt());
		map.insert("selectSpriteType", false);
	}
	
	return map;
}

void EntityProperties::setDefaultValue()
{
	numGenerate->setText("0");
	generateDelay->setText("0.0");
	sleepTime->setText("0.0");
	entity_Type->clear();
}

void EntityProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QFile sFile(file);
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("EntityProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["entityProperties"].toObject();

	setCheckState(selectNameVec, properties);

	bool hasEntityProperties = obj["hasEntityProperties"].toBool();
	if (targetSprite){
		targetSprite->setHasEntityProperties(hasEntityProperties);
	}
	if (!hasEntityProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		numGenerate->setText(QString::number(properties["numGenerate"].toInt()));
		generateDelay->setText(QString::number(properties["generateDelay"].toDouble()));
		sleepTime->setText(QString::number(properties["sleepTime"].toDouble()));
		entity_Type->clear();
		switch (properties["spriteType"].toInt()){
		case 0:
			break;
		case 1:
			entity_Type->addItem(FontChina::G2U("动态块"));
			break;
		case 2:
			entity_Type->addItem(FontChina::G2U("怪"));
			entity_Type->addItem(FontChina::G2U("角色"));
			entity_Type->setCurrentIndex(0);
			break;
		case 3:
			entity_Type->addItem(FontChina::G2U("怪"));
			entity_Type->addItem(FontChina::G2U("角色"));
			entity_Type->setCurrentIndex(1);
			break;
		}
	}
	InternalOperation = false;
}

void EntityProperties::changeSpriteValue()
{
	InternalOperation = true;
	if (targetSprite && this->isVisible() && targetSprite->isHasEntityProperties()){
		targetSprite->setNumGenerate(numGenerate->text().toInt());
		targetSprite->setGenerateDelay(generateDelay->text().toDouble());
		targetSprite->setSleepTime(sleepTime->text().toDouble());
	}
	InternalOperation = false;
}

void EntityProperties::setWidgetValue()
{
	InternalOperation = true;
	if (targetSprite->isHasEntityProperties()){
		this->setVisible(true);
		
		numGenerate->setText(QString::number(targetSprite->getNumGenerate()));
		generateDelay->setText(QString::number(targetSprite->getGenerateDelay()));
		sleepTime->setText(QString::number(targetSprite->getSleepTime()));
		switch (targetSprite->getType()){
		case SpriteType::DYNAMIC_BLOCK:
			entity_Type->clear();
			entity_Type->addItem(FontChina::G2U("动态块"));
			break;
		case SpriteType::MONSTER:
			entity_Type->clear();
			entity_Type->addItem(FontChina::G2U("怪"));
			entity_Type->addItem(FontChina::G2U("角色"));
			entity_Type->setCurrentIndex(0);
			break;
		case SpriteType::ROLE:
			entity_Type->clear();
			entity_Type->addItem(FontChina::G2U("怪"));
			entity_Type->addItem(FontChina::G2U("角色"));
			entity_Type->setCurrentIndex(1);
			break;
		}
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
	InternalOperation = false;
}

void EntityProperties::resetTemplateValue(int index)
{
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (index){
	case resetNumGenerate:
		resetTemplateNumGenerate(obj);
		break;
	case resetGenerateDelay:
		resetTemplateGenerateDelay(obj);
		break;
	case resetSleepTime:
		resetTemplateSleepTime(obj);
		break;
	case resetEntityType:
		resetTemplateEntityType(obj);
		break;
	}
	writeToFile(templateFile, obj);
}

void EntityProperties::resetTemplateNumGenerate(QJsonObject& obj)
{
	QJsonObject entityProperties = obj["entityProperties"].toObject();
	double num = numGenerate->text().toDouble();
	entityProperties["numGenerate"] = num;
	obj["entityProperties"] = entityProperties;
}

void EntityProperties::resetTemplateGenerateDelay(QJsonObject& obj)
{
	QJsonObject entityProperties = obj["entityProperties"].toObject();
	double delay = generateDelay->text().toDouble();
	entityProperties["generateDelay"] = delay;
	obj["entityProperties"] = entityProperties;
}

void EntityProperties::resetTemplateSleepTime(QJsonObject& obj)
{
	QJsonObject entityProperties = obj["entityProperties"].toObject();
	double sleep = sleepTime->text().toDouble();
	entityProperties["sleepTime"] = sleep;
	obj["entityProperties"] = entityProperties;
}
 
void EntityProperties::resetTemplateEntityType(QJsonObject& obj)
{
	QJsonObject entityProperties = obj["entityProperties"].toObject();
	int __type = entity_Type->currentIndex();
	if (entity_Type->count() == 1){
		entityProperties["spriteType"] = 1;
	}
	else if (entity_Type->count() == 2){
		if (__type == 0){
			entityProperties["spriteType"] = 2;
		}
		else{
			entityProperties["spriteType"] = 3;
		}
	}
	obj["entityProperties"] = entityProperties;
}

void EntityProperties::numGenerateRedo()
{
	if (!InternalOperation && targetSprite){
		targetSprite->setNumGenerate(numGenerate->text().toDouble());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetNumGenerate);
	}
}

void EntityProperties::generateDelayRedo()
{
	if (!InternalOperation && targetSprite){
		targetSprite->setGenerateDelay(generateDelay->text().toDouble());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetGenerateDelay);
	}
}

void EntityProperties::sleepTimeRedo()
{
	if (!InternalOperation && targetSprite){
		targetSprite->setSleepTime(sleepTime->text().toDouble());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetSleepTime);
	}
}

void EntityProperties::entityTypeChange(int index)
{
	if (!InternalOperation && targetSprite && (targetSprite->getType() == SpriteType::MONSTER || targetSprite->getType() == SpriteType::ROLE)){
		switch (index){
		case 0:
			targetSprite->setType(SpriteType::MONSTER);
			break;
		case 1:
			targetSprite->setType(SpriteType::ROLE);
			break;
		}
		auto controlView = dynamic_cast<ControlView*>(g_ControlView);
		controlView->setValueFromChildSprite(targetSprite);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetEntityType);
	}
}

void EntityProperties::addController()
{
	if (targetSprite && (targetSprite->getType() == SpriteType::NORMAL_BLOCK || targetSprite->getType() == SpriteType::DYNAMIC_BLOCK)){
		targetSprite->setType(SpriteType::DYNAMIC_BLOCK);
		targetSprite->setHasEntityProperties(true);
		entity_Type->clear();
		entity_Type->addItem(FontChina::G2U("动态块"));
	}
}

void EntityProperties::deleteController()
{
	if (targetSprite && (targetSprite->getType() == SpriteType::NORMAL_BLOCK || targetSprite->getType() == SpriteType::DYNAMIC_BLOCK)){
		targetSprite->setType(SpriteType::NORMAL_BLOCK);
		targetSprite->setHasEntityProperties(false);
		entity_Type->clear();
	}
}