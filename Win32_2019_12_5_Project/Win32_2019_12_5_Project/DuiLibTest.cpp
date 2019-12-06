#include "UIlib.h"

using namespace DuiLib;

#pragma comment(lib, "DuiLib_ud.lib")
#if 0
class CDuiFramWnd : public CWindowWnd,public INotifyUI
{
public:
	// CWindowWnd类的纯虚函数，在该函数中必须返回用户所定义窗口的类名称，注册窗口时需要用到
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("DuiFramWnd");
	}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)//处理系统消息
	{
		if (WM_CREATE == uMsg)
		{
			////初始化绘画管理器
			//m_PaintManager.Init(m_hWnd);//m_hWnd：基类中的句柄

			////在窗口创建期间创建一个按钮
			//CControlUI* pBTN = new CButtonUI;//定义按钮
			//pBTN->SetText(_T("hello"));//设置窗口按钮文本
			//pBTN->SetBkColor(0xFF00FF00);//设置窗口按钮颜色

			//m_PaintManager.AttachDialog(pBTN);//将按钮关联到绘制管理器上

			//m_PaintManager.AddNotifier(this);//将按钮增加到消息循环中
			m_PaintManager.Init(m_hWnd);
			CDialogBuilder builder;
			// duilib.xml需要放到exe目录下
			CControlUI* pRoot = builder.Create(_T("111.xml"), (UINT)0, NULL, &m_PaintManager);//要设置路径
			m_PaintManager.AttachDialog(pRoot);
			m_PaintManager.AddNotifier(this);

			return 0;
		}
		else if (uMsg == WM_NCACTIVATE)
		{
			if (!::IsIconic(m_hWnd))
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}
		else if (uMsg == WM_NCCALCSIZE)
		{
			return 0;
		}
		else if (uMsg == WM_NCPAINT)
		{
			return 0;
		}
		//拦截会话相关消息
		LRESULT lRse = 0;
		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRse))
		{
			return lRse;
		}
		//其他消息
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	virtual void Notify(TNotifyUI& msg)
	{
		//响应按钮单击消息
		if (msg.sType == _T("click"))//注意这里不能交换常量和sType
		{
			MessageBox(m_hWnd, _T("按钮单击"), _T("Test"), IDOK);
		}

	}
private:
	CPaintManagerUI m_PaintManager;
};


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	//对应111.xml文件，因为要找到路径
	CPaintManagerUI::SetInstance(hInstance);
	// 设置资源的默认路径（此处设置为和exe在同一目录）
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框
	framWnd.Create(NULL, _T("DulibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	//显示窗口，激活消息循环
	framWnd.ShowModal();
	return 0;
}
#endif
class CDuiFramWnd : public WindowImplBase
{
protected:
	virtual CDuiString GetSkinFolder()
	{
		return _T("");//这里不用给xml文件的路径，因为在主函数中已经给过了
	}
	virtual CDuiString GetSkinFile()
	{
		return _T("111.xml");

	}
	virtual LPCTSTR GetWindowClassName(void)const
	{
		return _T("DuiWnd");
	}
	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			MessageBox(m_hWnd, _T("按钮单击"), _T("标题"), IDOK);
		}
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	//对应111.xml文件，因为要找到路径
	CPaintManagerUI::SetInstance(hInstance);
	// 设置资源的默认路径（此处设置为和exe在同一目录）
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框
	framWnd.Create(NULL, _T("DulibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	framWnd.CenterWindow();
	//显示窗口，激活消息循环
	framWnd.ShowModal();
	return 0;
}

