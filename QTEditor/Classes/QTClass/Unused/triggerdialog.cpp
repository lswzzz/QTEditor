#include "triggerdialog.h"
#include "ui_triggerdialog.h"
#include "FontChina.h"
#include "QLabel"
#include "QLineEdit"
#include "Global.h"
#include "ControlView.h"

TriggerDialog::TriggerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TriggerDialog)
{
    ui->setupUi(this);
	this->setWindowTitle(FontChina::G2U("编辑触发器"));
	_setLayout();
	createAllDetail();
	_connect();
	setStyleSheet("background-color: rgb(191, 191, 191);");
}

TriggerDialog::~TriggerDialog()
{
    delete ui;
}

void TriggerDialog::_setLayout()
{
	eventBox = new QComboBox();
	conditionBox = new QComboBox();
	actionBox = new QComboBox();
	eventList = new QScrollArea;
	conditionList = new QScrollArea;
	actionList = new QScrollArea;
	sureButton = new QPushButton(FontChina::G2U("确定"));
	cancelButton = new QPushButton(FontChina::G2U("取消"));
	detailView = new QScrollArea;

	eventList->setWidgetResizable(true);
	eventList->setAlignment(Qt::AlignTop);
	eventList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	eventList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	conditionList->setWidgetResizable(true);
	conditionList->setAlignment(Qt::AlignTop);
	conditionList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	conditionList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	actionList->setWidgetResizable(true);
	actionList->setAlignment(Qt::AlignTop);
	actionList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	actionList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	detailView->setMaximumWidth(300);
	detailView->setWidgetResizable(true);
	detailView->setAlignment(Qt::AlignTop);
	detailView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	detailView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	createEventBoxItem();
	createConditionBoxItem();
	createActionBoxItem();
	QGridLayout* glayout = new QGridLayout;
	glayout->addWidget(new QLabel(FontChina::G2U("事件")), 0, 0);
	glayout->addWidget(eventBox, 0, 1, 1, 3);
	glayout->addWidget(eventList, 1, 0, 1, 4);
	glayout->addWidget(new QLabel(FontChina::G2U("条件")), 2, 0);
	glayout->addWidget(conditionBox, 2, 1, 1, 3);
	glayout->addWidget(conditionList, 3, 0, 1, 4);
	glayout->addWidget(new QLabel(FontChina::G2U("动作")), 4, 0);
	glayout->addWidget(actionBox, 4, 1, 1, 3);
	glayout->addWidget(actionList, 5, 0, 1, 4);
	glayout->addWidget(sureButton, 6, 2);
	glayout->addWidget(cancelButton, 6, 3);
	QGridLayout* glayout2 = new QGridLayout;
	glayout2->addLayout(glayout, 0, 0, 10, 1);
	glayout2->addWidget(detailView, 0, 1, 4, 1);
	setLayout(glayout2);
}

void TriggerDialog::_connect()
{
	connect(sureButton, SIGNAL(clicked()), this, SLOT(on_sureButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
	connect(eventBox, SIGNAL(currentIndexChanged(int)), this, SLOT(eventChange(int)));
	connect(conditionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(conditionChange(int)));
	connect(actionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(actionChange(int)));
}

void TriggerDialog::createEventBoxItem()
{
	eventBox->addItem(FontChina::G2U(""));
	eventBox->addItem(FontChina::G2U("TouchBegan"));
	eventStringList << "TouchBegan";
	eventBox->addItem(FontChina::G2U("TouchMoved"));
	eventStringList << "TouchMoved";
	eventBox->addItem(FontChina::G2U("TouchEnded"));
	eventStringList << "TouchEnded";
	eventBox->addItem(FontChina::G2U("SceneUpdate"));
	eventStringList << "SceneUpdate";
	addEventToScrollList(eventStringList);
}

void TriggerDialog::createConditionBoxItem()
{
	conditionBox->addItem(FontChina::G2U(""));
	conditionBox->addItem(FontChina::G2U("Intersection"));
	conditionStringList << "Intersection";
	conditionBox->addItem(FontChina::G2U("TimeOut"));
	conditionStringList << "TimeOut";
	conditionBox->addItem(FontChina::G2U("ActionState"));
	conditionStringList << "ActionState";
	conditionBox->addItem(FontChina::G2U("Visible"));
	conditionStringList << "Visible";
	addConditionToScrollList(conditionStringList);
}

void TriggerDialog::createActionBoxItem()
{
	
	actionBox->addItem("");
	actionBox->addItem("MoveBy");
	actionStringList << "MoveBy";
	actionBox->addItem("MoveTo");
	actionStringList << "MoveTo";
	actionBox->addItem("RotateTo");
	actionStringList << "RotateTo";
	actionBox->addItem("RotateBy");
	actionStringList << "RotateBy";
	actionBox->addItem("ScaleTo");
	actionStringList << "ScaleTo";
	actionBox->addItem("ScaleBy");
	actionStringList << "ScaleBy";
	actionBox->addItem("PlayMusic");
	actionStringList << "PlayMusic";
	actionBox->addItem("StopAllAction");
	actionStringList << "StopAllAction";
	actionBox->addItem("TriggerState");
	actionStringList << "TriggerState";
	actionBox->addItem("SetVisible");
	actionStringList << "SetVisible";
	actionBox->addItem("RunAction");
	actionStringList << "RunAction";
	actionBox->addItem("UserDefine");
	actionStringList << "UserDefine";
	addActionToScrollList(actionStringList);
}

void TriggerDialog::addEventToScrollList(QStringList list)
{
	for (int i = 0; i < list.size(); i++){
		QString str = list.at(i);
		QPushButton* label = new QPushButton(str);
		QPushButton* sub = new QPushButton();
		label->setFlat(true);
		label->setStyleSheet(" border: none; ");
		sub->setIcon(QIcon(":/icon/Resources/sub.ico"));
		sub->setMaximumSize(QSize(30, 30));
		sub->setFlat(true);
		sub->setStyleSheet(" border: none; ");
		connect(label, &QPushButton::clicked, [this]{
		});
		connect(sub, &QPushButton::clicked, [this, label, sub]{
			label->setVisible(false);
			sub->setVisible(false);
		});
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, sub);
		event_map.insert(str, map);
		label->setVisible(false);
		sub->setVisible(false);
	}
	QGridLayout* layout = new QGridLayout;
	for (int i = 0; i < list.size(); i++){
		QMap<int, QWidget*> map = event_map[list.at(i)];
		layout->addWidget(map[0], i, 0);
		layout->addWidget(map[1], i, 1);
		layout->setRowStretch(i, 1);
	}
	QWidget* eventListContents = new QWidget();
	layout->addWidget(new QLabel, layout->rowCount(), 0);
	layout->setRowStretch(layout->rowCount() - 1, 100);
	eventListContents->setLayout(layout);
	eventList->setWidget(eventListContents);
}

void TriggerDialog::addConditionToScrollList(QStringList list)
{
	for (int i = 0; i < list.size(); i++){
		QString str = list.at(i);
		QPushButton* label = new QPushButton(str);
		QPushButton* sub = new QPushButton();
		label->setFlat(true);
		label->setStyleSheet(" border: none; ");
		sub->setIcon(QIcon(":/icon/Resources/sub.ico"));
		sub->setMaximumSize(QSize(30, 30));
		sub->setFlat(true);
		sub->setStyleSheet(" border: none; ");
		connect(label, &QPushButton::clicked, [this, i]{
			switch (i+1){
			case Intersection:
				break;
			case TimeOut:
				timeOutCondition();
				break;
			case ActionState:
				actionStateCondition();
				break;
			case Visible:
				VisibleCondition();
				break;
			}
		});
		connect(sub, &QPushButton::clicked, [this, label, sub]{
			label->setVisible(false);
			sub->setVisible(false);
		});
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, sub);
		condition_map.insert(str, map);
		label->setVisible(false);
		sub->setVisible(false);
	}
	QGridLayout* layout = new QGridLayout;
	for (int i = 0; i < list.size(); i++){
		QMap<int, QWidget*> map = condition_map[list.at(i)];
		layout->addWidget(map[0], i, 0);
		layout->addWidget(map[1], i, 1);
		layout->setRowStretch(i, 1);
	}
	
	QWidget* conditionListContents = new QWidget();
	layout->addWidget(new QLabel, layout->rowCount(), 0);
	layout->setRowStretch(layout->rowCount() - 1, 100);
	conditionListContents->setLayout(layout);
	conditionList->setWidget(conditionListContents);
}

void TriggerDialog::addActionToScrollList(QStringList list)
{
	for (int i = 0; i < list.size(); i++){
		QString str = list.at(i);
		QPushButton* label = new QPushButton(str);
		QPushButton* sub = new QPushButton();
		label->setFlat(true);
		label->setStyleSheet(" border: none; ");
		sub->setIcon(QIcon(":/icon/Resources/sub.ico"));
		sub->setMaximumSize(QSize(30, 30));
		sub->setFlat(true);
		sub->setStyleSheet(" border: none; ");
		connect(label, &QPushButton::clicked, [this, i]{
			switch (i + 1){
			case MoveBy:
				MoveByAction();
				break;
			case MoveTo:
				MoveToAction();
				break;
			case RotateTo:
				RotateToAction();
				break;
			case RotateBy:
				RotateByAction();
				break;
			case ScaleTo:
				ScaleToAction();
				break;
			case ScaleBy:
				ScaleByAction();
				break;
			case PlayMusic:
				PlayMusicAction();
				break;
			case StopAllAction:
				break;
			case TriggerState:
				TriggerStateAction();
				break;
			case SetVisible:
				SetVisibleAction();
				break;
			case RunAction:
				RunActionAction();
				break;
			case UserDefine:
				UserDefineAction();
				break;
			}
		});
		connect(sub, &QPushButton::clicked, [this, label, sub]{
			label->setVisible(false);
			sub->setVisible(false);
		});
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, sub);
		action_map.insert(str, map);
		label->setVisible(false);
		sub->setVisible(false);
	}
	QGridLayout* layout = new QGridLayout;
	for (int i = 0; i < list.size(); i++){
		QMap<int, QWidget*> map = action_map[list.at(i)];
		layout->addWidget(map[0], i, 0);
		layout->addWidget(map[1], i, 1);
		layout->setRowStretch(i, 1);
	}
	QWidget* actionListContents = new QWidget();
	layout->addWidget(new QLabel, layout->rowCount(), 0);
	layout->setRowStretch(layout->rowCount() - 1, 100);
	actionListContents->setLayout(layout);
	//必须是这样才行
	actionList->setWidget(actionListContents);
}

void TriggerDialog::on_sureButton_clicked()
{
	QMap<int, QMap<int, std::vector<float>>> map;
	map.insert(0, getEventMap());
	map.insert(1, getConditionMap());
	map.insert(2, getActionMap());
	auto controlView = dynamic_cast<ControlView*>(g_ControlView);
	controlView->getTriggerProperties()->addItem(map);
	this->close();
}

void TriggerDialog::on_cancelButton_clicked()
{
	this->close();
}

void TriggerDialog::eventChange(int index)
{
	if (index == 0)return;
	QString str = eventBox->itemText(index);
	QMap<int, QWidget*> map = event_map[str];
	auto widget = map[0];
	auto widget2 = map[1];
	widget->setVisible(true);
	widget2->setVisible(true);
}

void TriggerDialog::conditionChange(int index)
{
	if (index == 0)return;
	QString str = conditionBox->itemText(index);
	QMap<int, QWidget*> map = condition_map[str];
	auto widget = map[0];
	auto widget2 = map[1];
	widget->setVisible(true);
	widget2->setVisible(true);
}

void TriggerDialog::actionChange(int index)
{
	if (index == 0)return;
	QString str = actionBox->itemText(index);
	QMap<int, QWidget*> map = action_map[str];
	auto widget = map[0];
	auto widget2 = map[1];
	widget->setVisible(true);
	widget2->setVisible(true);
}

QMap<int, std::vector<float>> TriggerDialog::getEventMap()
{
	QMap<int, std::vector<float>> map;
	for (int i = 0; i < eventStringList.size(); i++){
		QMap<int, QWidget*> __map = event_map[eventStringList.at(i)];
		auto widget = __map[0];
		if (widget->isVisible()){
			std::vector<float> vec;
			map.insert(i+1, vec);
		}
	}
	return map;
}

QMap<int, std::vector<float>> TriggerDialog::getConditionMap()
{
	QMap<int, std::vector<float>> map;
	for (int i = 0; i < conditionStringList.size(); i++){
		QMap<int, QWidget*> __map = condition_map[conditionStringList.at(i)];
		auto widget = __map[0];
		if (widget->isVisible()){
			std::vector<float> vec;
			QMap<int, QWidget*> data = m_map[conditionStringList.at(i)];
			if (data[1]->inherits("QLineEdit")){
				auto edit = dynamic_cast<QLineEdit*>(data[1]);
				vec.push_back(edit->text().toFloat());
			}
			else if (data[1]->inherits("QComboBox")){
				auto box = dynamic_cast<QComboBox*>(data[1]);
				vec.push_back(box->currentIndex());
			}
			map.insert(i+1, vec);
		}
	}
	return map;
}

QMap<int, std::vector<float>> TriggerDialog::getActionMap()
{
	QMap<int, std::vector<float>> map;
	for (int i = 0; i < actionStringList.size(); i++){
		QMap<int, QWidget*> __map = action_map[actionStringList.at(i)];
		auto widget = __map[0];
		if (widget->isVisible()){
			std::vector<float> vec;
			QMap<int, QWidget*> data = m_map[actionStringList.at(i)];
			for (int k = 1; k < data.size(); k += 2){
				if (data[1]->inherits("QLineEdit")){
					auto edit = dynamic_cast<QLineEdit*>(data[1]);
					vec.push_back(edit->text().toFloat());
				}
				else if (data[1]->inherits("QComboBox")){
					auto box = dynamic_cast<QComboBox*>(data[1]);
					vec.push_back(box->currentIndex());
				}
			}
			map.insert(i, vec);
		}
	}
	return map;
}






































void TriggerDialog::resetVisibleDetail(QString name)
{
	for (int i = 0; i < detailVec.size(); i++){
		QString str = detailVec.at(i);
		if (str != name){
			QMap<int, QWidget*> map = m_map[str];
			int count = map.size();
			for (int k = 0; k < count; k++){
				auto widget = map[k];
				widget->setVisible(false);
			}
		}
		else{
			QMap<int, QWidget*> map = m_map[str];
			int count = map.size();
			for (int k = 0; k < count; k++){
				auto widget = map[k];
				widget->setVisible(true);
			}
		}
	}
}

void TriggerDialog::createAllDetail()
{
	gridLayout = new QGridLayout;
	gridLayout->setSpacing(5);
	//event

	//condition
	timeOutCondition();
	actionStateCondition();
	VisibleCondition();

	//action
	MoveByAction();
	MoveToAction();
	RotateToAction();
	RotateByAction();
	ScaleToAction();
	ScaleByAction();
	PlayMusicAction();
	TriggerStateAction();
	SetVisibleAction();
	RunActionAction();
	UserDefineAction();
	gridLayout->addWidget(new QLabel, gridLayout->rowCount(), 0);
	gridLayout->setRowStretch(gridLayout->rowCount() - 1, 100);
	QWidget* widget = new QWidget();
	widget->setLayout(gridLayout);
	detailView->setWidget(widget);
	resetVisibleDetail("");
}

void TriggerDialog::timeOutCondition()
{
	if (!m_map.contains("TimeOut")){
		detailVec.push_back("TimeOut");
		QLabel* label = new QLabel(FontChina::G2U("超时时间"));
		QLineEdit* edit = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		m_map.insert("TimeOut", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->setRowStretch(row, 1);
	}
	else{
		resetVisibleDetail("TimeOut");
	}
}

void TriggerDialog::actionStateCondition()
{
	if (!m_map.contains("ActionState")){
		detailVec.push_back("ActionState");
		QLabel* label = new QLabel(FontChina::G2U("动作状态"));
		QComboBox* state = new QComboBox();
		state->addItem(FontChina::G2U("开启"));
		state->addItem(FontChina::G2U("关闭"));
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, state);
		m_map.insert("ActionState", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->setRowStretch(row, 1);
	}
	else{
		resetVisibleDetail("ActionState");
	}
}

void TriggerDialog::VisibleCondition()
{
	if (!m_map.contains("Visible")){
		detailVec.push_back("Visible");
		QLabel* label = new QLabel(FontChina::G2U("是否可见"));
		QComboBox* state = new QComboBox();
		state->addItem(FontChina::G2U("可见"));
		state->addItem(FontChina::G2U("不可见"));
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, state);
		m_map.insert("Visible", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->setRowStretch(row, 1);
	}
	else{
		resetVisibleDetail("Visible");
	}
}

void TriggerDialog::MoveByAction()
{
	if (!m_map.contains("MoveBy")){
		detailVec.push_back("MoveBy");
		QLabel* label = new QLabel(FontChina::G2U("X值"));
		QLabel* label2 = new QLabel(FontChina::G2U("Y值"));
		QLabel* label3 = new QLabel(FontChina::G2U("时间"));
		QLineEdit* edit = new QLineEdit();
		QLineEdit* edit2 = new QLineEdit();
		QLineEdit* edit3 = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		map.insert(2, label2);
		map.insert(3, edit2);
		map.insert(4, label3);
		map.insert(5, edit3);
		m_map.insert("MoveBy", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->addWidget(map[2], row+1, 0);
		gridLayout->addWidget(map[3], row+1, 1);
		gridLayout->addWidget(map[4], row+2, 0);
		gridLayout->addWidget(map[5], row+2, 1);
		gridLayout->setRowStretch(row, 1);
		gridLayout->setRowStretch(row+1, 1);
		gridLayout->setRowStretch(row+2, 1);
	}
	else{
		resetVisibleDetail("MoveBy");
	}
}

void TriggerDialog::MoveToAction()
{
	if (!m_map.contains("MoveTo")){
		detailVec.push_back("MoveTo");
		QLabel* label = new QLabel(FontChina::G2U("X值"));
		QLabel* label2 = new QLabel(FontChina::G2U("Y值"));
		QLabel* label3 = new QLabel(FontChina::G2U("时间"));
		QLineEdit* edit = new QLineEdit();
		QLineEdit* edit2 = new QLineEdit();
		QLineEdit* edit3 = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		map.insert(2, label2);
		map.insert(3, edit2);
		map.insert(4, label3);
		map.insert(5, edit3);
		m_map.insert("MoveTo", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->addWidget(map[2], row+1, 0);
		gridLayout->addWidget(map[3], row+1, 1);
		gridLayout->addWidget(map[4], row+2, 0);
		gridLayout->addWidget(map[5], row+2, 1);
		gridLayout->setRowStretch(row, 1);
		gridLayout->setRowStretch(row+1, 1);
		gridLayout->setRowStretch(row+2, 1);
	}
	else{
		resetVisibleDetail("MoveTo");
	}
}

void TriggerDialog::RotateToAction()
{
	if (!m_map.contains("RotateTo")){
		detailVec.push_back("RotateTo");
		QLabel* label = new QLabel(FontChina::G2U("角度"));
		QLabel* label2 = new QLabel(FontChina::G2U("时间"));
		QLineEdit* edit = new QLineEdit();
		QLineEdit* edit2 = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		map.insert(2, label2);
		map.insert(3, edit2);
		m_map.insert("RotateTo", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->addWidget(map[2], row + 1, 0);
		gridLayout->addWidget(map[3], row + 1, 1);
		gridLayout->setRowStretch(row, 1);
		gridLayout->setRowStretch(row+1, 1);
	}
	else{
		resetVisibleDetail("RotateTo");
	}
}

void TriggerDialog::RotateByAction()
{
	if (!m_map.contains("RotateBy")){
		detailVec.push_back("RotateBy");
		QLabel* label = new QLabel(FontChina::G2U("角度"));
		QLabel* label2 = new QLabel(FontChina::G2U("时间"));
		QLineEdit* edit = new QLineEdit();
		QLineEdit* edit2 = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		map.insert(2, label2);
		map.insert(3, edit2);
		m_map.insert("RotateBy", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->addWidget(map[2], row+1, 0);
		gridLayout->addWidget(map[3], row+1, 1);
		gridLayout->setRowStretch(row, 1);
		gridLayout->setRowStretch(row+1, 1);
	}
	else{
		resetVisibleDetail("RotateBy");
	}
}

void TriggerDialog::ScaleToAction()
{
	if (!m_map.contains("ScaleTo")){
		detailVec.push_back("ScaleTo");
		QLabel* label = new QLabel(FontChina::G2U("X值"));
		QLabel* label2 = new QLabel(FontChina::G2U("Y值"));
		QLabel* label3 = new QLabel(FontChina::G2U("时间"));
		QLineEdit* edit = new QLineEdit();
		QLineEdit* edit2 = new QLineEdit();
		QLineEdit* edit3 = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		map.insert(2, label2);
		map.insert(3, edit2);
		map.insert(4, label3);
		map.insert(5, edit3);
		m_map.insert("ScaleTo", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->addWidget(map[2], row+1, 0);
		gridLayout->addWidget(map[3], row+1, 1);
		gridLayout->addWidget(map[4], row+2, 0);
		gridLayout->addWidget(map[5], row+2, 1);
		gridLayout->setRowStretch(row, 1);
		gridLayout->setRowStretch(row+1, 1);
		gridLayout->setRowStretch(row+2, 1);
	}
	else{
		resetVisibleDetail("ScaleTo");
	}
}

void TriggerDialog::ScaleByAction()
{
	if (!m_map.contains("ScaleBy")){
		detailVec.push_back("ScaleBy");
		QLabel* label = new QLabel(FontChina::G2U("X值"));
		QLabel* label2 = new QLabel(FontChina::G2U("Y值"));
		QLabel* label3 = new QLabel(FontChina::G2U("时间"));
		QLineEdit* edit = new QLineEdit();
		QLineEdit* edit2 = new QLineEdit();
		QLineEdit* edit3 = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		map.insert(2, label2);
		map.insert(3, edit2);
		map.insert(4, label3);
		map.insert(5, edit3);
		m_map.insert("ScaleBy", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->addWidget(map[2], row+1, 0);
		gridLayout->addWidget(map[3], row+1, 1);
		gridLayout->addWidget(map[4], row+2, 0);
		gridLayout->addWidget(map[5], row+2, 1);
		gridLayout->setRowStretch(row, 1);
		gridLayout->setRowStretch(row+1, 1);
		gridLayout->setRowStretch(row+2, 1);
	}
	else{
		resetVisibleDetail("ScaleBy");
	}
}

void TriggerDialog::PlayMusicAction()
{
	if (!m_map.contains("PlayMusic")){
		detailVec.push_back("PlayMusic");
		QLabel* label = new QLabel(FontChina::G2U("音乐文件"));
		QLineEdit* edit = new QLineEdit();
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, edit);
		m_map.insert("PlayMusic", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->setRowStretch(row, 1);
	}
	else{
		resetVisibleDetail("PlayMusic");
	}
}

void TriggerDialog::TriggerStateAction()
{
	if (!m_map.contains("TriggerState")){
		detailVec.push_back("TriggerState");
		QLabel* label = new QLabel(FontChina::G2U("是否开启"));
		QComboBox* state = new QComboBox();
		state->addItem(FontChina::G2U("开启"));
		state->addItem(FontChina::G2U("关闭"));
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, state);
		m_map.insert("TriggerState", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->setRowStretch(row, 1);
	}
	else{
		resetVisibleDetail("TriggerState");
	}
}

void TriggerDialog::SetVisibleAction()
{
	if (!m_map.contains("SetVisible")){
		detailVec.push_back("SetVisible");
		QLabel* label = new QLabel(FontChina::G2U("是否可见"));
		QComboBox* state = new QComboBox();
		state->addItem(FontChina::G2U("可见"));
		state->addItem(FontChina::G2U("不可见"));
		QMap<int, QWidget*> map;
		map.insert(0, label);
		map.insert(1, state);
		m_map.insert("SetVisible", map);
		int row = gridLayout->rowCount();
		gridLayout->addWidget(map[0], row, 0);
		gridLayout->addWidget(map[1], row, 1);
		gridLayout->setRowStretch(row, 1);
	}
	else{
		resetVisibleDetail("SetVisible");
	}
}

void TriggerDialog::RunActionAction()
{
	
}

void TriggerDialog::UserDefineAction()
{
	
}