#pragma once
#include <qplaintextedit.h>
#include "cocos2d.h"
#include <qobject.h>
#include "AppDelegate.h"
#include "QMainWindow"

using namespace cocos2d;

enum class Image_Type{
    NormalBlock,
    Entity,
	AnimationBlock,
};

extern QPlainTextEdit* logText;
extern QPlainTextEdit* infoText;
extern Layer* g_scene;
extern std::string g_filePath;
extern std::string g_animationName;
extern Image_Type g_ImageType;
extern QMainWindow* g_editor;
extern QWidget* g_LayerManager;
extern QWidget* g_TemplateView;
extern QWidget* g_TextureView;
extern QWidget* g_resourceView;
extern QWidget* g_exteralView;
extern QWidget* g_gridWidgetContents;
extern QWidget* g_configView;
extern std::string g_textureCMD;
extern std::string g_output_Dir;
extern std::string g_imageOptimizeCMD;
extern std::string g_output_Name;
extern std::string g_templateDir;
extern std::string g_TemplateFile;
extern std::string g_openConfileDir;
extern int g_dynamic_count;
extern int g_static_count;
extern unsigned int g_template_count;
extern QWidget* g_ControlView;
extern QWidget* g_AnimationView;
extern bool g_openConfile;
extern QSize g_CocosWindowInitSize;

void resetOldAndNewWindowSize(QSize oldSize, QSize newSize);
void resetWindowSize(QSize size);
QSize getOldWindowSize();
QSize getNewWindowSize();

//void addConsoleInfo(std::string str);
void addConsoleInfo(std::string str, ...);

void setAttributeInfo(std::string str);

void clearConsoleInfo();

int isDigitStr(QString src);

enum class Cursor_Type{
    addSprite,
    addTemplate,
    Move_Scene,
    Normal,
    Move,
    Block,
    Ctrl,
    Alt,
    Wait,
    SubBlock,
    BlockDirection,
};

void setCocCursor(Cursor_Type type);

Cursor_Type getCursorType();

Cursor_Type getOldCursorType();

void setEditorCursor(Cursor_Type type);

Cursor_Type getEditorCursor();

cocos2d::Point changePosFromAnchr(Node* node);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
extern HWND g_cocos_HWND;
void setEditorHwndVec(std::vector<HWND> vec);
#else
extern id g_cocView;
void resetCocosSize(int width, int height);
#endif