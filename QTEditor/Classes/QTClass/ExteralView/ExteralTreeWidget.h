#pragma once
#include "qtreewidget.h"
#include "ImageTreeWidget.h"

class ExteralTreeWidget : public ImageTreeWidget
{
	Q_OBJECT
public:
	ExteralTreeWidget(QWidget *parent = 0);
	virtual ~ExteralTreeWidget();
protected:
	virtual void init();
};

