#pragma once
#include "QTreeWidget"
#include "QMouseEvent"

class ResModifyTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	ResModifyTreeWidget(QWidget *parent = 0);
	~ResModifyTreeWidget();
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);
};

