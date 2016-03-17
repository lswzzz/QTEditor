#pragma once
#include "BaseDockWidget.h"
#include "TemplateListWidget.h"
#include <qtreewidget.h>
#include <qpushbutton.h>
#include "repairtemplatedialog.h"

class TemplateView : public BaseDockWidget
{
	Q_OBJECT
public:
	TemplateView(char* title, QSize size);
	~TemplateView();
	//第二个参数指定是否当前实在打开配置文件的时候加入的
	bool addTemplate(QString file, bool isOpenConf = false);
	QStringList getAllTemplateFile();
	void removeAllTemplate();
	void itemAddSprite(ImageSprite* sprite);
	void itemDeleteSprite(ImageSprite* sprite);
	TemplateListItem* getItem(int index);
	QString getNewTemplateName();
	void deleteTemplateDir();
private slots:
	void on_addTemplateButton_clicked();
	void on_useTemplateButton_clicked();
	void on_repairTemplateButton_clicked();
protected:
	void _connect();
	void setDragImageData(QJsonObject& pro);
public:
	QPushButton* addTemplateButton;
	QPushButton* useTemplateButton;
	QPushButton* repairButton;
	TemplateListWidget* templateList;
	RepairTemplateDialog* dialog;
};

