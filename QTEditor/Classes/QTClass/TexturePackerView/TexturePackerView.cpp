#include "TexturePackerView.h"
#include "Global.h"
#include <qprocess.h>
#include "FontChina.h"
#include <qdir.h>
#include "FontChina.h"
#include <QLayout>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include "HelloWorldScene.h"
#include "qthread.h"
#include <qtimer.h>
#include "QTOutput.h"
#include "qteditor.h"
#include "ImageSpriteLayer.h"
#include "CocEventCenter.h"
#include <QDomDocument>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextstream.h>
#include "PlistTool.h"
#include <QRunnable>
#include <QThreadPool>
#include <QDir>
#include "MyLogger.h"
#include "MySqlLiteManager.h"
#include "GlobalConstant.h"
#include "RunTimePackerThread.h"
#include "ConcurrentTaskQueue.h"

#define TEXTURE_MAX_SIZE 4194304
#define TEXTURE_MAX_TIME 3000000

static QString s_command = "";

//http://blog.csdn.net/hn307165411/article/details/6858614 cmd
//http://blog.justbilt.com/2013/12/12/use_tp_on_command_line/
TexturePackerView::TexturePackerView(char* title, QSize size)
:BaseDockWidget(title, size)
{
	QLabel* label5 = new QLabel(tr(FontChina::G2U("打包选项:")));
	QLabel* label1 = new QLabel(tr(FontChina::G2U("选择导出平台:")));
	QLabel* label2 = new QLabel(tr(FontChina::G2U("导出文件名:")));
	QLabel* label3 = new QLabel(tr(FontChina::G2U("导出路径")));
	QLabel* label4 = new QLabel(tr(FontChina::G2U("导出格式")));
	platform = new QComboBox();
	platform->addItem("android");
	platform->addItem("ios");
	platform->addItem("win32");
	format = new QComboBox();
	format->addItem("png");
	format->addItem("pvr2ccz");
	outputName = new QTextEdit();
	outputName->setMaximumHeight(35);
	outputDir = new QPlainTextEdit();
	outputDir->setMaximumHeight(70);
	outputDir->setEnabled(false);
	sureButton = new QPushButton(tr(FontChina::G2U("打包")));
	dirButton = new QPushButton(tr(FontChina::G2U("路径")));
	dirButton->setIcon(QIcon(":/icon/Resources/add2.ico"));
	QHBoxLayout* hlayout = new QHBoxLayout();
	QHBoxLayout* hlayout2 = new QHBoxLayout();
	QHBoxLayout* hlayout3 = new QHBoxLayout();
	//QHBoxLayout* hlayout4 = new QHBoxLayout();
	QVBoxLayout* vlayout = new QVBoxLayout();
	hlayout->addWidget(label1);
	hlayout->addWidget(platform);
	hlayout->addWidget(label4);
	hlayout->addWidget(format);
	hlayout2->addWidget(label2);
	hlayout2->addWidget(outputName);
	hlayout3->addWidget(label3);
	hlayout3->addWidget(dirButton);
	//hlayout4->addWidget(label4);
	//hlayout4->addWidget(format);
	vlayout->addWidget(label5);
	vlayout->addLayout(hlayout);
	vlayout->addLayout(hlayout2);
	//vlayout->addLayout(hlayout4);
	vlayout->addLayout(hlayout3);
	vlayout->addWidget(outputDir);
	vlayout->addWidget(sureButton);



	QLabel* label6 = new QLabel(FontChina::G2U("解大图"));
	QHBoxLayout* unphlayout = new QHBoxLayout;
	QHBoxLayout* unphlayout2 = new QHBoxLayout;
	chooseFile = new QPushButton(FontChina::G2U("选择plist文件"));
	unpackerFile = new QLabel("");
	chooseDir = new QPushButton(FontChina::G2U("选择导出目录"));
	unpackerDir = new QLabel("");
	unPackerButton = new QPushButton(FontChina::G2U("unPacker"));
	unphlayout->addWidget(chooseFile);
	unphlayout->addWidget(unpackerFile);
	unphlayout2->addWidget(chooseDir);
	unphlayout2->addWidget(unpackerDir);
	vlayout->addWidget(label6);
	vlayout->addLayout(unphlayout);
	vlayout->addLayout(unphlayout2);
	vlayout->addWidget(unPackerButton);

	widget->setLayout(vlayout);
	connect(dirButton, SIGNAL(clicked()), this, SLOT(on_dirButton_clicked()));
	connect(sureButton, SIGNAL(clicked()), this, SLOT(on_sureButton_clicked()));
	connect(chooseFile, SIGNAL(clicked()), this, SLOT(on_chooseFile_clicked()));
	connect(chooseDir, SIGNAL(clicked()), this, SLOT(on_chooseDir_clicked()));
	connect(unPackerButton, SIGNAL(clicked()), this, SLOT(on_unPackerButton_clicked()));
}

TexturePackerView::~TexturePackerView()
{

}

void TexturePackerView::on_dirButton_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, FontChina::G2U("请选择一个目录"), MySqlLiteManager::getInstance()->getOpenDir());
	MySqlLiteManager::getInstance()->setOpenDir(dir);
	g_output_Dir = dir.toUtf8().data();
	outputDir->setPlainText(FontChina::G2U(dir.toUtf8().data()));
}

void TexturePackerView::on_chooseFile_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, FontChina::G2U("打开一个plist文件"), MySqlLiteManager::getInstance()->getOpenDir(), tr("Plist (*.plist)"));
	if (file != ""){
		QString suffix = file.section(".", -1);
		if (QString::compare(suffix, QString("plist"), Qt::CaseInsensitive) != 0){
			QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("请选择正确的plist文件"));
			return;
		}
		unpackerFile->setText(file);
	}
}

void TexturePackerView::on_chooseDir_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, FontChina::G2U("请选择一个目录"), MySqlLiteManager::getInstance()->getOpenDir());
	MySqlLiteManager::getInstance()->setOpenDir(dir);
	unpackerDir->setText(FontChina::G2U(dir.toUtf8().data()));
}

void TexturePackerView::on_unPackerButton_clicked()
{
	if (unpackerFile->text().isEmpty() || unpackerDir->text().isEmpty()){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("请选择plist文件和导出目录"));
		return;
	}
	std::thread t1(&TexturePackerView::ThreadUnPacker, this);
	t1.detach();
}

void TexturePackerView::ThreadUnPacker()
{
	PlistTool *tool = new PlistTool();
	std::vector<std::string> vec;
	vec.push_back(unpackerFile->text().toLocal8Bit().data());
	tool->setSavePath((unpackerDir->text() + "/").toLocal8Bit().data());
	tool->addUnpackList(vec);
	//addConsoleInfo((unpackerDir->text() + "/").toLocal8Bit().data());
	tool->startUnpack([](){
		addConsoleInfo("解图完成!");
	});
}

bool TexturePackerView::PackerFrame(QString strdir)
{
	QDir dir(strdir);
	QString outputDir = strdir;
	QString outputName = strdir.section("/", -1);
	//第一步打包
	if (dir.exists()){
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		QStringList filters;
		filters << QString("*.jpeg") << QString("*.jpg") << QString("*.png") << QString("*.tiff") << QString("*.bmp");
		dir.setNameFilters(filters);
		QFileInfoList list = dir.entryInfoList();
		int file_count = list.count();
		if (file_count < 0){
			return false;
		}
		QStringList string_list;
		for (int i = 0; i < file_count; i++){
			QFileInfo file_info = list.at(i);
			QString path = file_info.absoluteFilePath();
			string_list << path;
		}
		
		prePacker(string_list, outputName, outputDir);
	}
	else{
		return false;
	}
	//第二步生成配置文件
	QDir dirDir(strdir);
	if (dirDir.exists()){
		dirDir.setFilter(QDir::Dirs);
		QFileInfoList list = dirDir.entryInfoList();
		int file_count = list.count();
		if (file_count < 0){
			return false;
		}
		QStringList string_list;
		for (int i = 2; i < file_count; i++){
			QFileInfo file_info = list.at(i);
			QString path = file_info.absoluteFilePath();
			addFrameConfigFile(path, outputDir + "/" + outputName + "0.plist", outputDir + "/" + outputName + "0.png", path.section("/", -1));
		}
	}
	else{
		return false;
	}
	return true;
}

void TexturePackerView::addFrameConfigFile(QString strdir, QString parentplist, QString parentpng, QString filename)
{
	MyLogger::getInstance()->addInfo("the TexturePackerView::addFrameConfigFile be called, the args is " + strdir + " " + parentpng + " " + filename);
	QDir dir(strdir);
	if (dir.exists()){
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		QStringList filters;
		filters << QString("*.jpeg") << QString("*.jpg") << QString("*.png") << QString("*.tiff") << QString("*.bmp");
		dir.setNameFilters(filters);
		QFileInfoList list = dir.entryInfoList();
		int file_count = list.count();
		QStringList string_list;
		for (int i = 0; i < file_count; i++){
			QFileInfo file_info = list.at(i);
			QString path = file_info.absoluteFilePath();
			string_list << path;
		}
		QVariantMap data;
		data.insert("type", "frame");
		data.insert("parentplist", parentplist);
		data.insert("parentpng", parentpng);
		data.insert("sourceDir", strdir.section("/", 0, -3));
		QVariantList Vlist;
		for (int i = 0; i < string_list.size(); i++){
			Vlist.push_back(string_list.at(i).section("/", -1));
		}
		data.insert("png", Vlist);
		QJsonDocument jsonDocument = QJsonDocument::fromVariant(data);
		QFile sFile(strdir + "/" + filename + ".json");
		if (!sFile.open(QIODevice::WriteOnly | QIODevice::Text)){
			return;
		}
		QTextStream out(&sFile);
		out << jsonDocument.toJson();
		sFile.close();
	}
}

int TexturePackerView::getPngCount(QString outdir, QString outName)
{
	QDir dir(outdir);
 	if (dir.exists()){
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		//string_list只有png和jpg格式的文件
		QStringList string_list;
		for (int i = 0; i < list.size(); i++)
		{
			QFileInfo file_info = list.at(i);
			QString suffix = file_info.suffix();
			if (QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0)
			{
				QString absolute_file_path = file_info.absoluteFilePath();
				string_list.append(absolute_file_path);
			}
			else if (QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0){
				QString absolute_file_path = file_info.absoluteFilePath();
				string_list.append(absolute_file_path);
			}
		}
		int num = 0;
		for (int i = 0; i < string_list.size(); i++){
			QString str = string_list.at(i).section("/", -1).section("_", 0, -2);
			str += "_";
			if (str == outName){
				num++;
			}
		}
		return num;
	}
	return 0;
}

void TexturePackerView::prePackerThread()
{
	QProcess p(0);
	p.start(s_command);
	p.waitForStarted();
	if (p.waitForFinished(TEXTURE_MAX_TIME)){
		addConsoleInfo("异步打包成功");
	}
}

//场景图片异步打包 包含角色资源
void TexturePackerView::PackerAsyn(QStringList fileList, QString outputName, QString outputDir, Node* layer, bool needShowLabel, QString platform, QString format)
{
	{
		MyLogger::getInstance()->addInfo("the TexturePackerView::PackerAsyn be called, the args is " + outputName + " " + outputDir +
			" filelist size is " + QString::number(fileList.size()));
		if (fileList.size() == 0){
			MyLogger::getInstance()->addError("TexturePackerView::PackerAsyn error filelist is 0 outputName is " + outputName + " outputDir is "
				+ outputDir);
		}
		if (outputName == "" || outputDir == ""){
			MyLogger::getInstance()->addError("TexturePackerView::PackerAsyn error f outputName is null outputDir is null");
		}
		auto scene = static_cast<HelloWorld*>(g_scene);
		scene->setPackerLabelVisible(needShowLabel);
		QDir dir;
		if (!dir.exists(outputDir))
			dir.mkdir(outputDir);
		ImageSpriteLayer* _layer = static_cast<ImageSpriteLayer*>(layer);
		RunTimePackerThread* thread_ = new RunTimePackerThread;
		QStringList fileList2;
		for (int i = 0; i < fileList.size(); i++){
			fileList2.push_back(fileList.at(i).section("/", -3));
		}
		thread_->generateCommand(fileList2, outputName, outputDir, layer, format);
		ConcurrentTaskQueue::getInstance()->getTaskQueue(_layer->getTagName())->setCheck(true);
		ConcurrentTaskQueue::getInstance()->getTaskQueue(_layer->getTagName())->addTask(thread_);
	}
}

bool TexturePackerView::prePacker(QStringList fileList, QString name, QString dir, QString plat, QString form)
{
	QString outputDir = dir;
	QString outputName = name;
	QString format = form;
	QString platform = plat;
	{
		MyLogger::getInstance()->addInfo("the TexturePackerView::prePacker be called, the args is " + outputName + " " + outputDir);
		QString scale = "1";
		s_command = g_textureCMD.c_str();
		s_command += " --smart-update";
		//格式
		s_command += " --format cocos2d";
		//plist文件
		s_command += " --multipack";
		s_command += " --data " + outputDir + "/" + outputName + "{n}.plist";
		s_command += " --maxrects-heuristics best";
		s_command += " --enable-rotation";
		s_command += " --scale " + scale;
		s_command += " --shape-padding 2";
		s_command += " --extrude 1";
		//最大大小包括max-width max-height
		s_command += " --max-size 2048";
		s_command += " --size-constraints AnySize";
		//图片输出格式
		if (platform == "android"){
			s_command += " --opt RGBA8888";
		}
		else if (platform == "win32"){
			s_command += " --opt RGBA8888";
		}
		else if (platform == "ios"){
			s_command += " --opt RGBA8888";
		}
		s_command += " --trim ";
		if (format == "pvr2ccz"){
			s_command += " --algorithm Base";
			s_command += " --texture-format " + QString("pvr2ccz");
			s_command += " --sheet " + outputDir + "/" + outputName + "{n}.pvr2ccz";
		}
		else if (format == "png"){
			s_command += " --texture-format " + QString("png");
			s_command += " --sheet " + outputDir + "/" + outputName + "{n}.png";
		}
		s_command += " ";
		for (int i = 0; i < fileList.size(); i++){
			s_command += fileList.at(i) + " ";
		}
		//addConsoleInfo(s_command.toLocal8Bit().data());
	}
	std::thread t1(&TexturePackerView::prePackerThread, this);
	t1.detach();
	return true;
}

//http://pngquant.com/
//图片质量保存完好
bool TexturePackerView::ImageOptimize(QString file, QString outputName, QString outputDir)
{
	QProcess p(0);
	QString command = g_imageOptimizeCMD.c_str();
	command += " --force";
	command += " --output " + outputDir + "/" + outputName;
	command += " --speed 1";
	command += " --quality 65-80";
	command += " " + file;
	p.start(command);
	p.waitForStarted();
	return p.waitForFinished();
}

void TexturePackerView::runPacker(QString command, int type_)
{
	QProcess p(0);
	p.start(command);
	p.waitForStarted();
	if (p.waitForFinished(TEXTURE_MAX_TIME)){
		g_mutex.lock();
		switch (type_){
		case 0:
			OutputInfo::getInstance()->addDynamicCount();
			break;
		case 1:
			break;
		case 2:
			OutputInfo::getInstance()->addStaticCount();
			break;
		}
		checkOutputCount();
		g_mutex.unlock();
	}
}

void TexturePackerView::checkOutputCount()
{
	OutputInfo::getInstance()->addCurrentOutputCount();
	if (OutputInfo::getInstance()->getCurrentOutputCount() >= OutputInfo::getInstance()->getOutputCount()){
		//ImageOptimize(outName + "_dynamic0.png", "output.png", outdir);
		DeleteAllDirectoryNoFile(OutputInfo::getInstance()->getOutputDir());
		auto scene = dynamic_cast<HelloWorld*>(g_scene);
		scene->getForeManager()->setAllyLayerNeedUpdateOutputState(false);
		switch (checkFile(OutputInfo::getInstance()->getOutputDir(), OutputInfo::getInstance()->getOutputName())){
		case 0:
			if (OutputInfo::getInstance()->getShowOutputState()){
				auto curtime_ = time(0);
				auto time_ = curtime_ - g_curTime;
				g_curTime = curtime_;
				MyLogger::getInstance()->addInfo("the TexturePackerView::checkOutputCount be called, the result is preShow");
				QTOutput* output = new QTOutput;
				QString saveFile = OutputInfo::getInstance()->getOutputDir() + "/" + "config.lv";
				g_output_Name = OutputInfo::getInstance()->getOutputName().toLocal8Bit().data();
				if (output->OutputFileBinary(saveFile)){
					QString mapDirString = OutputInfo::getInstance()->getOutputDir() + "/map";
					QDir mapDir = mapDirString;
					if (!mapDir.exists()){
						mapDir.mkdir(mapDirString);
					}
					createDirAndCopyFile(mapDirString, getDirRootFile(OutputInfo::getInstance()->getOutputDir()));
					addConsoleInfo("保存成功");
					auto curtime_ = time(0);
					auto time_ = curtime_ - g_curTime;
					g_curTime = curtime_;
					auto editor = dynamic_cast<QTEditor*>(g_editor);
					if (editor->isShowPreview){
						editor->showPreView();
					}
				}
			}
			else{
				addConsoleInfo("导出成功！");
				g_output_Name = OutputInfo::getInstance()->getOutputName().toLocal8Bit().data();
			}
			break;
		case 1:
			addConsoleInfo("文件夹不存在");
			break;
		case 2:
			addConsoleInfo("静态层使用的文件过多");
			break;
		case 3:
			addConsoleInfo("文件夹下没有文件");
			break;
		case 4:
			addConsoleInfo("导出失败程序内部出错");
			break;
		}
	}
}

bool TexturePackerView::outputPacker(QString resDir, QString name, QString dir, QString plat, QString form, int _type)
{
	QString outputDir = dir;
	QString outputName = name;
	QString format = form;
	QString platform = plat;
	QString command;
	{
		MyLogger::getInstance()->addInfo("the TexturePackerView::outputPacker be called, "
			+ outputName + " " + outputDir);
		QString scale = "1";
		command = g_textureCMD.c_str();
		command += " --smart-update";
		//格式
		command += " --format cocos2d";
		//plist文件
		command += " --multipack";
		command += " --data " + outputDir + "/" + outputName + "{n}.plist";
		command += " --maxrects-heuristics best";
		command += " --enable-rotation";
		command += " --scale " + scale;
		command += " --shape-padding 2";
		command += " --extrude 1";
		//最大大小包括max-width max-height
		command += " --max-size 2048";
		command += " --size-constraints AnySize";
		//图片输出格式
		if (platform == "android"){
			command += " --opt RGBA8888";
		}
		else if (platform == "win32"){
			command += " --opt RGBA8888";
		}
		else if (platform == "ios"){
			command += " --opt RGBA8888";
		}
		command += " --trim ";
		if (format == "pvr2ccz"){
			command += " --algorithm Base";
			command += " --texture-format " + QString("pvr2ccz");
			command += " --sheet " + outputDir + "/" + outputName + "{n}.pvr2ccz";
		}
		else if (format == "png"){
			command += " --texture-format " + QString("png");
			command += " --sheet " + outputDir + "/" + outputName + "{n}.png";
		}
		command += " ";
		command += resDir;
	}
	std::thread t1(&TexturePackerView::runPacker, this, command, _type);
	t1.detach();
	return true;
}

int TexturePackerView::outputTest(QString resDir, QString name, QString dir, QString plat, QString form, int _type)
{
	QString outputDir = dir;
	QString outputName = name;
	QString format = form;
	QString platform = plat;
	QString command;
	{
		MyLogger::getInstance()->addInfo("the TexturePackerView::outputTest be called, the filelist size is "
			+ outputName + " " + outputDir);
		QString scale = "1";
		command = g_textureCMD.c_str();
		command += " --smart-update";
		//格式
		command += " --format cocos2d";
		//plist文件
		command += " --multipack";
		command += " --data " + outputDir + "/" + outputName + "{n}.plist";
		command += " --maxrects-heuristics best";
		command += " --enable-rotation";
		command += " --scale " + scale;
		command += " --shape-padding 2";
		command += " --extrude 1";
		//最大大小包括max-width max-height
		command += " --max-size 2048";
		command += " --size-constraints AnySize";
		//图片输出格式
		if (platform == "android"){
			command += " --opt RGBA8888";
		}
		else if (platform == "win32"){
			command += " --opt RGBA8888";
		}
		else if (platform == "ios"){
			command += " --opt RGBA8888";
		}
		command += " --trim ";
		if (format == "pvr2ccz"){
			command += " --algorithm Base";
			command += " --texture-format " + QString("pvr2ccz");
			command += " --sheet " + outputDir + "/" + outputName + "{n}.pvr2ccz";
		}
		else if (format == "png"){
			command += " --texture-format " + QString("png");
			command += " --sheet " + outputDir + "/" + outputName + "{n}.png";
		}
		command += " ";
		command += resDir;
		MyLogger::getInstance()->addError(command.toStdString());
	}
	return outputTestCommnad(command, outputName, outputDir);
}

int TexturePackerView::outputTestCommnad(QString command, QString name, QString dir)
{
	QProcess p(0);
	p.start(command);
	p.waitForStarted();
	p.waitForFinished(TEXTURE_MAX_TIME);
	addConsoleInfo("OutputCommandInfo:\n%s", p.readAllStandardOutput().toStdString().c_str());
	g_mutex.lock();
	int count = getOutputTestPngCount(name, dir);
	g_mutex.unlock();
	return count;
}		

int TexturePackerView::getOutputTestPngCount(QString name, QString outdir)
{
	int count = 0;
	QDir dir(outdir);
	if (dir.exists()){
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); i++)
		{
			QFileInfo file_info = list.at(i);
			if (file_info.fileName().contains(name) && QString::compare(file_info.suffix(), QString("plist"), Qt::CaseInsensitive) != 0){
				count++;
			}
		}
	}
	return count;
}

void TexturePackerView::addToVec(std::vector<std::string>& sourceVec, std::vector<std::string>& dstVec)
{
	for (int i = 0; i < dstVec.size(); i++){
		auto dstr = dstVec.at(i);
		dstr = dstr.substr(dstr.find_last_of("/") + 1, dstr.length());
		bool repeat = false;
		for (int k = 0; k < sourceVec.size(); k++){
			auto sstr = sourceVec.at(k);
			sstr = sstr.substr(sstr.find_last_of("/") + 1, sstr.length());
			if (sstr == dstr)repeat = true;
		}
		if (!repeat)sourceVec.push_back(dstVec.at(i));
	}
}

std::vector<std::string> TexturePackerView::subToVec(std::vector<ImageSpriteLayer*>& vecLayer, int startIndex, int endedIndex)
{
	std::vector<std::string> vec;
	for (int i = startIndex; i < vecLayer.size() && i <= endedIndex; i++){
		auto curVec = vecLayer.at(i)->getAllResourceImage();
		addToVec(vec, curVec);
	}
	return vec;
}

void TexturePackerView::on_sureButton_clicked()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	if (outputDir->toPlainText() == ""){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("请先选择导出目录"));
		return;
	}
	if (outputName->toPlainText() == ""){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("请选择导出文件名"));
		return;
	}
	QStringList list = getUsingImage();
	if (list.size() == 0){
		QMessageBox::information(this, FontChina::G2U("警告"), FontChina::G2U("当前未使用任何图片"));
		return;
	}
	OutputInfo::getInstance()->initData();
	OutputInfo::getInstance()->setOutputDir(outputDir->toPlainText());
	OutputInfo::getInstance()->setOutputName(outputName->toPlainText());
	QString form;
	QString plat;
	switch (format->currentIndex()){
	case 0:
		form = "png";
		break;
	case 1:
		form = "pvr2ccz";
		break;
	}
	switch (platform->currentIndex()){
	case 0:
		plat = "android";
		break;
	case 1:
		plat = "ios";
		break;
	case 2:
		plat = "win32";
		break;
	}
	deleteVecLayer();
	QStringList dynamicList = convertToStringList(scene->getAllImageNoStatic());
	if (dynamicList.size() > 0){
		OutputInfo::getInstance()->addOutputCount();
	}
	outputStaticLayer();
	if (dynamicList.size() > 0){
		outputPacker(copyFileAndReturnDir(dynamicList, OutputInfo::getInstance()->getOutputDir(), "dynamic"), OutputInfo::getInstance()->getOutputName() + "_dynamic_", OutputInfo::getInstance()->getOutputDir(), plat, form, 0);
	}
}

unsigned int TexturePackerView::getImagesSize(std::vector<std::string>& vec)
{
	unsigned int size = 0;
	for (int i = 0; i < vec.size(); i++){
		auto image = new CCImage();
		image->initWithImageFile(vec.at(i));
		int width = image->getWidth()+1;
		int height = image->getHeight() + 1;
		size += width* height;
		image->release();
	}
	return size;
}

int TexturePackerView::writeAStaticLayer(std::vector<ImageSpriteLayer*>& vecLayer, QVector<ImageSpriteLayer*>* qvec, int bigIndex, int smallIndex, int& static_index)
{
	while (bigIndex >= smallIndex){
		auto curVec = vecLayer.at(bigIndex)->getAllResourceImage();
		auto vec = subToVec(vecLayer, smallIndex, bigIndex);
		int count = outputTest(copyFileAndReturnDir(convertToStringList(vec), OutputInfo::getInstance()->getOutputDir(), "static" + QString::number(static_index)),
			OutputInfo::getInstance()->getOutputName() + "_static" + "_" + QString::number(static_index) + "_", OutputInfo::getInstance()->getOutputDir(), "win32", "png", 2);
		if (count == 1){
			writeALayerVec(qvec);
			OutputInfo::getInstance()->addOutputCount();
			OutputInfo::getInstance()->addCurrentOutputCount();
			static_index++;
			return bigIndex + 1;
		}
		else{
			QString outdir = OutputInfo::getInstance()->getOutputDir() + "/" + QString("static" + QString::number(static_index)) + "/";
			DeleteDirectory(outdir);
		}
		bigIndex--;
		qvec->pop_back();
	}
	return -1;
}

bool TexturePackerView::recursionVec(std::vector<ImageSpriteLayer*>& vecLayer, int i, int& index)
{
	std::vector<std::string> vecStr;
	std::vector<std::string> vecPre;
	//最后一个加入栈的下标
	int smallIndex = i;
	auto qvec = new QVector<ImageSpriteLayer*>();
	unsigned int preSize = 0;
	unsigned int curSize = 0;
	for (int k = i; k < vecLayer.size(); k++){
		vecPre = vecStr;
		auto curVec = vecLayer.at(k)->getAllResourceImage();
		vecLayer.at(k)->setStaticIndex(index);
		qvec->push_back(vecLayer.at(k));
		addToVec(vecStr, curVec);
		preSize = curSize;
		curSize = getImagesSize(vecStr);
		//完全没有超过一张贴图大小的话
		if (curSize < TEXTURE_MAX_SIZE && k >= vecLayer.size() - 1){
			int count = outputTest(copyFileAndReturnDir(convertToStringList(vecStr), OutputInfo::getInstance()->getOutputDir(), "static" + QString::number(index)),
				OutputInfo::getInstance()->getOutputName() + "_static" + "_" + QString::number(index) + "_", OutputInfo::getInstance()->getOutputDir(), "win32", "png", 2);
			if (count == 1){
				writeALayerVec(qvec);
				OutputInfo::getInstance()->addOutputCount();
				OutputInfo::getInstance()->addCurrentOutputCount();
				OutputInfo::getInstance()->addStaticCount();
				index++;
				smallIndex = k;
			}
			else if(count > 1){
				if (i <= k - 1){
					qvec->pop_back();
					int vecIndex = writeAStaticLayer(vecLayer, qvec, k - 1, smallIndex, index);
					if (vecIndex == -1){
						addConsoleInfo("%s静态层图片过多,程序检测错误,检查静态层设置是否合理", vecLayer.at(k)->getTagName().c_str());
						MyLogger::getInstance()->addError(QString("%1静态层图片过多,程序检测错误,检查静态层设置是否合理").arg(vecLayer.at(k)->getTagName().c_str()));
						return false;
					}
					return recursionVec(vecLayer, vecIndex, index);
				}
				else{
					addConsoleInfo("%s静态层图片过多,程序检测错误", vecLayer.at(k)->getTagName().c_str());
					MyLogger::getInstance()->addError(QString("%1静态层图片过多,程序检测错误").arg(vecLayer.at(k)->getTagName().c_str()));
					return false;
				}
			}
			else{
				MyLogger::getInstance()->addError("ERROR outputTexture count is 0");
				addConsoleInfo("ERROR, 导出层的数量为0");
			}
		}
		//出现了超过贴图大小的情况
		else if (curSize >= TEXTURE_MAX_SIZE){
			if (curSize - preSize >= TEXTURE_MAX_SIZE){
				addConsoleInfo("%s静态层图片过多", vecLayer.at(k)->getTagName().c_str());
				MyLogger::getInstance()->addError(QString("%1静态层图片过多").arg(vecLayer.at(k)->getTagName().c_str()));
				return false;
			}
			int count = outputTest(copyFileAndReturnDir(convertToStringList(vecPre), OutputInfo::getInstance()->getOutputDir(), "static" + QString::number(index)),
				OutputInfo::getInstance()->getOutputName() + "_static" + "_" + QString::number(index) + "_", OutputInfo::getInstance()->getOutputDir(), "win32", "png", 2);
			if (count == 1){
				qvec->pop_back();
				writeALayerVec(qvec);
				OutputInfo::getInstance()->addOutputCount();
				OutputInfo::getInstance()->addCurrentOutputCount();
				OutputInfo::getInstance()->addStaticCount();
				index++;
				smallIndex = k;
				return recursionVec(vecLayer, k, index);
			}
			else if(count > 1){
				if (i <= k - 2){
					qvec->pop_back();
					qvec->pop_back();
					int vecIndex = writeAStaticLayer(vecLayer, qvec, k - 2, smallIndex, index);
					if (vecIndex == -1){
						addConsoleInfo("%s静态层图片过多,程序检测错误,检查静态层设置是否合理", vecLayer.at(k)->getTagName().c_str());
						MyLogger::getInstance()->addError(QString("%1静态层图片过多,程序检测错误,检查静态层设置是否合理").arg(vecLayer.at(k)->getTagName().c_str()));
						return false;
					}
					return recursionVec(vecLayer, vecIndex, index);
				}
				else{
					addConsoleInfo("%s静态层图片过多,程序检测错误", vecLayer.at(k)->getTagName().c_str());
					MyLogger::getInstance()->addError(QString("%1静态层图片过多,程序检测错误").arg(vecLayer.at(k)->getTagName().c_str()));
					return false;
				}
			}
			else{
				MyLogger::getInstance()->addError("ERROR outputTexture count is 0");
				addConsoleInfo("ERROR, 导出层的数量为0");
			}
		}
	}
	if (vecLayer.size() == 0){
		delete qvec;
	}
	return true;
}

void TexturePackerView::outputStaticLayer()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	auto vec = scene->getForeManager()->getAllStaticMergeLayerVec();
	int index = 0;
	for (int i = 0; i < vec.size(); i++){
		auto vecC = vec.at(i);
		if (!recursionVec(*vecC, 0, index)){
			scene->getForeManager()->deleteStaticMergeLayerVecData(vec);
			return;
		}
	}
	scene->getForeManager()->deleteStaticMergeLayerVecData(vec);
}

void TexturePackerView::PreViewSave(char* path, char* name)
{
	g_curTime = time(0);
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	bool needUpdateOutputTexture = scene->getForeManager()->judgeNeeedUpdateOutputTexture();
	if (needUpdateOutputTexture){
		OutputInfo::getInstance()->initData();
		OutputInfo::getInstance()->setShowOutputState(true);
		DeleteDirectory(path);
		QDir dir;
		dir.mkdir(path);
		deleteVecLayer();
		OutputInfo::getInstance()->setOutputDir(path);
		OutputInfo::getInstance()->setOutputName(name);
		MyLogger::getInstance()->addInfo("the TexturePackerView::PreViewSave be called, the args is " + OutputInfo::getInstance()->getOutputDir() + " " + OutputInfo::getInstance()->getOutputName());
		QStringList dynamicList = convertToStringList(scene->getAllImageNoStatic());
		if (dynamicList.size() > 0){
			OutputInfo::getInstance()->addOutputCount();
		}
		outputStaticLayer();
		auto curtime_ = time(0);
		auto time_ = curtime_ - g_curTime;
		g_curTime = curtime_;
		if (dynamicList.size() > 0){
			outputPacker(copyFileAndReturnDir(dynamicList, OutputInfo::getInstance()->getOutputDir(), "dynamic"), OutputInfo::getInstance()->getOutputName() + "_dynamic_",
				OutputInfo::getInstance()->getOutputDir(), "win32", "png", 0);
		}
		else{
			std::thread t1(&TexturePackerView::checkOutputCount, this);
			t1.detach();
		}
	}
	else{
		OutputInfo::getInstance()->setShowOutputState(true);
		std::thread t1(&TexturePackerView::checkOutputCount, this);
		t1.detach();
	}
	
}

void TexturePackerView::deleteVecLayer()
{
	for (int i = 0; i < m_vecLayer.size(); i++){
		delete m_vecLayer.at(i);
	}
	m_vecLayer.clear();
}

QStringList TexturePackerView::convertToStringList(std::vector<std::string>& vec)
{
	QStringList list;
	for (int i = 0; i < vec.size(); i++){
		list.push_back(vec.at(i).c_str());
	}
	return list;
}

int TexturePackerView::checkFile(QString outdir, QString outName)
{
	QString staticStr = outName + "_static_";
	QString dynamicStr = outName + "_dynamic_";
	QDir dir(outdir);
	if (dir.exists()){
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		int file_count = list.count();
		if (file_count <= 0)
		{
			return 3;
		}
		//string_list只有png和jpg格式的文件
		QStringList string_list;
		for (int i = 0; i<list.size(); i++)
		{
			QFileInfo file_info = list.at(i);
			QString suffix = file_info.suffix();
			if (QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0)
			{
				QString absolute_file_path = file_info.absoluteFilePath();
				string_list.append(absolute_file_path);
			}
			else if (QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0){
				QString absolute_file_path = file_info.absoluteFilePath();
				string_list.append(absolute_file_path);
			}
		}
		
		QStringList list2;
		int num = 0;
		int staticnum = 0;
		int dynamicnum = 0;
		for (int i = 0; i < string_list.size(); i++){
			QString str = string_list.at(i);
			if (str.contains(staticStr)){
				staticnum++;
				bool isRepeat = false;
				for (int k = 0; k < list2.size(); k++){
					QString suffix1 = list2.at(k).section("_", 0, -2);
					QString suffix2 = str.section("_", 0, -2);
					if (suffix1 == suffix2){
						isRepeat = true;
						num++;
						break;
					}
				}
				if (isRepeat == false){
					list2 << str;
				}
			}
			if (str.contains(dynamicStr)){
				dynamicnum++;
			}
		}
		OutputInfo::getInstance()->setDynamicCount(dynamicnum);
		if (num > 0){
			//所有格式的文件
			string_list.clear();
			for (int i = 0; i<list.size(); i++)
			{
				QFileInfo file_info = list.at(i);
				QString suffix = file_info.suffix();
				QString absolute_file_path = file_info.absoluteFilePath();
				string_list.append(absolute_file_path);
			}
			for (int i = 0; i < string_list.size(); i++){
				QString str = string_list.at(i);
				if (str.contains(staticStr) || str.contains(dynamicStr)){
					QFile::remove(str);
				}
			}
			return 2;
		}
		g_dynamic_count = OutputInfo::getInstance()->getDynamicCount();
		g_static_count = OutputInfo::getInstance()->getStaticCount();
		if (dynamicnum == 0 && staticnum == 0){
			return 4;
		}
	}
	else{
		return 1;
	}
	return 0;
}

QStringList TexturePackerView::getUsingImage()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	QStringList list;
	std::vector<std::string> vec = scene->getAllImage();
	int size = vec.size();
	for (int i = 0; i < size; i++){
		list << vec.at(i).c_str();
	}
	return list;
}

std::vector<QStringList> TexturePackerView::getStaticLayerImage()
{
	HelloWorld* scene = dynamic_cast<HelloWorld*>(g_scene);
	std::vector<QStringList> list;
	std::vector<std::vector<std::string>> vec = scene->getAllStaticImageLayerByLayer();
	int size = vec.size();
	for (int i = 0; i < size; i++){
		std::vector<std::string> imagevec = vec.at(i);
		QStringList imageList;
		for (int k = 0; k < imagevec.size(); k++){
			imageList << imagevec.at(k).c_str();
		}
		list.push_back(imageList);
	}
	return list;
}

//转换字符串并复制文件
QString TexturePackerView::copyFileAndReturnDir(QStringList list, QString outdir, QString prefix)
{
	outdir = outdir + "/" + prefix + "/";
	QDir dir;
	dir.mkdir(outdir);
	MyLogger::getInstance()->addInfo(QString("Start copyFile and resetCopyFile path, dir is %1").arg(prefix));
	for (int i = 0; i < list.size(); i++){
		QString name = list.at(i).section("/", -1);
		QString file = outdir + prefix + name;
		QFile::copy(list.at(i), file);
	}
	MyLogger::getInstance()->addInfo(QString("End copyFile and resetCopyFile path, dir is %1").arg(prefix));
	return outdir;
}

void TexturePackerView::createDirAndCopyFile(QString dir, QStringList fileList)
{
	QDir dir_(dir);
	if (!dir_.exists()){
		dir_.mkdir(dir);
	}
	if (!dir.endsWith("/")){
		dir += "/";
	}
	for (int i = 0; i < fileList.size(); i++){
		auto file = fileList.at(i);	
		QString newFile = dir + file.section("/", -1);
		QFile::copy(file, newFile);
	}
}

QStringList TexturePackerView::getDirRootFile(QString strdir)
{
	QStringList fileList;
	QDir dir(strdir);
	if (dir.exists()){
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		auto count = list.count();
		for (int i = 0; i < count; i++){
			QFileInfo file_info = list.at(i);
			QString path = file_info.absoluteFilePath();
			fileList << path;
		}
	}
	return fileList;
}

void TexturePackerView::writeALayerVec(QVector<ImageSpriteLayer*>* vec)
{
	std::unique_lock<std::mutex> lk(writeToVecMutex);
	m_vecLayer.push_back(vec);
}