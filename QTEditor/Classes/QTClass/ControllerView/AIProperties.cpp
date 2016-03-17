#include "AIProperties.h"
#include "FontChina.h"
#include "QFile"
#include "QFileDialog"
#include "MyLogger.h"

AIProperties::AIProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_AIProperties;
	setLayout();
	__connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
	listKey = "Key";
	listValue = "Value";
	selectListKey = "selectKey";
	selectListValue = "selectValue";
	listname = "list";
}

AIProperties::~AIProperties()
{
}

void AIProperties::addSelectedCheckBox()
{
	checkVec.push_back(minXSelectedBox);
	checkVec.push_back(maxXSelectedBox);
	checkVec.push_back(minYSelectedBox);
	checkVec.push_back(maxYSelectedBox);
	checkVec.push_back(aiFileSelectedBox);
	selectNameVec << "selectMinX" << "selectMaxX" << "selectMinY" << "selectMaxY" << "selectAiFile";
}

void AIProperties::__connect()
{
	connect(minX, SIGNAL(editingFinished()), this, SLOT(minXRedo()));
	connect(maxX, SIGNAL(editingFinished()), this, SLOT(maxXRedo()));
	connect(minY, SIGNAL(editingFinished()), this, SLOT(minYRedo()));
	connect(maxY, SIGNAL(editingFinished()), this, SLOT(maxYRedo()));
	connect(addAi, SIGNAL(clicked()), this, SLOT(on_addAI_clicked()));
	connect(aiFile, SIGNAL(editingFinished()), this, SLOT(aiFileRedo()));
	connect(addUserDefine, SIGNAL(clicked()), this, SLOT(on_addUserDefine_clicked()));
}

void AIProperties::setLayout()
{
	QHBoxLayout* hlayout = new QHBoxLayout;
	QHBoxLayout* hlayout2 = new QHBoxLayout;
	QGridLayout* glayout = new QGridLayout;
	gListlayout = new QGridLayout;
	minX = new QLineEdit;
	maxX = new QLineEdit;
	minY = new QLineEdit;
	maxY = new QLineEdit;
	addAi = new QPushButton;
	addAi->setMaximumSize(QSize(30, 30));
	addAi->setIcon(QIcon(":/icon/Resources/open.ico"));
	addAi->setFlat(true);
	addAi->setStyleSheet(" border: none; ");
	addUserDefine = new QPushButton;
	addUserDefine->setMaximumSize(QSize(30, 30));
	addUserDefine->setIcon(QIcon(":/icon/Resources/add2.ico"));
	addUserDefine->setFlat(true);
	addUserDefine->setStyleSheet(" border: none; ");
	aiFile = new QLineEdit;
	minXSelectedBox = new QSelectedCheckBox(FontChina::G2U("MinX"));
	hlayout->addWidget(minXSelectedBox);
	hlayout->addWidget(minX);
	maxXSelectedBox = new QSelectedCheckBox(FontChina::G2U("MaxX"));
	hlayout->addWidget(maxXSelectedBox);
	hlayout->addWidget(maxX);
	minYSelectedBox = new QSelectedCheckBox(FontChina::G2U("MinY"));
	hlayout2->addWidget(minYSelectedBox);
	hlayout2->addWidget(minY);
	maxYSelectedBox = new QSelectedCheckBox(FontChina::G2U("MaxY"));
	hlayout2->addWidget(maxYSelectedBox);
	hlayout2->addWidget(maxY);
	aiFileSelectedBox = new QSelectedCheckBox(FontChina::G2U("aiFile"));
	glayout->addWidget(aiFileSelectedBox, 0, 0, 1, 2);
	glayout->addWidget(aiFile, 0, 2, 1, 6);
	glayout->addWidget(addAi, 0, 8, 1, 1);
	glayout->addWidget(addUserDefine, 0, 9, 1, 1);
	QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addLayout(hlayout);
	vlayout->addLayout(hlayout2);
	vlayout->addLayout(glayout);
	vlayout->addLayout(gListlayout);
	widget->setLayout(vlayout);
}

bool AIProperties::checkSprite()
{
	if (targetSprite && (targetSprite->getType() == SpriteType::MONSTER || targetSprite->getType() == SpriteType::DYNAMIC_BLOCK || targetSprite->getType() == SpriteType::ROLE)){
		return true;
	}
	return false;
}

QVariantMap AIProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["aiProperties"].toObject();
	if (minXSelectedBox->getState()){
		map.insert("minX", minX->text().toInt());
		map.insert("selectMinX", true);
	}
	else{
		map.insert("minX", properties["minX"].toInt());
		map.insert("selectMinX", false);
	}

	if (maxXSelectedBox->getState()){
		map.insert("maxX", maxX->text().toInt());
		map.insert("selectMaxX", true);
	}
	else{
		map.insert("maxX", properties["maxX"].toInt());
		map.insert("selectMaxX", false);
	}

	if (minYSelectedBox->getState()){
		map.insert("minY", minY->text().toInt());
		map.insert("selectMinY", true);
	}
	else{
		map.insert("minY", properties["minY"].toInt());
		map.insert("selectMinY", false);
	}

	if (maxYSelectedBox->getState()){
		map.insert("maxY", maxY->text().toInt());
		map.insert("selectMaxY", true);
	}
	else{
		map.insert("maxY", properties["maxY"].toInt());
		map.insert("selectMaxY", false);
	}

	if (aiFileSelectedBox->getState()){
		map.insert("aiFile", aiFile->text());
		map.insert("selectAiFile", true);
	}
	else{
		map.insert("aiFile", properties["aiFile"].toString());
		map.insert("selectAiFile", false);
	}
	QVariantList Vlist;
	for (int i = 0; i < list.size(); i++){
		if (list.at(i)->labelKey->checkState() == Qt::CheckState::Checked && list.at(i)->labelValue->checkState() == Qt::CheckState::Checked){
			QVariantMap data;
			data.insert("key", list.at(i)->editKey->text());
			data.insert("value", list.at(i)->editValue->text());
			Vlist.push_back(data);
		}
	}
	map.insert("list", Vlist);
	return map;
}

void AIProperties::setDefaultValue()
{
	minX->setText("0");
	maxX->setText("0");
	minY->setText("0");
	maxY->setText("0");
	aiFile->setText("");
	deleteAllList();
}

void AIProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("AIProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["aiProperties"].toObject();

	setCheckState(selectNameVec, properties);

	bool hasAIProperties = obj["hasAIProperties"].toBool();
	if (targetSprite){
		targetSprite->setHasAIProperties(hasAIProperties);
	}
	if (!hasAIProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		minX->setText(QString::number(properties["minX"].toInt()));
		maxX->setText(QString::number(properties["maxX"].toInt()));
		minY->setText(QString::number(properties["minY"].toInt()));
		maxY->setText(QString::number(properties["maxY"].toInt()));
		aiFile->setText(properties["aiFile"].toString());
		deleteAllList();
		auto __list = properties["list"].toArray();
		for (int i = 0; i < __list.size(); i++){
			ListData* data = new ListData;
			QJsonObject jo = __list.at(i).toObject();
			createListItem(data, false, jo["key"].toString(), jo["value"].toString());
			setCheckLabelState(jo, data);
		}
	} 
	InternalOperation = false;
}

void AIProperties::changeSpriteValue()
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	if (targetSprite && this->isVisible() && targetSprite->isHasAIProperties()){
		sprite->setRangeMinX(minX->text().toInt(), false);
		sprite->setRangeMaxX(maxX->text().toInt(), false);
		sprite->setRangeMinY(minY->text().toInt(), false);
		sprite->setRangeMaxY(maxY->text().toInt(), false);
		sprite->setAIFile(aiFile->text().toStdString());
		if (list.size() > 0){
			sprite->deleteUserAIList();
			for (int i = 0; i < list.size(); i++){
				sprite->addUserAI(list.at(i)->editKey->text(), list.at(i)->editValue->text());
			}
		}
	}
}

void AIProperties::minXRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setRangeMinX(minX->text().toInt(), true);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetRangeMinX);
	}
}

void AIProperties::maxXRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setRangeMaxX(maxX->text().toInt(), true);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetRangeMaxX);
	}
}

void AIProperties::minYRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setRangeMinY(minY->text().toInt(), true);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetRangeMinY);
	}
}

void AIProperties::maxYRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setRangeMaxY(maxY->text().toInt(), true);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetRangeMaxY);
	}
}

void AIProperties::aiFileRedo()
{
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setAIFile(aiFile->text().toStdString());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetAIFile);
	}
}

void AIProperties::on_addAI_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, FontChina::G2U("ai文件"), NULL, NULL);
	if (file.isEmpty()){
		return;
	}
	aiFile->setText(file.section("/", -1));
	if (!InternalOperation && targetSprite){
		auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
		sprite->setAIFile(file.section("/", -1).toStdString());
	}
	else if(!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetAIFile);
	}
}

void AIProperties::on_addUserDefine_clicked()
{
	ListData* data = new ListData;
	createListItem(data, true);
}

void AIProperties::resetTemplateValue(int item, QWidget* edit, int index)
{
	QFile sFile(templateFile);
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (item){
	case resetRangeMinX:
		resetTemplateMinX(obj);
		break;
	case resetRangeMaxX:
		resetTemplateMaxX(obj);
		break;
	case resetRangeMinY:
		resetTemplateMinY(obj);
		break;
	case resetRangeMaxY:
		resetTemplateMaxY(obj);
		break;
	case resetAIFile:
		resetTemplateAIFile(obj);
		break;
	case resetListItemKey:
		resetTemplateListKey(obj, edit, index);
		break;
	case resetListItemValue:
		resetTemplateListValue(obj, edit, index);
		break;
	case subListItem:
		subTemplateListItem(obj, edit, index);
		break;
	}
	writeToFile(templateFile, obj);
}

void AIProperties::resetTemplateMinX(QJsonObject& obj)
{
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	int x = minX->text().toInt();
	aiProperties["minX"] = x;
	obj["aiProperties"] = aiProperties;
}

void AIProperties::resetTemplateMaxX(QJsonObject& obj)
{
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	int x = maxX->text().toInt();
	aiProperties["maxX"] = x;
	obj["aiProperties"] = aiProperties;
}

void AIProperties::resetTemplateMinY(QJsonObject& obj)
{
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	int y = minY->text().toInt();
	aiProperties["minY"] = y;
	obj["aiProperties"] = aiProperties;
}

void AIProperties::resetTemplateMaxY(QJsonObject& obj)
{
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	int y = maxY->text().toInt();
	aiProperties["maxY"] = y;
	obj["aiProperties"] = aiProperties;
}

void AIProperties::resetTemplateAIFile(QJsonObject& obj)
{
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	QString file = aiFile->text();
	aiProperties["aiFile"] = file;
	obj["aiProperties"] = aiProperties;
}

void AIProperties::resetTemplateListKey(QJsonObject& obj, QWidget* edit, int index)
{
	auto editor = dynamic_cast<QLineEdit*>(edit);
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	QJsonArray listdata = aiProperties["list"].toArray();
	if (index >= listdata.size()){
		QJsonObject data;
		data["key"] = editor->text();
		data["value"] = "";
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			listother.push_back(listdata.at(i).toObject());
		}
		listother.push_back(data);
		aiProperties["list"] = listother;
	}
	else{
		auto data = listdata.at(index).toObject();
		data["key"] = editor->text();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			if (i != index){
				listother.push_back(listdata.at(i).toObject());
			}
			else{
				listother.push_back(data);
			}
		}
		aiProperties["list"] = listother;
	}
	obj["aiProperties"] = aiProperties;
}

void AIProperties::resetTemplateListValue(QJsonObject& obj, QWidget* edit, int index)
{
	auto editor = dynamic_cast<QLineEdit*>(edit);
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	QJsonArray listdata = aiProperties["list"].toArray();
	if (index >= listdata.size()){
		QJsonObject data;
		data["key"] = "";
		data["value"] = editor->text();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			listother.push_back(listdata.at(i).toObject());
		}
		listother.push_back(data);
		aiProperties["list"] = listother;
	}
	else{
		auto data = listdata.at(index).toObject();
		data["value"] = editor->text();
		QJsonArray listother;
		for (int i = 0; i < listdata.size(); i++){
			if (i != index){
				listother.push_back(listdata.at(i).toObject());
			}
			else{
				listother.push_back(data);
			}
		}
		aiProperties["list"] = listother;
	}
	obj["aiProperties"] = aiProperties;
}

void AIProperties::subTemplateListItem(QJsonObject& obj, QWidget* edit, int index)
{
	auto sub = dynamic_cast<QPushButton*>(edit);
	QJsonObject aiProperties = obj["aiProperties"].toObject();
	QJsonArray listdata = aiProperties["list"].toArray();
	QJsonArray listother;
	for (int i = 0; i < listdata.size(); i++){
		if (i != index){
			listother.push_back(listdata.at(i).toObject());
		}
	}
	aiProperties["list"] = listother;
	obj["aiProperties"] = aiProperties;
}

void AIProperties::setWidgetValue()
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	InternalOperation = true;
	if (targetSprite->isHasAIProperties()){
		this->setVisible(true);
		minX->setText(QString::number(sprite->getRangeMinX()));
		maxX->setText(QString::number(sprite->getRangeMaxX()));
		minY->setText(QString::number(sprite->getRangeMinY()));
		maxY->setText(QString::number(sprite->getRangeMaxY()));
		aiFile->setText(sprite->getAIFile().c_str());
		deleteAllList();
		auto vec = sprite->getUserAIList();
		for (int i = 0; i < vec.size(); i++){
			ListData* data = new ListData;
			createListItem(data, false, vec.at(i)->key, vec.at(i)->value);
		}
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
	InternalOperation = false;
}

void AIProperties::addListData(QString key, QString value, QString extra)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->addUserAI(key, value);
}

void AIProperties::resetSpriteListKey(int index, QString key)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->resetUserAIKey(index, key);
}

void AIProperties::resetSpriteListValue(int index, QString value)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->resetUserAIValue(index, value);
}

void AIProperties::resetTemplateListKey(int index, QWidget* w)
{
	resetTemplateValue(resetListItemKey, w, index);
}

void AIProperties::resetTemplateListValue(int index, QWidget* w)
{
	resetTemplateValue(resetListItemValue, w, index);
}

void AIProperties::subSpriteListItem(int index)
{
	auto sprite = dynamic_cast<EntityImageSprite*>(targetSprite);
	sprite->deleteUserAI(index);
}

void AIProperties::subTemplateListItem(int index, QWidget* w)
{
	resetTemplateValue(subListItem, w, index);
}

