#include "FilterProperties.h"
#include "FontChina.h"
#include "Global.h"
#include "MyLogger.h"

FilterProperties::FilterProperties(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	m_ControlView_Type = Type_FilterProperties;
	__setLayout();
	__connect();
	addSelectedCheckBox();
	addCheckBoxConnect();
}

FilterProperties::~FilterProperties()
{
}

QVariantMap FilterProperties::getVariantMap()
{
	QVariantMap map;
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	QJsonObject properties = obj["filterProperties"].toObject();
	if (filterTypeSelectedBox->getState()){
		map.insert("filterType", filterType->currentIndex());
		map.insert("selectFilterType", true);
	}
	else{
		map.insert("filterType", filterType->currentIndex());
		map.insert("selectFilterType", false);
	}
	return map;
}

void FilterProperties::setDefaultValue()
{
	filterType->setCurrentIndex(0);
}

void FilterProperties::changeSpriteValue()
{
	InternalOperation = true;
	if (targetSprite){
		targetSprite->setHasFilterProperties(true);
		targetSprite->setFilterType(filterType->currentIndex());
	}
	InternalOperation = false;
}

void FilterProperties::setValueFromFile(QString file, bool clearTarget)
{
	InternalOperation = true;
	if (clearTarget){
		targetSprite = NULL;
	}
	QJsonObject obj = readFileGetQJsonObject(file);
	if (fileError){
		MyLogger::getInstance()->addWarning("FilterProperties::setValueFromFile readTemplateFile error, filename " + file);
		InternalOperation = false;
		return;
	}
	templateFile = file;
	QJsonObject properties = obj["filterProperties"].toObject();
	bool hasFilterProperties = obj["hasFilterProperties"].toBool();
	if (!hasFilterProperties){
		this->setVisible(false);
		setDefaultValue();
	}
	else{
		this->setVisible(true);
		filterType->setCurrentIndex(properties["filterType"].toInt());
	}
	setCheckState(selectNameVec, properties);
	InternalOperation = false;
}

void FilterProperties::__setLayout()
{
	filterTypeSelectedBox = new QSelectedCheckBox(FontChina::G2U("ÂË¾µÀàÐÍ"));
	filterType = new QComboBox();
	filterType->addItem(FontChina::G2U("ÎÞ"));
	filterType->addItem(FontChina::G2U("µþ¼Ó"));
	filterType->addItem(FontChina::G2U("ÑÕÉ«¼õµ­"));
	filterType->addItem(FontChina::G2U("¸²¸Ç"));
	QHBoxLayout* hlayout1 = new QHBoxLayout();
	hlayout1->addWidget(filterTypeSelectedBox);
	hlayout1->addWidget(filterType);
	widget->setLayout(hlayout1);
}

void FilterProperties::__connect()
{
	connect(filterType, SIGNAL(currentIndexChanged(int)), this, SLOT(filterTypeChange(int)));
}

void FilterProperties::setWidgetValue()
{
	InternalOperation = true;
	filterType->setCurrentIndex(targetSprite->getFilterType());
	InternalOperation = false;
}

void FilterProperties::addSelectedCheckBox()
{
	checkVec.push_back(filterTypeSelectedBox);
	selectNameVec << "selectFilterType";
}

void FilterProperties::resetTemplateValue(int index)
{
	QJsonObject obj = readFileGetQJsonObject(templateFile);
	if (fileError)return;
	switch (index){
	case resetFilterType:
		resetTemplateFilterType(obj);
		break;
	}
	writeToFile(templateFile, obj);
}

void FilterProperties::resetTemplateFilterType(QJsonObject& obj)
{
	QJsonObject filterProperties = obj["filterProperties"].toObject();
	int type_ = filterType->currentIndex();
	filterProperties["filterType"] = type_;
	obj["filterProperties"] = filterProperties;
}

void FilterProperties::filterTypeChange(int type_)
{
	if (!InternalOperation && targetSprite){
		targetSprite->setFilterType(type_);
	}
}