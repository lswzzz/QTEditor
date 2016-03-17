#pragma once
#include <qdockwidget.h>
#include "QToolButton"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>

class BaseDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	BaseDockWidget(char* title, QSize size, bool isNeed = false);
	~BaseDockWidget();
	QDockWidget* getDockWidget();

	void NeedMinOrMaxToolButton();

private slots:
	void on_minButton_clicked();

	void featuresChanged(QDockWidget::DockWidgetFeatures features);
	void topLevelChanged(bool topLevel);
	void allowedAreasChanged(Qt::DockWidgetAreas allowedAreas);
	void visibilityChanged(bool visible);
	void dockLocationChanged(Qt::DockWidgetArea area);
protected:
	
protected:
	QWidget* widget;
	QToolButton *minButton;
	QSize size;
	QJsonObject m_obj;
	QString title;
};

