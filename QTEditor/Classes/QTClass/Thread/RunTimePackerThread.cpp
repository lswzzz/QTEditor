#include "RunTimePackerThread.h"
#include "Global.h"
#include "MyLogger.h"
#include "TexturePackerView.h"
#include "CocEventCenter.h"

RunTimePackerThread::RunTimePackerThread()
{
}

RunTimePackerThread::~RunTimePackerThread()
{
}

void RunTimePackerThread::run()
{
	runPacker();
}

bool RunTimePackerThread::isSameTask(Task_Implement* task_)
{
	auto tTask = dynamic_cast<RunTimePackerThread*>(task_);
	if (tTask){
		if (_layer->getTagName() == tTask->_layer->getTagName()){
			return true;
		}
	}
	return false;
}

void RunTimePackerThread::generateCommand(QStringList fileList, QString outputName, QString outputDir, Node* layer, QString format)
{
	this->_layer = dynamic_cast<ImageSpriteLayer*>(layer);
	tag = _layer->generateThreadTag();
	getTexturePackerCommnad(fileList, outputName, outputDir, format, "win32", false);
}

void RunTimePackerThread::runPacker()
{
	TexturePackerView* view = dynamic_cast<TexturePackerView*>(g_TextureView);
	QString str = _layer->getTagName().c_str();
	MyLogger::getInstance()->addInfo("the RunTimePackerThread::runPacker be called, the args is dir:" + _outputDir +
		" name:" + _outputName + " layer" + str);
	if (PackerCommandThread::runProcess()){
		unsigned int* data = new unsigned int[2];
		int count = view->getPngCount(_outputDir, _outputName);
		data[0] = count;
		data[1] = tag;
		_layer->NeedUpdateSprite(data);
		delete data;
	}
}