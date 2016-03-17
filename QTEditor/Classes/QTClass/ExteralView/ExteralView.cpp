#include "ExteralView.h"
#include <QLayout>
#include "FontChina.h"
#include "Global.h"

ExteralView::ExteralView(char* title, QSize size)
:BaseDockWidget(title, size)
{
	QVBoxLayout* vlayout = new QVBoxLayout(widget);
	QHBoxLayout* hlayout = new QHBoxLayout(widget);
	exterListWidget = new ExterImageListWidget();
	exterTreeWidget = new ExteralTreeWidget();
	exterTreeWidget->setHeaderHidden(true);
	exterTreeWidget->setItemDelegate(new CustomItemDelegate());
	showMode = new QComboBox();
	showMode->addItem(FontChina::G2U("文字模式"));
	showMode->addItem(FontChina::G2U("图标模式"));
	hlayout->addWidget(showMode);
	vlayout->addLayout(hlayout);
	vlayout->addWidget(exterTreeWidget);
	vlayout->addWidget(exterListWidget);
	exterListWidget->setVisible(false);
	m_mode = 0;
	widget->setLayout(vlayout);
	connect(showMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setResShowMode(int)));
}

ExteralView::~ExteralView()
{
}

void ExteralView::changedResRef(std::string path, int count) {
	exterTreeWidget->changedResRef(path, count);
}

void ExteralView::setResShowMode(int mode)
{
	switch (mode){
	case 0:
		exterListWidget->setVisible(false);
		exterTreeWidget->setVisible(true);
		exterListWidget->widgetVisible(false);
		m_mode = mode;
		break;
	case 1:
		exterListWidget->widgetVisible(true);
		m_mode = mode;
		exterListWidget->setVisible(true);
		exterTreeWidget->setVisible(false);
		break;
	}
}

ImageTreeWidget* ExteralView::getTreeWidget()
{
	return exterTreeWidget;
}

ImageListWidget* ExteralView::getListWidget()
{
	return exterListWidget;
}

int ExteralView::getMode()
{
	return m_mode;
}

void ExteralView::setCurItem(int listpngindex)
{
	this->show();
	this->raise();
	if (exterListWidget->isVisible()){
		exterTreeWidget->setCurRes(listpngindex);
		exterListWidget->update();
	}
	else{
		exterTreeWidget->setCurRes(listpngindex);
	}
}