#include "ActionView.h"
#include "FontChina.h"
#include <qlayout.h>
#include <qfiledialog.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "Global.h"
#include "TemplateView.h"
#include <qmessagebox.h>
#include <QDomDocument>
#include "ImageSprite.h"
#include "MyLogger.h"
#include "MySqlLiteManager.h"
#include "ImgTransPath.h"
#include "AnimationTreeWidget.h"
#include "AnimationView.h"

ActionView::ActionView(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_ActionView;
	_setLayout();
	__connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}

void ActionView::_setLayout()
{
	QLabel* label = new QLabel(FontChina::G2U("帧动画名"));
	frameFilename = new QLabel(tr(FontChina::G2U("")));
	perFrameTime = new QLineEdit();
	perFrameTimeSelectedBox = new QSelectedCheckBox(FontChina::G2U("设置帧间隔"));
	playTypeSelectedBox = new QSelectedCheckBox(FontChina::G2U("设置播放类型"));
	playType = new QComboBox();
	playType->addItem(FontChina::G2U("无"));
	playType->addItem(FontChina::G2U("循环播放"));
	playType->addItem(FontChina::G2U("往复循环"));
	playType->addItem(FontChina::G2U("播放一次"));
	perSpriteFrame = new QComboBox();
	QLabel* label3 = new QLabel(tr(FontChina::G2U("查看单帧")));
	QHBoxLayout* hlayout = new QHBoxLayout();
	QHBoxLayout* hlayout2 = new QHBoxLayout();
	QVBoxLayout* vlayout = new QVBoxLayout();
	QHBoxLayout* hlayout4 = new QHBoxLayout();
	hlayout->addWidget(label);
	hlayout->addWidget(frameFilename);
	hlayout2->addWidget(perFrameTimeSelectedBox);
	hlayout2->addWidget(perFrameTime);
	hlayout2->addWidget(playTypeSelectedBox);
	hlayout2->addWidget(playType);
	vlayout->addLayout(hlayout);
	hlayout4->addWidget(label3);
	hlayout4->addWidget(perSpriteFrame);
	vlayout->addLayout(hlayout4);
	vlayout->addLayout(hlayout2);
	widget->setLayout(vlayout);
}

void ActionView::__connect()
{
	connect(perFrameTime, SIGNAL(editingFinished()), this, SLOT(setPerFrame()));
	connect(playType, SIGNAL(currentIndexChanged(int)), this, SLOT(playTypeChanged(int)));
	connect(perSpriteFrame, SIGNAL(currentIndexChanged(int)), this, SLOT(lookPerSpriteFrame(int)));
}

void ActionView::addSelectedCheckBox()
{
	checkVec.push_back(perFrameTimeSelectedBox);
	checkVec.push_back(playTypeSelectedBox);
	selectNameVec << "selectPerFrameTime" << "selectPlayType";
}

QVariantMap ActionView::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["actionProperties"].toObject();
	if (perFrameTimeSelectedBox->getState()){
		map.insert("perFrameTime", perFrameTime->text().toDouble());
		map.insert("selectPerFrameTime", true);
	}
	else{
		map.insert("perFrameTime", properties["perFrameTime"].toDouble());
		map.insert("selectPerFrameTime", false);
	}
	if (playTypeSelectedBox->getState()){
		map.insert("playType", playType->currentIndex());
		map.insert("selectPlayType", true);
	}
	else{
		map.insert("playType", properties["playType"].toInt());
		map.insert("selectPlayType", false);
	}
	map.insert("animationName", frameFilename->text());
	return map;
}

int ActionView::getFrameCount()
{
	return perSpriteFrame->count();
}

QStringList ActionView::transIndexToRealImageName(std::vector<int> list)
{
	QStringList stringList;
	for (size_t i = 0; i < list.size(); i++){
		int index = list.at(i);
		QString frameName = ImgTransPath::getRealImageName(index);
		stringList << frameName;
	}
	return stringList;
}

//加载xml
//QStringList ActionView::getALLAnimatePng(QString plist)
//{
//	QFile sFile(plist);
//	QStringList list;
//	if (plist == "" || !sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
//		return list;
//	}
//	QDomDocument document;
//	if (!document.setContent(&sFile)){
//		return list;
//	}
//	QDomElement root = document.documentElement();
//	QDomElement dict = root.firstChildElement();
//	QDomElement dictChild = dict.firstChildElement().nextSiblingElement();
//	QDomNodeList child_list = dictChild.childNodes();
//	int childCount = child_list.size();
//	for (int i = 0; i < childCount; i += 2){
//		QDomNode child_Node = child_list.item(i);
//		QDomElement ele = child_Node.toElement();
//		QString key = ele.text();
//		QString number = key.section("_", -1).section(".", 0, 0);
//		list << key;
//	}
//	return list;
//}

void ActionView::resetTemplateValue(int index)
{
	QFile sFile(templateFile);
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (index){
	case resetPerFrame:
		resetTemplatePerFrame(obj);
		break;
	case resetPlayType:
		resetTemplatePlayType(obj);
		break;
	}
	writeToFile(templateFile, obj);
}

void ActionView::resetTemplatePerFrame(QJsonObject& obj)
{
	QJsonObject actionProperties = obj["actionProperties"].toObject();
	double perTime = perFrameTime->text().toDouble();
	actionProperties["perFrameTime"] = perTime;
	obj["actionProperties"] = actionProperties;
}

void ActionView::resetTemplatePlayType(QJsonObject& obj)
{
	QJsonObject actionProperties = obj["actionProperties"].toObject();
	int __type = playType->currentIndex();
	actionProperties["playType"] = __type;
	obj["actionProperties"] = actionProperties;
}

void ActionView::setPerSpriteFrameList(QStringList list)
{
	InternalOperation = true;
	int count = perSpriteFrame->count();
	for (int i = 0; i < count; i++){
		perSpriteFrame->removeItem(0);
	}
	int a = 2;
	count = list.size();
	for (int i = 0; i < count; i++){
		QString str = list.at(i);
		perSpriteFrame->addItem(FontChina::G2U(str.toLocal8Bit().data()));
	}
	InternalOperation = false;
}

void ActionView::setPerFrame()
{
	double perframe = perFrameTime->text().toDouble();
	if (!InternalOperation && targetSprite){
		targetSprite->setPerTime(perframe);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetPerFrame);
	}
}

void ActionView::playTypeChanged(int index)
{
	if (!InternalOperation && targetSprite){
		targetSprite->playerAnimation(index);
	}
	else if (!InternalOperation && !targetSprite && templateFile != ""){
		resetTemplateValue(resetPlayType);
	}
}

void ActionView::lookPerSpriteFrame(int index)
{
	if (targetSprite && targetSprite->getAnimation() && !InternalOperation){
		targetSprite->pauseInPerSpriteFrame(index);
	}
}

void ActionView::setDefaultValue()
{
	frameFilename->setText("");
	perFrameTime->setText("0.5");
	playType->setCurrentIndex(0);
	QStringList list;
	setPerSpriteFrameList(list);
}

void ActionView::changeSpriteValue()
{
	InternalOperation = true;
	if (targetSprite){
		//这里不确定要不要重置动画
		targetSprite->setAnimationName(frameFilename->text().toStdString());
		targetSprite->resetAnimation();
		targetSprite->setPerTime(perFrameTime->text().toDouble());
		targetSprite->playerAnimation(playType->currentIndex());
	}
	InternalOperation = false;
}

void ActionView::setWidgetValue()
{
	InternalOperation = true;
	if (targetSprite->isHasAnimation()){
		this->setVisible(true);
		frameFilename->setText(targetSprite->getAnimationName().c_str());
		perFrameTime->setText(QString::number(targetSprite->getPerTime()));
		QStringList list = transIndexToRealImageName(targetSprite->getAnimationList());
		playType->setCurrentIndex(targetSprite->getPlayerType());
		setPerSpriteFrameList(list);
	}
	else{
		this->setVisible(false);
		setDefaultValue();
	}
	InternalOperation = false;
}

void ActionView::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("ActionView::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject actionProperties = obj["actionProperties"].toObject();
	bool hasActionProperties = obj["hasAnimation"].toBool();
	setCheckState(selectNameVec, actionProperties);

	if (!hasActionProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		perFrameTime->setText(QString::number(actionProperties["perFrameTime"].toDouble()));
		QString animationName = actionProperties["animationName"].toString();
		playType->setCurrentIndex(actionProperties["playType"].toInt());
		auto animationView = dynamic_cast<AnimationView*>(g_AnimationView);
		auto animationTree = animationView->getTreeWidget();
		auto vec = animationTree->getSinpleFrameNameListQString(animationName);
		QStringList list;
		for (size_t i = 0; i < vec.size(); i++){
			list.push_back(vec.at(i));
		}
		setPerSpriteFrameList(list);
	}
	InternalOperation = false;
}

ActionView::~ActionView()
{
}
