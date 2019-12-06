#include "UIlib.h"

using namespace DuiLib;

#pragma comment(lib, "DuiLib_ud.lib")
#if 0
class CDuiFramWnd : public CWindowWnd,public INotifyUI
{
public:
	// CWindowWnd��Ĵ��麯�����ڸú����б��뷵���û������崰�ڵ������ƣ�ע�ᴰ��ʱ��Ҫ�õ�
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("DuiFramWnd");
	}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)//����ϵͳ��Ϣ
	{
		if (WM_CREATE == uMsg)
		{
			////��ʼ���滭������
			//m_PaintManager.Init(m_hWnd);//m_hWnd�������еľ��

			////�ڴ��ڴ����ڼ䴴��һ����ť
			//CControlUI* pBTN = new CButtonUI;//���尴ť
			//pBTN->SetText(_T("hello"));//���ô��ڰ�ť�ı�
			//pBTN->SetBkColor(0xFF00FF00);//���ô��ڰ�ť��ɫ

			//m_PaintManager.AttachDialog(pBTN);//����ť���������ƹ�������

			//m_PaintManager.AddNotifier(this);//����ť���ӵ���Ϣѭ����
			m_PaintManager.Init(m_hWnd);
			CDialogBuilder builder;
			// duilib.xml��Ҫ�ŵ�exeĿ¼��
			CControlUI* pRoot = builder.Create(_T("111.xml"), (UINT)0, NULL, &m_PaintManager);//Ҫ����·��
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
		//���ػỰ�����Ϣ
		LRESULT lRse = 0;
		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRse))
		{
			return lRse;
		}
		//������Ϣ
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	virtual void Notify(TNotifyUI& msg)
	{
		//��Ӧ��ť������Ϣ
		if (msg.sType == _T("click"))//ע�����ﲻ�ܽ���������sType
		{
			MessageBox(m_hWnd, _T("��ť����"), _T("Test"), IDOK);
		}

	}
private:
	CPaintManagerUI m_PaintManager;
};


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	//��Ӧ111.xml�ļ�����ΪҪ�ҵ�·��
	CPaintManagerUI::SetInstance(hInstance);
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�
	framWnd.Create(NULL, _T("DulibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	//��ʾ���ڣ�������Ϣѭ��
	framWnd.ShowModal();
	return 0;
}
#endif
class CDuiFramWnd : public WindowImplBase
{
protected:
	virtual CDuiString GetSkinFolder()
	{
		return _T("");//���ﲻ�ø�xml�ļ���·������Ϊ�����������Ѿ�������
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
			MessageBox(m_hWnd, _T("��ť����"), _T("����"), IDOK);
		}
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	//��Ӧ111.xml�ļ�����ΪҪ�ҵ�·��
	CPaintManagerUI::SetInstance(hInstance);
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�
	framWnd.Create(NULL, _T("DulibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	framWnd.CenterWindow();
	//��ʾ���ڣ�������Ϣѭ��
	framWnd.ShowModal();
	return 0;
}

