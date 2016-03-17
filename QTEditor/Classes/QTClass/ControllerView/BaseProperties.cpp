#include "BaseProperties.h"
#include "Global.h"
#include "QLabel"
#include "FontChina.h"
#include <QLayout>
#include "MyLogger.h"

BaseProperties::BaseProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_BaseProperties;
	_setLayout();
	__connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}

BaseProperties::~BaseProperties()
{
}

void BaseProperties::_setLayout()
{
	scaleXEdit = new QLineEdit();
	scaleXEdit->setText("1");
	scaleYEdit = new QLineEdit();
	scaleYEdit->setText("1");
	rotateEdit = new QLineEdit();
	scaleXSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("缩放值X")));
	scaleYSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("缩放值Y")));
	rotateSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("旋转值")));
	flipXButton = new QCheckBox();
	flipYButton = new QCheckBox();
	spriteMergeBox = new QCheckBox();
	flipXSelectedBox = new QSelectedCheckBox(FontChina::G2U("水平翻转"));
	flipYSelectedBox = new QSelectedCheckBox(FontChina::G2U("垂直翻转"));
	spriteMergeSelectBox = new QSelectedCheckBox(FontChina::G2U("禁用合并"));
	QHBoxLayout* hlayout1 = new QHBoxLayout();
	hlayout1->addWidget(scaleXSelectedBox);
	hlayout1->addWidget(scaleXEdit);
	hlayout1->addWidget(scaleYSelectedBox);
	hlayout1->addWidget(scaleYEdit);
	hlayout1->addWidget(rotateSelectedBox);
	hlayout1->addWidget(rotateEdit);
	QGridLayout* gridLayout1 = new QGridLayout();
	gridLayout1->addWidget(flipXButton, 0, 0, 1, 1);
	gridLayout1->addWidget(flipXSelectedBox, 0, 1, 1, 10);
	gridLayout1->addWidget(flipYButton, 0, 10, 1, 1);
	gridLayout1->addWidget(flipYSelectedBox, 0, 11, 1, 10);
	gridLayout1->addWidget(spriteMergeBox, 0, 21, 1, 1);
	gridLayout1->addWidget(spriteMergeSelectBox, 0, 22, 1, 10);
	QHBoxLayout* hlayout3 = new QHBoxLayout();
	anchrXSelectedBox = new QSelectedCheckBox(FontChina::G2U("锚点X"));
	anchrYSelectedBox = new QSelectedCheckBox(FontChina::G2U("锚点Y"));
	anchrXEdit = new QLineEdit("0.5");
	anchrYEdit = new QLineEdit("0.5");
	hlayout3->addWidget(anchrXSelectedBox);
	hlayout3->addWidget(anchrXEdit);
	hlayout3->addWidget(anchrYSelectedBox);
	hlayout3->addWidget(anchrYEdit);
	QHBoxLayout* hlayout4 = new QHBoxLayout();
	hlayout4->addWidget(new QLabel(FontChina::G2U("id:")));
	idLabel = new QLineEdit;
	wLabel = new QLineEdit;
	hLabel = new QLineEdit;
	idLabel->setReadOnly(true);
	wLabel->setReadOnly(true);
	hLabel->setReadOnly(true);
	hlayout4->addWidget(idLabel);
	hlayout4->addWidget(new QLabel(FontChina::G2U("w:")));
	hlayout4->addWidget(wLabel);
	hlayout4->addWidget(new QLabel(FontChina::G2U("h:")));
	hlayout4->addWidget(hLabel);

	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->addLayout(hlayout1);
	vlayout->addLayout(gridLayout1);
	vlayout->addLayout(hlayout3);
	vlayout->addLayout(hlayout4);
	widget->setLayout(vlayout);

}

void BaseProperties::addSelectedCheckBox()
{
	checkVec.push_back(scaleXSelectedBox);
	checkVec.push_back(scaleYSelectedBox);
	checkVec.push_back(rotateSelectedBox);
	checkVec.push_back(flipXSelectedBox);
	checkVec.push_back(flipYSelectedBox);
	checkVec.push_back(spriteMergeSelectBox);
	checkVec.push_back(anchrXSelectedBox);
	checkVec.push_back(anchrYSelectedBox);
	selectNameVec << "selectScaleX" << "selectScaleY" << "selectRotation" << "selectFlipx" << "selectFlipy" << "selectDontMerge" << "selectAnchrx" << "selectAnchry";
}

void BaseProperties::setDefaultValue()
{
	scaleXEdit->setText("1");
	scaleYEdit->setText("1");
	rotateEdit->setText("");
	anchrXEdit->setText("0.5");
	anchrYEdit->setText("0.5");
	flipXButton->setCheckState(Qt::CheckState::Unchecked);
	flipYButton->setCheckState(Qt::CheckState::Unchecked);
	spriteMergeBox->setCheckState(Qt::CheckState::Unchecked);
}

void BaseProperties::changeSpriteValue()
{
	InternalOperation = true;
	if (targetSprite){
		targetSprite->setHasBaseProperties(true);
		targetSprite->setScaleX(scaleXEdit->text().toDouble());
		targetSprite->setScaleY(scaleYEdit->text().toDouble());
		targetSprite->setRotation(rotateEdit->text().toDouble());
		Point anchr;
		anchr.x = anchrXEdit->text().toDouble();
		anchr.y = anchrYEdit->text().toDouble();
		targetSprite->anchr = anchr;
		targetSprite->setAnchorPoint(targetSprite->anchr);
		switch (flipXButton->checkState()){
		case Qt::CheckState::Unchecked:
			targetSprite->setFlippedX(false);
			targetSprite->flipx = false;
			break;
		case Qt::CheckState::Checked:
			targetSprite->setFlippedX(true);
			targetSprite->flipx = true;
			break;
		}
		switch (flipYButton->checkState()){
		case Qt::CheckState::Unchecked:
			targetSprite->setFlippedY(false);
			targetSprite->flipy = false;
			break;
		case Qt::CheckState::Checked:
			targetSprite->setFlippedY(true);
			targetSprite->flipy = true;
			break;
		}
		switch (spriteMergeBox->checkState()){
		case Qt::CheckState::Unchecked:
			targetSprite->setDontMerge(false);
			break;
		case Qt::CheckState::Checked:
			targetSprite->setDontMerge(true);
			break;
		}
	}
	InternalOperation = false;
}

void BaseProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("BaseProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["baseProperties"].toObject();
	bool hasBaseProperties = obj["hasBaseProperties"].toBool();
	
	setCheckState(selectNameVec, properties);


	if (!hasBaseProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		if (properties.contains("scaleX")){
			scaleXEdit->setText(QString::number(properties["scaleX"].toDouble()));
			scaleYEdit->setText(QString::number(properties["scaleY"].toDouble()));
		}
		else{
			scaleXEdit->setText(QString::number(properties["scale"].toDouble()));
			scaleYEdit->setText(QString::number(properties["scale"].toDouble()));
		}
		rotateEdit->setText(QString::number(properties["rotation"].toInt()));
		anchrXEdit->setText(QString::number(properties["anchrx"].toDouble()));
		anchrYEdit->setText(QString::number(properties["anchry"].toDouble()));
		if (properties["flipx"].toBool()){
			flipXButton->setCheckState(Qt::CheckState::Checked);
		}
		else{
			flipXButton->setCheckState(Qt::CheckState::Unchecked);
		}
		if (properties["flipy"].toBool()){
			flipYButton->setCheckState(Qt::CheckState::Checked);
		}
		else{
			flipYButton->setCheckState(Qt::CheckState::Unchecked);
		}
		if (properties["dontMerge"].toBool()){
			spriteMergeBox->setCheckState(Qt::CheckState::Checked);
		}
		else{
			spriteMergeBox->setCheckState(Qt::CheckState::Unchecked);
		}
		
	}
	InternalOperation = false;
}

void BaseProperties::setWidgetValue()
{
	InternalOperation = true;
	idLabel->setText(QString::number(targetSprite->getId()));
	wLabel->setText(QString::number(targetSprite->getContentSize().width));
	hLabel->setText(QString::number(targetSprite->getContentSize().height));
	this->setVisible(true);
	if (targetSprite->isFlippedX()){
		flipXButton->setCheckState(Qt::CheckState::Checked);
	}
	else{
		flipXButton->setCheckState(Qt::CheckState::Unchecked);
	}
	if (targetSprite->isFlippedY()){
		flipYButton->setCheckState(Qt::CheckState::Checked);
	}
	else{
		flipYButton->setCheckState(Qt::CheckState::Unchecked);
	}
	if (targetSprite->isDontMerge()){
		spriteMergeBox->setCheckState(Qt::CheckState::Checked);
	}
	else{
		spriteMergeBox->setCheckState(Qt::CheckState::Unchecked);
	}
	scaleXEdit->setText(QString::number(targetSprite->getScaleX()));
	scaleYEdit->setText(QString::number(targetSprite->getScaleY()));
	rotateEdit->setText(QString::number(targetSprite->getRotation()));
	Point anchr = targetSprite->anchr;
	anchrXEdit->setText(QString::number(anchr.x));
	anchrYEdit->setText(QString::number(anchr.y));
	InternalOperation = false;
}

void BaseProperties::resetTemplateValue(int index)
{
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (index){
	case resetScaleX:
		resetTemplateScaleX(obj);
		break;
	case resetScaleY:
		resetTemplateScaleY(obj);
		break;
	case resetRotation:
		resetTemplateRotation(obj);
		break;
	case resetFlipX:
		resetTemplateFlipX(obj);
		break;
	case resetFlipY:
		resetTemplateFlipY(obj);
		break;
	case resetDontMerge:
		resetTemplateDontMerge(obj);
		break;
	case resetAnchrX:
		resetTemplateAnchrX(obj);
		break;
	case resetAnchrY:
		resetTemplateAnchrY(obj);
		break;
	}
	writeToFile(templateFile, obj);
}

void BaseProperties::resetTemplateScaleX(QJsonObject &obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	double scaleX = scaleXEdit->text().toDouble();
	baseProperties["scaleX"] = scaleX;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::resetTemplateScaleY(QJsonObject &obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	double scaleY = scaleYEdit->text().toDouble();
	baseProperties["scaleY"] = scaleY;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::resetTemplateRotation(QJsonObject &obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	int rotation = rotateEdit->text().toInt();
	baseProperties["rotation"] = rotation;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::resetTemplateFlipX(QJsonObject &obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	bool flipx;
	switch (flipXButton->checkState()){
	case Qt::CheckState::Checked:
		flipx = true;
		break;
	case Qt::CheckState::Unchecked:
		flipx = false;
		break;
	}
	baseProperties["flipx"] = flipx;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::resetTemplateFlipY(QJsonObject &obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	bool flipy;
	switch (flipYButton->checkState()){
	case Qt::CheckState::Checked:
		flipy = true;
		break;
	case Qt::CheckState::Unchecked:
		flipy = false;
		break;
	}
	baseProperties["flipy"] = flipy;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::resetTemplateAnchrX(QJsonObject &obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	float anchrx = anchrXEdit->text().toDouble();
	baseProperties["anchrx"] = anchrx;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::resetTemplateAnchrY(QJsonObject &obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	float anchry = anchrYEdit->text().toDouble();
	baseProperties["anchry"] = anchry;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::resetTemplateDontMerge(QJsonObject& obj)
{
	QJsonObject baseProperties = obj["baseProperties"].toObject();
	bool dontmerge;
	switch (spriteMergeSelectBox->checkState()){
	case Qt::CheckState::Checked:
		dontmerge = true;
		break;
	case Qt::CheckState::Unchecked:
		dontmerge = false;
		break;
	}
	baseProperties["dontMerge"] = dontmerge;
	obj["baseProperties"] = baseProperties;
}

void BaseProperties::__connect()
{
	connect(scaleXEdit, SIGNAL(editingFinished()), this, SLOT(ScaleXTextRedo()));
	connect(scaleYEdit, SIGNAL(editingFinished()), this, SLOT(ScaleYTextRedo()));
	connect(rotateEdit, SIGNAL(editingFinished()), this, SLOT(RotationTextRedo()));
	connect(flipXButton, SIGNAL(stateChanged(int)), this, SLOT(flipXSelected(int)));
	connect(flipYButton, SIGNAL(stateChanged(int)), this, SLOT(flipYSelected(int)));
	connect(anchrXEdit, SIGNAL(editingFinished()), this, SLOT(anchrXRedo()));
	connect(anchrYEdit, SIGNAL(editingFinished()), this, SLOT(anchrYRedo()));
	connect(spriteMergeBox, SIGNAL(stateChanged(int)), this, SLOT(spriteDontMergeState(int)));
}

void BaseProperties::ScaleXTextRedo()
{
	double scaleX = scaleXEdit->text().toDouble();
	if (!InternalOperation && scaleX != 0.0f && targetSprite){
		targetSprite->setScaleX(scaleX);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetScaleX);
	}
}

void BaseProperties::ScaleYTextRedo()
{
	double scaleY = scaleYEdit->text().toDouble();
	if (!InternalOperation && scaleY != 0.0f && targetSprite){
		targetSprite->setScaleY(scaleY);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetScaleY);
	}
}

void BaseProperties::RotationTextRedo()
{
	double rotation = rotateEdit->text().toDouble();
	if (!InternalOperation && targetSprite){
		targetSprite->setRotation(rotation);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetRotation);
	}
}

void BaseProperties::anchrXRedo()
{
	if (!InternalOperation && targetSprite){
		Point anchr = targetSprite->getAnchorPoint();
		targetSprite->anchr = Vec2(anchrXEdit->text().toDouble(), anchr.y);
		targetSprite->setAnchorPoint(targetSprite->anchr);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetAnchrX);
	}
}

void BaseProperties::anchrYRedo()
{
	if (!InternalOperation && targetSprite){
		Point anchr = targetSprite->getAnchorPoint();
		targetSprite->anchr = Vec2(anchr.x, anchrYEdit->text().toDouble());
		targetSprite->setAnchorPoint(targetSprite->anchr);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetAnchrY);
	}
}

void BaseProperties::spriteDontMergeState(int state)
{
	if (!InternalOperation && targetSprite){
		switch (state){
		case Qt::CheckState::Unchecked:
			targetSprite->setDontMerge(false);
			break;
		case  Qt::CheckState::Checked:
			targetSprite->setDontMerge(true);
			break;
		}
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetDontMerge);
	}
}

void  BaseProperties::flipXSelected(int state)
{
	if (!InternalOperation && targetSprite)
		switch (state){
		case Qt::CheckState::Unchecked:
			targetSprite->setFlippedX(false);
			targetSprite->flipx = false;
			break;
		case  Qt::CheckState::Checked:
			targetSprite->setFlippedX(true);
			targetSprite->flipx = true;
			break;
		}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetFlipX);
	}
}

void  BaseProperties::flipYSelected(int state)
{
	if (!InternalOperation && targetSprite)
		switch (state){
		case Qt::CheckState::Unchecked:
			targetSprite->setFlippedY(false);
			targetSprite->flipy = false;
			break;
		case Qt::CheckState::Checked:
			targetSprite->setFlippedY(true);
			targetSprite->flipy = true;
			break;
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetFlipY);
	}
}

QVariantMap BaseProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["baseProperties"].toObject();
	if (scaleXSelectedBox->getState()){
		map.insert("scaleX", scaleXEdit->text().toDouble());
		map.insert("selectScaleX", true);
	}
	else{
		map.insert("scaleX", properties["scaleX"].toDouble());
		map.insert("selectScaleX", false);
	}

	if (scaleYSelectedBox->getState()){
		map.insert("scaleY", scaleYEdit->text().toDouble());
		map.insert("selectScaleY", true);
	}
	else{
		map.insert("scaleY", properties["scaleY"].toDouble());
		map.insert("selectScaleY", false);
	}

	if (rotateSelectedBox->getState()){
		map.insert("rotation", rotateEdit->text().toInt());
		map.insert("selectRotation", true);
	}
	else{
		map.insert("rotation", properties["rotation"].toInt());
		map.insert("selectRotation", false);
	}

	if (flipXSelectedBox->getState()){
		switch (flipXButton->checkState()){
		case Qt::CheckState::Unchecked:
			map.insert("flipx", false);
			break;
		case Qt::CheckState::Checked:
			map.insert("flipx", true);
			break;
		}
		map.insert("selectFlipx", true);
	}
	else{
		map.insert("flipx", properties["flipx"].toBool());
		map.insert("selectFlipx", false);
	}

	if (flipYSelectedBox->getState()){
		switch (flipYButton->checkState()){
		case Qt::CheckState::Unchecked:
			map.insert("flipy", false);
			break;
		case Qt::CheckState::Checked:
			map.insert("flipy", true);
			break;
		}
		map.insert("selectFlipy", true);
	}
	else{
		map.insert("flipy", properties["flipy"].toBool());
		map.insert("selectFlipy", false);
	}

	if (spriteMergeSelectBox->getState()){
		switch (spriteMergeBox->checkState()){
		case Qt::CheckState::Unchecked:
			map.insert("dontMerge", false);
			break;
		case Qt::CheckState::Checked:
			map.insert("dontMerge", true);
			break;
		}
		map.insert("selectDontMerge", true);
	}
	else{
		map.insert("dontMerge", properties["dontMerge"].toBool());
		map.insert("selectDontMerge", false);
	}

	if (anchrXSelectedBox->getState()){
		map.insert("anchrx", anchrXEdit->text().toDouble());
		map.insert("selectAnchrx", true);
	}
	else{
		map.insert("anchrx", properties["anchrx"].toDouble());
		map.insert("selectAnchrx", false);
	}

	if (anchrYSelectedBox->getState()){
		map.insert("anchry", anchrYEdit->text().toDouble());
		map.insert("selectAnchry", true);
	}
	else{
		map.insert("anchry", properties["anchry"].toDouble());
		map.insert("selectAnchry", false);
	}

	return map;
}

void BaseProperties::switchFlipXState()
{
	switch (flipXButton->checkState()){
	case Qt::CheckState::Checked:
		flipXButton->setCheckState(Qt::CheckState::Unchecked);
		break;
	case Qt::CheckState::Unchecked:
		flipXButton->setCheckState(Qt::CheckState::Checked);
		break;
	}
}

void BaseProperties::switchFlipYState()
{
	switch (flipYButton->checkState()){
	case Qt::CheckState::Checked:
		flipYButton->setCheckState(Qt::CheckState::Unchecked);
		break;
	case Qt::CheckState::Unchecked:
		flipYButton->setCheckState(Qt::CheckState::Checked);
		break;
	}
}