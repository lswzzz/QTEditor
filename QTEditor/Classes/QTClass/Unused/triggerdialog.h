#ifndef TRIGGERDIALOG_H
#define TRIGGERDIALOG_H

#include <QDialog>
#include "QComboBox"
#include "QPushButton"
#include "QScrollArea"
#include "QLayout"

namespace Ui {
class TriggerDialog;
}

class TriggerDialog : public QDialog
{
    Q_OBJECT
public:
	enum event_Type{
		None_event,
		TouchBegan,
		TouchMoved,
		TouchEnded,
		SceneUpdate,
	};
	enum condition_Type{
		None_condition,
		Intersection,
		TimeOut,
		ActionState,
		Visible,
	};
	enum action_Type{
		None_action,
		MoveBy,
		MoveTo,
		RotateTo,
		RotateBy,
		ScaleTo,
		ScaleBy,
		PlayMusic,
		StopAllAction,
		TriggerState,
		SetVisible,
		RunAction,
		UserDefine
	};
public:
    explicit TriggerDialog(QWidget *parent = 0);
    ~TriggerDialog();

protected:
	void _setLayout();
	void _connect();
	void createEventBoxItem();
	void createConditionBoxItem();
	void createActionBoxItem();
	void addEventToScrollList(QStringList list);
	void addConditionToScrollList(QStringList list);
	void addActionToScrollList(QStringList list);

	void timeOutCondition();
	void actionStateCondition();
	void VisibleCondition();

	void MoveByAction();
	void MoveToAction();
	void RotateToAction();
	void RotateByAction();
	void ScaleToAction();
	void ScaleByAction();
	void PlayMusicAction();
	void TriggerStateAction();
	void SetVisibleAction();
	void RunActionAction();
	void UserDefineAction();


	
	void createAllDetail();
	void resetVisibleDetail(QString name);
	QMap<int, std::vector<float>> getEventMap();
	QMap<int, std::vector<float>> getConditionMap();
	QMap<int, std::vector<float>> getActionMap();
private slots:
	void on_sureButton_clicked();
	void on_cancelButton_clicked();
	void eventChange(int index);
	void conditionChange(int index);
	void actionChange(int index);
private:
    Ui::TriggerDialog *ui;
	QComboBox* eventBox;
	QComboBox* conditionBox;
	QComboBox* actionBox;
	QPushButton* sureButton;
	QPushButton* cancelButton;
	QScrollArea* eventList;
	QScrollArea* conditionList;
	QScrollArea* actionList;
	QScrollArea* detailView;
	QGridLayout* gridLayout;
	std::vector<QString> detailVec;
	QStringList eventStringList;
	QStringList conditionStringList;
	QStringList actionStringList;
	QMap<QString, QMap<int, QWidget*>> m_map;
	QMap<QString, QMap<int, QWidget*>> event_map;
	QMap<QString, QMap<int, QWidget*>> condition_map;
	QMap<QString, QMap<int, QWidget*>> action_map;
};

#endif // TRIGGERDIALOG_H
