#include "BaseDockWidget.h"
#include "FontChina.h"
#include "QToolButton"
#include "QStyle"
#include "Global.h"

BaseDockWidget::BaseDockWidget(char* title, QSize size, bool isNeed)
{
	this->title = FontChina::G2U(title);
	widget = new QWidget();
	this->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	this->setAllowedAreas(Qt::AllDockWidgetAreas);
	this->size = size;
	this->setMinimumSize(size);
	this->setMaximumSize(size);
	this->setWindowTitle(FontChina::G2U(title));
	this->setWidget(widget);
	widget->setStyleSheet("background-color: rgb(191, 191, 191);");
	connect(this, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)), this, SLOT(featuresChanged(QDockWidget::DockWidgetFeatures)));
	connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(topLevelChanged(bool)));
	connect(this, SIGNAL(allowedAreasChanged(Qt::DockWidgetAreas)), this, SLOT(allowedAreasChanged(Qt::DockWidgetAreas)));
	connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityChanged(bool)));
	connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(dockLocationChanged(Qt::DockWidgetArea)));
	if (isNeed){
		NeedMinOrMaxToolButton();
	}
}

void BaseDockWidget::NeedMinOrMaxToolButton()
{
	int width = this->width();
	minButton = new QToolButton(this);
	QPixmap minPix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
	minButton->setIcon(minPix);
	minButton->setGeometry(width - 55, 0, 20, 20);
	minButton->setToolTip(tr(FontChina::G2U("最小化")));
	minButton->setStyleSheet("background-color:transparent;");
	minButton->setCheckable(true);
	connect(minButton, SIGNAL(clicked()), this, SLOT(on_minButton_clicked()));
}

BaseDockWidget::~BaseDockWidget()
{
}

QDockWidget* BaseDockWidget::getDockWidget()
{
	return NULL;
}

void BaseDockWidget::on_minButton_clicked()
{
	QWidget* widget = this->widget;
	QSize minSize = this->minimumSize();
	if (minSize.height() == 20){
		QPixmap minPix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
		minButton->setIcon(minPix);
		this->setMaximumSize(size);
		this->setMinimumSize(size);
	}
	else{
		QPixmap maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
		minButton->setIcon(maxPix);
		this->setMaximumSize(QSize(size.width(), 20));
		this->setMinimumSize(QSize(size.width(), 20));
	}
}

void BaseDockWidget::featuresChanged(QDockWidget::DockWidgetFeatures features)
{
}

void BaseDockWidget::topLevelChanged(bool topLevel)
{
	QRect rect = this->geometry();
	rect.setWidth(size.width());
	rect.setHeight(size.height());
	this->setGeometry(rect);
	if (topLevel){
		this->setMaximumSize(this->size);
	}
	else{
		this->setMaximumSize(this->size);
	}
}

void BaseDockWidget::allowedAreasChanged(Qt::DockWidgetAreas allowedAreas)
{
}

void BaseDockWidget::visibilityChanged(bool visible)
{
}

void BaseDockWidget::dockLocationChanged(Qt::DockWidgetArea area)
{
}