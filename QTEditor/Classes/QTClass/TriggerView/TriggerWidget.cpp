#include "TriggerWidget.h"

TriggerWidget::TriggerWidget()
:QTreeWidget()
{
	count = 0;
	setHeaderHidden(true);
}


TriggerWidget::~TriggerWidget()
{
}

void TriggerWidget::addItem(QMap<int, QMap<int, std::vector<float>>> map)
{
	QString str = "Trigger" + QString::number(count);
	auto triggerItem = new TriggerItem;
	triggerItem->setData(map);
	triggerItem->setText(0, str);
	this->insertTopLevelItem(count++, triggerItem);
}

void TriggerWidget::deleteCurrentItem()
{
	auto item = this->currentItem();
	if (item){
		delete item;
	}
}