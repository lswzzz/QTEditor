#include "qteditor.h"
#include "AppDelegate.h"
#include <qdebug.h>
#include <QTimer>
#include <QDockWidget>
#include <QListWidget>
#include <qfiledialog.h>
#include "Global.h"
#include "FontChina.h"
#include "cocos2d.h"
#include "qmessagebox.h"
#include "ImageListWidget.h"
#include <qtreewidget.h>
#include <qevent.h>
#include <qmimedata.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qscrollbar.h>
#include <qdir.h>
#include "QTOutput.h"
#include <qtooltip.h>
#include "QTInput.h"
#include "HelloWorldScene.h"
#include "TexturePackerView.h"
#include <QProcess>
#include "LoadExterModel.h"
#include "ExterImageListWidget.h"
#include "MyLogger.h"
#include "MySqlLiteManager.h"
#include "QTimer"
#include "GlobalConstant.h"

using namespace cocos2d;

bool isResetWindowSize = true;

QTEditor::QTEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	logText = (QPlainTextEdit*)ui.logWidgetContents->children().at(0);
	infoText = (QPlainTextEdit*)ui.infoWidgetContents->children().at(0);
	g_editor = this;
	ui.GridWidget->setAcceptDrops(true);
	menu = new QMenu(this);
	QAction *action1 = new QAction(FontChina::G2U("删除网格"), this);
	QAction* action2 = new QAction(FontChina::G2U("删除背景"), this);
	connect(action1, SIGNAL(triggered()), this, SLOT(action_delete_grid()));
	connect(action2, SIGNAL(triggered()), this, SLOT(action_delete_image()));
	menu->addAction(action1);
	menu->addAction(action2);
	setDockWidget();
	setMouseTracking(true);
	isShowPreview = false;
	__connect();
	//安装过滤器
	QMainWindow::installEventFilter(this);
	keyboardDialog = new KeyboardMappingDialog;
}

void QTEditor::setDockWidget()
{
	gridWidget = ui.GridWidget;
	g_gridWidgetContents = ui.GridWidgetContents;
	sceneWidget = ui.SceneWidget;
	infoWidget = ui.infoWidget;
	logWidget = ui.logWidget;
	//不再使用
	this->removeDockWidget(ui.resourcesWidget);
	this->removeDockWidget(ui.ActionWidget);
	createResourceView();
	createControlView();
	createTextureView();
	createExteralView();
	createLayerManagerWidget();
	createTemplateWidget();
	createConfigView();
	createAnimationView();
	tabifyDockWidget(gridWidget, layerManagerWidget);
	tabifyDockWidget(layerManagerWidget, configView);
	gridWidget->show();
	gridWidget->raise();
	tabifyDockWidget(resourceView, animationView);
	tabifyDockWidget(resourceView, exteralView);
	tabifyDockWidget(templateView, textureView);
	resourceView->show();
	resourceView->raise();
	templateView->show();
	templateView->raise();
}

void QTEditor::__connect()
{
	QObjectList list = ui.GridWidgetContents->children();
	foreach(QObject* qo, list){
		QWidget* qw = (QWidget*)qo;
		QString str = qw->objectName();	
	}
}

void QTEditor::createControlView()
{
	controlView = new ControlView("控制器视图", QSize(350, 500));
	g_ControlView = controlView;
	this->addDockWidget(Qt::RightDockWidgetArea, controlView);
}

void QTEditor::createExteralView()
{
	exteralView = new ExteralView("外部角色视图", QSize(350, 350));
	g_exteralView = exteralView;
	this->addDockWidget(Qt::LeftDockWidgetArea, exteralView);
}

void QTEditor::createConfigView()
{
	configView = new ConfigView("配置视图", QSize(350, 400));
	g_configView = configView;
	this->addDockWidget(Qt::RightDockWidgetArea, configView);
}

void QTEditor::createAnimationView()
{
	animationView = new AnimationView("动画视图", QSize(350, 350));
	g_AnimationView = animationView;
	this->addDockWidget(Qt::LeftDockWidgetArea, animationView);
}

void QTEditor::createTextureView()
{
	textureView = new TexturePackerView("TexturePacker视图", QSize(350, 500));
	g_TextureView = textureView;
	this->addDockWidget(Qt::LeftDockWidgetArea, textureView);
}

void QTEditor::createLayerManagerWidget()
{
	layerManagerWidget = new LayerManagerWidget("层管理视图", QSize(350, 400));
	g_LayerManager = layerManagerWidget;
	this->addDockWidget(Qt::RightDockWidgetArea, layerManagerWidget);
}

void QTEditor::createResourceView()
{
	resourceView = new ResourceView("资源视图", QSize(350, 350));
	g_resourceView = resourceView;
	this->addDockWidget(Qt::LeftDockWidgetArea, resourceView);
}

void QTEditor::createTemplateWidget()
{
	templateView = new TemplateView("模板视图", QSize(350, 500));
	g_TemplateView = templateView;
	this->addDockWidget(Qt::LeftDockWidgetArea, templateView);
}

void QTEditor::closeEvent(QCloseEvent* e)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    cocos2d::Director::getInstance()->end();
    qApp->quit();
#else
    CCQApplication::getInstance()->endRun();
#endif
}

void QTEditor::keyPressEvent(QKeyEvent *e)
{

	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	switch (e->key()){
	case Qt::Key_Shift:
		scene->setShiftKeyStates(true);
		break;
	case Qt::Key_Backspace:
		//重置当前操作为Type_None;
		scene->resetState();
		g_TemplateFile = "";
		scene->setType(Type_None);
		break;
	case Qt::Key_Space:
		setCocCursor(Cursor_Type::Move_Scene);
		scene->setSpaceKeyStates(true);
		break;
	case Qt::Key_Control:
		scene->setCtrlKeyStates(true);
		break;
	case Qt::Key_S:
		bool ctrlStates = scene->getCtrlKeyStates();
		if (ctrlStates){
			saveConf(MySqlLiteManager::getInstance()->getConfigFile());
		}
	}
	
}

void QTEditor::keyReleaseEvent(QKeyEvent* e)
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	switch (e->key()){
	case Qt::Key_Shift:
		scene->setShiftKeyStates(false);
		break;
	case Qt::Key_Space:
		scene->setSpaceKeyStates(false);
		setCocCursor(Cursor_Type::Normal);
		break;
	case Qt::Key_Control:
		scene->setCtrlKeyStates(false);
		break;
	case Qt::Key_P:
		scene->setPKeyStates(false);
		break;
	case Qt::Key_O:
		scene->setOKeyStates(false);
		break;
	}
}

void QTEditor::mousePressEvent(QMouseEvent *e)
{
	//qDebug("mousePress");
}

void QTEditor::mouseMoveEvent(QMouseEvent *e)
{
	//在QT designer中设置mouseTracking为true
	
}

void QTEditor::wheelEvent(QWheelEvent *e)
{
	//qDebug("wheel");
}
	
void QTEditor::resizeEvent(QResizeEvent *r)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (glview){
		QSize size = r->size();
		QSize oldSize = r->oldSize();
        //addConsoleInfo("%d %d %d %d", size.width(), size.height(), oldSize.width(), oldSize.height());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		reSetSize(size, oldSize);
#else
        
#endif
	}
}

bool QTEditor::eventFilter(QObject *o, QEvent *e)
{
	if (o == this){
		if (QEvent::WindowDeactivate == e->type()){
			return true;
		}
		else{
			return false;
		}
	}
	return false;
}

void QTEditor::reSetSize(QSize size, QSize oldSize)
{
	auto glview = Director::getInstance()->getOpenGLView();
	QSize off = size - oldSize;
	if ((off.width() < 50 && off.width() > -50) || (off.height() < 50 && off.height() > -50))return;
	ui.SceneWidget->setMinimumSize(ui.SceneWidget->minimumWidth() + off.width(), ui.SceneWidget->minimumHeight() + off.height());
	ui.SceneWidget->setMaximumSize(ui.SceneWidget->minimumWidth(), ui.SceneWidget->minimumHeight());
	ui.SceneWidget->setGeometry(ui.SceneWidget->geometry().left(), ui.SceneWidget->geometry().top(),
		ui.SceneWidget->minimumWidth(), ui.SceneWidget->minimumHeight());
	ui.SceneWidgetContents->setMinimumSize(ui.SceneWidgetContents->minimumWidth() + off.width(), ui.SceneWidgetContents->minimumHeight() + off.height());
	ui.SceneWidgetContents->setMaximumSize(ui.SceneWidgetContents->minimumWidth(), ui.SceneWidgetContents->minimumHeight());
	ui.SceneWidgetContents->setGeometry(ui.SceneWidgetContents->geometry().left(), ui.SceneWidgetContents->geometry().top(),
		ui.SceneWidgetContents->minimumWidth(), ui.SceneWidgetContents->minimumHeight());
	ui.MainWidget->setMinimumSize(ui.MainWidget->minimumWidth() + off.width(), ui.MainWidget->minimumHeight() + off.height());
	ui.MainWidget->setMaximumSize(ui.MainWidget->minimumWidth(), ui.MainWidget->minimumHeight());
	ui.MainWidget->setGeometry(ui.MainWidget->geometry().left(), ui.MainWidget->geometry().top(),
		ui.MainWidget->minimumWidth(), ui.MainWidget->minimumHeight());
	ui.logWidget->setMinimumSize(ui.logWidget->minimumWidth() + off.width(), ui.logWidget->minimumHeight());
	ui.logWidget->setMaximumSize(ui.logWidget->minimumWidth(), ui.logWidget->minimumHeight());
	ui.logWidget->setGeometry(ui.logWidget->geometry().left(), ui.logWidget->geometry().top() + off.height(),
		ui.logWidget->minimumWidth(), ui.logWidget->minimumHeight());
	ui.plainTextEdit->setMinimumSize(ui.plainTextEdit->minimumWidth() + off.width(), ui.plainTextEdit->minimumHeight());
	ui.plainTextEdit->setMaximumSize(ui.plainTextEdit->minimumWidth(), ui.plainTextEdit->minimumHeight());
	ui.plainTextEdit->setGeometry(ui.plainTextEdit->geometry().left(), ui.plainTextEdit->geometry().top(),
		ui.plainTextEdit->minimumWidth(), ui.plainTextEdit->minimumHeight());
	ui.infoWidget->setGeometry(ui.infoWidget->geometry().left() + off.width(), ui.infoWidget->geometry().top() + off.height(),
		ui.infoWidget->minimumWidth(), ui.infoWidget->minimumHeight());
	auto controlView = dynamic_cast<ControlView*>(g_ControlView);
	controlView->setMinimumSize(controlView->minimumWidth(), controlView->minimumHeight()+off.height());
	controlView->setMaximumSize(controlView->minimumWidth(), controlView->minimumHeight());
	controlView->setGeometry(controlView->geometry().left()+off.width(), controlView->geometry().top(),
		controlView->minimumWidth(), controlView->minimumHeight());
	auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	templateView->setMinimumSize(templateView->minimumWidth(), templateView->minimumHeight() + off.height());
	templateView->setMaximumSize(templateView->minimumWidth(), templateView->minimumHeight());
	templateView->setGeometry(templateView->geometry().left() + off.width(), templateView->geometry().top(),
		templateView->minimumWidth(), templateView->minimumHeight());

	QSize size2 = ui.MainWidget->minimumSize();
	glview->setFrameSize(size2.width(), size2.height());
	glview->setDesignResolutionSize(size2.width(), size2.height(), ResolutionPolicy::NO_BORDER);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ::SetWindowPos(((GLViewImpl*)glview)->getHwnd(), HWND_TOP, 0, 0, size2.width(), size2.height(), SWP_NOCOPYBITS);
#else
    resetCocosSize(size2.width(), size2.height());
    addConsoleInfo("%d %d %d %d", size.width(), size.height(), oldSize.width(), oldSize.height());
#endif
	if (isResetWindowSize)resetWindowSize(QSize(size2.width(), size2.height()));
	isResetWindowSize = true;
	MyLogger::getInstance()->addInfo("QTEditor::reSetSize be called, reset Editor size, the old size is :width " + QString::number(getOldWindowSize().width()) +
		" height " + QString::number(getOldWindowSize().height()) + " new size is : width " + QString::number(getNewWindowSize().width()) + " height " +
		QString::number(getNewWindowSize().height()));
}

//设置后会在下一帧切换大小
void QTEditor::reSetQTEditorBySize(QSize cocosWindowSize)
{
	auto glview = Director::getInstance()->getOpenGLView();
	QSize oldSize = QSize(glview->getFrameSize().width, glview->getFrameSize().height);
	if (oldSize != cocosWindowSize){
		if (cocosWindowSize == QSize(960, 640)){
			this->showNormal();
		}
		else{
			this->showMaximized();
		}
		isResetWindowSize = false;
	}
}

void QTEditor::showToolTip(std::string str, QPoint p)
{
	/*QScrollArea* scrollArea = (QScrollArea*)ui.SceneWidgetContents->children().at(0);
	QScrollBar* hs = scrollArea->horizontalScrollBar();
	QScrollBar* vs = scrollArea->verticalScrollBar();
	int valueh = hs->value();
	int valuev = vs->value();
	rightPos = QPoint(p.x() - valueh, p.y() - valuev);
	if (resourceView->isVisible() || propertiesWidget->isVisible()){
		rightPos.setX(rightPos.x() + 310);
	}
	QToolTip::showText(QPoint(rightPos.x(), rightPos.y() + 50), FontChina::G2U(str.c_str()));*/
}

void QTEditor::on_sizeButton_clicked()
{
	QLineEdit* width_line, *height_line;
	QObjectList list = ui.GridWidgetContents->children();
	foreach (QObject* qo, list)
	{
		QLineEdit* qw = (QLineEdit*)qo;
		QString str = qw->objectName();
		if (str == "width_Line"){
			width_line = qw;
		}else if(str == "height_Line"){
			height_line = qw;
		}
	}
	//大小无法超过你的屏幕大小 此问题跟cocos无关 
	if (width_line->text().toInt()>=480 && height_line->text().toInt()>= 320){
		auto director = Director::getInstance();
		auto glview = director->getOpenGLView();
		reSetSize(QSize(glview->getFrameSize().width, glview->getFrameSize().height), QSize(width_line->text().toInt(), height_line->text().toInt()));
		addConsoleInfo("cocos当前窗口大小:width:%d height:%d", width_line->text().toInt(), height_line->text().toInt());
	}
}

void QTEditor::on_gridButton_clicked()
{
	QMessageBox::StandardButton rb = QMessageBox::warning(this, FontChina::G2U("..."), FontChina::G2U("are you sure?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (rb == QMessageBox::No){
		return;
	}
	QLineEdit *col_line, *row_line;
	QLineEdit * gridHeight, *gridWidth;
	QObjectList list = ui.GridWidgetContents->children();
	foreach(QObject* qo, list)
	{
		QLineEdit* qw = (QLineEdit*)qo;
		QString str = qw->objectName();
		if (str == "col_Line"){
			col_line = qw;
		}
		else if (str == "row_Line"){
			row_line = qw;
		}
		else if (str == "gridHeight"){
			gridHeight = qw;
		}
		else if (str == "gridWidth"){
			gridWidth = qw;
		}
	}
	if (row_line->text().toInt() >= 5 && col_line->text().toInt() >= 5){
		HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
		scene->createGrid(row_line->text().toInt(), col_line->text().toInt(), gridHeight->text().toInt(), gridWidth->text().toInt());
	}
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	g_CocosWindowInitSize = QSize(frameSize.width, frameSize.height);
	resourceView->getTreeWidget()->initResourceDir();
	exteralView->getTreeWidget()->initResourceDir();
	animationView->getTreeWidget()->initResourceDir();
	templateView->deleteTemplateDir();
}

void QTEditor::on_actionOpen_triggered()
{
	QString openFile = QFileDialog::getOpenFileName(this, FontChina::G2U("打开一个配置文件"), MySqlLiteManager::getInstance()->getConfigFile().section("/", 0, -2), tr("Conf (*.qconf)"));
	if (!openFile.isEmpty()){
		g_openConfileDir = openFile.section("/", 0, -2).toStdString();
		fileName = openFile;
		MySqlLiteManager::getInstance()->setOpenDir(fileName.section("/", 0, -2));
		MySqlLiteManager::getInstance()->setConfigFile(fileName);
		char str[256];
		sprintf(str, "打开的配置文件为:%s", fileName.toUtf8().data());
		addConsoleInfo(str);
		QTInput* input = new QTInput();
		input->ReadConfFile(fileName);
		delete input;
	}
}

void QTEditor::on_actionSave_triggered()
{
	if (fileName.isEmpty()){
		fileName = QFileDialog::getSaveFileName(this, FontChina::G2U("保存文件"), MySqlLiteManager::getInstance()->getConfigFile().section("/", 0, -2), tr("Conf (*.qconf)"));
	}
	if (fileName.isEmpty()){
		return;
	}
	MySqlLiteManager::getInstance()->setOpenDir(fileName.section("/", 0, -2));
	saveConf(fileName);
	MySqlLiteManager::getInstance()->setConfigFile(fileName);
}

void QTEditor::on_actionSava_as_triggered()
{
	fileName = QFileDialog::getSaveFileName(this, FontChina::G2U("另存为"), MySqlLiteManager::getInstance()->getConfigFile().section("/", 0, -2), tr("Conf (*.qconf)"));
	if (fileName.isEmpty())
		return;
	on_actionSave_triggered();
}

void QTEditor::saveConf(QString filename)
{
	MyLogger::getInstance()->addInfo("the QTEditor::saveConf be call, the arg is " + filename);
	QString file1 = fileName.section(".", -1);
	bool need = false;
	if (QString::compare(file1, "QConf", Qt::CaseInsensitive) == 0){
		need = false;
	}
	else{
		need = true;
	}
	if (need){
		fileName += ".QConf";
	}

	QTOutput* output = new QTOutput();
	if (output->OutputJsonConf(fileName, true)){
		char str[256];
		sprintf(str, "保存成功:保存文件名为:%s", (fileName).toUtf8().data());
		addConsoleInfo(str);
	}
	else{
		addConsoleInfo("导出失败");
	}
	delete output;
}

void QTEditor::on_actionRelease_triggered()
{
	QString file = QFileDialog::getSaveFileName(this, FontChina::G2U("发布"), MySqlLiteManager::getInstance()->getOpenDir(), tr("Lv (*.lv)"));
	if (file.isEmpty())
		return;
	MySqlLiteManager::getInstance()->setOpenDir(file.section("/", 0, -2));
	QString save_file = file;
	QString file1 = file.section(".", -1);
	bool need = false;
	if (QString::compare(file1, "lv", Qt::CaseInsensitive) == 0){
		need = false;
	}
	else{
		need = true;
	}
	if (need){
		file += ".lv";
		save_file += ".lv";
	}
	isShowPreview = false;
	OutputTexture(file.section("/", 0, -2).toLocal8Bit().data(), file.section("/", -1).section(".", 0, -2).toLocal8Bit().data());
}

void QTEditor::on_actionPreview_triggered()
{
	MyLogger::getInstance()->addInfo("the QTEditor::on_actionPreview_triggered be called");
	QString path;
	path = QDir::currentPath() + "/autoSave";
	QDir dir;
	if (!dir.exists(path)){
		dir.mkdir(path);
	}
    
	char* name = "autoSave";
	isShowPreview = true;
	addConsoleInfo("正在保存配置文件!!!!");
	OutputTexture(path.toLocal8Bit().data(), name);
}

void QTEditor::OutputTexture(char* path, char* name)
{
	textureView->PreViewSave(path, name);
}

void QTEditor::showPreView()
{
	isShowPreview = false;
	QString command;
	if (MySqlLiteManager::getInstance()->getPreShowPath().isEmpty()){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		command = "../Castle/Castle.exe ";
#else
        command = "../Castle.app/Contents/MacOS/Castle ";
#endif
	}
	else{
		command = MySqlLiteManager::getInstance()->getPreShowPath() + " ";
	}
	QString path;
#ifdef _DEBUG
	path = QDir::currentPath() + "/autoSave";
#else
	path = QDir::currentPath() + "/../Resources/autoSave";
#endif 
	command += path;
    MyLogger::getInstance()->addInfo("the QTEditor::showPreView be called, command is " + command);
	QProcess p(0);
	p.start(command);
	p.waitForStarted();
	p.waitForFinished(20000000);
}

void QTEditor::on_actionOutputBinary_triggered()
{
	MyLogger::getInstance()->addInfo("the QTEditor::on_actionOutputBinary_triggered be called");
	//1.格式先导出基本的显示信息
	//2.导出
	QString file = QFileDialog::getSaveFileName(this, FontChina::G2U("导出二进制"), MySqlLiteManager::getInstance()->getOpenDir(), tr("Lv (*.lv)"));
	if (file.isEmpty())
		return;
	MySqlLiteManager::getInstance()->setOpenDir(file.section("/", 0, -2));
	QString save_file = file;
	QString file1 = file.section(".", -1);
	bool need = false;
	if (QString::compare(file1, "lv", Qt::CaseInsensitive) == 0){
		need = false;
	}
	else{
		need = true;
	}
	if (need){
		file += ".lv";
		save_file += ".lv";
	}
	QTOutput* output = new QTOutput();
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	std::vector<std::string> imagevec = scene->getAllImage();
	if (imagevec.size() > 0 && g_output_Name == ""){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("必须先导出资源"));
		return;
	}
	if (output->OutputFileBinary(save_file)){
		char str[256];
		sprintf(str, "导出成功:导出文件名为:%s", (file).toUtf8().data());
		addConsoleInfo(str);
	}
	else{
		addConsoleInfo("导出失败");
	}
}

void QTEditor::on_actionAddPlist_triggered()
{
	QString strdir = QFileDialog::getExistingDirectory();
	auto dir__ = strdir;
	MySqlLiteManager::getInstance()->setOpenDir(dir__);
	if (strdir == "") return;
	TexturePackerView* view = dynamic_cast<TexturePackerView*>(g_TextureView);
	view->PackerFrame(strdir);
	
}

void QTEditor::setDockWidgetVisible(QDockWidget* widget, QString objName)
{
	if (widget->isVisible()){
		widget->setVisible(false);
		QList<QAction*> list = ui.menuView->actions();
		for (int i = 0; i < list.size(); i++){
			auto action = list.at(i);
			if (action->objectName() == objName){
				action->setIcon(QIcon(":/icon/Resources/hide.ico"));
			}
		}
	}
	else{
		widget->setVisible(true);
		QList<QAction*> list = ui.menuView->actions();
		for (int i = 0; i < list.size(); i++){
			auto action = list.at(i);
			if (action->objectName() == objName){
				action->setIcon(QIcon(":/icon/Resources/show.ico"));
			}
		}
	}
}

void QTEditor::setControllViewPropertiesVisible(QDockWidget* widget, QString objName)
{
	if (widget->isVisible()){
		widget->setVisible(false);
		QList<QAction*> list = ui.menuControlManage->actions();
		for (int i = 0; i < list.size(); i++){
			auto action = list.at(i);
			if (action->objectName() == objName){
				action->setIcon(QIcon(":/icon/Resources/hide.ico"));
			}
		}
	}
	else{
		widget->setVisible(true);
		QList<QAction*> list = ui.menuControlManage->actions();
		for (int i = 0; i < list.size(); i++){
			auto action = list.at(i);
			if (action->objectName() == objName){
				action->setIcon(QIcon(":/icon/Resources/show.ico"));
			}
		}
	}
}

void QTEditor::on_actionGrid_triggered()
{
	setDockWidgetVisible(gridWidget, "actionGrid");
}

void QTEditor::on_actionResources_triggered()
{
	setDockWidgetVisible(resourceView, "actionResources");
}

void QTEditor::on_actionActions_triggered()
{
	setControllViewPropertiesVisible(controlView->getActionView(), "actionActions");
}

void QTEditor::on_actionBaseProperties_triggered()
{
	setControllViewPropertiesVisible(controlView->getBaseProperties(), "actionBaseProperties");
}

void QTEditor::on_actionMoveProperties_triggered()
{
	setControllViewPropertiesVisible(controlView->getMoveProperties(), "actionMoveProperties");
	if (controlView->getMoveProperties()->isVisible()){
		controlView->getMoveProperties()->addController();
	}
	else{
		controlView->getMoveProperties()->deleteController();
	}
}

void QTEditor::on_actionBoxProperties_triggered()
{
	setControllViewPropertiesVisible(controlView->getBoxProperties(), "actionBoxProperties");
	if (controlView->getBoxProperties()->isVisible()){
		controlView->getBoxProperties()->addController();
		controlView->getCircleProperties()->setVisible(false);
		controlView->getCircleProperties()->deleteController();
	}
	else{
		controlView->getBoxProperties()->deleteController();
	}
}

void QTEditor::on_actionCircleProperties_triggered()
{
	setControllViewPropertiesVisible(controlView->getCircleProperties(), "actionCircleProperties");
	if (controlView->getCircleProperties()->isVisible()){
		controlView->getCircleProperties()->addController();
		controlView->getBoxProperties()->setVisible(false);
		controlView->getBoxProperties()->deleteController();
	}
	else{
		controlView->getCircleProperties()->deleteController();
	}
}

void QTEditor::on_actionTrigger_triggered()
{
	if (controlView->getSimplerTriggerProperties()->checkSprite()){
		setControllViewPropertiesVisible(controlView->getSimplerTriggerProperties(), "actionTrigger");
		if (controlView->getSimplerTriggerProperties()->isVisible()){
			controlView->getSimplerTriggerProperties()->addController();
		}
		else{
			controlView->getSimplerTriggerProperties()->deleteController();
		}
	}
}

void QTEditor::on_actionEntityProperties_triggered()
{
	setControllViewPropertiesVisible(controlView->getEntityProperties(), "actionEntityProperties");
	if (controlView->getEntityProperties()->isVisible()){
		controlView->getEntityProperties()->addController();
	}
	else{
		controlView->getEntityProperties()->deleteController();
	}
}

void QTEditor::on_actionMonsterProperties_triggered()
{
	if (controlView->getMonsterProperties()->checkSprite()){
		setControllViewPropertiesVisible(controlView->getMonsterProperties(), "actionMonsterProperties");
		if (controlView->getMonsterProperties()->isVisible()){
			controlView->getMonsterProperties()->addController();
		}
		else{
			controlView->getMonsterProperties()->deleteController();
		}
	}
}

void QTEditor::on_actionMonsterDropProperties_triggered()
{
	if (controlView->getMonsterDropProperties()->checkSprite()){
		setControllViewPropertiesVisible(controlView->getMonsterDropProperties(), "actionMonsterDropProperties");
		if (controlView->getMonsterDropProperties()->isVisible()){
			controlView->getMonsterDropProperties()->addController();
		}
		else{
			controlView->getMonsterDropProperties()->deleteController();
		}
	}	
}

void QTEditor::on_actionAiProperties_triggered()
{
	if (controlView->getAIProperties()->checkSprite()){
		setControllViewPropertiesVisible(controlView->getAIProperties(), "actionAiProperties");
		if (controlView->getAIProperties()->isVisible()){
			controlView->getAIProperties()->addController();
		}
		else{
			controlView->getAIProperties()->deleteController();
		}
	}
}

void QTEditor::on_actionFilterProperties_triggered()
{
	setControllViewPropertiesVisible(controlView->getFilterProperties(), "actionFilterProperties");
	if (controlView->getFilterProperties()->isVisible()){
		controlView->getFilterProperties()->addController();
	}
	else{
		controlView->getFilterProperties()->deleteController();
	}
}

void QTEditor::on_actionFbx_conv_triggered()
{
	QStringList openFiles = QFileDialog::getOpenFileNames(this, FontChina::G2U("fbx文件选择"), MySqlLiteManager::getInstance()->getOpenDir(), tr("FBX file (*.fbx)"));
	for (int i = 0; i < openFiles.size(); i++){
		QString openFile = openFiles.at(i);
		if (!openFile.isEmpty()){
			MySqlLiteManager::getInstance()->setOpenDir(openFile.section("/", 0, -2));
			QString command = "../TexturePacker/fbx-conv.exe";
			command += " -a ";
			command += openFile;
			QProcess p(0);
			p.start(command);
			p.waitForStarted();
			if (p.waitForFinished()){
				if (i == openFiles.size() - 1){
					addConsoleInfo("解析完成，请点击文件查看");
				}
			}
		}
	}
}

void QTEditor::on_actionPreShowPath_triggered()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	QString file = QFileDialog::getOpenFileName(this, FontChina::G2U("预览文件选择"), MySqlLiteManager::getInstance()->getOpenDir(), tr("EXE file (*.exe)"));
#else
    QString file = QFileDialog::getOpenFileName(this, FontChina::G2U("预览文件选择"), MySqlLiteManager::getInstance()->getOpenDir(), tr("App file (*)"));
#endif
	if (!file.isEmpty()){
		MySqlLiteManager::getInstance()->setPreShowPath(file);
	}
}

void QTEditor::on_actionKeyboardMapping_triggered()
{
	keyboardDialog->show();
}

void QTEditor::on_actionLayerManager_triggered()
{
	setDockWidgetVisible(layerManagerWidget, "actionLayerManager");
}

void QTEditor::on_actionTexturePacker_triggered()
{
	setDockWidgetVisible(textureView, "actionTexturePacker");
}

void QTEditor::on_actionTemplate_triggered()
{
	setDockWidgetVisible(templateView, "actionTemplate");
}

void QTEditor::on_actionExteralView_triggered()
{
	setDockWidgetVisible(exteralView, "actionExteralView");
}

void QTEditor::on_actionConfigView_triggered()
{
	setDockWidgetVisible(configView, "actionConfigView");
}

void QTEditor::on_actionAnimationView_triggered()
{
	setDockWidgetVisible(animationView, "actionAnimationView");
}

void QTEditor::on_actionVersion_triggered()
{
	QMessageBox message(QMessageBox::NoIcon, FontChina::G2U("版本信息"), FontChina::G2U("Version 0.33.final"));
	message.setIconPixmap(QPixmap(":/icon/Resources/Sprite.png"));
	message.exec();
}

void QTEditor::on_actionHelp_triggered()
{
	/*QString path = QDir::currentPath() + "/help.txt";
	QString content = readFile(path, false);
	QMessageBox message(QMessageBox::NoIcon, FontChina::G2U("帮助信息"), content);
	message.setIconPixmap(QPixmap(":/icon/Resources/Sprite.png"));
	message.exec();*/
	QProcess p;
	QStringList env;
	env << "--smart-update";
	env << "--format";
	env << "cocos2d";
	env << "--multipack";
	env << "--data";
	env << "E:/cocos2d-x-3.9/projects/GameEditor/res/test{n}.plist";
	/*env << "--maxrects-heuristics best";
	env << "--enable-rotation";
	env << "--shape-padding 2";
	env << "--scale 1";
	env << "--extrude 1";
	env << "--max-size 2048";
	env << "--size-constraints AnySize";
	env << "--opt RGBA8888";
	env << "--trim";
	env << "--texture-format png";*/
	env << "--sheet";
	env << "E:/cocos2d-x-3.9/projects/GameEditor/res/test{n}.png";
	QString path__ = "ResList/";
	for (size_t i = 0; i < 2; i++){
		env << (path__+QString::number(i) + ".png");
	}
	p.start("E:/cocos2d-x-3.9/projects/GameEditor/QTEditor/TexturePacker/bin/TexturePacker.exe", env);
	p.waitForStarted();
	if (p.waitForFinished()){
		addConsoleInfo("%s", p.readAllStandardOutput().toStdString().c_str());
	}
}

void QTEditor::on_actionAddModel_triggered()
{
	QStringList openFiles = QFileDialog::getOpenFileNames(this, FontChina::G2U("添加模型文件"), MySqlLiteManager::getInstance()->getOpenDir(), tr("Model Files (*.c3b *.c3t)"));
	if (openFiles.size() > 0){
		LoadExterModel* model = new LoadExterModel();
		HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
		model->loadRole(scene, openFiles.at(0).toStdString());
		delete model;
	}
}

void QTEditor::on_createGridButton_clicked()
{
	QObjectList list = ui.GridWidgetContents->children();
	QComboBox* combox;
	foreach(QObject* qo, list){
		QWidget* qw = (QWidget*)qo;
		QString str = qw->objectName();
		if (str == "createComboBox"){
			combox = dynamic_cast<QComboBox*>(qw);
			break;
		}
	}
	int index = combox->currentIndex();
	switch (index){
	case 0:
	{
			   //创建方体
			   HelloWorld* scene = (HelloWorld*)g_scene;
			   scene->setType(Type_Box);
			   break;
	}
	case 1:
	{
			  //创建斜体
			  HelloWorld* scene = (HelloWorld*)g_scene;
			  scene->setType(Type_Italic);
			  break;
	}
	case 2:
	{
			  //创建浮体
			  HelloWorld* scene = (HelloWorld*)g_scene;
			  scene->setType(Type_Fluid);
			  break;
	}
	}
}

//右键弹出菜单操作
void QTEditor::contextMenuEvent(QContextMenuEvent* e)
{
	
}

void QTEditor::myCustomEvent(QPoint p)
{
	
}

void QTEditor::on_deleteGridButton_clicked()
{
	HelloWorld* scene = (HelloWorld*)g_scene;
	scene->setType(Type_Delete_Block);
	setCocCursor(Cursor_Type::SubBlock);
}

void QTEditor::on_directionButton_clicked()
{
	QObjectList list = ui.GridWidgetContents->children();
	QComboBox* combox;
	foreach(QObject* qo, list){
		QWidget* qw = (QWidget*)qo;
		QString str = qw->objectName();
		if (str == "directSelect"){
			combox = dynamic_cast<QComboBox*>(qw);
			break;
		}
	}
	int index = combox->currentIndex();
	HelloWorld* scene = (HelloWorld*)g_scene;
	scene->setType(Type_Select_Direction, index);
	setCocCursor(Cursor_Type::BlockDirection);
}

void QTEditor::on_addGridButton_clicked()
{
	QObjectList list = ui.GridWidgetContents->children();
	QComboBox* combox;
	QLineEdit* lineEdit;
	QLineEdit* row_Line;
	QLineEdit* col_Line;
	foreach(QObject* qo, list){
		QWidget* qw = (QWidget*)qo;
		QString str = qw->objectName();
		if (str == "addDirectionBox"){
			combox = dynamic_cast<QComboBox*>(qw);
			continue;
		}
		if (str == "addGridLine"){
			lineEdit = dynamic_cast<QLineEdit*>(qw);
			continue;
		}
		if (str == "row_Line"){
			row_Line = dynamic_cast<QLineEdit*>(qw);
			continue;
		}
		if (str == "col_Line"){
			col_Line = dynamic_cast<QLineEdit*>(qw);
			continue;
		}
	}
	int index = combox->currentIndex();
	if (lineEdit->text() == "" || lineEdit->text().toInt() == 0){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("必须填入数字"));
		return;
	}
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	char str[256];
	int width;
	int height;
	switch (index){
	case 0:
	{
			  int y = scene->gridManagerLayer->getGridSpriteLayer()->getMinY();
			  int gridCount = scene->gridManagerLayer->getGridSpriteLayer()->getGridCount();
			  if (lineEdit->text().toInt() < 0 && gridCount > 0 && y < -lineEdit->text().toInt() - 1){
				  QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("无法执行当前操作,因为操作后有网格不可见"));
				  return;
			  }
			  scene->row = scene->row + lineEdit->text().toInt();
			  row_Line->setText(QString::number(scene->row));
	}
		break;
	case 1:
	{
		int y = scene->gridManagerLayer->getGridSpriteLayer()->getMaxY();
		int gridCount = scene->gridManagerLayer->getGridSpriteLayer()->getGridCount();
			  int row = scene->row + lineEdit->text().toInt();
			  if (lineEdit->text().toInt() < 0 && gridCount > 0 && y > row){
				  QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("无法执行当前操作,因为操作后有网格不可见"));
				  return;
			  }
			  scene->row = scene->row + lineEdit->text().toInt();
			  row_Line->setText(QString::number(scene->row));
	}
		break;
	case 2:
	{
		int x = scene->gridManagerLayer->getGridSpriteLayer()->getMinX();
		int gridCount = scene->gridManagerLayer->getGridSpriteLayer()->getGridCount();
			  if (lineEdit->text().toInt() < 0 && gridCount > 0 && x < -lineEdit->text().toInt() - 1){
				  QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("无法执行当前操作,因为操作后有网格不可见"));
				  return;
			  }
			  scene->col = scene->col + lineEdit->text().toInt();
			  col_Line->setText(QString::number(scene->col));
	}
		break;
	case 3:
	{
		int x = scene->gridManagerLayer->getGridSpriteLayer()->getMaxX();
		int gridCount = scene->gridManagerLayer->getGridSpriteLayer()->getGridCount();
			  int col = scene->col + lineEdit->text().toInt();
			  if (lineEdit->text().toInt() < 0 && gridCount > 0 && x > col){
				  QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("无法执行当前操作,因为操作后有网格不可见"));
				  return;
			  }
			  scene->col = scene->col + lineEdit->text().toInt();
			  col_Line->setText(QString::number(scene->col));
	}
		break;
	}
	scene->addGrid(lineEdit->text().toInt(), index);
}

QTEditor::~QTEditor()
{

}
