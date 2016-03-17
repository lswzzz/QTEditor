#pragma once
#include <qlistwidget.h>
#include "TemplateListItem.h"
#include "ImageSprite.h"

class TemplateListWidget : public QListWidget
{
	Q_OBJECT
public:
	TemplateListWidget(QWidget *parent = 0);
	~TemplateListWidget();

	void addTemplate(QString file, bool isOpen = false);
	void itemAddSprite(ImageSprite* sprite);
	void itemDeleteSprite(ImageSprite* sprite);
	QString getNewTemplateName();
	void deleteTemplateDir();
private slots:
	//http://doc.qt.io/qt-4.8/qlistwidget.html
	void currentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
	void itemClicked(QListWidgetItem * item);
	void itemDoubleClicked(QListWidgetItem * item);
	void itemChanged(QListWidgetItem * item);
	void deleteItem();
	void changeTemplate();
	void updateAllSpriteValue();

	void checkCurrentLayerUseTemplateSprite();
protected:

	void contextMenuEvent(QContextMenuEvent *event);
	void _connect();
	QString savaName;
};

