#include "repairtemplatedialog.h"
#include "ui_repairtemplatedialog.h"
#include "Global/Global.h"
#include "ControlView.h"
#include "HelloWorldScene.h"
#include "TemplateView.h"
#include "EntityImageSprite.h"
#include "FontChina.h"
#include "TemplateValueGenerator.h"
#include "ImgTransPath.h"

RepairTemplateDialog::RepairTemplateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTemplateDialog)
{
    ui->setupUi(this);
}

RepairTemplateDialog::~RepairTemplateDialog()
{
    delete ui;
}

void RepairTemplateDialog::on_cancelButton_clicked()
{
	this->setVisible(false);
}

void RepairTemplateDialog::on_sureButton_clicked()
{
	auto tagname = ui->tagName->toPlainText();
	auto id = ui->IdName->toPlainText().toInt();
	auto templateName = ui->templteName->toPlainText();
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	if (tagname != ""){
		auto sprite = scene->getForeManager()->getImageChildFromTag(tagname.toStdString());
		if (sprite){
			createTemplate(sprite);
			this->setVisible(false);
			return;
		}
	}
	if (id != 0){
		auto sprite = scene->getForeManager()->getImageChildFromId(id);
		if (sprite){
			createTemplate(sprite);
			this->setVisible(false);
			return;
		}
	}
	this->setVisible(false);
}

void RepairTemplateDialog::addSpritesToTemplate(QString templateName)
{
	auto scene = dynamic_cast<HelloWorld*>(g_scene);
	auto layerVec = scene->getForeManager()->getALLImageLayer();
	TemplateView* templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	for (int i = 0; i < layerVec.size(); i++){
		auto layer = layerVec.at(i);
		auto spriteVec = layer->getAllSpriteNode();
		for (int k = 0; k < spriteVec.size(); k++){
			auto sprite = dynamic_cast<ImageSprite*>(spriteVec.at(k));
			QString templateN = sprite->getTemplateFile().c_str();
			if (templateN != "" && templateN == templateName){
				templateView->itemAddSprite(sprite);
			}
		}
	}
}

void RepairTemplateDialog::createTemplate(ImageSprite* sprite)
{
	TemplateView* templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	QString md5 = sprite->getTemplateMD5().c_str();
	QString tempalteName = sprite->getTemplateFile().c_str();
	auto list = templateView->getAllTemplateFile();
	for (int i = 0; i < list.size(); i++){
		auto templateN = list.at(i);
		if (templateN.section("/", -1) == tempalteName.section("/", -1)){
			addConsoleInfo("修复失败，模板已经存在!");
		}
	}
	QVariantMap map;
	map.insert("relaDir", ImgTransPath::getRelativePathByIndex(sprite->getImageIndex()));
	map.insert("imageIndex", sprite->getImageIndex());
	map.insert("type", "sprite");
	map.insert("md5", md5);
	map.insert("userOutput", false);
	addPropertiesToMap(map, sprite);
	switch (sprite->getType()){
	case SpriteType::NORMAL_BLOCK:
		map.insert("image_Type", 0);
		break;
	case SpriteType::DYNAMIC_BLOCK:
		if (sprite->isHasAnimation()){
			map.insert("image_Type", 2);
		}
		else{
			map.insert("image_Type", 0);
		}
		break;
	case SpriteType::MONSTER:
		map.insert("image_Type", 1);
		break;
	case SpriteType::ROLE:
		map.insert("image_Type", 1);
		break;
	}
	QJsonDocument jsonDocument = QJsonDocument::fromVariant(map);
	QString file = g_templateDir.c_str();
	file += "/" + tempalteName.section("/", -1);
	QFile sFile(file);
	if (!sFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		addConsoleInfo("文件异常");
		return;
	}
	QTextStream out(&sFile);
	out << jsonDocument.toJson();
	sFile.close();
	templateView->addTemplate(file);
	addSpritesToTemplate(file);
}

void RepairTemplateDialog::addPropertiesToMap(QVariantMap& map, ImageSprite* sprite)
{
	map.insert("hasBaseProperties", true);
	QVariantMap base = createBaseProperties(sprite);
	map.insert("baseProperties", base);

	if (sprite->isHasBoxProperties()){
		map.insert("hasBoxProperties", true);
		map.insert("hasCircleProperties", false);
		QVariantMap box = createBoxProperties(sprite);
		map.insert("boxProperties", box);
	}
	else if (sprite->isHasCircleProperties()){
		map.insert("hasCircleProperties", true);
		map.insert("hasBoxProperties", false);
		QVariantMap circle = createCircleProperties(sprite);
		map.insert("circleProperties", circle);
	}
	else{
		map.insert("hasBoxProperties", false);
		map.insert("hasCircleProperties", false);
	}

	map.insert("hasEntityProperties", sprite->isHasEntityProperties());
	if (sprite->isHasEntityProperties()){
		QVariantMap entity = createEntityProperties(sprite);
		map.insert("entityProperties", entity);
	}

	map.insert("hasMonsterProperties", sprite->isHasMonsterProperties());
	if (sprite->isHasMonsterProperties()){
		QVariantMap monster = createMonsterProperties(sprite);
		map.insert("monsterProperties", monster);
	}
	
	map.insert("hasMonsterDropProperties", sprite->isHasMonsterDropProperties());
	if (sprite->isHasMonsterDropProperties()){
		QVariantMap monsterDrop = createMonsterDropProperties(sprite);
		map.insert("monsterDropProperties", monsterDrop);
	}
	
	map.insert("hasAIProperties", sprite->isHasAIProperties());
	if (sprite->isHasAIProperties()){
		QVariantMap ai = createAiProperties(sprite);
		map.insert("aiProperties", ai);
	}
	
	map.insert("hasSimpleTriggerProperties", sprite->isHasSimplerTrigger());
	if (sprite->isHasSimplerTrigger()){
		QVariantMap simplerTrigger__ = createSimplerTriggerProperties(sprite);
		map.insert("simpleTriggerProperties", simplerTrigger__);
	}
	
	map.insert("hasAnimation", sprite->isHasAnimation());
	if (sprite->isHasAnimation()){
		QVariantMap action = createAnimationProperties(sprite);
		map.insert("actionProperties", action);
	}

}

QVariantMap RepairTemplateDialog::createBaseProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setBasePropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createBoxProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setBoxPropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createMoveProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setMovePropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createCircleProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setCirclePropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createAnimationProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setAnimationPropertiesFromSprite(sprite, true, false);
}

QVariantMap RepairTemplateDialog::createEntityProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setEntityPropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createMonsterProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setMonsterPropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createMonsterDropProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setMonsterDropPropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createAiProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setAIPropertiesFromSprite(sprite, true);
}

QVariantMap RepairTemplateDialog::createSimplerTriggerProperties(ImageSprite* sprite)
{
	return TemplateValueGenerator::setSimplerTriggerPropertiesFromSprite(sprite, true);
}