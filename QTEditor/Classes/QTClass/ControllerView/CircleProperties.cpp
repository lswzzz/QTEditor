#include "CircleProperties.h"
#include "QLayout"
#include "FontChina.h"
#include "QLabel"
#include "QFile"
#include "MyLogger.h"

CircleProperties::CircleProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_CircleProperties;
	_setLayout();
	_connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}

void CircleProperties::_setLayout()
{
	anchrX = new QLineEdit;
	anchrY = new QLineEdit;
	radius = new QLineEdit;
	showRigid = new QComboBox;
	showRigid->addItem(FontChina::G2U("显示"));
	showRigid->addItem(FontChina::G2U("隐藏"));
	QVBoxLayout* vlayout = new QVBoxLayout;
	QHBoxLayout* hlayout = new QHBoxLayout;
	QHBoxLayout* hlayout2 = new QHBoxLayout;
	anchorXSelectedBox = new QSelectedCheckBox(FontChina::G2U("锚点X"));
	hlayout->addWidget(anchorXSelectedBox);
	hlayout->addWidget(anchrX);
	anchorYSelectedBox = new QSelectedCheckBox(FontChina::G2U("锚点Y"));
	hlayout->addWidget(anchorYSelectedBox);
	hlayout->addWidget(anchrY);
	radiusSelectedBox = new QSelectedCheckBox(FontChina::G2U("半径"));
	hlayout2->addWidget(radiusSelectedBox);
	hlayout2->addWidget(radius);
	showRigidSelectedBox = new QSelectedCheckBox(FontChina::G2U("显示"));
	hlayout2->addWidget(showRigidSelectedBox);
	hlayout2->addWidget(showRigid);
	vlayout->addLayout(hlayout);
	vlayout->addLayout(hlayout2);
	widget->setLayout(vlayout);
}

CircleProperties::~CircleProperties()
{
}

void CircleProperties::addSelectedCheckBox()
{
	checkVec.push_back(anchorXSelectedBox);
	checkVec.push_back(anchorYSelectedBox);
	checkVec.push_back(radiusSelectedBox);
	checkVec.push_back(showRigidSelectedBox);
	selectNameVec << "selectAnchrx" << "selectAnchry" << "selectRadius" << "selectShowRagid";
}

void CircleProperties::_connect()
{
	connect(anchrX, SIGNAL(editingFinished()), this, SLOT(AnchrXRedo()));
	connect(anchrY, SIGNAL(editingFinished()), this, SLOT(AnchrYRedo()));
	connect(radius, SIGNAL(editingFinished()), this, SLOT(RadiusRedo()));
	connect(showRigid, SIGNAL(currentIndexChanged(int)), this, SLOT(showRigidChange(int)));
}

QVariantMap CircleProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["circleProperties"].toObject();
	if (anchorXSelectedBox->getState()){
		map.insert("anchrx", anchrX->text().toFloat());
		map.insert("selectAnchrx", true);
	}
	else{
		map.insert("anchrx", properties["anchrx"].toDouble());
		map.insert("selectAnchrx", false);
	}

	if (anchorYSelectedBox->getState()){
		map.insert("anchry", anchrY->text().toFloat());
		map.insert("selectAnchry", true);
	}
	else{
		map.insert("anchry", properties["anchry"].toDouble());
		map.insert("selectAnchry", false);
	}

	if (radiusSelectedBox->getState()){
		map.insert("radius", radius->text().toFloat());
		map.insert("selectRadius", true);
	}
	else{
		map.insert("radius", properties["radius"].toDouble());
		map.insert("selectRadius", false);
	}

	if (showRigidSelectedBox->getState()){
		map.insert("showRigid", showRigid->currentIndex());
		map.insert("selectShowRagid", true);
	}
	else{
		map.insert("showRigid", properties["showRigid"].toInt());
		map.insert("selectShowRagid", false);
	}
	return map;
}

void CircleProperties::setDefaultValue()
{
	anchrX->setText("0");
	anchrY->setText("0");
	radius->setText("0");
	showRigid->setCurrentIndex(0);
}

void CircleProperties::setValueFromFile(QString file, bool clearTarget)
{
	if (clearTarget){
		targetSprite = NULL;
	}
	QFile sFile(file);
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("CircleProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["circleProperties"].toObject();

	setCheckState(selectNameVec, properties);

	bool hasCircleProperties = obj["hasCircleProperties"].toBool();
	if(targetSprite){
		targetSprite->setHasCircleProperties(hasCircleProperties);
	}
	if (!hasCircleProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		anchrX->setText(QString::number(properties["anchrx"].toDouble()));
		anchrY->setText(QString::number(properties["anchry"].toDouble()));
		radius->setText(QString::number(properties["radius"].toDouble()));
		showRigid->setCurrentIndex(properties["showRigid"].toInt());
	}
}

void CircleProperties::changeSpriteValue()
{
	if (targetSprite && this->isVisible() && targetSprite->isHasCircleProperties()){
		targetSpriteDrawCircle();
		targetSprite->showRigid(showRigid->currentIndex());
	}
}

void CircleProperties::setWidgetValue()
{
	if (targetSprite->isHasCircleProperties()){
		anchrX->setText(QString::number(targetSprite->getCircleAnchrPos().x));
		anchrY->setText(QString::number(targetSprite->getCircleAnchrPos().y));
		radius->setText(QString::number(targetSprite->getRadius()));
		if (targetSprite->getDrawNodeVisible()){
			showRigid->setCurrentIndex(0);
		}
		else{
			showRigid->setCurrentIndex(1);
		}
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
}

void CircleProperties::resetTemplateValue(int index)
{
	QFile sFile(templateFile);
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (index){
	case resetAnchrX:
		resetTemplateAnchrX(obj);
		break;
	case resetAnchrY:
		resetTemplateAnchrY(obj);
		break;
	case resetRadius:
		resetTemplateRadius(obj);
		break;
	case resetShowType:
		resetTemplateShow(obj);
		break;
	}
	writeToFile(templateFile, obj);
}

void CircleProperties::resetTemplateAnchrX(QJsonObject& obj)
{
	QJsonObject circleProperties = obj["circleProperties"].toObject();
	double anx = anchrX->text().toDouble();
	circleProperties["anchrx"] = anx;
	obj["circleProperties"] = circleProperties;
}

void CircleProperties::resetTemplateAnchrY(QJsonObject& obj)
{
	QJsonObject circleProperties = obj["circleProperties"].toObject();
	double any = anchrY->text().toDouble();
	circleProperties["anchry"] = any;
	obj["circleProperties"] = circleProperties;
}

void CircleProperties::resetTemplateRadius(QJsonObject& obj)
{
	QJsonObject circleProperties = obj["circleProperties"].toObject();
	double radi = radius->text().toDouble();
	circleProperties["radius"] = radi;
	obj["circleProperties"] = circleProperties;
}

void CircleProperties::resetTemplateShow(QJsonObject& obj)
{
	QJsonObject circleProperties = obj["circleProperties"].toObject();
	int show = showRigid->currentIndex();
	circleProperties["showRigid"] = show;
	obj["circleProperties"] = circleProperties;
}

void CircleProperties::targetSpriteDrawCircle()
{
	float x = anchrX->text().toFloat();
	float y = anchrY->text().toFloat();
	float radi = radius->text().toFloat();
	targetSprite->drawRigid(Point(x, y), radi);
}

void CircleProperties::AnchrXRedo()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawCircle();
	}
	else if (!targetSprite && templateFile != ""){
		resetTemplateValue(resetAnchrX);
	}
}

void CircleProperties::AnchrYRedo()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawCircle();
	}
	else if (!targetSprite && templateFile != ""){
		resetTemplateValue(resetAnchrY);
	}
}

void CircleProperties::RadiusRedo()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawCircle();
	}
	else if (!targetSprite && templateFile != ""){
		resetTemplateValue(resetRadius);
	}
}

void CircleProperties::showRigidChange(int index)
{
	if (!InternalOperation && targetSprite){
		targetSprite->showRigid(index);
	}
	else if (!targetSprite && templateFile != ""){
		resetTemplateValue(resetShowType);
	}
}