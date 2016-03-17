#pragma once
#include "BaseDockWidget.h"
#include "ExterImageListWidget.h"
#include "ExteralTreeWidget.h"
#include <QComboBox>
#include "ResourceView.h"

class ExteralView : public BaseDockWidget
{
	Q_OBJECT
public:
	ExteralView(char* title, QSize size);
	~ExteralView();

	ImageTreeWidget* getTreeWidget();
	ImageListWidget* getListWidget();
	int getMode();
	void changedResRef(std::string path, int count);
	void setCurItem(int listpngindex);
private slots:
	void setResShowMode(int mode);

private:
	ExterImageListWidget* exterListWidget;
	ExteralTreeWidget* exterTreeWidget;
	QComboBox* showMode;
	int m_mode;
};

