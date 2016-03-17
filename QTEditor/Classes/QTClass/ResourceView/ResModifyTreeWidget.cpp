#include "ResModifyTreeWidget.h"

ResModifyTreeWidget::ResModifyTreeWidget(QWidget *parent)
	:QTreeWidget(parent)
{

}


ResModifyTreeWidget::~ResModifyTreeWidget()
{
}

void ResModifyTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (topLevelItemCount() > 0){
		QModelIndex clickedIndex = indexAt(event->pos());
		if (clickedIndex.isValid() == false){
			setCurrentItem(nullptr);
		}
	}
	QTreeWidget::mousePressEvent(event);
}

void ResModifyTreeWidget::mouseMoveEvent(QMouseEvent *event)
{ 
	QTreeWidget::mouseMoveEvent(event);
}

void ResModifyTreeWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QTreeWidget::mouseReleaseEvent(event);
}