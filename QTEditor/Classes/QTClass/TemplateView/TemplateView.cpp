 #include "TemplateView.h"
#include "FontChina.h"
#include <qboxlayout.h>
#include <qfiledialog.h>
#include "Global.h"
#include "TemplateListItem.h"
#include "LayerManagerWidget.h"
#include <qmessagebox.h>
#include <QTextStream>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "HelloWorldScene.h"
#include <QDir>
#include "ImgTransPath.h"
#include "DragImageSprite.h"


TemplateView::TemplateView(char* title, QSize size)
:BaseDockWidget(title, size)
{
	addTemplateButton = new QPushButton(FontChina::G2U("添加模板"));
	useTemplateButton = new QPushButton(FontChina::G2U("使用模板"));
	repairButton = new QPushButton(FontChina::G2U("修复模板"));
	QVBoxLayout* hlayout = new QVBoxLayout();
	QHBoxLayout* vlayout = new QHBoxLayout();
	templateList = new TemplateListWidget();
	
	dialog = new RepairTemplateDialog;

	vlayout->addWidget(addTemplateButton);
	vlayout->addWidget(useTemplateButton);
	vlayout->addWidget(repairButton);
	hlayout->addWidget(templateList);
	hlayout->addLayout(vlayout);
	widget->setLayout(hlayout);
	QDir* dir = new QDir();
	if (!dir->exists(g_templateDir.c_str())){
		dir->mkdir(g_templateDir.c_str());
	}
	_connect();
}

TemplateView::~TemplateView()
{
}

void TemplateView::_connect()
{
	connect(addTemplateButton, SIGNAL(clicked()), this, SLOT(on_addTemplateButton_clicked()));
	connect(useTemplateButton, SIGNAL(clicked()), this, SLOT(on_useTemplateButton_clicked()));
	connect(repairButton, SIGNAL(clicked()), this, SLOT(on_repairTemplateButton_clicked()));
}

void TemplateView::deleteTemplateDir()
{
	templateList->deleteTemplateDir();
}

void TemplateView::on_addTemplateButton_clicked()
{
	QStringList list = QFileDialog::getOpenFileNames();
	if (list.size() == 0){
		return;
	}
	for (int i = 0; i < list.size(); i++){
		QString file = list.at(i);
		addTemplate(file, false);
	}
}

void TemplateView::on_useTemplateButton_clicked()
{
	TemplateListItem* item = dynamic_cast<TemplateListItem*>(templateList->currentItem());
	if (item){
		QFile sFile(item->getFile());
		if (!sFile.open(QIODevice::ReadOnly | QIODevice::Text)){ sFile.close(); return; }
		QTextStream in(&sFile);
		in.setCodec("UTF-8");
		QString content = in.readAll();
		QJsonParseError jsonError;
		QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
		if (jsonError.error == QJsonParseError::NoError && document.isObject()){
			QJsonObject obj = document.object();
			QString relaDir = obj["relaDir"].toString();
			bool userOutput = obj["userOutput"].toBool();
			LayerManagerWidget* layer = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
			if(layer->getItemCount() <= 0){
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("请先创建层"));
				sFile.close();
				return;
			}
			if (relaDir != ""){
				g_filePath = ImgTransPath::getListImagePathByRelativeImagePath(relaDir).toStdString();
				switch (obj["image_Type"].toInt())
				{
				case 0:
					g_ImageType = Image_Type::NormalBlock;
					break;
				case 1:
					g_ImageType = Image_Type::Entity;
					break;
				case 2:
					g_ImageType = Image_Type::AnimationBlock;
					g_animationName = obj["actionProperties"].toObject()["animationName"].toString().toStdString();
					break;
				}
				auto baseProperties = obj["baseProperties"].toObject();
				setDragImageData(baseProperties);
				g_TemplateFile = item->getFile().toLocal8Bit().data();
				HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
				layer->add_ImageChild();
			}
			else{
				g_TemplateFile = item->getFile().toLocal8Bit().data();
				addConsoleInfo("当前模板中没有图片不能直接创建");
			}
			
			setCocCursor(Cursor_Type::addTemplate);
		}
		sFile.close();
	}
}

void TemplateView::setDragImageData(QJsonObject& baseProperties)
{
	if (!baseProperties.contains("scaleX")){
		if (baseProperties["selectScale"].toBool()){
			DragImageSprite::t_scaleX = baseProperties["scale"].toDouble();
			DragImageSprite::t_scaleY = baseProperties["scale"].toDouble();
		}
		else{
			DragImageSprite::t_scaleX = 1.0;
			DragImageSprite::t_scaleY = 1.0;
		}
	}
	else{
		if(baseProperties["selectScaleX"].toBool()){
			DragImageSprite::t_scaleX = baseProperties["scaleX"].toDouble();
		}
		else{
			DragImageSprite::t_scaleX = 1.0;
		}
		if (baseProperties["selectScaleY"].toBool()){
			DragImageSprite::t_scaleY = baseProperties["scaleY"].toDouble();
		}
		else{
			DragImageSprite::t_scaleY = 1.0;
		}
	}
	if (baseProperties["selectAnchrx"].toBool()){
		DragImageSprite::t_anchr.x = baseProperties["anchrx"].toDouble();
	}
	else{
		DragImageSprite::t_anchr.x = 0.5;
	}
	if (baseProperties["selectAnchry"].toBool()){
		DragImageSprite::t_anchr.y = baseProperties["anchry"].toDouble();
	}
	else{
		DragImageSprite::t_anchr.y = 0.5;
	}
	if (baseProperties["selectFlipx"].toBool()){
		DragImageSprite::t_flipX = baseProperties["flipx"].toBool();
	}
	else{
		DragImageSprite::t_flipX = false;
	}
	if (baseProperties["selectFlipy"].toBool()){
		DragImageSprite::t_flipY = baseProperties["flipy"].toBool();
	}
	else{
		DragImageSprite::t_flipY = false;
	}
	if (baseProperties["selectRotation"].toBool()){
		DragImageSprite::t_rotation = baseProperties["rotation"].toDouble( );
	}
	else{
		DragImageSprite::t_rotation = 0;
	}
}

void TemplateView::on_repairTemplateButton_clicked()
{
	dialog->show();
}

QStringList TemplateView::getAllTemplateFile()
{
	QStringList list;
	for (int i = 0; i < templateList->count(); i++){
		TemplateListItem* item = dynamic_cast<TemplateListItem*>(templateList->item(i));
		QString str = item->getFile();
		list.push_back(str);
	}
	return list;
}

bool TemplateView::addTemplate(QString file, bool isOpen)
{
	QString filename = file.section("/", -1);
	int size = templateList->count();
	for (int i = 0; i < size; i++){
		auto item = templateList->item(i);
		if (item){
			if (item->text() == filename){
				QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("模板名称重复请修改"));
				return false;
			}
		}
	}
	templateList->addTemplate(file, isOpen);
	return true;
}

void TemplateView::removeAllTemplate()
{
	int size = templateList->count();
	for (int i = 0; i < size; i++){
		auto item = dynamic_cast<TemplateListItem*>(templateList->takeItem(0));
		delete item;
	}
}

void TemplateView::itemAddSprite(ImageSprite* sprite)
{
	templateList->itemAddSprite(sprite);
}

void TemplateView::itemDeleteSprite(ImageSprite* sprite)
{
	templateList->itemDeleteSprite(sprite);
}

TemplateListItem* TemplateView::getItem(int index)
{
	return dynamic_cast<TemplateListItem*>(templateList->item(index));
}

QString TemplateView::getNewTemplateName()
{
	return templateList->getNewTemplateName();
}