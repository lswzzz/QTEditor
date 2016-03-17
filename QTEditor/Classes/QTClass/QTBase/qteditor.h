#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QtWidgets/QMainWindow>
#include <qlistwidget.h>
#include "ui_qteditor.h"
#include "BaseDockWidget.h"
#include "LayerManagerWidget.h"
#include "TexturePackerView.h"
#include "ByteArray.h"
#include "TemplateView.h"
#include <ResourceView.h>
#include "ControlView.h"
#include "infodialog.h"
#include "ExteralView.h"
#include "ConfigView.h"
#include "AnimationView.h"
#include "keyboardmappingdialog.h"

namespace Ui{
	class QTEditor;
}

class QTEditor : public QMainWindow
{
	Q_OBJECT

public:
	explicit QTEditor(QWidget *parent = 0);
	~QTEditor();
	void closeEvent(QCloseEvent* e);

	Ui::QTEditor getUI(){ return ui; }
	void myCustomEvent(QPoint p);
	void showToolTip(std::string str, QPoint p);

	void OutputTexture(char* path, char* name);
	void showPreView();
	void reSetSize(QSize size, QSize oldSize);
	void reSetQTEditorBySize(QSize cocosWindowSize);
	bool isShowPreview;
	void saveConf(QString filename);
	KeyboardMappingDialog* getKeyBoardMappingDialog(){ return keyboardDialog; };

private slots:
	void on_sizeButton_clicked();
	void on_gridButton_clicked();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSava_as_triggered();
	void on_actionRelease_triggered();
	void on_actionPreview_triggered();

	void on_actionGrid_triggered();
	void on_actionResources_triggered();
	void on_actionLayerManager_triggered();
	void on_actionTexturePacker_triggered();
	void on_actionOutputBinary_triggered();
	void on_actionAddPlist_triggered();
	void on_actionTemplate_triggered();
	void on_actionExteralView_triggered();
	void on_actionConfigView_triggered();
	void on_actionAnimationView_triggered();

	void on_actionVersion_triggered();
	void on_actionHelp_triggered();

	void on_actionAddModel_triggered();

	void on_actionActions_triggered();
	void on_actionBaseProperties_triggered();
	void on_actionMoveProperties_triggered();
	void on_actionBoxProperties_triggered();
	void on_actionCircleProperties_triggered();
	void on_actionTrigger_triggered();
	void on_actionEntityProperties_triggered();
	void on_actionMonsterProperties_triggered();
	void on_actionMonsterDropProperties_triggered();
	void on_actionAiProperties_triggered();
	void on_actionFilterProperties_triggered();
	void on_actionFbx_conv_triggered();
	
	void on_actionPreShowPath_triggered(); 
	void on_actionKeyboardMapping_triggered();

	void on_createGridButton_clicked();
	void on_deleteGridButton_clicked();
	void contextMenuEvent(QContextMenuEvent* e);
	void on_directionButton_clicked();
	void on_addGridButton_clicked();

protected:
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void keyReleaseEvent(QKeyEvent* e);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void wheelEvent(QWheelEvent *e);
	virtual void resizeEvent(QResizeEvent *r);
	bool eventFilter(QObject *o, QEvent *e);//事件过滤器

	void setDockWidget();
	void createLayerManagerWidget();
	void createTemplateWidget();
	void createResourceView();
	void createTextureView();
	void createControlView();
	void createExteralView();
	void createConfigView();
	void createAnimationView();

	void __connect();
	void setDockWidgetVisible(QDockWidget* widget, QString objName);
	void setControllViewPropertiesVisible(QDockWidget* widget, QString objName);
private:
	Ui::QTEditor ui;
	QString fileName;
	QMenu *menu;
	QPoint rightPos;

	QDockWidget* gridWidget;
	QDockWidget* sceneWidget;
	QDockWidget* infoWidget;
	QDockWidget* logWidget;
	LayerManagerWidget* layerManagerWidget;
	TexturePackerView* textureView;
	TemplateView* templateView;
	ResourceView* resourceView;
	ControlView* controlView;
	ExteralView* exteralView;
	ConfigView* configView;
	AnimationView* animationView;
	InfoDialog* dialog;
	KeyboardMappingDialog* keyboardDialog;
	QLineEdit* sceneScale;
	QLineEdit* minScale;
	QLineEdit* maxScale;
};

#endif // QTEDITOR_H
