#pragma once
#include "QCheckBox"

class QSelectedCheckBox : public QCheckBox
{
	Q_OBJECT
public:
	QSelectedCheckBox(QWidget* widget = 0);
	QSelectedCheckBox(const QString &text, QWidget *parent = 0);
	~QSelectedCheckBox();

	bool getState();
	void setState(bool state);
protected slots:
	void stateChanged(int state);

private:
	bool state;
};

