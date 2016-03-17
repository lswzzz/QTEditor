#pragma once
#include "qlistwidget.h"
#include "ImageListWidget.h"

class ExterImageListWidget : public ImageListWidget
{
	Q_OBJECT
public:
	ExterImageListWidget(QWidget *parent = 0);
	~ExterImageListWidget();
	virtual QTreeWidget* getTreeWidget();
};

