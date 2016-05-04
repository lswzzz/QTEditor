#include "qteditor.h"
#include <QtWidgets/QApplication>
#include "cocos2d.h"
#include "AppDelegate.h"
#include "Global.h"
#include <Dbghelp.h>
#include <strsafe.h>
#include <QTime>
#include <QMessageBox>
#include "QTime"
#include "qdir.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif   
#pragma comment(lib, "Dbghelp.lib")

#define ADD_DUMP 1

//最主要的函数, 生成Dump
static void DumpMiniDump(HANDLE hFile, PEXCEPTION_POINTERS excpInfo)
{
	if (excpInfo == NULL) //如果没有传入异常, 比如是在程序里面调用的, 生成一个异常
	{
		// Generate exception to get proper context in dump
		__try
		{
			const char* x = "raising exception\r\n";
			WCHAR y[64];
			LPCWSTR py = y;
			MultiByteToWideChar(0, 0, x, -1, y, 64);
			OutputDebugString(py);
			RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
		}
		__except (DumpMiniDump(hFile, GetExceptionInformation()),
			EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}
	else
	{
		const char* x = "writing minidump\r\n";
		WCHAR y[64];
		LPCWSTR py = y;
		MultiByteToWideChar(0, 0, x, -1, y, 64);
		OutputDebugString(py);
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId(); //把需要的信息添进去
		eInfo.ExceptionPointers = excpInfo;
		eInfo.ClientPointers = FALSE;

		// 调用, 生成Dump. 98不支持
		// Dump的类型是小型的, 节省空间. 可以参考MSDN生成更详细的Dump.
		MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			MiniDumpNormal,
			excpInfo ? &eInfo : NULL,
			NULL,
			NULL);
	}
}

int main_wrapper(int argc, char *argv[])
{
	auto path = QDir::currentPath() + "/../Resources/";
	QDir::setCurrent(path);
	FileUtils::getInstance()->addSearchPath(QDir::currentPath().toStdString(), true);
	CCQApplication::addLibraryPath("./plugins");
	AppDelegate app(argc, argv);
	QTEditor w;
	w.show();
	RECT t;
	
	auto widget = w.getUI().MainWidget;
	t.left = widget->rect().left();
	t.right = widget->rect().right()+1;
	t.top = widget->rect().top();
	t.bottom = widget->rect().bottom()+1;
	
	app.setParent((HWND)widget->winId(), t);
	
	/*QFile qss(path + "../QTEditor/Resources/StyleSheets/QTDark.stylesheet");
	qss.open(QFile::ReadOnly);
	app.setStyleSheet(qss.readAll());*/

	//http://blog.sina.com.cn/s/blog_3e62c50d010118iy.html
	return cocos2d::CCQApplication::getInstance()->run();
}

int main(int argc, char *argv[])
{
#if ADD_DUMP
	HANDLE hFile;
#if _DEBUG
	hFile = CreateFile(L"./../Debug.win32/MiniDump.dmp", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#else
	hFile = CreateFile(L"./MiniDump.dmp", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#endif
	int code;
	__try
	{
		// 把自己实现的main函数包装一下, 放在try .. except 块中. 这样出现了异常可以自动生成dump
		main_wrapper(argc, argv);
	}
	__except (code = GetExceptionCode(), DumpMiniDump(hFile, GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) //出现了异常, 记录异常的code, 生成dump!!
	{
		char msg[512];
		sprintf(msg, "Exception happened. Exception code is %x", code);
		MessageBox(msg, "Exception"); //显示消息给用户
	}
	CloseHandle(hFile); //关闭Dump文件
#else
	main_wrapper(argc, argv);
#endif
	return 0;
}
