#include <Windows.h>
#include <tchar.h>
#include "resource2.h"
//��Ϣ�ص�����
POINT start;//���
POINT end;//�յ�
int state = 0;//�տ�ʼ��״̬�������ƻ�ʲôͼ��
//�û��Լ����壺�����ĸ�ʽ���밴��ϵͳ�Ĺ̶���ʽ�����壬
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND://�˵�����Ӧ����Ϣ
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
	case WM_MOUSEMOVE://����ƶ���Ҫ��ȡ��㣬���ƶ��ڼ䣬�����ػ�
		end.x = LOWORD(lParam);//�ı��յ�
		end.y = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, true);//�����ػ�
		break;
	case WM_LBUTTONDOWN://������Ҫ�����ػ�
		//MessageBox(NULL, _T("LBTNDOWN"), _T("Test"), IDOK);
	{
		start.x = LOWORD(lParam);
		start.y = HIWORD(lParam);
		return 0;
	}
	case WM_LBUTTONUP://�ſ����
	{
		end.x = LOWORD(lParam);
		end.y = HIWORD(lParam);
		HDC hdc = GetDC(hWnd);
		switch (state)
		{
		case 1://��ֱ��
			MoveToEx(hdc, start.x, start.y, NULL);//������Ƶ�����λ��
			LineTo(hdc, end.x, end.y);//�����յ��λ��
			break;
		case 2://������
			Rectangle(hdc, start.x, start.y, end.x, end.y);//������
			break;
		case 3://����Բ
			Ellipse(hdc, start.x, start.y, end.x, end.y);
			break;
		}	
		ReleaseDC(hWnd, hdc);//������Ҫ�����ͷ�
		start = end;//ÿ�λ���������յ����ͬһλ��
		return 0;
	}
	case WM_PAINT://�ػ�
	{
	    HDC hdc = GetDC(hWnd);
		switch (state)
		{
		case 1://��ֱ��
			MoveToEx(hdc, start.x, start.y, NULL);//������Ƶ�����λ��
			LineTo(hdc, end.x, end.y);//�����յ��λ��
			break;
		case 2://������
			Rectangle(hdc, start.x, start.y, end.x, end.y);//������
			break;
		case 3://����Բ
			Ellipse(hdc, start.x, start.y, end.x, end.y);
			break;
		}
		ReleaseDC(hWnd, hdc);//������Ҫ�����ͷ�
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
	nCmdShow)//hInstance�൱�����Ӧ�ó���
{
	//MessageBox(NULL,_T("hell Win32"),_T("Win32"),IDCANCEL);//IDCANCEL/IDOK/�ȸı䰴ť
	//Step1:ע��һ��������
	HWND hwnd; //���ڵľ��
	WNDCLASSEX wc; //������ṹ
	wc.cbSize = sizeof(WNDCLASSEX);//��ǰ�ṹ���ܹ�ռ�˶��ٸ��ֽ�
	wc.style = CS_VREDRAW | CS_HREDRAW;//����ˢ�»���
	wc.lpszMenuName = 0;//�Ƿ��в˵�
	wc.lpszClassName = _T("Win32");//����������
	wc.lpfnWndProc = WinProc; //��Ϣ�ص��������û������ṩ
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc); //ע�ᴰ��
	//Step2:����һ������
	hwnd = CreateWindow(
		_T("Win32"), //���ڵ�������Ҳ�������������Զ���Ĵ����������
		_T("�ҵĵ�һ��Win32����"), //���ڵı���
		WS_OVERLAPPEDWINDOW, //����style
		500, //����λ��x����
		300, //����λ��y����
		800, //���ڿ��
		600, //���ڸ߶�
		NULL, //�����ھ��
		LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1)),//�˵����,���˵�תΪ�ַ�����ʽ
		hInstance, //ʵ�����
		NULL //��������
		);
	if (!hwnd)
	{
		return FALSE;
	}
	ShowWindow(hwnd, SW_SHOW); //��ʾ����
	UpdateWindow(hwnd); //ˢ��
	//Step3:��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
