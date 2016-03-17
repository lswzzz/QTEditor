#include "TemplateListWidget.h"
#include "Global.h"
#include <qmenu.h>
#include "FontChina.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include "ControlView.h"
#include "HelloWorldScene.h"
#include "LayerManagerWidget.h"
#include "convertMapToJsonFile.h"
#include "QDir"
#include "GlobalConstant.h"

TemplateListWidget::TemplateListWidget(QWidget *parent)
:QListWidget(parent)
{
	_connect();
}

TemplateListWidget::~TemplateListWidget()
{

}

void TemplateListWidget::_connect()
{
	connect(this, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
	connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
	connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemDoubleClicked(QListWidgetItem *)));
	connect(this, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(itemChanged(QListWidgetItem *)));
}

void TemplateListWidget::deleteTemplateDir()
{
	DeleteDirectory(g_c_relaTemplateDir, false);
}

void TemplateListWidget::currentItemChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
}

void TemplateListWidget::itemClicked(QListWidgetItem * item)
{
	changeTemplate();
	savaName = item->text();
}

void TemplateListWidget::itemDoubleClicked(QListWidgetItem * item)
{
}

void TemplateListWidget::itemChanged(QListWidgetItem * item)
{
	int size = this->count();
	for (int i = 0; i < size; i++){
		auto item__ = this->item(i);
		if (item__){
			if (item != item__ && item__->text() == item->text()){
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("名称重复!请修改名称"));
				item->setText(savaName);
				return;
			}
		}
	}
	TemplateListItem* temItem = dynamic_cast<TemplateListItem*>(item);
	QString dir = temItem->getFile().section("/", 0, -2);
	QString oldFile = temItem->getFile();
	QFile file(oldFile);
	QString newFile = dir + "/" + item->text();
	temItem->setFile(newFile);
	QFile file2(newFile);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text) && file2.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		QString content = in.readAll();
		QTextStream out(&file2);
		out << content;
		file.close();
		QFile::remove(oldFile);
		temItem->updateAllSpriteTemplateNameAndMd5();
	}
	file2.close();
	convertMapToJsonFile::changeFileToUtf8(newFile);
}

void TemplateListWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* menu = new QMenu(this);
	QAction* action = new QAction(FontChina::G2U("删除"), this);
	QAction* action2 = new QAction(FontChina::G2U("同步所有精灵的值"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(action2, SIGNAL(triggered()), this, SLOT(updateAllSpriteValue()));
	menu->addAction(action);
	menu->addAction(action2);
	QListWidgetItem* item = this->currentItem();
	if (item){
		menu->exec(QCursor::pos());
	}
}

void TemplateListWidget::addTemplate(QString file, bool isOpen)
{
	QString filename = file.section("/", -1);
	QFile sFile(file);
	if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		addConsoleInfo("文件打开错误");
		return;
	}
	QTextStream in(&sFile);
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toLatin1(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		if (obj["type"].toString() == "sprite"){
			QString md5 = obj["md5"].toString();
			TemplateListItem* item = new TemplateListItem();
			item->setText(filename);
			item->setIcon(QIcon(":/icon/Resources/sprite.ico"));
			item->setType("sprite");
			item->setFile(file);
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			item->setMD5(md5);
			this->addItem(item);
			if (!isOpen){
				g_template_count += 1;
			}
		}
		else{
			addConsoleInfo("文件内容错误");
		}
	}
	else{
		addConsoleInfo("文件内容错误");
	}
	sFile.close();
}

void TemplateListWidget::itemAddSprite(ImageSprite* sprite)
{
	QString spriteMD5 = sprite->getTemplateMD5().c_str();
	//QString spriteTemplate = sprite->getTemplateFile().c_str();
	for (int i = 0; i < this->count(); i++){
		auto item = dynamic_cast<TemplateListItem*>(this->item(i));
		//auto templateFile = item->getFile();
		if (item->getMD5() == spriteMD5){
			item->addNode(sprite);
			break;
		}
	}
}

void TemplateListWidget::itemDeleteSprite(ImageSprite* sprite)
{
	//QString spriteTemplate = sprite->getTemplateFile().c_str();
	QString spriteMD5 = sprite->getTemplateMD5().c_str();
	if (sprite->getTemplateFile() == "")return;
	for (int i = 0; i < this->count(); i++){
		auto item = dynamic_cast<TemplateListItem*>(this->item(i));
		//auto templateFile = item->getFile();
		// || spriteTemplate == templateFile
		if (item->getMD5() == spriteMD5){
			item->removeNode(sprite);
			break;
		}
	}
}

//template_
QString TemplateListWidget::getNewTemplateName()
{
	int size = this->count();
	int index = 0;
	for (int i = 0; i < size; i++){
		auto item = this->item(i);
		auto str = item->text().mid(9);
		if (str.toInt() >= index){
			index = str.toInt() + 1;
		}
	}
	return "template_" + QString::number(index);
}

void TemplateListWidget::checkCurrentLayerUseTemplateSprite()
{
	TemplateListItem* item = dynamic_cast<TemplateListItem*>(currentItem());
	if (item){
		LayerManagerWidget* layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
		layerManager->SelectedAllUseTemplate(item->getFile());
	}
}

void TemplateListWidget::updateAllSpriteValue()
{
	TemplateListItem* item = dynamic_cast<TemplateListItem*>(currentItem());
	if (item){
		item->updateAllSpriteValue();
	}
}

void TemplateListWidget::deleteItem()
{
	TemplateListItem* item = dynamic_cast<TemplateListItem*>(this->currentItem());
	if (item){
		QString str = item->getFile();
		QFile sFile(str);
		if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
			delete item;
			sFile.close();
			return;
		}
		item->updateAllSpriteTemplateNameAndMd5();
		sFile.remove();
		delete item;
		sFile.close();
	}
}

void TemplateListWidget::changeTemplate()
{
	TemplateListItem* item = dynamic_cast<TemplateListItem*>(this->currentItem());
	QString file = item->getFile();
	QString filename = file.section("/", -1);
	QFile sFile(file);
	if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QTextStream in(&sFile);
	in.setCodec("UTF-8");
	QString content = in.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		ControlView* controlView = dynamic_cast<ControlView*>(g_ControlView);
		controlView->setValueFromTemplateFile(file);
	}
	sFile.close();
}