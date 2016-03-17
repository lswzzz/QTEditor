#include "MoveProperties.h"
#include "Global.h"
#include "QLabel"
#include "FontChina.h"
#include <QLayout>
#include "MyLogger.h"

MoveProperties::MoveProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_MoveProperties;
	_setLayout();
	_connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}

void MoveProperties::_setLayout()
{
	QVBoxLayout* vlayout = new QVBoxLayout();
	QGridLayout* gridlayout = new QGridLayout();
	translatXSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("水平位移")));
	translatYSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("垂直位移")));
	runTimeSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("运行时间")));
	delayTimeSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("延迟时间")));
	runTypeSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("移动预览")));
	backDelayTimeSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("返回延迟")));
	startRunTimeSelectedBox = new QSelectedCheckBox(tr(FontChina::G2U("初始运行时间")));
	translatXEdit = new QLineEdit();
	translatYEdit = new QLineEdit();
	runTime = new QLineEdit();
	runType = new QComboBox();
	delayTime = new QLineEdit();
	backDelayTime = new QLineEdit();
	startRunTime = new QLineEdit();
	runType->addItem(FontChina::G2U("停止"));
	runType->addItem(FontChina::G2U("暂停"));
	runType->addItem(FontChina::G2U("移动"));
	gridlayout->addWidget(translatXSelectedBox, 1, 0);
	gridlayout->addWidget(translatXEdit, 1, 1);
	gridlayout->addWidget(translatYSelectedBox, 1, 2);
	gridlayout->addWidget(translatYEdit, 1, 3);
	gridlayout->addWidget(runTimeSelectedBox, 2, 0);
	gridlayout->addWidget(runTime, 2, 1);
	gridlayout->addWidget(delayTimeSelectedBox, 2, 2);
	gridlayout->addWidget(delayTime, 2, 3);
	gridlayout->addWidget(backDelayTimeSelectedBox, 3, 0);
	gridlayout->addWidget(backDelayTime, 3, 1);
	gridlayout->addWidget(startRunTimeSelectedBox, 3, 2);
	gridlayout->addWidget(startRunTime, 3, 3);
	gridlayout->addWidget(runTypeSelectedBox, 4, 0);
	gridlayout->addWidget(runType, 4, 1);
	vlayout->addLayout(gridlayout);
	widget->setLayout(vlayout);
}

MoveProperties::~MoveProperties()
{
}

void MoveProperties::addSelectedCheckBox()
{
	checkVec.push_back(translatXSelectedBox);
	checkVec.push_back(translatYSelectedBox);
	checkVec.push_back(runTimeSelectedBox);
	checkVec.push_back(delayTimeSelectedBox);
	checkVec.push_back(backDelayTimeSelectedBox);
	checkVec.push_back(startRunTimeSelectedBox);
	checkVec.push_back(runTypeSelectedBox);
	selectNameVec << "selectTranslatex" << "selectTranslatey" << "selectRunTime" << "selectDelay" << 
		"selectBackDelay" << "selectStartRunTime" << "selectRunType";
}

void MoveProperties::_connect()
{
	connect(runTime, SIGNAL(editingFinished()), this, SLOT(runTimeEditFinished()));
	connect(translatXEdit, SIGNAL(editingFinished()), this, SLOT(translatXEditFinished()));
	connect(translatYEdit, SIGNAL(editingFinished()), this, SLOT(translatYEditFinished()));
	connect(runType, SIGNAL(currentIndexChanged(int)), this, SLOT(SpriterunAction(int)));
	connect(delayTime, SIGNAL(editingFinished()), this, SLOT(delayTimeEditFinished()));
	connect(backDelayTime, SIGNAL(editingFinished()), this, SLOT(backDelayTimeEditFinished()));
	connect(startRunTime, SIGNAL(editingFinished()), this, SLOT(startRunTImeEditFinished()));
}

void MoveProperties::wheelEvent(QWheelEvent *e)
{
	InternalOperation = true;
	if (translatXEdit->hasFocus() && targetSprite){
		float x = translatXEdit->text().toDouble();
		if (e->delta() > 0)//向上滚
		{
			x -= 10;
		}
		if (e->delta() < 0)//向下滚
		{
			x += 10;
		}
		translatXEdit->setText(QString::number(x));
		//targetSpriteDrawLine();
	}
	else if (translatYEdit->hasFocus() && targetSprite){
		float y = translatYEdit->text().toDouble();
		if (e->delta() > 0)//向上滚
		{
			y -= 10;
		}
		if (e->delta() < 0)//向下滚
		{
			y += 10;
		}
		translatYEdit->setText(QString::number(y));
		//targetSpriteDrawLine();
	}
	InternalOperation = false;
}

void MoveProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QFile sFile(file);
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("MoveProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["moveProperties"].toObject();

	setCheckState(selectNameVec, properties);

	bool hasMoveProperties = obj["hasMoveProperties"].toBool();
	if (targetSprite){
		targetSprite->setHasMoveProperties(hasMoveProperties);
	}
	if (!hasMoveProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		translatXEdit->setText(QString::number(properties["translatex"].toDouble()));
		translatYEdit->setText(QString::number(properties["translatey"].toDouble()));
		runTime->setText(QString::number(properties["runTime"].toDouble()));
		runType->setCurrentIndex(properties["runType"].toInt());
		delayTime->setText(QString::number(properties["delay"].toDouble()));
		if (!properties.contains("backDelay")){
			backDelayTime->setText(QString::number(properties["delay"].toDouble()));
		}
		else{
			backDelayTime->setText(QString::number(properties["backDelay"].toDouble()));
		}
		
		startRunTime->setText(QString::number(properties["startRunTime"].toDouble()));
	}
	InternalOperation = false;
}

void MoveProperties::setDefaultValue()
{
	translatYEdit->setText("");
	translatXEdit->setText("");
	runTime->setText("");
	delayTime->setText("");
	backDelayTime->setText("");
	startRunTime->setText("");
	runType->setCurrentText(0);
}

void MoveProperties::changeSpriteValue()
{
	if (targetSprite && this->isVisible()&& targetSprite->isHasMoveProperties()){
		targetSpriteDrawLine();
		SpriterunAction(runType->currentIndex());
	}
}

void MoveProperties::setWidgetValue()
{
	InternalOperation = true;
	if (targetSprite->isHasMoveProperties()){
		this->setVisible(true);
		auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
		translatXEdit->setText(QString::number(targetSprite->getTranslatDis().x));
		translatYEdit->setText(QString::number(targetSprite->getTranslatDis().y));
		runTime->setText(QString::number(targetSprite->getMoveTime()));
		runType->setCurrentIndex(targetSprite->getMoveType());
		delayTime->setText(QString::number(targetSprite->getMoveDelayTime()));
		backDelayTime->setText(QString::number(targetSprite->getMoveBackDelayTime()));
		startRunTime->setText(QString::number(targetSprite->getStartRunTime()));
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
	InternalOperation = false;
}

void MoveProperties::translatXEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawLine();
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetTranslateX);
	}
}

void MoveProperties::translatYEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSpriteDrawLine();
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetTranslateY);
	}
}

void MoveProperties::delayTimeEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSprite->setMoveDelayTime(delayTime->text().toDouble());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetDelayTime);
	}
}

void MoveProperties::backDelayTimeEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSprite->setMoveBackDelayTime(backDelayTime->text().toDouble());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetBackDelayTime);
	}
}

void MoveProperties::startRunTImeEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSprite->setStartRunTime(startRunTime->text().toDouble());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetStartRunTime);
	}
}

void MoveProperties::targetSpriteDrawLine()
{
	float x = translatXEdit->text().toDouble();
	float y = translatYEdit->text().toDouble();
	targetSprite->translatLine(Point(x, y));
}

void MoveProperties::resetTemplateValue(int index)
{
	InternalOperation = true;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (index){
	case resetTranslateX:
		resetTemplateTranslateX(obj);
		break;
	case resetTranslateY:
		resetTemplateTranslateY(obj);
		break;
	case resetRunTime:
		resetTemplateRunTime(obj);
		break;
	case resetDelayTime:
		resetTemplateDelayTime(obj);
		break;
	case resetRunType:
		resetTemplateRunType(obj);
		break;
	case resetBackDelayTime:
		resetTemplateBackDelayTime(obj);
		break;
	case resetStartRunTime:
		resetTemplateStartRunTime(obj);
		break;
	}
	writeToFile(templateFile, obj);
	InternalOperation = false;
}

void MoveProperties::resetTemplateTranslateX(QJsonObject& obj)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	double translatex = translatXEdit->text().toDouble();
	moveProperties["translatex"] = translatex;
	obj["moveProperties"] = moveProperties;
}

void MoveProperties::resetTemplateTranslateY(QJsonObject& obj)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	double translatey = translatYEdit->text().toDouble();
	moveProperties["translatey"] = translatey;
	obj["moveProperties"] = moveProperties;
}

void MoveProperties::resetTemplateRunTime(QJsonObject& obj)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	double time = runTime->text().toDouble();
	moveProperties["runTime"] = time;
	obj["moveProperties"] = moveProperties;
}

void MoveProperties::resetTemplateDelayTime(QJsonObject& obj)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	double delay = delayTime->text().toDouble();
	moveProperties["delay"] = delay;
	obj["moveProperties"] = moveProperties;
}

void MoveProperties::resetTemplateRunType(QJsonObject& obj)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	double __type = runType->currentIndex();
	moveProperties["runType"] = __type;
	obj["moveProperties"] = moveProperties;
}

void MoveProperties::resetTemplateBackDelayTime(QJsonObject& obj)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	double backDelay = backDelayTime->text().toDouble();
	moveProperties["backDelay"] = backDelay;
	obj["moveProperties"] = moveProperties;
}

void MoveProperties::resetTemplateStartRunTime(QJsonObject& obj)
{
	QJsonObject moveProperties = obj["moveProperties"].toObject();
	double __startRunTime = startRunTime->text().toDouble();
	moveProperties["startRunTime"] = __startRunTime;
	obj["moveProperties"] = moveProperties;
}

void MoveProperties::runTimeEditFinished()
{
	if (!InternalOperation && targetSprite){
		targetSprite->setMoveTime(runTime->text().toDouble());
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetRunTime);
	}
}

void MoveProperties::SpriterunAction(int index)
{
	if (targetSprite){
		targetSpriteDrawLine();
		switch (index){
		case 0:
			targetSprite->stopMove();
			break;
		case 1:
			targetSprite->pauseMove();
			break;
		case 2:
			if (runTime->text().toDouble() > 0.0f){
				targetSprite->runMove(runTime->text().toDouble(), delayTime->text().toDouble(), backDelayTime->text().toDouble());
			}
			break;
		}
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetRunType);
	}
}

QVariantMap MoveProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["moveProperties"].toObject();
	if (translatXSelectedBox->getState()){
		map.insert("translatex", translatXEdit->text().toDouble());
		map.insert("selectTranslatex", true);
	}
	else{
		map.insert("translatex", properties["translatex"].toDouble());
		map.insert("selectTranslatex", false);
	}
	
	if (translatYSelectedBox->getState()){
		map.insert("translatey", translatYEdit->text().toDouble());
		map.insert("selectTranslatey", true);
	}
	else{
		map.insert("translatey", properties["translatey"].toDouble());
		map.insert("selectTranslatey", false);
	}

	if (runTimeSelectedBox->getState()){
		map.insert("runTime", runTime->text().toDouble());
		map.insert("selectRunTime", true);
	}
	else{
		map.insert("runTime", properties["runTime"].toDouble());
		map.insert("selectRunTime", false);
	}
	
	if (runTypeSelectedBox->getState()){
		map.insert("runType", runType->currentIndex());
		map.insert("selectRunType", true);
	}
	else{
		map.insert("runType", properties["runType"].toInt());
		map.insert("selectRunType", false);
	}
	
	if (delayTimeSelectedBox->getState()){
		map.insert("delay", delayTime->text().toDouble());
		map.insert("selectDelay", true);
	}
	else{
		map.insert("delay", properties["delay"].toDouble());
		map.insert("selectDelay", false);
	}
	
	if (backDelayTimeSelectedBox->getState()){
		map.insert("backDelay", backDelayTime->text().toDouble());
		map.insert("selectBackDelay", true);
	}
	else{
		if (!properties.contains("backDelay")){
			map.insert("backDelay", delayTime->text().toDouble());
		}
		else{
			map.insert("backDelay", properties["backDelay"].toDouble());
		}
		
		map.insert("selectBackDelay", false);
	}

	if (startRunTimeSelectedBox->getState()){
		map.insert("startRunTime", startRunTime->text().toDouble());
		map.insert("selectStartRunTime", true);
	}
	else{
		map.insert("startRunTime", properties["startRunTime"].toDouble());
		map.insert("selectStartRunTime", false);
	}

	return map;
}
