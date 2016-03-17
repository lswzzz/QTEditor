#pragma once
#include "QTreeWidget"
#include "TriggerItem.h"

class TriggerWidget : public QTreeWidget
{
public:
	TriggerWidget();
	~TriggerWidget();
	void addItem(QMap<int, QMap<int, std::vector<float>>> map);
	void deleteCurrentItem();
private:
	int count;
};

