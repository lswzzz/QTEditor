#pragma once
#include "QTreeWidget"

class TriggerItem : public QTreeWidgetItem
{
public:
	TriggerItem();
	~TriggerItem();
	void setData(QMap<int, QMap<int, std::vector<float>>> map);
private:
	QMap<int, QMap<int, std::vector<float>>> _map;
};

