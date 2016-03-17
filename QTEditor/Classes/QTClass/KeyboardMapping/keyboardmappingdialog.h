#ifndef KEYBOARDMAPPINGDIALOG_H
#define KEYBOARDMAPPINGDIALOG_H

#include <QMainWindow>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "QLineEdit"
#include "cocos2d.h"
#include "InvincibleFriendClass.h"

USING_NS_CC;

namespace Ui {
class KeyboardMappingDialog;
}

class KeyboardMappingDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit KeyboardMappingDialog(QWidget *parent = 0);
    ~KeyboardMappingDialog();

	

	void catchedKeyEvent(EventKeyboard::KeyCode key);
	void catchedKeyEvent(Qt::Key key);

private:
	enum EditMapping{
		spriteFlipX,
		spriteFlipY,
		collapseCurrentLayer,
		expandCurrentLayer,
	};
	bool createMappingFile();
	void generateVector();
	void generateQTMap();
	void generateCocosMap();
	void generateMappingInfo();
	void keyMappingToLineEdit();
	Qt::Key getFlipXMappingKey();
	Qt::Key getFlipYMappingKey();
	void setFlipXMappingKey(Qt::Key key);
	void setFlipYMappingKey(Qt::Key key);
	void setEditMappingKey(Qt::Key key, EditMapping edit);
	Qt::Key getEditMappingKey(EditMapping edit);
	int getFocusIndex();
private:
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent* e);
	void closeEvent(QCloseEvent *event);
	//dialog设置键盘
	void dispatchKeyEvent(int key);
	//处理Cocos键盘事件
	void dealWithKeyEvent(int key);
private:
    Ui::KeyboardMappingDialog *ui;
	QVector<QLineEdit*> editVector;
	//Qt::Key, QString
	QHash<int, QString> QTStringMap;
	//QString, Qt::Key
	QHash<QString, int> QTKeyMap;
	//EventKeyboard::KeyCode, QString
	QHash<int, QString> CocosStringMap;
	//QString, EventKeyboard::KeyCode
	QHash<QString, int> CocosKeyMap;
	//Qt::Key, EditMapping
	QHash<int, int> keyMapping;
	InvincibleFriendClass* invincibleFriendClass;
	QString path;
};

#endif // KEYBOARDMAPPINGDIALOG_H
