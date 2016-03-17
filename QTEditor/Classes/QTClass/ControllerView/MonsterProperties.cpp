#include "MonsterProperties.h"
#include "QLayout"
#include "QLabel"
#include "FontChina.h"
#include "QFile"
#include "MyLogger.h"

MonsterProperties::MonsterProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_MonsterProperties;
	_setLayout();
	_connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}

void MonsterProperties::_setLayout()
{
	hp = new QLineEdit();
	mp = new QLineEdit();
	attack = new QLineEdit();
	defense = new QLineEdit();
	magic_attack = new QLineEdit();
	magic_defense = new QLineEdit();
	speed = new QLineEdit();
	hardFactor = new QLineEdit();
	QGridLayout* gridLayout = new QGridLayout;
	hpSelectedBox = new QSelectedCheckBox(FontChina::G2U("生命值"));
	mpSelectedBox = new QSelectedCheckBox(FontChina::G2U("魔法值"));
	attackSelectedBox = new QSelectedCheckBox(FontChina::G2U("物攻"));
	defenseSelectedBox = new QSelectedCheckBox(FontChina::G2U("物防"));
	magic_attackSelectedBox = new QSelectedCheckBox(FontChina::G2U("魔攻"));
	magic_defenseSelectedBox = new QSelectedCheckBox(FontChina::G2U("魔防"));
	speedSelectedBox = new QSelectedCheckBox(FontChina::G2U("速度"));
	hardFactorSelectedBox = new QSelectedCheckBox(FontChina::G2U("难度系数"));
	gridLayout->addWidget(hpSelectedBox, 0, 0);
	gridLayout->addWidget(hp, 0, 1);
	gridLayout->addWidget(mpSelectedBox, 0, 2);
	gridLayout->addWidget(mp, 0, 3);
	gridLayout->addWidget(attackSelectedBox, 1, 0);
	gridLayout->addWidget(attack, 1, 1);
	gridLayout->addWidget(defenseSelectedBox, 1, 2);
	gridLayout->addWidget(defense, 1, 3);
	gridLayout->addWidget(magic_attackSelectedBox, 2, 0);
	gridLayout->addWidget(magic_attack, 2, 1);
	gridLayout->addWidget(magic_defenseSelectedBox, 2, 2);
	gridLayout->addWidget(magic_defense, 2, 3);
	gridLayout->addWidget(speedSelectedBox, 3, 0);
	gridLayout->addWidget(speed, 3, 1);
	gridLayout->addWidget(hardFactorSelectedBox, 3, 2);
	gridLayout->addWidget(hardFactor, 3, 3);
	widget->setLayout(gridLayout);
}

MonsterProperties::~MonsterProperties()
{
}

void MonsterProperties::addSelectedCheckBox()
{
	checkVec.push_back(hpSelectedBox);
	checkVec.push_back(mpSelectedBox);
	checkVec.push_back(attackSelectedBox);
	checkVec.push_back(defenseSelectedBox);
	checkVec.push_back(magic_attackSelectedBox);
	checkVec.push_back(magic_defenseSelectedBox);
	checkVec.push_back(speedSelectedBox);
	checkVec.push_back(hardFactorSelectedBox);

	selectNameVec << "selectHp" << "selectMp" << "selectAttack" << "selectDefense" 
		<< "selectMagic_Attack" << "selectMagic_Defense" << "selectSpeed" << "selectHardFactor";
}

void MonsterProperties::_connect()
{
	connect(hp, SIGNAL(editingFinished()), this, SLOT(hpRedo()));
	connect(mp, SIGNAL(editingFinished()), this, SLOT(mpRedo()));
	connect(attack, SIGNAL(editingFinished()), this, SLOT(attackRedo()));
	connect(defense, SIGNAL(editingFinished()), this, SLOT(defenseRedo()));
	connect(magic_attack, SIGNAL(editingFinished()), this, SLOT(magic_attackRedo()));
	connect(magic_defense, SIGNAL(editingFinished()), this, SLOT(magic_defenseRedo()));
	connect(speed, SIGNAL(editingFinished()), this, SLOT(speedRedo()));
	connect(hardFactor, SIGNAL(editingFinished()), this, SLOT(hardFactorRedo()));
}

QVariantMap MonsterProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["monsterProperties"].toObject();
	if (hpSelectedBox->getState()){
		map.insert("hp", hp->text().toInt());
		map.insert("selectHp", true);
	}
	else{
		map.insert("hp", properties["hp"].toInt());
		map.insert("selectHp", false);
	}

	if (mpSelectedBox->getState()){
		map.insert("mp", mp->text().toInt());
		map.insert("selectMp", true);
	}
	else{
		map.insert("mp", properties["mp"].toInt());
		map.insert("selectMp", false);
	}

	if (attackSelectedBox->getState()){
		map.insert("attack", attack->text().toInt());
		map.insert("selectAttack", true);
	}
	else{
		map.insert("attack", properties["attack"].toInt());
		map.insert("selectAttack", false);
	}
	
	if (defenseSelectedBox->getState()){
		map.insert("defense", defense->text().toInt());
		map.insert("selectDefense", true);
	}
	else{
		map.insert("defense", properties["defense"].toInt());
		map.insert("selectDefense", false);
	}

	if (magic_attackSelectedBox->getState()){
		map.insert("magic_attack", magic_attack->text().toInt());
		map.insert("selectMagic_Attack", true);
	}
	else{
		map.insert("magic_attack", properties["magic_attack"].toInt());
		map.insert("selectMagic_Attack", false);
	}

	if (magic_defenseSelectedBox->getState()){
		map.insert("magic_defense", magic_defense->text().toInt());
		map.insert("selectMagic_Defense", true);
	}
	else{
		map.insert("magic_defense", properties["magic_defense"].toInt());
		map.insert("selectMagic_Defense", false);
	}

	if (speedSelectedBox->getState()){
		map.insert("speed", speed->text().toFloat());
		map.insert("selectSpeed", true);
	}
	else{
		map.insert("speed", properties["speed"].toDouble());
		map.insert("selectSpeed", false);
	}
	
	if (hardFactorSelectedBox->getState()){
		map.insert("hardFactor", hardFactor->text().toFloat());
		map.insert("selectHardFactor", true);
	}
	else{
		map.insert("hardFactor", properties["hardFactor"].toDouble());
		map.insert("selectHardFactor", false);
	}
	
	return map;
}

void MonsterProperties::setDefaultValue()
{
	hp->setText("1");
	mp->setText("0");
	attack->setText("0");
	defense->setText("0");
	magic_attack->setText("0");
	magic_defense->setText("0");
	speed->setText("0");
	hardFactor->setText("0");
}

void MonsterProperties::changeSpriteValue()
{
	if (targetSprite && this->isVisible() && targetSprite->isHasMonsterProperties()){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setHp(hp->text().toInt());
		sprite->setMp(mp->text().toInt());
		sprite->setAttack(attack->text().toInt());
		sprite->setDefense(attack->text().toInt());
		sprite->setMagic_Attack(magic_attack->text().toInt());
		sprite->setMagic_Defense(magic_defense->text().toInt());
		sprite->setSpeed(speed->text().toFloat());
		sprite->setHardFactor(hardFactor->text().toFloat());
	}
}

bool MonsterProperties::checkSprite()
{
	if (targetSprite && targetSprite->getType() == SpriteType::MONSTER){
		return true;
	}
	return false;
}

void MonsterProperties::setWidgetValue()
{
	InternalOperation = true;
	if (targetSprite->isHasMonsterProperties()){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		this->setVisible(true);
		hp->setText(QString::number(sprite->getHp()));
		mp->setText(QString::number(sprite->getMp()));
		attack->setText(QString::number(sprite->getAttack()));
		defense->setText(QString::number(sprite->getDefense()));
		magic_attack->setText(QString::number(sprite->getMagicAttack()));
		magic_defense->setText(QString::number(sprite->getMagicDefense()));
		speed->setText(QString::number(sprite->getSpeed()));
		hardFactor->setText(QString::number(sprite->getHardFactor()));
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
	InternalOperation = false;
}

void MonsterProperties::resetTemplateValue(int index)
{
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (index){
	case resetHp:
		resetTemplateHp(obj);
		break;
	case resetMp:
		resetTemplateMp(obj);
		break;
	case resetAttack:
		resetTemplateAttack(obj);
		break;
	case resetDefense:
		resetTemplateDefense(obj);
		break;
	case resetMagic_Attack:
		resetTemplateMagic_Attack(obj);
		break;
	case resetMagic_Defense:
		resetTemplateMagic_Defense(obj);
		break;
	case resetSpeed:
		resetTemplateSpeed(obj);
		break;
	case resetHard_Factor:
		resetTemplateHardFactor(obj);
		break;
	}
	writeToFile(templateFile, obj);
}

void MonsterProperties::resetTemplateHp(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int hp_ = hp->text().toInt();
	monsterProperties["hp"] = hp_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::resetTemplateMp(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int mp_ = mp->text().toInt();
	monsterProperties["mp"] = mp_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::resetTemplateAttack(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int attack_ = attack->text().toInt();
	monsterProperties["attack"] = attack_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::resetTemplateDefense(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int defense_ = defense->text().toInt();
	monsterProperties["defense"] = defense_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::resetTemplateMagic_Attack(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int magic_attack_ = magic_attack->text().toInt();
	monsterProperties["magic_attack"] = magic_attack_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::resetTemplateMagic_Defense(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int magic_defense_ = magic_defense->text().toInt();
	monsterProperties["magic_defense"] = magic_defense_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::resetTemplateSpeed(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int speed_ = speed->text().toInt();
	monsterProperties["speed"] = speed_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::resetTemplateHardFactor(QJsonObject& obj)
{
	QJsonObject monsterProperties = obj["monsterProperties"].toObject();
	int hardFactor_ = hardFactor->text().toInt();
	monsterProperties["hardFactor"] = hardFactor_;
	obj["monsterProperties"] = monsterProperties;
}

void MonsterProperties::hpRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setHp(hp->text().toInt());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetHp);
	}
}

void MonsterProperties::mpRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setMp(mp->text().toInt());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetMp);
	}
}

void MonsterProperties::attackRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setAttack(attack->text().toInt());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetAttack);
	}
}

void MonsterProperties::defenseRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setDefense(defense->text().toInt());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetDefense);
	}
}

void MonsterProperties::magic_attackRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setMagic_Attack(magic_attack->text().toInt());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetMagic_Attack);
	}
}

void MonsterProperties::magic_defenseRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setMagic_Defense(magic_defense->text().toInt());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetMagic_Defense);
	}
}

void MonsterProperties::speedRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setSpeed(speed->text().toFloat());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetSpeed);
	}
}

void MonsterProperties::hardFactorRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setHardFactor(hardFactor->text().toFloat());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetHard_Factor);
	}
}

void MonsterProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QFile sFile(file);
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("MonsterProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["monsterProperties"].toObject();

	setCheckState(selectNameVec, properties);

	bool hasMonsterProperties = obj["hasMonsterProperties"].toBool();
	if (targetSprite){
		targetSprite->setHasMoveProperties(hasMonsterProperties);
	}
	if (!hasMonsterProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		hp->setText(QString::number(properties["hp"].toInt()));
		mp->setText(QString::number(properties["mp"].toInt()));
		attack->setText(QString::number(properties["attack"].toInt()));
		defense->setText(QString::number(properties["defense"].toInt()));
		magic_attack->setText(QString::number(properties["magic_attack"].toInt()));
		magic_defense->setText(QString::number(properties["magic_defense"].toInt()));
		speed->setText(QString::number(properties["speed"].toDouble()));
		hardFactor->setText(QString::number(properties["hardFactor"].toDouble()));
	}
	InternalOperation = false;
}