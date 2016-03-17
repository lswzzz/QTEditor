#include "ControlView.h"
#include "Global.h"
#include "FontChina.h"
#include "TemplateView.h"
#include "ImageSprite.h"
#include "SpriteTreeItem.h"
#include  "LayerManagerWidget.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include "EntityImageSprite.h"
#include "MyLogger.h"
#include "ImgTransPath.h"
#include "convertMapToJsonFile.h"
#include "ResourceView.h"
#include "ExteralView.h"
#include "AnimationView.h"

ControlView::ControlView(char* title, QSize size)
:BaseDockWidget(title, size)
{
	show_type = uiSprite;
	targetLayer = NULL;
	targetSprite = NULL;
	targetListPngIndex = -1;
	templateFile = "";
	templateMd5 = "";
	scrollArea = new QScrollArea();
	scrollArea->setWidgetResizable(true);
	scrollArea->setAlignment(Qt::AlignTop);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	saveButton = new QPushButton(FontChina::G2U("保存为新模板"));
	modifyButton = new QPushButton(FontChina::G2U("修改模板"));
	resetButton = new QPushButton(FontChina::G2U("重置为模板值"));
	allSelectedButton = new QPushButton(FontChina::G2U("全选"));
	locationButton = new QPushButton(FontChina::G2U("定位"));
	contentSizeLabel = new QLabel();
	templateLabel = new QLabel();
	targetLabel = new QLabel();
	targetPngLabel = new QLabel();
	this->setWidget(scrollArea);
	scrollArea->setStyleSheet("background-color: rgb(191, 191, 191);");
	addDock();
	__connect();
}

ControlView::~ControlView()
{
}

QScrollArea* ControlView::getScrollArea()
{
	return scrollArea;
}

void ControlView::__connect()
{
	connect(saveButton, SIGNAL(clicked()), this, SLOT(on_saveButton_clicked()));
	connect(modifyButton, SIGNAL(clicked()), this, SLOT(on_modifyButton_clicked()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(on_resetButton_clicked()));
	connect(allSelectedButton, SIGNAL(clicked()), this, SLOT(on_allSelectedButton_clicked()));
	connect(locationButton, SIGNAL(clicked()), this, SLOT(on_locationButton_clicked()));
}

void ControlView::setTargetSprite(ImageSprite* sprite)
{
	if (!sprite){
		MyLogger::getInstance()->addInfo("the ControlView::setTargetSprite be called, the sprite tag is null");
	}
	else{
		MyLogger::getInstance()->addInfo("the ControlView::setTargetSprite be called, the sprite tag is " + sprite->getTagName());
	}
	
	targetSprite = sprite;
	if (targetSprite){
		targetListPngIndex = ImgTransPath::getListPngName(targetSprite).section(".", 0, -2).toInt();
		auto contentSize = targetSprite->getContentSize();
		contentSizeLabel->setText(QString::number(contentSize.width) + " * " + QString::number(contentSize.height));
	}
	else{
		targetListPngIndex = -1;
		targetSprite = NULL;
		contentSizeLabel->setText("");
	}
	
	setTargetPng(targetListPngIndex);
}

void ControlView::setTargetLayer(ImageSpriteLayer* layer)
{
	targetLayer = layer;
}

QString ControlView::getTemplateFile()
{
	return templateFile;
}

//当在场景中创建精灵使用模板或点击一个精灵的时候调用
void ControlView::setValueFromChildSprite(ImageSprite*sprite)
{
	clearProperties();
	setTargetName(sprite->getTagName().c_str());
	setTargetSprite(sprite);
	actionView->setTargetSprite(sprite);
	baseProperties->setTargetSprite(sprite);
	moveProperties->setTargetSprite(sprite);
	boxProperties->setTargetSprite(sprite);
	circleProperties->setTargetSprite(sprite);
	entityProperties->setTargetSprite(sprite);
	simplerTrigger->setTargetSprite(sprite);
	aiProperties->setTargetSprite(sprite);
	filterProperties->setTargetSprite(sprite);
	if (sprite->getType() == SpriteType::MONSTER){
		auto entitySprite = dynamic_cast<EntityImageSprite*>(sprite);
		monsterProperties->setTargetSprite(entitySprite);
		monsterDropProperties->setTargetSprite(entitySprite);
	}
	setSelectedButton_state(true);
	setUIViewShow(uiSprite);
	setTemplateFile(sprite->getTemplateFile().c_str(), sprite->getTemplateMD5().c_str());
	MyLogger::getInstance()->addInfo("ControlView::setValueFromChildSprite be called, the user create Sprite By Template or show some sprite properties");
}

void ControlView::setValueFromLayer(ImageSpriteLayer* layer)
{
	MyLogger::getInstance()->addInfo("the ControlView::setValueFromLayer be called");
	clearProperties();
	setTargetName(layer->getTagName().c_str());
	setTargetLayer(layer);
	setUIViewShow(uiLayer);
	layerBaseProperties->setTargetLayer(layer);
	setTemplateFile("", "");
}

void ControlView::setUIViewShow(showType type__)
{
	show_type = type__;
	switch (type__){
	case uiSprite:
		targetPngLabel->setVisible(true);
		templateLabel->setVisible(true);
		saveButton->setVisible(true);
		contentSizeLabel->setVisible(true);
		modifyButton->setVisible(true);
		resetButton->setVisible(true);
		allSelectedButton->setVisible(true);
		baseProperties->setVisible(true);
		targetSprite->isHasMoveProperties() ? moveProperties->setVisible(true) : moveProperties->setVisible(false);
		if (targetSprite->isHasBoxProperties()){
			boxProperties->setVisible(true);
			circleProperties->setVisible(false);
		}
		else if(targetSprite->isHasCircleProperties()){
			boxProperties->setVisible(false);
			circleProperties->setVisible(true);
		}
		else{
			boxProperties->setVisible(false);
			circleProperties->setVisible(false);
		}
		targetSprite->isHasAnimation() ? actionView->setVisible(true) : actionView->setVisible(false);
		targetSprite->isHasEntityProperties() ? entityProperties->setVisible(true) : entityProperties->setVisible(false);
		targetSprite->isHasMonsterProperties() ? monsterProperties->setVisible(true) : monsterProperties->setVisible(false);
		targetSprite->isHasMonsterDropProperties() ? monsterDropProperties->setVisible(true) : monsterDropProperties->setVisible(false);
		targetSprite->isHasAIProperties() ? aiProperties->setVisible(true) : aiProperties->setVisible(false);
		targetSprite->isHasSimplerTrigger() ? simplerTrigger->setVisible(true) : simplerTrigger->setVisible(false);
		targetSprite->isHasFilterProperties() ? filterProperties->setVisible(true) : filterProperties->setVisible(false);
		layerBaseProperties->setVisible(false);
		break;
	case uiLayer:
		targetPngLabel->setVisible(false);
		templateLabel->setVisible(false);
		contentSizeLabel->setVisible(false);
		saveButton->setVisible(false);
		modifyButton->setVisible(false);
		resetButton->setVisible(false);
		allSelectedButton->setVisible(false);
		baseProperties->setVisible(false);
		moveProperties->setVisible(false);
		boxProperties->setVisible(false);
		actionView->setVisible(false);
		entityProperties->setVisible(false);
		monsterProperties->setVisible(false);
		monsterDropProperties->setVisible(false);
		aiProperties->setVisible(false);
		simplerTrigger->setVisible(false);
		filterProperties->setVisible(false);
		layerBaseProperties->setVisible(true);
		break;
	}
}

void ControlView::setValueFromTemplateFile(QString file)
{
	this->setTargetName("");
	this->setTargetPng(-1);
	this->setTargetSprite(NULL);
	QString md5;
	QFile sFile(file);
	if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text)){return;}
	QTextStream in(&sFile);
	in.setCodec("UTF-8");
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		md5 = obj["md5"].toString();
	}
	sFile.close();
	this->setTemplateFile(file, md5);
	this->getLayerBaseProperties()->setVisible(false);
	this->getActionView()->setValueFromFile(file, true);
	this->getBaseProperties()->setValueFromFile(file, true);
	this->getMoveProperties()->setValueFromFile(file, true);
	this->getBoxProperties()->setValueFromFile(file, true);
	this->getEntityProperties()->setValueFromFile(file, true);
	this->getCircleProperties()->setValueFromFile(file, true);
	this->getMonsterProperties()->setValueFromFile(file, true);
	this->getMonsterDropProperties()->setValueFromFile(file, true);
	this->getAIProperties()->setValueFromFile(file, true);
	this->getSimplerTriggerProperties()->setValueFromFile(file, true);
	this->getFilterProperties()->setValueFromFile(file, true);
	MyLogger::getInstance()->addInfo("ControlView::setValueFromTemplateFile be called, user show template properties");
}

void ControlView::setInitState()
{
	baseProperties->setVisible(true);
	actionView->setVisible(false);
	boxProperties->setVisible(false);
	moveProperties->setVisible(false);
	entityProperties->setVisible(false);
	circleProperties->setVisible(false);
	//这里后来添加的
	monsterProperties->setVisible(false);
	monsterDropProperties->setVisible(false);
	aiProperties->setVisible(false);
	simplerTrigger->setVisible(false);
	filterProperties->setVisible(false);
}

void ControlView::setValueForSpriteFromTemplateFile()
{
	actionView->setValueFromFile(templateFile, false);
	baseProperties->setValueFromFile(templateFile, false);
	moveProperties->setValueFromFile(templateFile, false);
	boxProperties->setValueFromFile(templateFile, false);
	circleProperties->setValueFromFile(templateFile, false);
	entityProperties->setValueFromFile(templateFile, false);
	monsterProperties->setValueFromFile(templateFile, false);
	monsterDropProperties->setValueFromFile(templateFile, false);
	aiProperties->setValueFromFile(templateFile, false);
	simplerTrigger->setValueFromFile(templateFile, false);
	filterProperties->setValueFromFile(templateFile, false);
	if (targetSprite){
		actionView->changeSpriteValue();
		baseProperties->changeSpriteValue();
		moveProperties->changeSpriteValue();
		boxProperties->changeSpriteValue();
		entityProperties->changeSpriteValue();
		circleProperties->changeSpriteValue();
		filterProperties->changeSpriteValue();
		if (targetSprite->getType() == SpriteType::MONSTER){
			monsterProperties->changeSpriteValue();
			monsterDropProperties->changeSpriteValue();
		}
		if (targetSprite->getType() == SpriteType::MONSTER || targetSprite->getType() == SpriteType::DYNAMIC_BLOCK || targetSprite->getType() == SpriteType::ROLE){
			aiProperties->changeSpriteValue();
		}
		if (targetSprite->getType() == SpriteType::MONSTER || targetSprite->getType() == SpriteType::ROLE || targetSprite->getType() == SpriteType::DYNAMIC_BLOCK){
			simplerTrigger->changeSpriteValue();
		}
		targetSprite->setTemplateFile(templateFile.toStdString(), templateMd5.toStdString());
	}
	MyLogger::getInstance()->addInfo("ControlView::setValueForSpriteFromTemplateFile, user reset Sprite properties from template file");
}

void ControlView::clearProperties()
{
	this->setTargetName("");
	setTargetSprite(NULL);
	setTemplateFile("", "");
	this->getActionView()->clearTarget();
	this->getBaseProperties()->clearTarget();
	this->getMoveProperties()->clearTarget();
	this->getCircleProperties()->clearTarget();
	this->getBoxProperties()->clearTarget();
	this->getEntityProperties()->clearTarget();
	this->getMonsterProperties()->clearTarget();
	this->getMonsterDropProperties()->clearTarget();
	this->getAIProperties()->clearTarget();
	this->getSimplerTriggerProperties()->clearTarget();
	this->getFilterProperties()->clearTarget();
	MyLogger::getInstance()->addInfo(" the ControlView::clearProperties be called");
}

bool ControlView::addDockWidget(QDockWidget* widget)
{	
	vec.push_back(widget);
	QWidget* widget__ = new QWidget();
	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(new QLabel(FontChina::G2U("当前目标")), 0, 0, 1, 1);
	targetLabel->setText("");
	targetPngLabel->setText("");
	gridLayout->addWidget(targetLabel, 0, 1, 1, 2);
	gridLayout->addWidget(new QLabel(FontChina::G2U("图片")), 1, 0, 1, 1);
	gridLayout->addWidget(targetPngLabel, 1, 1, 1, 1);
	gridLayout->addWidget(locationButton, 1, 2, 1, 1);
	templateLabel->setText("");
	contentSizeLabel->setText("");
	gridLayout->addWidget(new QLabel(FontChina::G2U("模板名称")), 2, 0, 1, 1);
	gridLayout->addWidget(templateLabel, 2, 1, 1, 2);
	gridLayout->addWidget(new QLabel(FontChina::G2U("尺寸")), 3, 0, 1, 1);
	gridLayout->addWidget(contentSizeLabel, 3, 1, 1, 2);
	QGridLayout* gridLayout2 = new QGridLayout();
	gridLayout2->addWidget(modifyButton, 0, 0, 1, 2);
	gridLayout2->addWidget(saveButton, 0, 2, 1, 2);
	gridLayout2->addWidget(resetButton, 0, 4, 1, 2);
	gridLayout2->addWidget(allSelectedButton, 0, 6, 1, 1);
	gridLayout->addLayout(gridLayout2, 4, 0, 1, 3);
	gridLayout->setRowStretch(0, 1);
	gridLayout->setRowStretch(1, 1);
	gridLayout->setRowStretch(2, 1);
	gridLayout->setRowStretch(3, 1);
	gridLayout->setRowStretch(4, 1);
	for (int i = 0; i < vec.size(); i++){
		QDockWidget* dockWidget = vec.at(i);
		gridLayout->addWidget(dockWidget, i+5, 0, 1, 3);
		gridLayout->setRowStretch(i + 5, 1);
	}
	gridLayout->addWidget(new QLabel, vec.size()+10 , 0, 1, 3);
	gridLayout->setRowStretch(vec.size() + 10, 20);
	widget__->setLayout(gridLayout);
	scrollArea->setWidget(widget__);
	return true;
}

void ControlView::addDock()
{
	addLayerBaseView();
	addBaseView();
	addMoveView();
	addBoxView();
	addCircleView();
	addActionView();
	addFilterView();
	addEntityView();
	addMonsterView();
	addMonsterDropView();
	addTriggerView();
	addAIView();
	addSimpleTriggerView();
	
	moveProperties->setVisible(false);
	boxProperties->setVisible(false);
	circleProperties->setVisible(false);
	actionView->setVisible(false);
	entityProperties->setVisible(false);
	layerBaseProperties->setVisible(false);
	monsterProperties->setVisible(false);
	monsterDropProperties->setVisible(false);
	triggerProperties->setVisible(false);
	aiProperties->setVisible(false);
	simplerTrigger->setVisible(false);
	filterProperties->setVisible(false);
}

void ControlView::setTargetName(QString str)
{
	targetLabel->setText(str);
}

//label的显示和targetPNG不同
void ControlView::setTargetPng(int index)
{
	if (index >= 0){
		targetPngLabel->setText(ImgTransPath::getRealImageName(index));
	}
	else{
		targetPngLabel->setText("");
	}
	
}

void ControlView::on_resetButton_clicked()
{
	if (templateFile != "" && templateFile != NULL){
		setValueForSpriteFromTemplateFile();
	}
	else{
		QMessageBox::information(this, FontChina::G2U("错误"), FontChina::G2U("没有模板"));
		return;
	}
}

void ControlView::on_allSelectedButton_clicked()
{
	if (targetSprite == nullptr && templateFile == ""){
		QMessageBox::information(this, FontChina::G2U("错误"), FontChina::G2U("没有模板或精灵"));
		return;
	}
	static bool select = false;
	setSelectedButton_state(select);
	select = !select;
}

void ControlView::on_locationButton_clicked()
{
	if (targetSprite){
		if (targetSprite->getType() == SpriteType::MONSTER || targetSprite->getType() == SpriteType::ROLE){
			auto exteralView = static_cast<ExteralView*>(g_exteralView);
			exteralView->setCurItem(targetListPngIndex);
		}
		else if (targetSprite->isHasAnimation()){
			auto animationView = dynamic_cast<AnimationView*>(g_AnimationView);
			animationView->setCurAnimation(targetSprite->getAnimationName());
		}else{
			auto resourceView = static_cast<ResourceView*>(g_resourceView);
			resourceView->setCurItem(targetListPngIndex);
		}
	}
}

void ControlView::setSelectedButton_state(bool state)
{
	actionView->setSelectedBoxState(state);
	baseProperties->setSelectedBoxState(state);
	moveProperties->setSelectedBoxState(state);
	boxProperties->setSelectedBoxState(state);
	entityProperties->setSelectedBoxState(state);
	monsterProperties->setSelectedBoxState(state);
	monsterDropProperties->setSelectedBoxState(state);
	circleProperties->setSelectedBoxState(state);
	aiProperties->setSelectedBoxState(state);
	simplerTrigger->setSelectedBoxState(state);
	filterProperties->setSelectedBoxState(state);
	MyLogger::getInstance()->addInfo("the ControlView::setSelectedButton_state be called");
}

void ControlView::on_modifyButton_clicked()
{
	if (templateFile != NULL && templateFile != ""){
		TemplateView* templateView = dynamic_cast<TemplateView*>(g_TemplateView);
		templateView->show();
		templateView->raise();
		QFile sFile(templateFile);
		if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
			MyLogger::getInstance()->addWarning("ControlView::on_modifyButton_clicked be called, current template File " + templateFile + "lost");
			sFile.close();
			return;
		}
		QTextStream in(&sFile);
		QString content = in.readAll();
		sFile.close();
		QJsonParseError jsonError;
		QString md5;
		QJsonDocument document = QJsonDocument::fromJson(content.toLatin1(), &jsonError);
		if (jsonError.error == QJsonParseError::NoError && document.isObject()){
			QJsonObject obj = document.object();
			md5 = obj["md5"].toString();
		}
		MyLogger::getInstance()->addInfo("the ControlView::on_modifyButton_clicked be called");
		QVariantMap properties;
		QVariantMap base = baseProperties->getVariantMap();
		QVariantMap move = moveProperties->getVariantMap();
		QVariantMap box = boxProperties->getVariantMap();
		QVariantMap circle = circleProperties->getVariantMap();
		QVariantMap action = actionView->getVariantMap();
		QVariantMap entity = entityProperties->getVariantMap();
		QVariantMap monster = monsterProperties->getVariantMap();
		QVariantMap monsterDrop = monsterDropProperties->getVariantMap();
		QVariantMap ai = aiProperties->getVariantMap();
		QVariantMap simplerTrigger__ = simplerTrigger->getVariantMap();
		QVariantMap filter = filterProperties->getVariantMap();
		properties.insert("md5", md5);
		int imageIndex = targetListPngIndex;
		properties.insert("relaDir", FontChina::G2U(ImgTransPath::getRelativePathByIndex(imageIndex).toLocal8Bit()));
		properties.insert("imageIndex", imageIndex);
		addValueToMap(properties);
		properties.insert("type", "sprite");
		properties.insert("baseProperties", base);
		if (moveProperties->isVisible()){
			properties.insert("moveProperties", move);
		}

		if (boxProperties->isVisible()){
			properties.insert("boxProperties", box);
		}
		else if (circleProperties->isVisible()){
			properties.insert("circleProperties", circle);
		}

		if (actionView->isVisible()){
			properties.insert("actionProperties", action);
		}

		if (entityProperties->isVisible()){
			properties.insert("entityProperties", entity);
		}
		if (monsterProperties->isVisible()){
			properties.insert("monsterProperties", monster);
		}
		if (monsterDropProperties->isVisible()){
			properties.insert("monsterDropProperties", monsterDrop); 
		}
		if (aiProperties->isVisible()){
			properties.insert("aiProperties", ai);
		}
		if (simplerTrigger->isVisible()){
			properties.insert("simpleTriggerProperties", simplerTrigger__);
		}
		if (filterProperties->isVisible()){
			properties.insert("filterProperties", filter);
		}
		if (targetSprite){
			switch (targetSprite->getType()){
			case SpriteType::NORMAL_BLOCK:
				properties.insert("image_Type", 0);
				break;
			case SpriteType::DYNAMIC_BLOCK:
				if (targetSprite->isHasAnimation()){
					properties.insert("image_Type", 2);
				}
				else{
					properties.insert("image_Type", 0);
				}
				
				break;
			case SpriteType::MONSTER:
				properties.insert("image_Type", 1);
				break;
			case SpriteType::ROLE:
				properties.insert("image_Type", 1);
				break;
			}
		}
		QJsonDocument jsonDocument = QJsonDocument::fromVariant(properties);
		QString file = templateFile;
		QFile dFile(file);
		if (!dFile.open(QIODevice::WriteOnly | QIODevice::Text)){
			MyLogger::getInstance()->addError("ControlView::on_modifyButton_clicked be called error, the template file " + templateFile + "can't save");
			addConsoleInfo("文件错误,保存失败");
			return;
		}
		QTextStream out(&dFile);
		out << jsonDocument.toJson();
		dFile.close();
		convertMapToJsonFile::changeFileToUtf8(file);
		addConsoleInfo("修改成功");
	}
	else{
		QMessageBox::information(this, FontChina::G2U("错误"), FontChina::G2U("当前没有模板"));
		return;
	}
}

void ControlView::setTemplateFile(QString str, QString md5)
{
	templateFile = str;
	templateMd5 = md5;
	templateLabel->setText(templateFile.section("/", -1));
}

void ControlView::on_saveButton_clicked()
{
	MyLogger::getInstance()->addInfo(" the ControlView::on_saveButton_clicked be called");
	if (targetSprite == nullptr){
		QMessageBox::information(this, FontChina::G2U("错误"), FontChina::G2U("当前没有精灵"));
		return;
	}
	TemplateView* templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	templateView->show();
	templateView->raise();
	QVariantMap properties;
	QVariantMap base = baseProperties->getVariantMap();
	QVariantMap move = moveProperties->getVariantMap();
	QVariantMap box = boxProperties->getVariantMap();
	QVariantMap circle = circleProperties->getVariantMap();
	QVariantMap action = actionView->getVariantMap();
	QVariantMap entity = entityProperties->getVariantMap();
	QVariantMap monster = monsterProperties->getVariantMap();
	QVariantMap monsterDrop = monsterDropProperties->getVariantMap();
	QVariantMap ai = aiProperties->getVariantMap();
	QVariantMap simplerTrigger__ = simplerTrigger->getVariantMap();
	QVariantMap filter = filterProperties->getVariantMap();
	int imageIndex = targetListPngIndex;
	properties.insert("relaDir", FontChina::G2U(ImgTransPath::getRelativePathByIndex(imageIndex).toLocal8Bit()));
	properties.insert("imageIndex", imageIndex);
	addValueToMap(properties);
	QString md5 = GenerateMD5();
	properties.insert("type", "sprite");
	properties.insert("md5", md5);
	properties.insert("baseProperties", base);
	properties.insert("userOutput", false);
	if (moveProperties->isVisible()){
		properties.insert("moveProperties", move);
	}

	if (boxProperties->isVisible()){
		properties.insert("boxProperties", box);
	}
	else if (circleProperties->isVisible()){
		properties.insert("circleProperties", circle);
	}

	if (actionView->isVisible()){
		properties.insert("actionProperties", action);
	}
	if (entityProperties->isVisible()){
		properties.insert("entityProperties", entity);
	}
	if (monsterProperties->isVisible()){
		properties.insert("monsterProperties", monster);
	}
	if (monsterDropProperties->isVisible()){
		properties.insert("monsterDropProperties", monsterDrop);
	}
	if (aiProperties->isVisible()){
		properties.insert("aiProperties", ai);
	}
	if (simplerTrigger->isVisible()){
		properties.insert("simpleTriggerProperties", simplerTrigger__);
	}
	if (filterProperties->isVisible()){
		properties.insert("filterProperties", filter);
	}
	if (targetSprite){
		switch (targetSprite->getType()){
		case SpriteType::NORMAL_BLOCK:
			properties.insert("image_Type", 0);
			break;
		case SpriteType::DYNAMIC_BLOCK:
			if (targetSprite->isHasAnimation()){
				properties.insert("image_Type", 2);
			}
			else{
				properties.insert("image_Type", 0);
			}
			
			break;
		case SpriteType::MONSTER:
			properties.insert("image_Type", 1);
			break;
		case SpriteType::ROLE:
			properties.insert("image_Type", 1);
			break;
		}
	}
	QJsonDocument jsonDocument = QJsonDocument::fromVariant(properties);
	QString file = g_templateDir.c_str();
	file += "/" + templateView->getNewTemplateName();
	QFile sFile(file);
	if (!sFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		MyLogger::getInstance()->addError("ControlView::on_saveButton_clicked be called error, the template file " + file + "can't write, user try to save a template");
		addConsoleInfo("文件错误");
		return;
	}
	QTextStream out(&sFile);
	out << jsonDocument.toJson();
	sFile.close();
	convertMapToJsonFile::changeFileToUtf8(file);
	if (templateView->addTemplate(file) && targetSprite){
		templateView->itemDeleteSprite(targetSprite);
		targetSprite->setTemplateFile(file.toLocal8Bit().data(), md5.toStdString());
		LayerManagerWidget* layermanager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
		auto item = layermanager->getCurrentSpriteTreeItem();
		templateView->itemAddSprite(targetSprite);
		if (item){
			item->setIcon(1, QIcon(":/icon/Resources/template.ico"));
		}
	}
}

void ControlView::addValueToMap(QVariantMap& properties)
{
	properties.insert("hasBaseProperties", true);
	moveProperties->isVisible() ? properties.insert("hasMoveProperties", true) : properties.insert("hasMoveProperties", false);
	if (boxProperties->isVisible()){
		properties.insert("hasBoxProperties", true);
		properties.insert("hasCircleProperties", false);
	}
	else if (circleProperties->isVisible()){
		properties.insert("hasBoxProperties", false);
		properties.insert("hasCircleProperties", true);
	}
	else{
		properties.insert("hasBoxProperties", false);
		properties.insert("hasCircleProperties", false);
	}
	if (actionView->isVisible()){
		if (actionView->getFrameCount() > 0){
			properties.insert("hasAnimation", true);
		}
		else{
			properties.insert("hasAnimation", false);
		}
	}
	else{
		properties.insert("hasAnimation", false);
	}
	entityProperties->isVisible() ? properties.insert("hasEntityProperties", true) : properties.insert("hasEntityProperties", false);
	monsterProperties->isVisible() ? properties.insert("hasMonsterProperties", true) : properties.insert("hasMonsterProperties", false);
	monsterDropProperties->isVisible() ? properties.insert("hasMonsterDropProperties", true) : properties.insert("hasMonsterDropProperties", false);
	aiProperties->isVisible() ? properties.insert("hasAIProperties", true) : properties.insert("hasAIProperties", false);
	simplerTrigger->isVisible() ? properties.insert("hasSimpleTriggerProperties", true) : properties.insert("hasSimpleTriggerProperties", false);
	filterProperties->isVisible() ? properties.insert("hasFilterProperties", true) : properties.insert("hasFilterProperties", false);
}

void ControlView::addActionView()
{
	actionView = new ActionView("序列帧属性", QSize(320, 100));
	this->addDockWidget(actionView);
}

void ControlView::addBaseView()
{
	baseProperties = new BaseProperties("基础属性", QSize(320, 140));
	this->addDockWidget(baseProperties);
}

void ControlView::addBoxView()
{
	boxProperties = new BoxProperties("刚体属性", QSize(320, 120));
	this->addDockWidget(boxProperties);
}

void ControlView::addMoveView()
{
	moveProperties = new MoveProperties("移动属性", QSize(320, 130));
	this->addDockWidget(moveProperties);
}

void ControlView::addEntityView()
{
	entityProperties = new EntityProperties("实体属性", QSize(320, 100));
	this->addDockWidget(entityProperties);
}

void ControlView::addLayerBaseView()
{
	layerBaseProperties = new LayerBaseProperties("层属性", QSize(320, 150));
	this->addDockWidget(layerBaseProperties);
}

void ControlView::addMonsterView()
{
	monsterProperties = new MonsterProperties("怪物属性", QSize(320, 150));
	this->addDockWidget(monsterProperties);
}

void ControlView::addMonsterDropView()
{
	monsterDropProperties = new MonsterDropProperties("怪物掉落属性", QSize(320, 60));
	this->addDockWidget(monsterDropProperties);
}

void ControlView::addCircleView()
{
	circleProperties = new CircleProperties("圆形刚体属性", QSize(320, 100));
	this->addDockWidget(circleProperties);
}

void ControlView::addTriggerView()
{
	triggerProperties = new TriggerView("触发器属性", QSize(320, 250));
	this->addDockWidget(triggerProperties);
}

void ControlView::addSimpleTriggerView()
{
	simplerTrigger = new SimpleTriggerView("触发器属性", QSize(320, 60));
	this->addDockWidget(simplerTrigger);
}

void ControlView::addAIView()
{
	aiProperties = new AIProperties("AI属性", QSize(320, 110));
	this->addDockWidget(aiProperties);
}

void ControlView::addFilterView()
{
	filterProperties = new FilterProperties("过滤器属性", QSize(320, 60));
	this->addDockWidget(filterProperties);
}

ActionView* ControlView::getActionView()
{
	return actionView;
}

BaseProperties* ControlView::getBaseProperties()
{
	return baseProperties;
}

MoveProperties* ControlView::getMoveProperties()
{
	return moveProperties;
}

BoxProperties* ControlView::getBoxProperties()
{
	return boxProperties;
}

EntityProperties* ControlView::getEntityProperties()
{
	return entityProperties;
}

LayerBaseProperties* ControlView::getLayerBaseProperties()
{
	return layerBaseProperties;
}

MonsterProperties* ControlView::getMonsterProperties()
{
	return monsterProperties;
}

MonsterDropProperties* ControlView::getMonsterDropProperties()
{
	return monsterDropProperties;
}

CircleProperties* ControlView::getCircleProperties()
{
	return circleProperties;
}

TriggerView* ControlView::getTriggerProperties()
{
	return triggerProperties;
}

AIProperties* ControlView::getAIProperties()
{
	return aiProperties;
}

SimpleTriggerView* ControlView::getSimplerTriggerProperties()
{
	return simplerTrigger;
}

FilterProperties* ControlView::getFilterProperties()
{
	return filterProperties;
}

QString ControlView::GenerateMD5()
{
	time_t timep;
	time(&timep);
	struct tm *tm = localtime(&timep);
	char currentTime[256];
	memset(currentTime, 0, 256);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int sec = tm->tm_sec;
	sprintf(currentTime, "%d-%d-%d-%d-%d-%d", year, month, day, hour, min, sec);
	int number = 2047483648;
	int randomValue = qrand() % number;
	QString md5String = QString::number(randomValue);
	md5String += currentTime;
	QByteArray hashData = QCryptographicHash::hash(md5String.toLocal8Bit(), QCryptographicHash::Md5);
	QString md5;
	md5.append(hashData.toHex());
	return md5;
}

bool ControlView::targetPropertiesIsSprite()
{
	if (show_type == uiSprite){
		return true;
	}
	return false;
}