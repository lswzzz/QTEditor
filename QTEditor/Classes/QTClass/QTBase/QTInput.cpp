#include "QTInput.h"
#include "Global.h"
#include <QFile>
#include <qtextstream.h>
#include "ItalicBody.h"
#include "BaseBlock.h"
#include "BoxBody.h"
#include "HelloWorldScene.h"
#include "LayerManagerWidget.h"
#include "TexturePackerView.h"
#include "TemplateView.h"
#include <ResourceView.h>
#include "ActionView.h"
#include "FontChina.h"
#include <qwidget.h>
#include "ExterImageListWidget.h"
#include "qteditor.h"
#include <QDir>
#include "ConfigView.h"
#include "EntityImageSprite.h"
#include <QDesktopWidget>
#include "TemplateValueGenerator.h"
#include "ImageTreeWidget.h"
#include "MyLogger.h"
#include "MySqlLiteManager.h"
#include "GlobalConstant.h"
#include "ImgTransPath.h"
#include "ResMap.h"
#include "convertMapToJsonFile.h"
#include "ImageTreeWidget.h"
#include "AnimationView.h"

enum LayerType{
	smallbgLayer = 0,
	gridLayer = 1,
	blcokLayer = 2,
	touchSpriteLayer = 3,
	touchManLayer = 4,
	uiLayer = 10,
	dragLayer = 100,
};

QTInput::QTInput()
{
	outputImage = false;
}

QTInput::~QTInput()
{
}

bool QTInput::ReadConfFile(QString fileName)
{
	QFile sFile(fileName);
	sourceDir = fileName.section("/", 0, -2);
	sourceDir += "/";
	if (QDir(sourceDir) == QDir("../Resources/backup/")){
		QString searth = FileUtils::getInstance()->getSearchPaths().at(0).c_str();
		sourceDir = searth;
	}
	sourceCopyDir = sourceDir + g_c_CopyDir;
	if (!QDir(sourceCopyDir).exists()){
		hasCopyFileError = true;
		if (!QDir(sourceDir + "CopyFile-Backup").exists()){
			MyLogger::getInstance()->addWarning("请创建资源文件夹CopyFile-Backup，以便程序识别恢复数据");
		}
	}
	else{
		hasCopyFileError = false;
	}
	sourceResourceDir = sourceDir + g_c_relaResourceDir;
	sourceExteralDir = sourceDir + g_c_relaExteralDir;
	sourceAnimationDir = sourceDir + g_c_relaFrameDir;
	//copyDirectoryFiles(sourceCopyDir, sourceDir+"CopyFile-Backup");
	if (sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		auto resDir = fileName.section("/", 0, -2) + "/";
		searthPath = FileUtils::getInstance()->getSearchPaths().at(0).c_str();
		resourceDir = searthPath;
		exteralDir = searthPath;
		animationDir = searthPath;
		templateDir = searthPath;
		resListDir = searthPath;
		resListDir += g_c_rela_ResourceListDir;
		resourceDir += g_c_relaResourceDir;
		exteralDir += g_c_relaExteralDir;
		animationDir += g_c_relaFrameDir;
		templateDir += g_c_relaTemplateDir;
		if (QDir(fileName.section("/", 0, -2)) != QDir("../Resources/backup") && 
			(QDir(resDir) != QDir(searthPath) && QDir(resDir) != QDir(searthPath + "backup/"))){
			unusualConfDir = false;
			DeleteDirectory(resourceDir);
			DeleteDirectory(exteralDir);
			DeleteDirectory(templateDir);
			DeleteDirectory(animationDir);
			DeleteDirectory(resListDir);
		}
		else{
			DeleteDirectory(resListDir);
			unusualConfDir = true;
		}
		QDir* dir = new QDir();
		dir->rmdir("E:/cocos2d-x-3.9/projects/GameEditor/QTEditor/Resources/CopyFile/ResourceImage/room5");
		if (!dir->exists(searthPath + "CopyFile/")){
			dir->mkdir(searthPath + "CopyFile/");
		}
		if (!dir->exists(resourceDir)){
			dir->mkdir(resourceDir);
		}
		if (!dir->exists(exteralDir)){
			dir->mkdir(exteralDir);
		}
		if (!dir->exists(templateDir)){
			dir->mkdir(templateDir);
		}
		if (!dir->exists(resListDir)){
			dir->mkdir(resListDir);
		}
		if (!dir->exists(animationDir)){
			dir->mkdir(animationDir);
		}
		Cursor_Type __type = getCursorType();
		setCocCursor(Cursor_Type::Wait);
		QTextStream in(&sFile);
		in.setCodec("UTF-8");
		QString content = in.readAll();
		QJsonParseError jsonError;
		QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
		if (jsonError.error == QJsonParseError::NoError && document.isObject()){
			QJsonObject obj = document.object();
			if (obj.contains("fileName")){
				g_openConfile = true;
				int width = obj["width"].toInt();
				int height = obj["height"].toInt();
				int initWidth = obj["initWidth"].toInt();
				int initHeight = obj["initHeight"].toInt();
				int screenWidth = obj["screenWidth"].toInt();
				int screenHeight = obj["screenHeight"].toInt();
				int workRectWidth = obj["workRectWidth"].toInt();
				int workRectHeight = obj["workRectHeight"].toInt();
				bool hasAssortDir = obj["hasAssortDir"].toBool();
				hasRealDir = obj["hasRealDir"].toBool();
				MyLogger::getInstance()->addInfo("QTInput::ReadConfFile be called, the initWidth is " + QString::number(initWidth) +
					" initHeight is " + QString::number(initHeight) + " width is " + QString::number(width) +
					" height is " + QString::number(height) + " screenWidth is " + QString::number(screenWidth) + " screenHeight is "
					+ QString::number(screenHeight) + " workRectWidth is " + QString::number(workRectWidth) + " workRectHeight is " +
					QString::number(workRectHeight));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				if (workRectWidth == 0 && workRectHeight == 0){
					RECT workRect;
					SystemParametersInfo(SPI_GETWORKAREA, 0, &workRect, 0);
					int iHeight = workRect.bottom - workRect.top;
					int iWidth = workRect.right - workRect.left;
					workRectWidth = iWidth;
					workRectHeight = iHeight;
				}
#else
                if (workRectWidth == 0 && workRectHeight == 0){
                    QDesktopWidget widget;
                    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
                    int iHeight = mainScreenSize.width();
                    int iWidth = mainScreenSize.height();
                    workRectHeight = iHeight;
                    workRectWidth = iWidth;
                }
                
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				if ((width > 960 || height > 640) && screenWidth > 0 && screenHeight > 0){
					QDesktopWidget *desktop = QApplication::desktop();
					QRect screen = desktop->screenGeometry();
					RECT sWorkRect;
					SystemParametersInfo(SPI_GETWORKAREA, 0, &sWorkRect, 0);
					int _screenWidth = screen.width();
					int _screenHeight = screen.height();
					int taskbarWidth = screenWidth - width;
					int taskbarHeight = screenHeight - height;
					width = _screenWidth - taskbarWidth;
					height = _screenHeight - taskbarHeight;
					int iHeight = sWorkRect.bottom - sWorkRect.top;
					int iWidth = sWorkRect.right - sWorkRect.left;
					if (workRectWidth != iWidth){
						int off = workRectWidth - iWidth;
						width += off;
					}
					if (workRectHeight != iHeight){
						int off = workRectHeight - iHeight;
						height += off;
					}
					MyLogger::getInstance()->addInfo("QTInput::ReadConfFile be called, the current desktop screenWidth is " + QString::number(_screenWidth) +
						" screenHeight is " + QString::number(_screenHeight) + " workRectWidth is " + QString::number(iWidth) + " workRectHeight is " +
						QString::number(iHeight));
				}
#else
                if ((width > 960 || height > 640) && screenWidth > 0 && screenHeight > 0){
                    QDesktopWidget *desktop = QApplication::desktop();
                    QRect screen = desktop->screenGeometry();
                    QRect mainScreenSize = desktop->availableGeometry(desktop->primaryScreen());
                    int _screenWidth = screen.width();
                    int _screenHeight = screen.height();
                    int taskbarWidth = screenWidth - width;
                    int taskbarHeight = screenHeight - height;
                    width = _screenWidth - taskbarWidth;
                    height = _screenHeight - taskbarHeight;
                    int iHeight = mainScreenSize.height();
                    int iWidth = mainScreenSize.width();
                    if (workRectWidth != iWidth){
                        int off = workRectWidth - iWidth;
                        width += off;
                    }
                    if (workRectHeight != iHeight){
                        int off = workRectHeight - iHeight;
                        height += off;
                    }
                    MyLogger::getInstance()->addInfo("QTInput::ReadConfFile be called, the current desktop screenWidth is " + QString::number(_screenWidth) +
                                                     " screenHeight is " + QString::number(_screenHeight) + " workRectWidth is " + QString::number(iWidth) + " workRectHeight is " +
                                                     QString::number(iHeight));
                }
#endif
                
				if (initWidth == 0)initWidth = 960;
				if (initHeight == 0)initHeight = 640;
				double scene_Scale = obj["scenescale"].toDouble();
				int col = obj["col"].toInt();
				int row = obj["row"].toInt();
				int grid_Width = obj["gridWidth"].toInt();
				int grid_Height = obj["gridHeight"].toInt();
				outputImage = obj["outputImage"].toBool();
				if (outputImage){
					MyLogger::getInstance()->addInfo("QTInput::ReadConfFile be called, the user input a custom configure file");
				}
				else{
					MyLogger::getInstance()->addInfo("QTInput::ReadConfFile be called, the user input a default configure file");
				}
				QString outputImageDir = obj["ImageDir"].toString();
				QLineEdit* sceneScale;
				QLineEdit* width_Line;
				QLineEdit* height_Line;
				QLineEdit* row_Line;
				QLineEdit* col_Line;
				QLineEdit* gridHeight;
				QLineEdit* gridWidth;

				{
					QWidget* gridWidget = g_gridWidgetContents;
					QObjectList list = gridWidget->children();
					foreach(QObject* qo, list){
						QWidget* qw = (QWidget*)qo;
						QString str = qw->objectName();
						if (str == "width_Line"){
							width_Line = dynamic_cast<QLineEdit*>(qw);
							continue;
						}
						if (str == "height_Line"){
							height_Line = dynamic_cast<QLineEdit*>(qw);
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
						if (str == "gridHeight"){
							gridHeight = dynamic_cast<QLineEdit*>(qw);
							continue;
						}
						if (str == "gridWidth"){
							gridWidth = dynamic_cast<QLineEdit*>(qw);
							continue;
						}
					}
					auto configView = dynamic_cast<ConfigView*>(g_configView);
					sceneScale = configView->getSceneScale();
					sceneScale->setText(QString::number(scene_Scale));
					width_Line->setText(QString::number(width));
					height_Line->setText(QString::number(height));
					row_Line->setText(QString::number(row));
					col_Line->setText(QString::number(col));
					gridHeight->setText(QString::number(grid_Height));
					gridWidth->setText(QString::number(grid_Width));
				}
				g_output_Dir = obj["OutputDir"].toString().toStdString();
				g_output_Name = obj["OutputName"].toString().toStdString();
				g_dynamic_count = obj["dynamic_count"].toInt();
				g_static_count = obj["static_count"].toInt();
				g_template_count = obj["template_count"].toInt();
				bool layerStructure = obj["layerStructure"].toBool();
				int sprite_count = obj["sprite_count"].toInt();
				MySqlLiteManager::getInstance()->setOpenDir(obj["save_Dir"].toString());

				MyLogger::getInstance()->addInfo("Start read config data");
				auto configView = static_cast<ConfigView*>(g_configView);
				int colorR = obj["colorR"].toInt();
				int colorG = obj["colorG"].toInt();
				int colorB = obj["colorB"].toInt();
				auto glview = Director::getInstance()->getOpenGLView();
				HelloWorld* scene = (HelloWorld*)g_scene;
				scene->setLayerColor(Color3B(colorR, colorG, colorB));
				configView->setColorR(colorR);
				configView->setColorG(colorG);
				configView->setColorB(colorB);
				if (obj.contains("gridColorR")){
					scene->setGridColor(Color4B(obj["gridColorR"].toInt(), obj["gridColorG"].toInt(), obj["gridColorB"].toInt(),
						obj["gridColorA"].toInt()));
					configView->setGridColorR(obj["gridColorR"].toInt());
					configView->setGridColorG(obj["gridColorG"].toInt());
					configView->setGridColorB(obj["gridColorB"].toInt());
					configView->setGridColorA(obj["gridColorA"].toInt());
				}
				configView->setLayerStructureState(layerStructure);

				scene->setInit(obj["isInit"].toBool());
				QSize initSize(initWidth, initHeight);
				QSize size(width, height);
				if (initSize == size){
					g_CocosWindowInitSize = size;
					auto editor = dynamic_cast<QTEditor*>(g_editor);
					editor->reSetQTEditorBySize(QSize(width, height));
					scene->createGrid(row, col, grid_Height, grid_Width, false);
				}
				else{
					g_CocosWindowInitSize = initSize;
					scene->createGrid(row, col, grid_Height, grid_Width, false);
					auto editor = dynamic_cast<QTEditor*>(g_editor);
					editor->reSetQTEditorBySize(QSize(width, height));
				}
				if (initHeight > 640){
					if (height > 640){
						resetOldAndNewWindowSize(QSize(960, 640), initSize);
					}
					else{
						resetOldAndNewWindowSize(initSize, QSize(960, 640));
					}
				}
				else{
					if (height > 640){
						resetOldAndNewWindowSize(QSize(960, 640), size);
					}
					else{
						resetOldAndNewWindowSize(QSize(960, 640), QSize(960, 640));
					}
				}
				if (sceneScale->text().toDouble() > 0.0f){
					scene->setScale(sceneScale->text().toDouble());
				}
				scene->setPosition(obj["scenePosx"].toDouble(), obj["scenePosy"].toDouble());
				/*scene->deleteForeManager();
				scene->createForeManager();*/
				scene->getForeManager()->setOpenState(true);
				if (obj.contains("foreVisible")){
					scene->getForeManager()->setVisible(obj["foreVisible"].toBool());
				}
				if (obj.contains("gridVisible")){
					scene->getGridManagerLayer()->setVisible(obj["gridVisible"].toBool());
				}

				MyLogger::getInstance()->addInfo("Start read Resource Data");
				if (layerStructure){
					QJsonDocument resourcedoc;
					QString resoucefile = sourceDir + obj["resourcefile"].toString();
					if (readFile(resoucefile, resourcedoc, true)){
						QJsonObject resourceobj = resourcedoc.object();
						readResourceData(resourceobj);
					}
					else{
						MyLogger::getInstance()->addError("Error read resource file error");
						addConsoleInfo("错误，读取资源文件错误");
					}
				}
				else{
					readResourceData(obj);
				}
				if (hasCopyFileError){
					addConsoleInfo("资源文件夹出错，详细信息查看warning-info");
					return false;
				}
				if (obj.contains("data")){
					MyLogger::getInstance()->addInfo("Start read Tiles Data");
					if (layerStructure){
						QJsonDocument tilesdoc;
						QString tilesfile = sourceDir + obj["tilesfile"].toString();
						if (readFile(tilesfile, tilesdoc, true)){
							QJsonObject tilesobj = tilesdoc.object();
							readTilesData(tilesobj, initSize, grid_Width, grid_Height, row, col);
						}
					}
					else{
						QJsonObject tileData = obj["data"].toObject();
						readTilesData(tileData, initSize, grid_Width, grid_Height, row, col);
					}

					MyLogger::getInstance()->addInfo("Start read Layers Data");
					if (layerStructure){
						QJsonObject layerlistObj = obj["data"].toObject();
						readLayersFileData(layerlistObj, hasAssortDir, sprite_count);
					}
					else{
						QJsonObject layersData = obj["data"].toObject();
						readLayersData(layersData, hasAssortDir, sprite_count);
					}
					scene->getForeManager()->setOpenState(false);
				}
				g_openConfile = false;
			}
			else{
				MyLogger::getInstance()->addError("QTInput::ReadConfFile error, error file");
				addConsoleInfo("错误的配置文件");
			}
		}
		else{
			MyLogger::getInstance()->addError("QTInput::ReadConfFile error, error file");
			addConsoleInfo("读取过程出错，请检查文件是否正确");
		}
		setCocCursor(__type);
		sFile.close();
		return true;
	}
	return false;
}

void QTInput::setProperties(ImageSprite* sprite, QJsonObject& obj)
{
	sprite->isUserOutput = outputImage;
	if (outputImage){
		QString templateFile = obj["templateFile"].toString();
		QString relatFile = obj["relatFile"].toString();
		QString templatemd5 = obj["templatemd5"].toString();
		/*if (templatemd5 == ""){
			templatemd5 = getMD5(searthPath + obj["relatFile"].toString());
		}*/
		if (templateFile != "" && !templateFile.endsWith('/')){
			sprite->setTemplateFile((searthPath + obj["relatFile"].toString().section("/", 1)).toStdString(), templatemd5.toStdString());
			addSpriteToTemplate(sprite);
		}
	}
	else{
		QString templatemd5 = obj["templatemd5"].toString();
		/*if (templatemd5 == ""){
			templatemd5 = getMD5(obj["templateFile"].toString());
		}*/
		QString templateFile = obj["templateFile"].toString();
		if (templateFile != "" && !templateFile.endsWith('/')){
			sprite->setTemplateFile(templateFile.toStdString(), templatemd5.toStdString());
			addSpriteToTemplate(sprite);
		}
	}

	sprite->setPosition(Point(obj["start_x"].toDouble(), obj["start_y"].toDouble()));
	sprite->setHasBaseProperties(obj["hasBaseProperties"].toBool());
	if (obj["hasBaseProperties"].toBool()){
		//set BaseProperties
		TemplateValueGenerator::setSpriteBaseProperties(sprite, obj, false);
	}

	sprite->setHasBoxProperties(obj["hasBoxProperties"].toBool());
	sprite->setHasCircleProperties(obj["hasCircleProperties"].toBool());
	if (obj["hasBoxProperties"].toBool()){
		//set BoxProperties
		TemplateValueGenerator::setSpriteBoxProperties(sprite, obj, false);
	}
	else if (obj["hasCircleProperties"].toBool()){
		TemplateValueGenerator::setSpriteCircleProperties(sprite, obj, false);
	}

	sprite->setHasEntityProperties(obj["hasEntityProperties"].toBool());
	if (obj["hasEntityProperties"].toBool()){
		TemplateValueGenerator::setSpriteEntityProperties(sprite, obj, false);
	}

	if (obj["hasMonsterProperties"].toBool()){
		TemplateValueGenerator::setSpriteMonsterProperties(sprite, obj, false);
	}

	if (obj["hasMonsterDropProperties"].toBool()){
		TemplateValueGenerator::setSpriteMonsterDropProperties(sprite, obj, false);
	}
	sprite->setHasMonsterDropProperties(obj["hasMonsterDropProperties"].toBool());

	if (obj["hasAIProperties"].toBool()){
		TemplateValueGenerator::setSpriteAIProperties(sprite, obj, false);
	}
	sprite->setHasAIProperties(obj["hasAIProperties"].toBool());

	if (obj["hasSimpleTriggerProperties"].toBool()){
		TemplateValueGenerator::setSpriteSimplerTriggerProperties(sprite, obj, false);
	}
	sprite->setHasSimplerTrigger(obj["hasSimpleTriggerProperties"].toBool());

	sprite->setHasMoveProperties(obj["hasMoveProperties"].toBool());
	if (obj["hasMoveProperties"].toBool()){
		TemplateValueGenerator::setSpriteMoveProperties(sprite, obj, false);
	}

	sprite->setHasFilterProperties(obj["hasFilterProperties"].toBool());
	if (obj["hasFilterProperties"].toBool()){
		TemplateValueGenerator::setSpriteFilterProperties(sprite, obj, false);
	}

	sprite->setHasAnimation(obj["hasAnimation"].toBool());
	if (obj["hasAnimation"].toBool()){
		TemplateValueGenerator::setSpriteAnimationProperties(sprite, obj, false);
	}
}


QString QTInput::getMD5(QString templateFile)
{
	QString templatemd5;
	QFile sFile(templateFile);
	if (sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream in(&sFile);
		QString content = in.readAll();
		sFile.close();
		QJsonParseError jsonError;
		QJsonDocument document = QJsonDocument::fromJson(content.toLocal8Bit(), &jsonError);
		if (jsonError.error == QJsonParseError::NoError && document.isObject()){
			QJsonObject obj = document.object();
			templatemd5 = obj["templatemd5"].toString();
		}
	}
	return templatemd5;
	/*auto iter = md5Map.find(templateFile);
	if (iter != md5Map.end()){
		return *iter;
	}
	return "";*/
}

void QTInput::addSpriteToTemplate(ImageSprite* sprite)
{
	TemplateView* templateView = dynamic_cast<TemplateView*>(g_TemplateView);
	templateView->itemAddSprite(sprite);
}

void QTInput::copyImageArray(QJsonArray& array, ImageTreeWidget* treeWidget, QTreeWidgetItem* item, bool isResourceWidget)
{
	int size = array.size();
	for (int i = 0; i < size; i++){
		QJsonObject res = array.at(i).toObject();
		QString path;
		QString BackPath;
		if (res["isDir"].toBool()){
			treeWidget->setCurrentItem(item);
			QJsonArray list = res["res_List"].toArray();
			QString name = res["name"].toString();
			auto itemChild = treeWidget->addDir(false, name);
			treeWidget->setCurrentItem(itemChild);
			copyImageArray(list, treeWidget, itemChild, isResourceWidget);
		}
		else{
			if (outputImage){
				if (hasCopyFileError){
					if (isResourceWidget){
						BackPath = sourceDir + g_c_relaResourceBackupDir + res["relative"].toString().section("/", -1);
					}
					else{
						BackPath = sourceDir + g_c_relaExteralBackupDir + res["relative"].toString().section("/", -1);
					}
				}
				if (hasRealDir){
					path = sourceCopyDir + res["relative"].toString();
				}
				else{
					path = sourceDir + res["relative"].toString();
				}
			}
			else{ 
				if (hasCopyFileError){
					//错误处理
					if (isResourceWidget){
						BackPath = sourceDir + g_c_relaResourceBackupDir + res["res"].toString().section("/", -1);
					}
					else{
						BackPath = sourceDir + g_c_relaExteralBackupDir + res["res"].toString().section("/", -1);
					}
				}
				if (hasRealDir){
					path = res["res"].toString();
				}
				else{
					path = res["res"].toString();
				}
			}
			if (item == nullptr){
				treeWidget->setCurrentItem(nullptr);
			}
			if (!QFile(path).exists()){
				MyLogger::getInstance()->addWarning("warning Resource " + path + "      Don't exist");
			}
			if (hasCopyFileError){
				QString backPath2 = path.replace("CopyFile", "CopyFile-Backup");
				path.replace("CopyFile-Backup", "CopyFile");
				if (!QFile(backPath2).exists()){
					copyFileToPath(BackPath, path);
				}
				else{
					copyFileToPath(backPath2, path);
				}
			}
			else{
				treeWidget->addRes(path, false);
			}
			
		}
	}
}

void QTInput::copyAnimationArray(QJsonArray& array, AnimationTreeWidget* treeWidget, QTreeWidgetItem* item)
{
	int size = array.size();
	for (int i = 0; i < size; i++){
		QJsonObject res = array.at(i).toObject();
		QString path;
		QString BackPath;
		if (res["isDir"].toBool()){
			treeWidget->setCurrentItem(item);
			QJsonArray list = res["frame_List"].toArray();
			QString name = res["animationName"].toString();
			auto itemChild = treeWidget->addFrameDir(false, name);
			treeWidget->setCurrentItem(itemChild);
			copyAnimationArray(list, treeWidget, itemChild);
		}
		else{
			if (outputImage){
				if (hasCopyFileError){
					BackPath = sourceDir + g_c_relaFrameBackDir + res["relative"].toString().section("/", -1);
				}
				path = sourceCopyDir + res["relative"].toString();
			}
			else{
				if (hasCopyFileError){
					BackPath = sourceDir + g_c_relaFrameBackDir + res["res"].toString().section("/", -1);
				}
				path = res["res"].toString();
			}
			if (item == nullptr){
				treeWidget->setCurrentItem(nullptr);
			}
			if (!QFile(path).exists()){
				MyLogger::getInstance()->addWarning("warning Resource " + path + "      Don't exist");
			}
			else{
				if (hasCopyFileError){
					QString backPath2 = path.replace("CopyFile", "CopyFile-Backup");
					path.replace("CopyFile-Backup", "CopyFile");
					if (!QFile(backPath2).exists()){
						copyFileToPath(BackPath, path);
					}
					else{
						copyFileToPath(backPath2, path);
					}
				}
				else{
					treeWidget->addSingleFrame(path, false);
				}
			}
			

		}
	}
}

QString QTInput::copyTemplate(QJsonObject& obj)
{
	QString templateFile = sourceDir + obj["relative"].toString();
	if (hasCopyFileError){
		QString otherFile = templateFile.replace("CopyFile", "CopyFile-Backup");
		templateFile.replace("CopyFile-Backup", "CopyFile");
		copyFileToPath(otherFile, templateFile);
		QFile(otherFile).copy(templateFile);
	}
	QString templateCopyFile = templateDir + templateFile.section("/", -1);
	QFile sFile(templateFile);
	if (sFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream in(&sFile);
		in.setCodec("UTF-8");
		QString content = in.readAll();
		QFile dFile(templateCopyFile);
		if (dFile.open(QIODevice::WriteOnly | QIODevice::Text)){
			QTextStream out(&dFile);
			sFile.close();
			QJsonParseError jsonError;
			QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
			if (jsonError.error == QJsonParseError::NoError && document.isObject()){
				QJsonObject obj = document.object();
				if (hasRealDir){
					QString str = obj["relaDir"].toString();
					int index = ImgTransPath::getListImageIndex(obj["relaDir"].toString());
					obj["imageIndex"] = index;
				}
				else{
					QString str = obj["relaDir"].toString().section("/", -1);
					int index = ImgTransPath::getListImageIndex(ImgTransPath::getNewRelaPath(str));
					obj["imageIndex"] = index;
					obj["relaDir"] = ImgTransPath::getRelativePathByIndex(index);
				}
				md5Map.insert(templateCopyFile, obj["md5"].toString());
				obj["userOutput"] = false;
				QJsonDocument doc = QJsonDocument::fromVariant(obj.toVariantMap());
				out << doc.toJson();
			}
			dFile.close();
			convertMapToJsonFile::changeFileToUtf8(templateCopyFile);
		}
	}
	return templateCopyFile;
}

void QTInput::readResourceData(QJsonObject& obj)
{
	//Resource List
	ResMap::resClear();
	{
		MyLogger::getInstance()->addInfo("in QTInput::ReadConfFile, start read resource files");
		auto resourceView = dynamic_cast<ResourceView*>(g_resourceView);
		auto treeWidget = dynamic_cast<ImageTreeWidget*>(resourceView->getTreeWidget());
		if (treeWidget){
			treeWidget->removeAllItem(!unusualConfDir);
			QJsonArray resList = obj["res_tree"].toArray();
			copyImageArray(resList, treeWidget, nullptr, true);
		}
	}

	//Exteral List
	{
		MyLogger::getInstance()->addInfo("in QTInput::ReadConfFile, start read exteral files");
		auto resourceView = dynamic_cast<ExteralView*>(g_exteralView);
		auto treeWidget = dynamic_cast<ExteralTreeWidget*>(resourceView->getTreeWidget());
		if (treeWidget){
			treeWidget->removeAllItem(!unusualConfDir);
			QJsonArray resList = obj["exteral_tree"].toArray();
			copyImageArray(resList, treeWidget, nullptr, false);
		}
	}

	//Animation List
	{
		MyLogger::getInstance()->addInfo("in QTInput::ReadConfFile, start read frame files");
		auto resourceView = dynamic_cast<AnimationView*>(g_AnimationView);
		auto treeWidget = dynamic_cast<AnimationTreeWidget*>(resourceView->getTreeWidget());
		if (treeWidget){
			treeWidget->removeAllItem(!unusualConfDir);
			QJsonArray resList = obj["frame_tree"].toArray();
			copyAnimationArray(resList, treeWidget, nullptr);
		}
	}

	//TemplateList
	{
		MyLogger::getInstance()->addInfo("in QTInput::ReadConfFile, start read template files");
		auto templateView = dynamic_cast<TemplateView*>(g_TemplateView);
		if (templateView){
			templateView->removeAllTemplate();
			QJsonArray templateList = obj["template_list"].toArray();
			int size = templateList.size();
			for (int i = 0; i < templateList.size(); i++){
				QJsonObject template_ = templateList.at(i).toObject();
				QString str = copyTemplate(template_);
				templateView->addTemplate(str, true);
			}
		}
	}
}

void QTInput::readTilesData(QJsonObject& obj, QSize initSize, int grid_Width, int grid_Height, int row, int col)
{
	MyLogger::getInstance()->addInfo("in QTInput::ReadConfFile, start read block array");
	HelloWorld* scene = (HelloWorld*)g_scene;
	QJsonArray tiles = obj["tiles"].toArray();
	for (int i = 0; i < tiles.size(); i++){
		QJsonObject tile = tiles.at(i).toObject();
		//斜体
		if (tile["type"].toInt() == 1){
			int x = tile["pos"].toObject()["x"].toInt();
			int y = tile["pos"].toObject()["y"].toInt();
			auto block = ItalicBody::create("gridBg.png", grid_Width, grid_Height);
			scene->gridManagerLayer->addBlock(block, y, x);
			auto pos = Point(x * grid_Width + grid_Width / 2, 640 - y*grid_Height - grid_Height / 2);
			if (initSize.height() > 640){
				pos.y = initSize.height() - y*grid_Height - grid_Height / 2;
			}
			block->setPosition(pos);
			block->setTag(y*col + x);
			block->setOpacity(80);
			block->setType(Type_Italic);
			Point start, end, pos1, pos2;
			pos1 = Point(tile["pos1_x"].toDouble(), tile["pos1_y"].toDouble());
			pos2 = Point(tile["pos2_x"].toDouble(), tile["pos2_y"].toDouble());
			start = Point(tile["start_x"].toDouble(), tile["start_y"].toDouble());
			end = Point(tile["end_x"].toDouble(), tile["end_y"].toDouble());
			block->addDrawNode(pos1, pos2, tile["direction"].toInt());
		}
		//方体
		else if (tile["type"].toInt() == 0){
			int x = tile["pos"].toObject()["x"].toInt();
			int y = tile["pos"].toObject()["y"].toInt();
			auto block = BoxBody::create("gridBg.png", grid_Width, grid_Height);
			scene->gridManagerLayer->addBlock(block, y, x);
			auto pos = Point(x * grid_Width + grid_Width / 2, 640 - y*grid_Height - grid_Height / 2);
			if (initSize.height() > 640){
				pos.y = initSize.height() - y*grid_Height - grid_Height / 2;
			}
			block->setPosition(pos);
			block->setTag(y*col + x);
			block->setOpacity(80);
			block->setType(Type_Box);
			block->setWay((One_Way)tile["way"].toInt());
			block->DrawGrid();
		}
		else if (tile["type"].toInt() == 2){

		}
	}
}

void QTInput::readLayersData(QJsonObject& obj, bool hasAssortDir, int sprite_count)
{
	LayerManagerWidget* layerManagerWidget = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	layerManagerWidget->removeAllItem();
	QJsonArray alllayer = obj["alllayer"].toArray();
	QMap<int, QJsonObject> map__;
	for (int i = 0; i < alllayer.size(); i++){
		QJsonObject simpleLayer = alllayer.at(i).toObject();
		map__.insert(simpleLayer["index"].toInt(), simpleLayer);
		//readSimpleLayerData(simpleLayer, hasAssortDir, i);
	}
	for (int i = 0; i<map__.size(); i++){
		auto simpleLayer = map__.value(i);
		readSimpleLayerData(simpleLayer, hasAssortDir, i);
	}
	if (sprite_count > layerManagerWidget->getCurrItemIndex()){
		layerManagerWidget->setCurrItemIndex(sprite_count);
	}
}

void QTInput::readLayersFileData(QJsonObject& obj, bool hasAssortDir, int sprite_count)
{
	LayerManagerWidget* layerManagerWidget = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	layerManagerWidget->removeAllItem();
	QJsonArray alllayerFile = obj["layerFileList"].toArray();
	QMap<int, QJsonObject> map__;
	for (int i = 0; i < alllayerFile.size(); i++){
		QString filename = sourceDir + alllayerFile.at(i).toObject()["layerfile"].toString();
		QJsonDocument layerdoc;
		if (readFile(filename, layerdoc, true)){
			QJsonObject layerobj = layerdoc.object();
			map__.insert(layerobj["index"].toInt(), layerobj);
			//readSimpleLayerData(layerObj, hasAssortDir, i);
		}
	}
	for (int i = 0; i<map__.size(); i++){
		auto layerObj = map__.value(i);
		readSimpleLayerData(layerObj, hasAssortDir, i);
	}
	if (sprite_count > layerManagerWidget->getCurrItemIndex()){
		layerManagerWidget->setCurrItemIndex(sprite_count);
	}
}

void QTInput::readSimpleLayerData(QJsonObject& obj, bool hasAssortDir, int i)
{
	auto resourceView = static_cast<ResourceView*>(g_resourceView);
	auto exteralView = static_cast<ExteralView*>(g_exteralView);
	auto imageTreeWidget = static_cast<ImageTreeWidget*>(resourceView->getTreeWidget());
	auto exteralTreeWidget = static_cast<ExteralTreeWidget*>(exteralView->getTreeWidget());
	LayerManagerWidget* layerManagerWidget = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	HelloWorld* scene = (HelloWorld*)g_scene;
	QJsonObject layer = obj;
	QString tagname = layer["tagname"].toString();
	QJsonArray images = layer["images"].toArray();
	int count = images.size();
	scene->addForeImageLayer(FontChina::G2U(tagname.toUtf8().data()), layer["z_order"].toInt());
	int index = layerManagerWidget->addItemLayerNoSort(FontChina::G2U(tagname.toUtf8().data()), layer["z_order"].toInt(), i);
	MyLogger::getInstance()->addInfo("the QTInput::ReadConfFile be called, start input imageLayer, the layerIndex is " +
		QString::number(index) + " the loop index is " + QString::number(i) + " the layer name is " + tagname);
	auto lay = scene->getLayerFromForeManager(FontChina::G2U(tagname.toUtf8().data()));
	lay->setLayerIndex(i);
	lay->setVisible(layer["visible"].toBool());
	lay->setisSportState(layer["state"].toBool());
	lay->setLock(layer["isLock"].toBool());
	lay->setMoveScaleX(layer["moveScaleX"].toDouble());
	lay->setMoveScaleY(layer["moveScaleY"].toDouble());
	lay->setSceneScaleFluence(layer["sceneScaleFluence"].toDouble());
	lay->setFilterType(layer["filterType"].toInt());
	auto fiiterType = lay->getFilterType();
	lay->setDontOutputState(layer["dontOutputState"].toBool());
	if (layer.contains("drawVisible")){
		bool visible = layer["drawVisible"].toBool();
		int col1 = layer["col1"].toInt();
		int row1 = layer["row1"].toInt();
		int col2 = layer["col2"].toInt();
		int row2 = layer["row2"].toInt();
		scene->getForeManager()->getLayerMoveRangeManage()->setDrawDataRect(tagname.toStdString(), Vec2(col1, row1), Vec2(col2, row2), true);
		scene->getForeManager()->getLayerMoveRangeManage()->getLayerDrawNode(tagname.toStdString())->setVisible(visible);
	}
	layerManagerWidget->setLayerVisibleIcon(index, layer["visible"].toBool());
	layerManagerWidget->setLayerStateIcon(index, layer["state"].toBool());
	layerManagerWidget->setLayerLockStateIcon(index, layer["isLock"].toBool());
	layerManagerWidget->setLayerDontOutputStateIcon(FontChina::G2U(tagname.toUtf8().data()), lay->isDontOutput());
	for (int j = 0; j < images.size(); j++){
		QJsonObject obj = images.at(j).toObject();
		Point pos = Point(obj["pos_x"].toDouble(), obj["pos_y"].toDouble());
		int zOrder = obj["z_order"].toInt();
		int type__ = obj["Image_Type"].toInt();
		std::string filename;
		if (!hasRealDir){
			filename = ImgTransPath::getNewRelaPath(obj["relative"].toString().section("/", -1)).toStdString();
		}
		else{
			filename = obj["relative"].toString().toStdString();
		}
		std::string tagname = FontChina::G2U(obj["tagname"].toString().toUtf8().data());
		std::string parent = FontChina::G2U(layer["tagname"].toString().toUtf8().data());
		ImageSprite* sprite;
		if (type__ == 2){
			std::string animationname = obj["animationName"].toString().toStdString();
			sprite = lay->addSpriteInOpenFile(pos, filename, tagname, Image_Type(type__), animationname);
		}
		else{
			sprite = lay->addSpriteInOpenFile(pos, filename, tagname, Image_Type(type__));
		}
		sprite->setZOrder(zOrder);
		sprite->setVisible(obj["visible"].toBool());
		int copyCount = obj["copyCount"].toInt();
		if (obj["id"].toInt() == 0){
			static unsigned int id = 0;
			sprite->setId(++id);
		}
		else{
			sprite->setId(obj["id"].toInt());
		}

		sprite->setCopyCount(copyCount);
		setProperties(sprite, obj);
		if (!layerManagerWidget->addItemSpriteAsyn(parent.c_str(), tagname.c_str(), zOrder, sprite)){
			MyLogger::getInstance()->addError("QTInput::ReadConfFile error, has some error in configure file");
			addConsoleInfo("加入子节点错误");
		}
	}
	lay->addSpriteOver();
	//加载完图片再进行打包
	lay->AsynPackerImageInOpenFile();
}