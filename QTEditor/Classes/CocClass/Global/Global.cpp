#include "Global.h"
#include "FontChina.h"
#include <cstdarg>
#include <qplaintextedit.h>
#include <qtextcursor.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include "resource.h"
#include <atlstr.h>
#endif
#include <QTextStream>
#include <QJsonDocument>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "MyLogger.h"

QPlainTextEdit* logText = nullptr;
QPlainTextEdit* infoText = nullptr;
Layer* g_scene = nullptr;
std::string g_filePath = "";
std::string g_animationName = "";
Image_Type g_ImageType = Image_Type::NormalBlock;
QMainWindow* g_editor = nullptr;
QWidget* g_LayerManager = nullptr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
std::string g_textureCMD = "../TexturePacker/bin/TexturePacker.exe ";
std::string g_imageOptimizeCMD = "../TexturePacker/pngquant.exe";
#else
std::string g_textureCMD = "../TexturePacker.app/Contents/MacOS/TexturePacker ";
std::string g_imageOptimizeCMD = "";
#endif
std::string g_output_Dir = "";
std::string g_output_Name = "";
QWidget* g_TemplateView = nullptr;
QWidget* g_TextureView = nullptr;
QWidget* g_resourceView = nullptr;
QWidget* g_exteralView = nullptr;
QWidget* g_gridWidgetContents = nullptr;
QWidget* g_ControlView = nullptr;
QWidget* g_configView = nullptr;
QWidget* g_AnimationView = nullptr;
std::string g_openConfileDir = "";
std::string g_templateDir = "./../Resources/template";
int g_dynamic_count = 0;
int g_static_count = 0;
int g_animate_count = 0;
unsigned int g_template_count = 0;
std::string g_TemplateFile = "";
bool g_openConfile = false;
QSize g_CocosWindowInitSize = QSize(960,640);

QSize oldWindowSize(960,640);
QSize newWindowSize(960,640);

void resetOldAndNewWindowSize(QSize oldSize, QSize newSize)
{
    oldWindowSize = oldSize;
    newWindowSize = newSize;
}
void resetWindowSize(QSize size)
{
    oldWindowSize = newWindowSize;
    newWindowSize = size;
}
QSize getOldWindowSize()
{
    return oldWindowSize;
}
QSize getNewWindowSize()
{
    return newWindowSize;
}

std::recursive_mutex g_ConsoleInfomutex;

void addConsoleInfo(std::string fmt, ...)
{
	std::unique_lock<std::recursive_mutex> lk(g_ConsoleInfomutex);
    MyLogger::getInstance()->addInfo("addConsoleInfo be Called, the arg is " + fmt);
    char buf[4096] = {0};
    char* p = buf;
    std::string str;
    va_list args;
    int count = 0;
    size_t pos = 0;
    while (pos != std::string::npos){
        pos = fmt.find("%", pos);
        if (pos != std::string::npos){
            count++;
            pos += (fmt.size() + 1);
        }
    }
    if (count != 0){
        va_start(args, fmt);
        vsprintf(p, fmt.c_str(), args);
        va_end(args);
        str = buf;
        str += "\n";
    }
    else{
        str = fmt;
        str += "\n";
    }
    
    QTextCursor cursor = logText->textCursor();
    cursor.movePosition(QTextCursor::End);
    logText->setTextCursor(cursor);
    logText->insertPlainText(FontChina::G2U(str.c_str()));
    cursor.movePosition(QTextCursor::End);
    logText->setTextCursor(cursor);
    logText->ensureCursorVisible();
}

std::mutex g_AttributeMutex;
void setAttributeInfo(std::string str)
{
	g_AttributeMutex.lock();
	infoText->clear();
    QTextCursor cursor = infoText->textCursor();
    infoText->setTextCursor(cursor);
    infoText->insertPlainText(FontChina::G2U((str).c_str()));
    cursor.movePosition(QTextCursor::End);
    infoText->setTextCursor(cursor);
    infoText->ensureCursorVisible();
	g_AttributeMutex.unlock();
}

void clearConsoleInfo()
{
    logText->clear();
}

int isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();
    const char *s = ba.data();
    
    while (*s && *s >= '0' && *s <= '9') s++;
    
    if (*s)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//IDC_CROSS
//IDC_ARROW
//IDC_SIZEALL
//IDC_HAND
//IDC_SIZENS
//IDC_WAIT
static Cursor_Type _type = Cursor_Type::Normal;
static Cursor_Type old_type = Cursor_Type::Normal;
static Cursor_Type editor_type = Cursor_Type::Normal;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
void setNSViewCuror(std::string str, NSView* view){
    NSString* nss = [NSString stringWithCString:str.c_str() encoding:[NSString defaultCStringEncoding]];
    NSImage *image = [NSImage imageNamed:nss];
    [image setSize:NSMakeSize(32, 32)];
    NSCursor *cursor = [[NSCursor alloc] initWithImage:image hotSpot:NSMakePoint(1, 1)];
    [view resetCursorRects];
    [view addCursorRect:[view bounds] cursor:cursor];
}
#endif

void setCocCursor(Cursor_Type type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    HICON icon;
    CString temp;
    std::string str;
    old_type = _type;
    _type = type;
    HINSTANCE hInstance = (HINSTANCE)::GetWindowLong(g_cocos_HWND, GWL_HINSTANCE);
    switch (type){
        case Cursor_Type::addSprite:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/add.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::addTemplate:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/template.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::Move_Scene:
            icon = LoadCursor(NULL, IDC_HAND);
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::Normal:
            icon = LoadCursor(NULL, IDC_ARROW);
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::Move:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/move.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::Block:
            icon = LoadCursor(NULL, IDC_CROSS);
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::Ctrl:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/copyway.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::Alt:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/cross_r.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::SubBlock:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/sub.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::BlockDirection:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/direction.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
        case Cursor_Type::Wait:
            icon = LoadCursor(NULL, IDC_WAIT);
            SetClassLong(g_cocos_HWND, GCL_HCURSOR, (LONG)icon);
            break;
    }
#else
    std::string str;
    NSView *view = g_cocView;
    old_type = _type;
    _type = type;
    switch (type){
        case Cursor_Type::addSprite:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/add.png");
            setNSViewCuror(str, view);
            break;
        case Cursor_Type::addTemplate:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/template.png");
            setNSViewCuror(str, view);
            break;
        case Cursor_Type::Move_Scene:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/move.png");
            break;
        case Cursor_Type::Normal:
            str = FileUtils::getInstance()->fullPathForFilename("");
            break;
        case Cursor_Type::Move:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/move.png");
            setNSViewCuror(str, view);
            break;
        case Cursor_Type::Block:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/block.png");
            break;
        case Cursor_Type::Ctrl:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/copyway.png");
            setNSViewCuror(str, view);
            break;
        case Cursor_Type::Alt:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/cross_r.png");
            setNSViewCuror(str, view);
            break;
        case Cursor_Type::SubBlock:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/sub.png");
            setNSViewCuror(str, view);
            break;
        case Cursor_Type::BlockDirection:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/direction.png");
            setNSViewCuror(str, view);
            break;
        case Cursor_Type::Wait:
            break;
    }
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
std::vector<HWND> g_EditorHwndVec;
HWND g_cocos_HWND = NULL;
void setEditorHwndVec(std::vector<HWND> vec)
{
    g_EditorHwndVec.clear();
    for (int i = 0; i < vec.size(); i++){
        g_EditorHwndVec.push_back(vec.at(i));
    }
}
#else
id g_cocView;
void resetCocosSize(int width, int height)
{
    NSView* view = g_cocView;
    NSSize size = NSMakeSize(width, height);
    NSRect rect = NSMakeRect(0, 0, width, height);
    [view setFrameSize:size];
}
#endif

void setEditorCursor(Cursor_Type type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    HICON icon;
    CString temp;
    std::string str;
    editor_type = type;
    HINSTANCE hInstance = (HINSTANCE)::GetWindowLong(g_cocos_HWND, GWL_HINSTANCE);
    switch (type){
        case Cursor_Type::addSprite:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/add.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            for (int i = 0; i < g_EditorHwndVec.size(); i++){
                SetCursor(icon);
            }
            break;
        case Cursor_Type::addTemplate:
            str = FileUtils::getInstance()->fullPathForFilename("./../Resources/CURSOR/template.cur");
            temp = str.c_str();
            icon = LoadCursorFromFile((LPCWSTR)(temp.AllocSysString()));
            for (int i = 0; i < g_EditorHwndVec.size(); i++){
                SetCursor(icon);
            }
            break;
        case Cursor_Type::Normal:
            icon = LoadCursor(hInstance, IDC_ARROW);
            for (int i = 0; i < g_EditorHwndVec.size(); i++){
                SetCursor(icon);
            }
            break;
    }
#else
    editor_type = type;
    switch (type){
        case Cursor_Type::addSprite:
            break;
        case Cursor_Type::addTemplate:
            break;
        case Cursor_Type::Normal:
            break;
    }
#endif
}

Cursor_Type getCursorType()
{
    return _type;
}

Cursor_Type getOldCursorType()
{
    return old_type;
}

Cursor_Type getEditorCursor()
{
    return editor_type;
}

cocos2d::Point changePosFromAnchr(Node* node)
{
	auto position = node->getPosition();
	auto anchr = node->getAnchorPoint();
	auto size = cocos2d::Size(node->getContentSize().width*node->getScaleX(), node->getContentSize().height*node->getScaleY());

	return position;
}