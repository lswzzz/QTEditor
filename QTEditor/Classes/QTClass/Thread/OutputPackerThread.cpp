#include "OutputPackerThread.h"
#include "Global.h"
#include "QProcess"
#include "OutputInfo.h"
#include "TexturePackerView.h"

OutputPackerThread::OutputPackerThread()
	: _type(-1)
{
}

OutputPackerThread::~OutputPackerThread()
{
}

void OutputPackerThread::run()
{
	runPacker();
}

void OutputPackerThread::generateCommand(QStringList fileList, QString name, QString dir, QString form, int _type)
{
	this->_type = _type;
	getTexturePackerCommnad(fileList, name, dir, form, "win32", true);
}

void OutputPackerThread::runPacker()
{
	if (PackerCommandThread::runProcess()){
		switch (_type){
		case 0:
			OutputInfo::getInstance()->addDynamicCount();
			break;
		case 1:
			break;
		case 2:
			OutputInfo::getInstance()->addStaticCount();
			break;
		}
		auto view = dynamic_cast<TexturePackerView*>(g_TextureView);
		view->checkOutputCount();
	}
}