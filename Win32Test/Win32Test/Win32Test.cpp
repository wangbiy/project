#include <Windows.h>
#include <tchar.h>
#include "resource2.h"
//消息回调函数
POINT start;//起点
POINT end;//终点
int state = 0;//刚开始的状态，来控制画什么图形
//用户自己定义：函数的格式必须按照系统的固定格式来定义，
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND://菜单所对应的消息
	{
		switch (wParam)
        {
		case ID_LINE:
			state = 1;
			break;
		case ID_RECT:
			state = 2;
			break;
		case ID_CIRCLE:
			state = 3;
			break;
	    }	
		return 0;
	}
	case WM_MOUSEMOVE://鼠标移动，要获取起点，在移动期间，触发重绘
		end.x = LOWORD(lParam);//改变终点
		end.y = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, true);//触发重绘
		break;
	case WM_LBUTTONDOWN://按鼠标就要触发重绘
		//MessageBox(NULL, _T("LBTNDOWN"), _T("Test"), IDOK);
	{
		start.x = LOWORD(lParam);
		start.y = HIWORD(lParam);
		return 0;
	}
	case WM_LBUTTONUP://放开鼠标
	{
		end.x = LOWORD(lParam);
		end.y = HIWORD(lParam);
		HDC hdc = GetDC(hWnd);
		switch (state)
		{
		case 1://画直线
			MoveToEx(hdc, start.x, start.y, NULL);//将光标移到起点的位置
			LineTo(hdc, end.x, end.y);//画到终点的位置
			break;
		case 2://画矩形
			Rectangle(hdc, start.x, start.y, end.x, end.y);//画矩形
			break;
		case 3://画椭圆
			Ellipse(hdc, start.x, start.y, end.x, end.y);
			break;
		}	
		ReleaseDC(hWnd, hdc);//用完了要进行释放
		start = end;//每次画完后将起点和终点放在同一位置
		return 0;
	}
	case WM_PAINT://重绘
	{
	    HDC hdc = GetDC(hWnd);
		switch (state)
		{
		case 1://画直线
			MoveToEx(hdc, start.x, start.y, NULL);//将光标移到起点的位置
			LineTo(hdc, end.x, end.y);//画到终点的位置
			break;
		case 2://画矩形
			Rectangle(hdc, start.x, start.y, end.x, end.y);//画矩形
			break;
		case 3://画椭圆
			Ellipse(hdc, start.x, start.y, end.x, end.y);
			break;
		}
		ReleaseDC(hWnd, hdc);//用完了要进行释放
		return 0;
	}
	case WM_CLOSE:
		DestroyWindow(hWnd);
	    return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)//hInstance相当于这个应用程序
{
	//MessageBox(NULL,_T("hell Win32"),_T("Win32"),IDCANCEL);//IDCANCEL/IDOK/等改变按钮
	//Step1:注册一个窗口类
	HWND hwnd; //窗口的句柄
	WNDCLASSEX wc; //窗口类结构
	wc.cbSize = sizeof(WNDCLASSEX);//当前结构体总共占了多少个字节
	wc.style = CS_VREDRAW | CS_HREDRAW;//窗口刷新机制
	wc.lpszMenuName = 0;//是否有菜单
	wc.lpszClassName = _T("Win32");//标记这个窗口
	wc.lpfnWndProc = WinProc; //消息回调函数，用户必须提供
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc); //注册窗口
	//Step2:创建一个窗口
	hwnd = CreateWindow(
		_T("Win32"), //窗口的类名，也就是上面我们自定义的窗口类的名字
		_T("我的第一个Win32程序"), //窗口的标题
		WS_OVERLAPPEDWINDOW, //窗口style
		500, //窗口位置x坐标
		300, //窗口位置y坐标
		800, //窗口宽度
		600, //窗口高度
		NULL, //父窗口句柄
		LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1)),//菜单句柄,将菜单转为字符串形式
		hInstance, //实例句柄
		NULL //创建数据
		);
	if (!hwnd)
	{
		return FALSE;
	}
	ShowWindow(hwnd, SW_SHOW); //显示窗口
	UpdateWindow(hwnd); //刷新
	//Step3:消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
