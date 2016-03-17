#include "WinEventDispatcher.h"
#include "Global.h"
#include "QDockWidget"
#include "QTEditor.h"
#include "HelloWorldScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static WinEventDispatcher* instance = nullptr;

HHOOK keyHook = NULL;

HHOOK mouseHook = NULL;
bool leftButtonDown = false;

LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

Point WinEventDispatcher::getMousePosInCocos()
{
	POINT pNow = { 0, 0 };
	GetCursorPos(&pNow);
	RECT rc;
	::GetWindowRect(g_cocos_HWND, &rc);
	Point pos;
	pos.y = rc.bottom - pNow.y;
	pos.x = pNow.x - rc.left;
	return pos;
}

void WinEventDispatcher::sceneTouchBegan()
{
	auto scene = static_cast<HelloWorld*>(g_scene);
	Point pos = getMousePosInCocos();
	startPos = pos;
	scene->onTouchBeganImageSprite(scene->getSceneScalePos(pos));
}

void WinEventDispatcher::sceneTouchMoved()
{
	auto scene = static_cast<HelloWorld*>(g_scene);
	Point pos = getMousePosInCocos(); 
	scene->onTouchMovedImageSprite(scene->getSceneScalePos(pos));
}

void WinEventDispatcher::sceneTouchEnded()
{
	Point pos = getMousePosInCocos();
	auto scene = static_cast<HelloWorld*>(g_scene);
	scene->onTouchEndedImageSprite(scene->getSceneScalePos(startPos), scene->getSceneScalePos(pos));
}

//鼠标钩子过程
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_LBUTTONDOWN && instance->inEditorHWND(instance->curHwnd)){
		leftButtonDown = true;
	}
	else if (wParam == WM_MOUSEMOVE && leftButtonDown){
		if (instance->inEditorHWND(instance->curHwnd)){

		}
		else if (instance->curHwnd == g_cocos_HWND && g_filePath != ""){
			if (!instance->hasBegin){
				instance->hasBegin = true;
				instance->sceneTouchBegan();
			}
			else{
				instance->sceneTouchMoved();
			}
		}
	}
	else if (wParam == WM_LBUTTONUP && leftButtonDown){
		leftButtonDown = false;
		if (getEditorCursor() != Cursor_Type::Normal){
			setEditorCursor(Cursor_Type::Normal);
		}
		if (instance->curHwnd == g_cocos_HWND){
			instance->sceneTouchEnded();
			instance->hasBegin = false;
		}
	}
	return 0;
}

void unHook()
{
	UnhookWindowsHookEx(keyHook);
	UnhookWindowsHookEx(mouseHook);
}

WinEventDispatcher::WinEventDispatcher()
{
	newHwnd = nullptr;
	hasBegin = false;
	oldHwnd = nullptr;
}

WinEventDispatcher::~WinEventDispatcher()
{
	unHook();
}

WinEventDispatcher* WinEventDispatcher::getInstance()
{
	if (!instance){
		instance = new WinEventDispatcher;
		instance->init();
		return instance;
	}
	return instance;
}

bool WinEventDispatcher::init()
{
	if (!Sprite::init()){
		return false;
	}
	instance = this;
	initEditorHWND();
	initHook();
	this->scheduleUpdate();
	return true;
}

void WinEventDispatcher::update(float dt)
{
	POINT pNow = { 0, 0 };
	if (GetCursorPos(&pNow)){
		HWND hwndPointNow = nullptr;
		hwndPointNow = WindowFromPoint(pNow);
		curHwnd = hwndPointNow;
		if (inEditorHWND(hwndPointNow) && hwndPointNow != newHwnd){
			oldHwnd = newHwnd;
			newHwnd = hwndPointNow;
		}
		else if (hwndPointNow == g_cocos_HWND && hwndPointNow != newHwnd){
			oldHwnd = newHwnd;
			newHwnd = hwndPointNow;
		}
	}
}

bool WinEventDispatcher::inEditorHWND(HWND hwnd)
{
	for (int i = 0; i < editorHwndVec.size(); i++){
		if (hwnd == editorHwndVec.at(i)){
			return true;
		}
	}
	return false;
}

void WinEventDispatcher::initEditorHWND()
{
	auto editor = static_cast<QTEditor*>(g_editor);
	auto templateView = static_cast<QDockWidget*>(g_TemplateView);
	auto controlView = static_cast<QDockWidget*>(g_ControlView);
	auto resourceView = static_cast<QDockWidget*>(g_resourceView);
	auto exteralView = static_cast<QDockWidget*>(g_exteralView);
	auto layerManagerView = static_cast<QDockWidget*>(g_LayerManager);
	editorHwndVec.push_back((HWND)editor->winId());
	editorHwndVec.push_back((HWND)templateView->winId());
	editorHwndVec.push_back((HWND)controlView->winId());
	editorHwndVec.push_back((HWND)resourceView->winId());
	editorHwndVec.push_back((HWND)exteralView->winId());
	editorHwndVec.push_back((HWND)layerManagerView->winId());
	setEditorHwndVec(editorHwndVec);
}

void WinEventDispatcher::initHook()
{
	//HINSTANCE hInstance = (HINSTANCE)::GetWindowLong(g_cocos_HWND, GWL_HINSTANCE);
	////底层键盘钩子
	////keyHook = SetWindowsHookEx(WH_KEYBOARD, keyProc, 0, GetCurrentThreadId());
	////底层鼠标钩子
	//mouseHook = SetWindowsHookEx(WH_MOUSE, mouseProc, 0, GetCurrentThreadId());
}
#endif

