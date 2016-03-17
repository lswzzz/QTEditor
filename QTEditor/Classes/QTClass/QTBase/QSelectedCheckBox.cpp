#include "QSelectedCheckBox.h"


QSelectedCheckBox::QSelectedCheckBox(QWidget* widget)
	:QCheckBox(widget)
{
	this->setStyleSheet("QCheckBox::indicator {width: 1px;height: 1px;}QCheckBox:checked{ color: rgb(0, 0, 0); }QCheckBox:unchecked{ color: rgb(119, 136, 153); }");
	this->setCheckState(Qt::CheckState::Checked);
	state = true;
	connect(this, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
}

QSelectedCheckBox::QSelectedCheckBox(const QString &text, QWidget *parent)
	: QCheckBox(text, parent)
{
	this->setStyleSheet("QCheckBox::indicator {width: 1px;height: 1px;}QCheckBox:checked{ color: rgb(0, 0, 0); }QCheckBox:unchecked{ color: rgb(119, 136, 153); }");
	this->setCheckState(Qt::CheckState::Checked);
	state = true;
	connect(this, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
}

QSelectedCheckBox::~QSelectedCheckBox()
{
}

bool QSelectedCheckBox::getState()
{
	return state;
}

void QSelectedCheckBox::setState(bool state)
{
	switch (state){
	case true:
		this->setCheckState(Qt::CheckState::Checked);
		break;
	case false:
		this->setCheckState(Qt::CheckState::Unchecked);
		break;
	}
}

void QSelectedCheckBox::stateChanged(int state)
{
	this->state = state;
	/*switch (state){
	case Qt::CheckState::Checked:
		this->setStyleSheet("color: rgb(0, 0, 0);");
		break;
	case Qt::CheckState::Unchecked:
		this->setStyleSheet("color: rgb(119, 136, 153);");
		break;
	}*/
}
