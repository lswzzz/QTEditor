#include "keyboardmappingdialog.h"
#include "ui_keyboardmappingdialog.h"
#include "QFile"
#include "QDir"
#include "GlobalConstant.h"
#include "QKeyEvent"

KeyboardMappingDialog::KeyboardMappingDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KeyboardMappingDialog)
{
    ui->setupUi(this);
	path = QDir::currentPath() + "/mappingfile";
	createMappingFile();
	generateMappingInfo();
	generateVector();
	keyMappingToLineEdit();
	invincibleFriendClass = new InvincibleFriendClass;
}

KeyboardMappingDialog::~KeyboardMappingDialog()
{
	delete invincibleFriendClass;
    delete ui;
}

void KeyboardMappingDialog::generateVector()
{
	editVector.push_back(ui->spriteFlipXEdit);
	editVector.push_back(ui->spriteFlipYEdit);
	editVector.push_back(ui->collapseCurrentLayerEdit);
	editVector.push_back(ui->expandCurrentLayerEdit);
	generateQTMap();
	generateCocosMap();
}

void KeyboardMappingDialog::generateQTMap()
{
	QString path = QDir::currentPath() + "/QTKeyboardStringMapping";
	QString content = readFile(path, false);
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		QJsonArray list = obj["List"].toArray();
		for (size_t i = 0; i < list.size(); i++){
			auto valueObject = list.at(i).toObject();
			Qt::Key key = (Qt::Key)valueObject["Key"].toInt();
			QString value = valueObject["Value"].toString();
			QTStringMap.insert(key, value);
			QTKeyMap.insert(value, key);
		}
	}
}

void KeyboardMappingDialog::generateCocosMap()
{
	QString path = QDir::currentPath() + "/CocosKeyboardStringMapping";
	QString content = readFile(path, false);
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
	if (jsonError.error == QJsonParseError::NoError && document.isObject()){
		QJsonObject obj = document.object();
		QJsonArray list = obj["List"].toArray();
		for (size_t i = 0; i < list.size(); i++){
			auto valueObject = list.at(i).toObject();
			int key = valueObject["Key"].toInt();
			QString value = valueObject["Value"].toString();
			CocosStringMap.insert(key, value);
			CocosKeyMap.insert(value, key);
		}
	}
}

int KeyboardMappingDialog::getFocusIndex()
{
	for (int i = 0; i < editVector.size(); i++){
		auto edit = editVector.at(i);
		if (edit->hasFocus()){
			return i;
		}
	}
	return -1;
}

bool KeyboardMappingDialog::createMappingFile()
{
	QFile file(path);
	if (!file.exists()){
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		file.close();
		return true;
	}
	return false;
}

void KeyboardMappingDialog::generateMappingInfo()
{
	QJsonDocument document;
	if (readFile(path, document, true))
	{
		QJsonObject rootObject = document.object();
		QJsonArray list = rootObject["List"].toArray();
		QJsonArray::iterator iter;
		for (iter = list.begin(); iter != list.end(); iter++){
			QJsonObject obj = iter->toObject();
			Qt::Key key = (Qt::Key)obj["Key"].toInt();
			EditMapping value = (EditMapping)obj["Value"].toInt();
			keyMapping.insert(key, value);
		}
	}
}

void KeyboardMappingDialog::keyMappingToLineEdit()
{
	QHash<int, int>::iterator iter;
	for (iter = keyMapping.begin(); iter != keyMapping.end(); iter++){
		auto key = iter.key();
		auto value = iter.value();
		editVector[value]->setText(QTStringMap[key]);
	}
}

void KeyboardMappingDialog::keyPressEvent(QKeyEvent *e)
{
	
}

void KeyboardMappingDialog::keyReleaseEvent(QKeyEvent* e)
{
	dispatchKeyEvent(e->key());
}

void KeyboardMappingDialog::closeEvent(QCloseEvent *event)
{
	QVariantMap map;
	QVariantList list;
	QHash<int, int>::iterator iter;
	for (iter = keyMapping.begin(); iter != keyMapping.end(); iter++){
		auto key = iter.key();
		auto value = iter.value();
		QVariantMap data;
		data.insert("Key", key);
		data.insert("Value", value);
		list.push_back(data);
	}
	map.insert("List", list);
	QJsonDocument doc = QJsonDocument::fromVariant(map);
	writeFile(path, doc.toJson(), false);
}

void KeyboardMappingDialog::dispatchKeyEvent(int key)
{
	Qt::Key k = (Qt::Key)key;
	int index = getFocusIndex();
	if (index >= 0){
		setEditMappingKey(k, (EditMapping)getFocusIndex());
	}
	/*switch (getFocusIndex()){
	case spriteFlipX:
		setFlipXMappingKey(k);
		break;
	case spriteFlipY:
		setFlipYMappingKey(k);
		break;
	default:
		break;
	}*/
}

void KeyboardMappingDialog::dealWithKeyEvent(int key)
{
	if (keyMapping.contains(key)){
		switch (keyMapping[key]){
		case spriteFlipX:
			invincibleFriendClass->flipXEvent();
			break;
		case spriteFlipY:
			invincibleFriendClass->flipYEvent();
			break;
		case collapseCurrentLayer:
			invincibleFriendClass->collapseCurrentLayer();
			break;
		case expandCurrentLayer:
			invincibleFriendClass->expandCurrentLayer();
			break;
		default:
			break;
		}
	}
}

Qt::Key KeyboardMappingDialog::getFlipXMappingKey()
{
	auto key = (int)ui->spriteFlipXEdit->userData(0);
	return (Qt::Key)key;
}

Qt::Key KeyboardMappingDialog::getFlipYMappingKey()
{
	auto key = (int)ui->spriteFlipYEdit->userData(0);
	return (Qt::Key)key;
}

void KeyboardMappingDialog::setFlipXMappingKey(Qt::Key key)
{
	/*keyMapping.remove(getFlipXMappingKey());
	ui->spriteFlipXEdit->setText(QTStringMap[key]);
	ui->spriteFlipXEdit->setUserData(0, (QObjectUserData*)key);
	keyMapping[key] = spriteFlipX;*/
}

void KeyboardMappingDialog::setFlipYMappingKey(Qt::Key key)
{
	/*keyMapping.remove(getFlipYMappingKey());
	ui->spriteFlipYEdit->setText(QTStringMap[key]);
	ui->spriteFlipYEdit->setUserData(0, (QObjectUserData*)key);
	keyMapping[key] = spriteFlipY;*/
}

void KeyboardMappingDialog::setEditMappingKey(Qt::Key key, EditMapping edit)
{
	keyMapping.remove(getEditMappingKey(edit));
	auto lineEdit = editVector.at(edit);
	lineEdit->setText(QTStringMap[key]);
	lineEdit->setUserData(0, (QObjectUserData*)key);
	keyMapping[key] = edit;
}

Qt::Key KeyboardMappingDialog::getEditMappingKey(EditMapping edit)
{
	auto lineEdit = editVector.at(edit);
	auto key = (int)lineEdit->userData(0);
	return (Qt::Key)key;
}

void KeyboardMappingDialog::catchedKeyEvent(EventKeyboard::KeyCode key)
{
	QString value = CocosStringMap[(int)key];
	int qtKey = QTKeyMap[value];
	dealWithKeyEvent(qtKey);
}

void KeyboardMappingDialog::catchedKeyEvent(Qt::Key key)
{
	dealWithKeyEvent(key);
}