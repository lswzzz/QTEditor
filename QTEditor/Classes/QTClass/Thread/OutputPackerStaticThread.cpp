#include "OutputPackerStaticThread.h"
#include "TexturePackerView.h"
#include "QDir"
#include "QFileInfoList"
#include "GlobalConstant.h"

#define TEXTURE_MAX_SIZE 4194304

OutputPackerStaticThread::OutputPackerStaticThread()
{
}


OutputPackerStaticThread::~OutputPackerStaticThread()
{
}

void OutputPackerStaticThread::run()
{

}

int OutputPackerStaticThread::generateCommand(QStringList fileList, QString name, QString dir, QString plat, QString form, int _type)
{
	this->_type = _type;
	getTexturePackerCommnad(fileList, name, dir, form, plat, true);
	return getCommandGenerateCount();
}

int OutputPackerStaticThread::getCommandGenerateCount()
{
	if (PackerCommandThread::runProcess()){
		return getFileCount(_outputName, _outputDir);
	}
	return 0;
}

int OutputPackerStaticThread::getFileCount(QString name, QString outdir)
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

bool OutputPackerStaticThread::recursionVec(std::vector<ImageSpriteLayer*>& vecLayer, int i, int& index)
{
	std::vector<std::string> vecStr;
	std::vector<std::string> vecPre;
	auto view = dynamic_cast<TexturePackerView*>(g_TextureView);
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
			int count = generateCommand(packerConvertStringList(convertToStringList(vecStr), OutputInfo::getInstance()->getOutputDir(), "static" + QString::number(index)),
				OutputInfo::getInstance()->getOutputName() + "_static" + "_" + QString::number(index) + "_", OutputInfo::getInstance()->getOutputDir(), "win32", "png", 2);
			if (count == 1){
				view->writeALayerVec(qvec);
				OutputInfo::getInstance()->addOutputCount();
				OutputInfo::getInstance()->addCurrentOutputCount();
				OutputInfo::getInstance()->addStaticCount();
				index++;
				smallIndex = k;
			}
			else{
				if (i <= k - 1){
					qvec->pop_back();
					int vecIndex = writeAStaticLayer(vecLayer, qvec, k - 1, smallIndex, index);
					if (vecIndex == -1){
						addConsoleInfo("%s静态层图片过多,程序检测错误,检查静态层设置是否合理", vecLayer.at(k)->getTagName().c_str());
						return false;
					}
					return recursionVec(vecLayer, vecIndex, index);
				}
				else{
					addConsoleInfo("%s静态层图片过多,程序检测错误", vecLayer.at(k)->getTagName().c_str());
					return false;
				}
			}
		}
		//出现了超过贴图大小的情况
		else if (curSize >= TEXTURE_MAX_SIZE){
			if (curSize - preSize >= TEXTURE_MAX_SIZE){
				addConsoleInfo("%s静态层图片过多", vecLayer.at(k)->getTagName().c_str());
				return false;
			}
			int count = generateCommand(packerConvertStringList(convertToStringList(vecPre), OutputInfo::getInstance()->getOutputDir(), "static" + QString::number(index)),
				OutputInfo::getInstance()->getOutputName() + "_static" + "_" + QString::number(index) + "_", OutputInfo::getInstance()->getOutputDir(), "win32", "png", 2);
			if (count == 1){
				qvec->pop_back();
				view->writeALayerVec(qvec);
				OutputInfo::getInstance()->addOutputCount();
				OutputInfo::getInstance()->addCurrentOutputCount();
				OutputInfo::getInstance()->addStaticCount();
				index++;
				smallIndex = k;
				return recursionVec(vecLayer, k, index);
			}
			else{
				if (i <= k - 2){
					qvec->pop_back();
					qvec->pop_back();
					int vecIndex = writeAStaticLayer(vecLayer, qvec, k - 2, smallIndex, index);
					if (vecIndex == -1){
						addConsoleInfo("%s静态层图片过多,程序检测错误,检查静态层设置是否合理", vecLayer.at(k)->getTagName().c_str());
						return false;
					}
					return recursionVec(vecLayer, vecIndex, index);
				}
				else{
					addConsoleInfo("%s静态层图片过多,程序检测错误", vecLayer.at(k)->getTagName().c_str());
					return false;
				}
			}
		}
	}
	if (vecLayer.size() == 0){
		delete qvec;
	}
	return true;
}

bool OutputPackerStaticThread::reCursionVecRoot(std::vector<ImageSpriteLayer*>& vecLayer, int i)
{
	return false;
}

int OutputPackerStaticThread::writeAStaticLayer(std::vector<ImageSpriteLayer*>& vecLayer, QVector<ImageSpriteLayer*>* qvec, int bigIndex, int smallIndex, int& static_index)
{
	auto view = dynamic_cast<TexturePackerView*>(g_TextureView);
	while (bigIndex >= smallIndex){
		auto curVec = vecLayer.at(bigIndex)->getAllResourceImage();
		auto vec = subToVec(vecLayer, smallIndex, bigIndex);
		int count = generateCommand(packerConvertStringList(convertToStringList(vec), OutputInfo::getInstance()->getOutputDir(), "static" + QString::number(static_index)),
			OutputInfo::getInstance()->getOutputName() + "_static" + "_" + QString::number(static_index) + "_", OutputInfo::getInstance()->getOutputDir(), "win32", "png", 2);
		if (count == 1){
			view->writeALayerVec(qvec);
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

void OutputPackerStaticThread::addToVec(std::vector<std::string>& sourceVec, std::vector<std::string>& dstVec)
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

std::vector<std::string> OutputPackerStaticThread::subToVec(std::vector<ImageSpriteLayer*>& vecLayer, int startIndex, int endedIndex)
{
	std::vector<std::string> vec;
	for (int i = startIndex; i < vecLayer.size() && i <= endedIndex; i++){
		auto curVec = vecLayer.at(i)->getAllResourceImage();
		addToVec(vec, curVec);
	}
	return vec;
}

unsigned int OutputPackerStaticThread::getImagesSize(std::vector<std::string>& vec)
{
	unsigned int size = 0;
	for (int i = 0; i < vec.size(); i++){
		auto image = new CCImage();
		image->initWithImageFile(vec.at(i));
		int width = image->getWidth() + 1;
		int height = image->getHeight() + 1;
		size += width* height;
		image->release();
	}
	return size;
}

QStringList OutputPackerStaticThread::convertToStringList(std::vector<std::string>& vec)
{
	QStringList list;
	for (int i = 0; i < vec.size(); i++){
		list.push_back(vec.at(i).c_str());
	}
	return list;
}

QStringList OutputPackerStaticThread::packerConvertStringList(QStringList list, QString outdir, QString prefix)
{
	QStringList othList;
	outdir = outdir + "/" + prefix + "/";
	QDir dir;
	dir.mkdir(outdir);
	for (int i = 0; i < list.size(); i++){
		QString name = list.at(i).section("/", -1);
		QString file = outdir + prefix + name;
		QFile::copy(list.at(i), file);
		othList.push_back(file);
	}
	return othList;
}

int OutputPackerStaticThread::getNewStaticTexturePackerIndex()
{
	return 1;
}