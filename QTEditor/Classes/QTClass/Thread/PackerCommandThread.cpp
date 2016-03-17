#include "PackerCommandThread.h"
#include "Global.h"
#include "QProcess"

PackerCommandThread::PackerCommandThread()
{
}


PackerCommandThread::~PackerCommandThread()
{
}

void PackerCommandThread::run()
{
}

QString PackerCommandThread::getTexturePackerCommnad(QStringList fileList, QString name, QString dir, QString format, QString platform, bool rotation)
{
	this->_outputName = name;
	this->_outputDir = dir;
	_command = g_textureCMD.c_str();
	_command += " --smart-update";
	//格式
	_command += " --format cocos2d";
	//plist文件
	_command += " --multipack";
	_command += " --data " + _outputDir + "/" + _outputName + "{n}.plist";
	_command += " --maxrects-heuristics best";
	if (rotation){
		_command += " --enable-rotation";
	}
	else{
		_command += " --disable-rotation";
	}	
	_command += " --scale " + QString::number(1);
	_command += " --shape-padding 2";
	_command += " --extrude 1";
	_command += " --max-size 2048";
	_command += " --size-constraints AnySize";
	if (platform == "android"){
		_command += " --opt RGBA8888";
	}
	else if (platform == "win32"){
		_command += " --opt RGBA8888";
	}
	else if (platform == "ios"){
		_command += " --opt RGBA8888";
	}
	_command += " --trim ";
	if (format == "pvr2ccz"){
		_command += " --algorithm Base";
		_command += " --texture-format " + QString("pvr2ccz");
		_command += " --sheet " + _outputDir + "/" + _outputName + "{n}.pvr2ccz";
	}
	else if (format == "png"){
		_command += " --texture-format " + QString("png");
		_command += " --sheet " + _outputDir + "/" + _outputName + "{n}.png";
	}
	_command += " ";
	for (int i = 0; i < fileList.size(); i++){
		_command += fileList.at(i) + " ";
	}
	return _command;
}

bool PackerCommandThread::runProcess()
{
	QProcess p(0);
	p.start(_command);
	p.waitForStarted();
	return p.waitForFinished();
}