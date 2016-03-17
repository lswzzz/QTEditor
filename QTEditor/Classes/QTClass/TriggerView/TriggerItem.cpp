#include "TriggerItem.h"

TriggerItem::TriggerItem()
:QTreeWidgetItem()
{
}


TriggerItem::~TriggerItem()
{
}

void TriggerItem::setData(QMap<int, QMap<int, std::vector<float>>> map)
{
	_map = map;
}