#pragma once
#include "GlobalConstant.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <QVariantMap>
#include <QVariantList>
#include "QDir"
#include "cocos2d.h"

USING_NS_CC;

QVariantMap getQTVariant(Qt::Key key, QString str);
QVariantMap getCocosVariant(EventKeyboard::KeyCode key, QString str);
void generateQTKeyBoardMapping();
void generateCocosKeyboardMapping();

void _______main()
{
	generateQTKeyBoardMapping();
}

void generateQTKeyBoardMapping()
{
	auto path = QDir::currentPath() + "/../Resources/QTKeyboardStringMapping";
	QVariantMap document;
	QVariantList list;
	list.push_back(getQTVariant(Qt::Key_0, "0"));
	list.push_back(getQTVariant(Qt::Key_1, "1"));
	list.push_back(getQTVariant(Qt::Key_2, "2"));
	list.push_back(getQTVariant(Qt::Key_3, "3"));
	list.push_back(getQTVariant(Qt::Key_4, "4"));
	list.push_back(getQTVariant(Qt::Key_5, "5"));
	list.push_back(getQTVariant(Qt::Key_6, "6"));
	list.push_back(getQTVariant(Qt::Key_7, "7"));
	list.push_back(getQTVariant(Qt::Key_8, "8"));
	list.push_back(getQTVariant(Qt::Key_9, "9"));
	list.push_back(getQTVariant(Qt::Key_Q, "Q"));
	list.push_back(getQTVariant(Qt::Key_W, "W"));
	list.push_back(getQTVariant(Qt::Key_E, "E"));
	list.push_back(getQTVariant(Qt::Key_R, "R"));
	list.push_back(getQTVariant(Qt::Key_T, "T"));
	list.push_back(getQTVariant(Qt::Key_Y, "Y"));
	list.push_back(getQTVariant(Qt::Key_U, "U"));
	list.push_back(getQTVariant(Qt::Key_I, "I"));
	list.push_back(getQTVariant(Qt::Key_O, "O"));
	list.push_back(getQTVariant(Qt::Key_P, "P"));
	list.push_back(getQTVariant(Qt::Key_A, "A"));
	list.push_back(getQTVariant(Qt::Key_S, "S"));
	list.push_back(getQTVariant(Qt::Key_D, "D"));
	list.push_back(getQTVariant(Qt::Key_F, "F"));
	list.push_back(getQTVariant(Qt::Key_G, "G"));
	list.push_back(getQTVariant(Qt::Key_H, "H"));
	list.push_back(getQTVariant(Qt::Key_J, "J"));
	list.push_back(getQTVariant(Qt::Key_K, "K"));
	list.push_back(getQTVariant(Qt::Key_L, "L"));
	list.push_back(getQTVariant(Qt::Key_Z, "Z"));
	list.push_back(getQTVariant(Qt::Key_X, "X"));
	list.push_back(getQTVariant(Qt::Key_C, "C"));
	list.push_back(getQTVariant(Qt::Key_V, "V"));
	list.push_back(getQTVariant(Qt::Key_B, "B"));
	list.push_back(getQTVariant(Qt::Key_N, "N"));
	list.push_back(getQTVariant(Qt::Key_M, "M"));
	list.push_back(getQTVariant(Qt::Key_F1, "F1"));
	list.push_back(getQTVariant(Qt::Key_F2, "F2"));
	list.push_back(getQTVariant(Qt::Key_F3, "F3"));
	list.push_back(getQTVariant(Qt::Key_F4, "F4"));
	list.push_back(getQTVariant(Qt::Key_F5, "F5"));
	list.push_back(getQTVariant(Qt::Key_F6, "F6"));
	list.push_back(getQTVariant(Qt::Key_F7, "F7"));
	list.push_back(getQTVariant(Qt::Key_F8, "F8"));
	list.push_back(getQTVariant(Qt::Key_F9, "F9"));
	list.push_back(getQTVariant(Qt::Key_F10, "F10"));
	list.push_back(getQTVariant(Qt::Key_F11, "F11"));
	list.push_back(getQTVariant(Qt::Key_F12, "F12"));
	list.push_back(getQTVariant(Qt::Key_Escape, "Escape"));
	list.push_back(getQTVariant(Qt::Key_Tab, "Tab"));
	list.push_back(getQTVariant(Qt::Key_Backtab, "Backtab"));
	list.push_back(getQTVariant(Qt::Key_Backspace, "Backspace"));
	list.push_back(getQTVariant(Qt::Key_Return, "Enter"));
	list.push_back(getQTVariant(Qt::Key_Enter, "Enter"));
	list.push_back(getQTVariant(Qt::Key_Insert, "Insert"));
	list.push_back(getQTVariant(Qt::Key_Delete, "Delete"));
	list.push_back(getQTVariant(Qt::Key_Pause, "Pause"));
	list.push_back(getQTVariant(Qt::Key_Print, "Print"));
	list.push_back(getQTVariant(Qt::Key_SysReq, "SysReq"));
	list.push_back(getQTVariant(Qt::Key_Clear, "Clear"));
	list.push_back(getQTVariant(Qt::Key_Home, "Home"));
	list.push_back(getQTVariant(Qt::Key_End, "End"));
	list.push_back(getQTVariant(Qt::Key_Left, "Left"));
	list.push_back(getQTVariant(Qt::Key_Up, "Up"));
	list.push_back(getQTVariant(Qt::Key_Right, "Right"));
	list.push_back(getQTVariant(Qt::Key_Down, "Down"));
	list.push_back(getQTVariant(Qt::Key_PageUp, "PageUp"));
	list.push_back(getQTVariant(Qt::Key_PageDown, "PageDown"));
	list.push_back(getQTVariant(Qt::Key_Shift, "Shift"));
	list.push_back(getQTVariant(Qt::Key_Control, "Control"));
	list.push_back(getQTVariant(Qt::Key_Meta, "Meta"));
	list.push_back(getQTVariant(Qt::Key_Alt, "Alt"));
	list.push_back(getQTVariant(Qt::Key_CapsLock, "CapsLock"));
	list.push_back(getQTVariant(Qt::Key_NumLock, "NumLock"));
	list.push_back(getQTVariant(Qt::Key_ScrollLock, "ScrollLock"));
	list.push_back(getQTVariant(Qt::Key_Plus, "Plus"));
	list.push_back(getQTVariant(Qt::Key_Comma, "Comma"));
	list.push_back(getQTVariant(Qt::Key_Minus, "Minus"));
	list.push_back(getQTVariant(Qt::Key_Period, "Period"));
	list.push_back(getQTVariant(Qt::Key_Slash, "Slash"));
	list.push_back(getQTVariant(Qt::Key_Space, "Space"));
	list.push_back(getQTVariant(Qt::Key_QuoteLeft, "QuoteLeft"));
	list.push_back(getQTVariant(Qt::Key_Equal, "Equal"));
	list.push_back(getQTVariant(Qt::Key_BraceLeft, "BraceLeft"));
	list.push_back(getQTVariant(Qt::Key_BraceRight, "BraceRight"));
	list.push_back(getQTVariant(Qt::Key_Semicolon, "Semicolon"));
	list.push_back(getQTVariant(Qt::Key_Apostrophe, "Apostrophe"));
	list.push_back(getQTVariant(Qt::Key_Backslash, "Backslash"));
	list.push_back(getQTVariant(Qt::Key_BracketLeft, "BracketLeft"));
	list.push_back(getQTVariant(Qt::Key_BracketRight, "BracketRight"));
	list.push_back(getQTVariant(Qt::Key_Asterisk, "Asterisk"));
	document.insert("List", list);
	QJsonDocument doc = QJsonDocument::fromVariant(document);
	writeFile(path, doc.toJson(), false);
}

void generateCocosKeyboardMapping()
{
	auto path = QDir::currentPath() + "/../Resources/CocosKeyboardStringMapping";
	QVariantMap document;
	QVariantList list;
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_0, "0"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_1, "1"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_2, "2"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_3, "3"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_4, "4"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_5, "5"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_6, "6"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_7, "7"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_8, "8"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_9, "9"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_A, "A"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_B, "B"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_C, "C"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_D, "D"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_E, "E"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F, "F"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_G, "G"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_H, "H"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_I, "I"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_J, "J"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_K, "K"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_L, "L"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_M, "M"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_N, "N"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_O, "O"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_P, "P"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_Q, "Q"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_R, "R"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_S, "S"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_T, "T"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_U, "U"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_V, "V"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_W, "W"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_X, "X"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_Y, "Y"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_Z, "Z"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F1, "F1"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F2, "F2"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F3, "F3"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F4, "F4"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F5, "F5"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F6, "F6"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F7, "F7"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F8, "F8"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F9, "F9"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F10, "F10")); 
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F11, "F11"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_F12, "F12"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_ESCAPE, "Escape"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_TAB, "Tab"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_BACK_TAB, "Backtab"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_BACKSPACE, "Backspace"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_RETURN, "Enter"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_ENTER, "Enter"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_KP_ENTER, "Enter"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_INSERT, "Insert"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_DELETE, "Delete"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_PAUSE, "Pause"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_PRINT, "Print"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_SYSREQ, "SysReq"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_HOME, "Home"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_END, "End"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_LEFT_ARROW, "Left"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_UP_ARROW, "Up"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, "Right"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_DOWN_ARROW, "Down"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_PG_UP, "PageUp"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_PG_DOWN, "PageDown"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_SHIFT, "Shift"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_CTRL, "Control"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_ALT, "Alt"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_CAPS_LOCK, "CapsLock"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_NUM_LOCK, "NumLock")); 
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_SCROLL_LOCK, "ScrollLock"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_PLUS, "Plus"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_COMMA, "Comma"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_MINUS, "Minus"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_PERIOD, "Period"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_SLASH, "Slash"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_SPACE, "Space"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_EQUAL, "Equal"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_LEFT_BRACE, "BraceLeft"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_RIGHT_BRACE, "BraceRight"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_SEMICOLON, "Semicolon"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_APOSTROPHE, "Apostrophe"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_BACK_SLASH, "Backslash"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_LEFT_BRACKET, "BracketLeft"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_RIGHT_BRACKET, "BracketRight"));
	list.push_back(getCocosVariant(EventKeyboard::KeyCode::KEY_ASTERISK, "Asterisk"));

	document.insert("List", list);
	QJsonDocument doc = QJsonDocument::fromVariant(document);
	writeFile(path, doc.toJson(), false);
}

QVariantMap getQTVariant(Qt::Key key, QString str){
	QVariantMap v;
	v.insert("Key", key);
	v.insert("Value", str);
	return v;
}

QVariantMap getCocosVariant(EventKeyboard::KeyCode key, QString str)
{
	QVariantMap v;
	v.insert("Key", (int)key);
	v.insert("Value", str);
	return v;
}