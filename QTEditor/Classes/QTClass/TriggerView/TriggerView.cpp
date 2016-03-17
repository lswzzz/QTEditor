#include "TriggerView.h"
#include "QLayout"
#include "triggerdialog.h"

TriggerView::TriggerView(char* title, QSize size)
	:ControlBaseDockWidget(title, size, true)
{
	_setLayout();
	_connect();
}

TriggerView::~TriggerView()
{
}

void TriggerView::_connect()
{
	connect(addTrigger, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
	connect(deleteTrigger, SIGNAL(clicked()), this, SLOT(on_deleteButton_clicked()));
}

void TriggerView::_setLayout()
{
	addTrigger = new QPushButton;
	addTrigger->setIcon(QIcon(":/icon/Resources/add2.ico"));
	deleteTrigger = new QPushButton;
	deleteTrigger->setIcon(QIcon(":/icon/Resources/sub.ico"));
	triggerList = new TriggerWidget;
	QVBoxLayout* layout = new QVBoxLayout;
	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(addTrigger, 0, 0);
	gridLayout->addWidget(deleteTrigger, 0, 1);
	layout->addLayout(gridLayout);
	layout->addWidget(triggerList);
	widget->setLayout(layout);
}

void TriggerView::setDefaultValue()
{

}

QVariantMap TriggerView::getVariantMap()
{
	QVariantMap map;
	return map;
}

void TriggerView::addController()
{
}

void TriggerView::deleteController()
{

}

bool TriggerView::checkSprite()
{
	if (targetSprite->getType() != SpriteType::NORMAL_BLOCK){
		return true;
	}
	return false;
}

void TriggerView::setValueFromFile(QString file, bool clearTarget)
{

}

void TriggerView::addItem(QMap<int, QMap<int, std::vector<float>>> map)
{
	triggerList->addItem(map);
}

void TriggerView::setWidgetValue()
{

}
  
void TriggerView::on_addButton_clicked()
{
	TriggerDialog* dialog = new TriggerDialog;
	dialog->show();
}

void TriggerView::on_deleteButton_clicked()
{
	triggerList->deleteCurrentItem();
}