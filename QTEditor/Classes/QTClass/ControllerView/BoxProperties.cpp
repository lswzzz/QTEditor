#include "BoxProperties.h"
#include "QLayout"
#include <QLabel>
#include "FontChina.h"
#include <QWheelEvent>
#include "MyLogger.h"

BoxProperties::BoxProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_BoxProperties;
	_setLayout();
	_connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}


BoxProperties::~BoxProperties()
{
}

void BoxProperties::_setLayout()
{
	QVBoxLayout* vlayout = new QVBoxLayout();
	QGridLayout* gridlayout = new QGridLayout();
	anchorXSelectedBox = new QSelectedCheckBox(FontChina::G2U("锚点X"));
	anchorYSelectedBox = new QSelectedCheckBox(FontChina::G2U("锚点Y"));
	halfWidthSelectedBox = new QSelectedCheckBox(FontChina::G2U("半宽"));
	halfHeightSelectedBox = new QSelectedCheckBox(FontChina::G2U("半高"));
	waySelectedBox = new QSelectedCheckBox(FontChina::G2U("方向"));
	showRigidSelectedBox = new QSelectedCheckBox(FontChina::G2U("显示刚体"));
	//leftExtrusionSelectedBox = new QSelectedCheckBox(FontChina::G2U("左挤压"));
	//rightExtrusionSelectedBox = new QSelectedCheckBox(FontChina::G2U("右挤压"));
	//upExtrusionSelectedBox = new QSelectedCheckBox(FontChina::G2U("上挤压"));
	//downExtrusionSelectedBox = new QSelectedCheckBox(FontChina::G2U("下挤压"));
	AnchorX = new QLineEdit();
	AnchorY = new QLineEdit();
	halfWidth = new QLineEdit();
	halfHeight = new QLineEdit();
	showRigid = new QComboBox();
	showRigid->addItem(FontChina::G2U("显示"));
	showRigid->addItem(FontChina::G2U("隐藏"));
	way = new QComboBox();
	way->addItem(FontChina::G2U("无"));
	way->addItem(FontChina::G2U("上"));
	way->addItem(FontChina::G2U("下"));
	way->addItem(FontChina::G2U("左"));
	way->addItem(FontChina::G2U("右"));
	//leftExtrusion = new QCheckBox();
	//rightExtrusion = new QCheckBox();
	//upExtrusion = new QCheckBox();
	//downExtrusion = new QCheckBox();
	gridlayout->addWidget(anchorXSelectedBox, 1, 0);
	gridlayout->addWidget(AnchorX, 1, 1);
	gridlayout->addWidget(anchorYSelectedBox, 1, 2);
	gridlayout->addWidget(AnchorY, 1, 3);
	gridlayout->addWidget(halfWidthSelectedBox, 2, 0);
	gridlayout->addWidget(halfWidth, 2, 1);
	gridlayout->addWidget(halfHeightSelectedBox, 2, 2);
	gridlayout->addWidget(halfHeight, 2, 3);
	gridlayout->addWidget(waySelectedBox, 3, 0);
	gridlayout->addWidget(way, 3, 1);
	gridlayout->addWidget(showRigidSelectedBox, 3, 2);
	gridlayout->addWidget(showRigid, 3, 3);
	/*QHBoxLayout* hlayout = new QHBoxLayout;
	hlayout->addWidget(leftExtrusion);
	hlayout->addWidget(leftExtrusionSelectedBox);
	hlayout->addWidget(rightExtrusion);
	hlayout->addWidget(rightExtrusionSelectedBox);
	hlayout->addWidget(upExtrusion);
	hlayout->addWidget(upExtrusionSelectedBox);
	hlayout->addWidget(downExtrusion);
	hlayout->addWidget(downExtrusionSelectedBox);
	gridlayout->addLayout(hlayout, 4, 0, 1, 4);*/
	vlayout->addLayout(gridlayout);
	widget->setLayout(vlayout);

}

void BoxProperties::addSelectedCheckBox()
{
	checkVec.push_back(anchorXSelectedBox);
	checkVec.push_back(anchorYSelectedBox);
	checkVec.push_back(halfWidthSelectedBox);
	checkVec.push_back(halfHeightSelectedBox);
	checkVec.push_back(waySelectedBox);
	checkVec.push_back(showRigidSelectedBox);
	selectNameVec << "selectAnchrx" << "selectAnchry" << "selectHalfWidth" << "selectHalfHeight" << "selectWay" << "selectShowRigid";
		//<< "selectLeftExtrusion" << "selectLeftExtrusion" << "selectLeftExtrusion" << "selectLeftExtrusion";
}

void BoxProperties::targetSpriteDrawRect()
{
	float y = AnchorY->text().toFloat();
	float x = AnchorX->text().toFloat();
	float width = halfWidth->text().toFloat();
	float height = halfHeight->text().toFloat();
	targetSprite->drawRigid(Point(x, y), Size(width, height), way->currentIndex());
}

void BoxProperties::_connect()
{
	connect(AnchorX, SIGNAL(editingFinished()), this, SLOT(AnchorXEditFinished()));
	connect(AnchorY, SIGNAL(editingFinished()), this, SLOT(AnchorYEditFinished()));
	connect(halfWidth, SIGNAL(editingFinished()), this, SLOT(halfWidthEditFinished()));
	connect(halfHeight, SIGNAL(editingFinished()), this, SLOT(halfHeightEditFinished()));
	connect(showRigid, SIGNAL(currentIndexChanged(int)), this, SLOT(showOrHideRigid(int)));
	connect(way, SIGNAL(currentIndexChanged(int)), this, SLOT(setRigidWay(int)));
	/*connect(leftExtrusion, SIGNAL(stateChanged(int)), this, SLOT(leftExtrusionSelected(int)));
	connect(rightExtrusion, SIGNAL(stateChanged(int)), this, SLOT(rightExtrusionSelected(int)));
	connect(upExtrusion, SIGNAL(stateChanged(int)), this, SLOT(upExtrusionSelected(int)));
	connect(downExtrusion, SIGNAL(stateChanged(int)), this, SLOT(downExtrusionSelected(int)));*/
}

void BoxProperties::setDefaultValue()
{
	AnchorX->setText("");
	AnchorY->setText("");
	halfWidth->setText("");
	halfHeight->setText("");
	way->setCurrentIndex(0);
	showRigid->setCurrentIndex(0);
}

void BoxProperties::changeSpriteValue()
{
	InternalOperation = true;
	if (targetSprite && this->isVisible() && targetSprite->isHasBoxProperties()){
		targetSpriteDrawRect();
		targetSprite->showRigid(showRigid->currentIndex());
	}
	InternalOperation = false;
}

void BoxProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("BoxProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["boxProperties"].toObject();

	setCheckState(selectNameVec, properties);

	bool hasBoxProperties = obj["hasBoxProperties"].toBool();
	if (targetSprite){
		targetSprite->setHasBoxProperties(hasBoxProperties);
	}
	if (!hasBoxProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		AnchorX->setText(QString::number(properties["anchrx"].toDouble()));
		AnchorY->setText(QString::number(properties["anchry"].toDouble()));
		halfWidth->setText(QString::number(properties["halfWidth"].toDouble()));
		halfHeight->setText(QString::number(properties["halfHeight"].toDouble()));
		way->setCurrentIndex(properties["way"].toInt());
		showRigid->setCurrentIndex(properties["showRigid"].toInt());
	}
	InternalOperation = false;
}

void BoxProperties::setWidgetValue()
{
	InternalOperation = true;
	if (targetSprite->isHasBoxProperties()){
		this->setVisible(true);
		AnchorX->setText(QString::number(targetSprite->getBoxAnchrPos().x));
		AnchorY->setText(QString::number(targetSprite->getBoxAnchrPos().y));
		halfWidth->setText(QString::number(targetSprite->getHalf().width));
		halfHeight->setText(QString::number(targetSprite->getHalf().height));
		way->setCurrentIndex(targetSprite->getBoxWay());
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
	InternalOperation = false;
}

void BoxProperties::resetTemplateValue(int index)
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
	case resetHalfW:
		resetTemplateHalfW(obj);
		break;
	case resetHalfH:
		resetTemplateHalfH(obj);
		break;
	case resetWay:
		resetTemplateWay(obj);
		break;
	case resetShow:
		resetTemplateShow(obj);
		break;
	}
	writeToFile(templateFile, obj);
}

void BoxProperties::resetTemplateAnchrX(QJsonObject& obj)
{
	QJsonObject boxProperties = obj["boxProperties"].toObject();
	double anchrx = AnchorX->text().toDouble();
	boxProperties["anchrx"] = anchrx;
	obj["boxProperties"] = boxProperties;
}

void BoxProperties::resetTemplateAnchrY(QJsonObject& obj)
{
	QJsonObject boxProperties = obj["boxProperties"].toObject();
	double anchry = AnchorY->text().toDouble();
	boxProperties["anchry"] = anchry;
	obj["boxProperties"] = boxProperties;
}

void BoxProperties::resetTemplateHalfW(QJsonObject& obj)
{
	QJsonObject boxProperties = obj["boxProperties"].toObject();
	double halfW = halfWidth->text().toDouble();
	boxProperties["halfWidth"] = halfW;
	obj["boxProperties"] = boxProperties;
}

void BoxProperties::resetTemplateHalfH(QJsonObject& obj)
{
	QJsonObject boxProperties = obj["boxProperties"].toObject();
	double halfH = halfHeight->text().toDouble();
	boxProperties["halfHeight"] = halfH;
	obj["boxProperties"] = boxProperties;
}

void BoxProperties::resetTemplateWay(QJsonObject& obj)
{
	QJsonObject boxProperties = obj["boxProperties"].toObject();
	int __way = way->currentIndex();
	boxProperties["way"] = __way;
	obj["boxProperties"] = boxProperties;
}

void BoxProperties::resetTemplateShow(QJsonObject& obj)
{
	QJsonObject boxProperties = obj["boxProperties"].toObject();
	int show = showRigid->currentIndex();
	boxProperties["showRigid"] = show;
	obj["boxProperties"] = boxProperties;
}
//
//void BoxProperties::resetTemplateleftExtrusion(QJsonObject& obj)
//{
//
//}
//
//void BoxProperties::resetTemplaterightExtrusion(QJsonObject& obj)
//{
//
//}
//
//void BoxProperties::resetTemplatedownExtrusion(QJsonObject& obj)
//{
//
//}
//
//void BoxProperties::resetTemplateupExtrusion(QJsonObject& obj)
//{
//
//}

void BoxProperties::wheelEvent(QWheelEvent *e)
{
	InternalOperation = true;
	if (AnchorX->hasFocus() && targetSprite){
		float x = AnchorX->text().toDouble();
		if (e->delta() > 0)//向上滚
		{
			x -= 0.1f;
		}
		if (e->delta() < 0)//向下滚
		{
			x += 0.1f;
		}
		AnchorX->setText(QString::number(x));
		targetSpriteDrawRect();
	}
	else if (AnchorY->hasFocus() && targetSprite){
		float y = AnchorY->text().toDouble();
		if (e->delta() > 0)//向上滚
		{
			y -= 0.1f;
		}
		if (e->delta() < 0)//向下滚
		{
			y += 0.1f;
		}
		AnchorY->setText(QString::number(y));
		targetSpriteDrawRect();
	}
	else if (halfWidth->hasFocus() && targetSprite){
		float width = halfWidth->text().toDouble();
		if (e->delta() > 0)//向上滚
		{
			width -= 10;
		}
		if (e->delta() < 0)//向下滚
		{
			width += 10;
		}
		halfWidth->setText(QString::number(width));
		targetSpriteDrawRect();
	}
	else if (halfHeight->hasFocus() && targetSprite){
		float height = halfHeight->text().toDouble();
		if (e->delta() > 0)//向上滚
		{
			height -= 10;
		}
		if (e->delta() < 0)//向下滚
		{
			height += 10;
		}
		halfHeight->setText(QString::number(height));
		targetSpriteDrawRect();
	}

	InternalOperation = false;
}

void BoxProperties::AnchorXEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawRect();
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetAnchrX);
	}
}

void BoxProperties::AnchorYEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawRect();
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetAnchrY);
	}
}

void BoxProperties::halfWidthEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawRect();
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetHalfW);
	}
}

void BoxProperties::halfHeightEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawRect();
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetHalfH);
	}
}

void BoxProperties::showOrHideRigid(int index)
{
	if (!InternalOperation && targetSprite){
		targetSprite->showRigid(index);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetShow);
	}
	
}

void BoxProperties::setRigidWay(int index)
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawRect();
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetWay);
	}
}
//
//void BoxProperties::leftExtrusionSelected(int state)
//{
//
//}
//
//void BoxProperties::rightExtrusionSelected(int state)
//{
//
//}
//
//void BoxProperties::downExtrusionSelected(int state)
//{
//
//}
//
//void BoxProperties::upExtrusionSelected(int state)
//{
//
//}

QVariantMap BoxProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["boxProperties"].toObject();

	if (anchorXSelectedBox->getState()){
		map.insert("anchrx", AnchorX->text().toDouble());
		map.insert("selectAnchrx", true);
	}
	else{
		map.insert("anchrx", properties["anchrx"].toDouble());
		map.insert("selectAnchrx", false);
	}

	if (anchorYSelectedBox->getState()){
		map.insert("anchry", AnchorY->text().toDouble());
		map.insert("selectAnchry", true);
	}
	else{
		map.insert("anchry", properties["anchry"].toDouble());
		map.insert("selectAnchry", false);
	}

	if (halfWidthSelectedBox->getState()){
		map.insert("halfWidth", halfWidth->text().toDouble());
		map.insert("selectHalfWidth", true);
	}
	else{
		map.insert("halfWidth", properties["halfWidth"].toDouble());
		map.insert("selectHalfWidth", false);
	}

	if (halfHeightSelectedBox->getState()){
		map.insert("halfHeight", halfHeight->text().toDouble());
		map.insert("selectHalfHeight", true);
	}
	else{
		map.insert("halfHeight", properties["halfHeight"].toDouble());
		map.insert("selectHalfHeight", false);
	}

	if (showRigidSelectedBox->getState()){
		map.insert("showRigid", showRigid->currentIndex());
		map.insert("selectShowRigid", true);
	}
	else{
		map.insert("showRigid", properties["showRigid"].toInt());
		map.insert("selectShowRigid", false);
	}

	if (waySelectedBox->getState()){
		map.insert("way", way->currentIndex());
		map.insert("selectWay", true);
	}
	else{
		map.insert("way", properties["way"].toInt());
		map.insert("selectWay", false);
	}
	return map;
}
