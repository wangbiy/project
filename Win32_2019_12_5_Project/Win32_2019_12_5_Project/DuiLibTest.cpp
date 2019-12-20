#include "UIlib.h"

using namespace DuiLib;
#include <fstream>
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
		return _T("gifMake.xml");//һ��Ҫ��Ŀ¼�ŵ�xml�ļ��ļ�������ͬ

	}
	virtual LPCTSTR GetWindowClassName(void)const
	{
		return _T("GIFMakeWnd");
	}
	virtual void Notify(TNotifyUI& msg)//��Ӧ
	{
		CDuiString strName = msg.pSender->GetName();//��ȡ�ؼ�������
		if (msg.sType == _T("click"))//����������
		{		
			if (strName == _T("btn_close"))//����ǹرհ�ť
			{
				Close();
			}
			else if (strName == _T("btn_min"))//�������С����ť
			{
				MessageBox(NULL, _T("��С��"), _T("����"), IDOK);
			}
			else if (strName == _T("btn_load"))
				LoadFile();
			else if (strName == _T("btn_cut"))//��ȡ��Ƶ
			{
				Cutview();
			}
			else if (strName == _T("btn_get_srt"))//��ȡ��Ļ
			{
				GetSRTFile();//��ȡ
				LoadSRT();//����
			}
			else if (strName == _T("btn_commit"))//�ύ��ť
			{
				CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("edit_word"));
				CDuiString strWord=pEdit->GetText();//��ȡ���edit��Ӧ���ı�

				//�����ı�д�ص�list�У���ʱ�Ѿ���edit�б༭����
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));//�Ȼ�ȡlist�ؼ�
				CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel());//����ǰlist����һ������ѡ��

				pListItem->SetText(1,strWord);//�±�Ϊ1
			}
			else if (strName == _T("btn_write_srt"))//д����Ļ
			{
				WriteSRT();
			}
			else if (strName == _T("btn_view"))//��ȡ��Ƶ
			{
				GenerateView();
			}
			else if (strName == _T("btn_bron"))//��¼
			{
				BornSRTtoView();
			}
			else if (strName == _T("btn_generate"))//����gif
			{
				//����CComboBox�ؼ���name�õ��ÿؼ���Ҳ���ǻ滭�������������й����
				CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
				if (0 == pCombo->GetCurSel())//�����ǰѡ����0
				{
					GenerateGifWithPic();//ʹ��ͼƬ����
				}
				else
				{
					GenerateGifWithView();//ʹ����Ƶ����
				}
			}
		}
		//�ڴ��ڳ�ʼ���ڼ䣬����Ĭ�ϵ���ͼƬ���ɣ�������Ҫ����Ƶ���ɵ���ؿؼ���Ч
		else if (msg.sType == _T("windowinit"))
		{
			SetControlEnable(false);
		}
		else if (msg.sType == _T("itemselect"))//���������Combo��ѡ��ı���
		{
			if (strName == _T("List_srt"))//��ʾѡ��ı䣬���ı������ݷ����Ҳ��edit�м���
			{
				//��ȡlist�ؼ�
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));
				//�ó�list��ĳһ�������
				CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel());
				//��listѡ�����еĶ�Ӧ�ı���Ϣ���ӵ�edit��
				CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("edit_word"));
				pEdit->SetText(pListItem->GetText(1));//��ȡ��һ��,�������ý�edit����
			}
			if (strName == _T("combo_select"))
			{		
				CComboBoxUI* pComboUI = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
				if (0 == pComboUI->GetCurSel())
				{
					//ѡ��ͼƬ��ʽ���ɣ������¿ؼ���Ч
					SetControlEnable(false);
				}
				else//ѡ����Ƶ��ʽ���ɣ������¿ؼ���Ч
				{
					SetControlEnable(true);
				}
			}
		}
	}
	void SetControlEnable(bool IsValid)//��һЩ�ؼ�����Ϊ��Ч������Ч
	{
		((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->SetEnabled(IsValid);//start�༭������
		((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->SetEnabled(IsValid);//end�༭������
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_cut")))->SetEnabled(IsValid);//��ȡ��ť����
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_get_srt")))->SetEnabled(IsValid);//��ȡSRT��ť����
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_write_srt")))->SetEnabled(IsValid);//д��SRT��ť����
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_view")))->SetEnabled(IsValid);//��ȡ��Ƶ��ť����
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_bron")))->SetEnabled(IsValid);//��¼��ť����

	}
	void SendMessage(CDuiString strCMD)//�����̨������
	{
		//1.��ʼ���ṹ��
		SHELLEXECUTEINFO strSEInfo;
		memset(&strSEInfo, 0, sizeof(SHELLEXECUTEINFO));
		strSEInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		strSEInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		strSEInfo.lpFile = _T("C:\\Windows\\System32\\cmd.exe");
		//windows������cmd���ڵ�·�������Ǳ�����cmd���ڵ�·����һ��Ҫ����

		strSEInfo.lpParameters = strCMD;//�򿪳���Ĳ���
		strSEInfo.nShow = SW_SHOW;//SW_HIDE������

		//���������д��ڣ��������з���Ϣ��
		//�ڸú����У����´���һ�����̣���������������д���ִ������
		ShellExecuteEx(&strSEInfo);
		
		//�ȴ�������Ӧ���
		WaitForSingleObject(strSEInfo.hProcess, INFINITE);
		MessageBox(NULL, _T("����������"), _T("MakeGif"), IDOK);
	}
	//1��ʹ��ͼƬ����gif��̬ͼ
	void GenerateGifWithPic()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ���̵�Ŀ¼���պ���exe�ļ���ͬһ��Ŀ¼
		strPath += _T("ffmpeg\\");
		//1.��������
		CDuiString strCMD;
		strCMD += _T("/c ");//���������ڼ��һ��������'/c'��Ҫ����/c����
		strCMD += strPath;
		strCMD+=_T("ffmpeg -r 3 -i ");
		strCMD += strPath;
		strCMD += _T(".\\Picture\\%d.jpg ");//������ͼƬ��·����ע��Ҫ����ת���ַ���ת��
		strCMD += strPath;
		strCMD += _T("output.gif -y");//���ɶ�̬ͼ��-y��ʾ�����Ŀ¼�´���output.gif���͸���

		//2.��cmd������
		SendMessage(strCMD);
	}

	//2��ʹ����Ƶ����gif��̬ͼ
	//(1)��ȡ����Ҫ��Ƭ��
	void Cutview()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");
		//��ʱ���������ַ�������Ϊ���ذ�ť��Ӧ�����ǿ��ԴӼ��ر༭���л�ȡ·��
		CDuiString strViewPath = ((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->GetText();//��ȡ���༭�������ݵ�·����Ҳ�����õ�����Ƶ������·��
		//1.��������
		CDuiString strCMD;
		strCMD += _T("/c ");//���������ڼ��һ��������'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//Ҫ����\c����
		//����ͨ�������б༭����������Ƶ·��
		if (!strViewPath.IsEmpty())
		{
			strCMD += strViewPath;
		}
		else//�ٵ�Ĭ��·���»�ȡ�ļ�
		{
			strCMD += strPath;
			strCMD += _T("input.mkv ");//��Ƶ�ļ���·��
		}	
		strCMD += _T("-vcodec copy -acodec copy ");
		strCMD += _T("-ss "); 

		//��ȡ��ʼʱ��ͽ�βʱ��
		CDuiString strStartTime=((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->GetText();
		if (!IsValidTime(strStartTime))
		{
			MessageBox(NULL, _T("��ʼʱ������"), _T("MakeGif"), IDOK);
		}
		CDuiString strEndTime=((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->GetText();
		if (!IsValidTime(strEndTime))
		{
			MessageBox(NULL, _T("��ֹʱ������"), _T("MakeGif"), IDOK);
		}
		strCMD += strStartTime;
		strCMD += _T(" -to ");
		strCMD += strEndTime;
		strCMD += _T(" ");

		//����ļ���·��
		strCMD += strPath;
		strCMD += _T("11.mkv -y");

		//2.��cmd��������
		SendMessage(strCMD);
	}
	void LoadFile()//������Ƶ�ļ���·��
	{
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(OPENFILENAME));
		//���ò���
		TCHAR strPath[MAX_PATH] = { 0 };//MAX_PATH--->260
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = strPath;
		ofn.nMaxFile = sizeof(strPath);
		ofn.lpstrFilter = _T("All(*.*)\0 *.*\0mkv(*.mkv)\0 *.mkv\0");//ʹ��������˴������������ǹ��˳�mkv�ļ�
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&ofn))
		{
			//���ļ���·�����õ�edit
			((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->SetText(strPath);//��·�����ý��༭����
		}

	}
	//(2)�ӽ�ȡ����Ƶ����ȡ��Ļ�������������Ļ��
	void GetSRTFile()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");
		//1.��������
		CDuiString strCMD;
		strCMD += _T("/c ");//���������ڼ��һ��������'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//Ҫ����\c����
		strCMD += strPath;
		strCMD += _T("11.mkv ");//��Ƶ�ļ���·��
		strCMD += strPath;
		strCMD += _T("input.srt -y");//�������Ļ�ļ�

		//2.��cmd��������
		SendMessage(strCMD);
	}

	//(3)�༭��Ļ���ڴ����б༭
	void LoadSRT()
	{
		//��srt��ʽ����Ļ�ļ������ص������е�list�ؼ�
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//�õ��ļ���·��
		strPath += _T("ffmpeg\\input.srt");
		std::ifstream fIn(strPath.GetData());//��ȡ�ַ������͵��ļ�

		char strSRTCon[512] = { 0 };
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));//�ؼ�����ͨ���滭�������������ģ��õ�list�ؼ�

		//��list�����ÿһ��
		pList->RemoveAll();//�ڶ��λ�ȡ��ĻʱҪ�����һ�λ�ȡ����Ļ
		while (!fIn.eof())//�ļ�ָ����û�����ļ���β
		{
			//��ȡ��Ļ���
			fIn.getline(strSRTCon,512);

			//����list�е��ı�Ԫ��
			CListTextElementUI* pListItem = new CListTextElementUI;
			pList->Add(pListItem);

			//��ȡʱ����
			fIn.getline(strSRTCon,512);
			pListItem->SetText(0, UTF8ToUniCode(strSRTCon));
			//��0��ʼ�����ı����ý�ȥ�����Ǵ�ʱstrSRTCon��LPCTSTR���͵ģ�
			//win32��Ŀ�ǻ���Unicode�ģ�����ASCII��ʽ�ģ����Ҫ����ת����UTF-8--->Unicode)

			//��ȡ��Ļ
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(1, UTF8ToUniCode(strSRTCon));

			//��ȡ����
			fIn.getline(strSRTCon, 512);
		}
		fIn.close();
	}
	void WriteSRT()
	{
		//�õ�srt�ļ���·��
		CDuiString strPath = CPaintManagerUI::GetInstancePath();
		strPath += _T("ffmpeg\\input.srt");
		std::ofstream fOut(strPath.GetData());//д��fout��

		//1.��list�ؼ��л�ȡ�ı�����
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));
		int szCount = pList->GetCount();//�õ��ܹ�����
		for (int i = 0; i < szCount; ++i)
		{
			CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(i);

			//���
			CDuiString strNo;
			strNo.Format(_T("%d"), i + 1);//��Ϊsrt�ļ�����Ǵ�1��ʼ�ģ���i�Ǵ�0��ʼ�ģ�����Ҫ+1
			//ʱ����
			CDuiString strTime = pListItem->GetText(0);
			//�ı�����

			CDuiString strWord = pListItem->GetText(1);

			//2.����ȡ��������д�ص�srt�ļ���

			string strNewLine = UnicodeToANSI(_T("\n"));
			//д�к�
			string itemNo = UnicodeToANSI(strNo);//���������õ����ַ�����Unicode��ʽ����ofstream���ǻ���ASCII��ĸ�ʽ��������Ҫת��
			fOut.write(itemNo.c_str(),itemNo.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());//��һ������
			//дʱ����
			string itemTime = UnicodeToANSI(strTime);
			fOut.write(itemTime.c_str(), itemTime.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());//��һ������

			//д�ı�
			string itemWord = UnicodeToANSI(strWord);
			fOut.write(itemWord.c_str(), itemWord.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());//��һ������

			//����ټ�һ������
			fOut.write(strNewLine.c_str(), strNewLine.size());//��һ������
		}
		fOut.close();
	}

	//(4)�ӽ�ȡ��Ƭ������ȡ��Ƶ����
	void GenerateView()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");
		//1.��������
		CDuiString strCMD;
		strCMD += _T("/c ");//���������ڼ��һ��������'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//Ҫ����\c����
		strCMD += strPath;
		strCMD += _T("11.mkv -vcodec copy -an -sn ");
		strCMD += strPath;
		strCMD += _T("22.mkv -y");

		SendMessage(strCMD);
	}
	void BornSRTtoView()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");
		//1.��������
		CDuiString strCMD;
		strCMD += _T("/c ");//���������ڼ��һ��������'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//Ҫ����\c����
		strCMD += strPath;
		strCMD += _T("22.mkv -vf subtitles=input.srt ");
		strCMD += strPath;
		strCMD += _T("33.mkv -y");

		SendMessage(strCMD);
	}
	void GenerateGifWithView()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//��ȡ·��
		strPath += _T("ffmpeg\\");
		//1.��������
		CDuiString strCMD;
		strCMD += _T("/c ");//���������ڼ��һ��������'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//Ҫ����\c����
		strCMD += strPath;
		strCMD += _T("33.mkv -vf scale=iw/2:ih/2 -f gif ");
		strCMD += strPath;
		strCMD += _T("output.gif -y");

		SendMessage(strCMD);
	}
	CDuiString UTF8ToUniCode(const char* str)//UTF-8--->Unicode 
	{
		//��һ�ε��ã���ȡת��֮���Ŀ�괮�ĳ���
		int szLen=::MultiByteToWideChar(CP_UTF8, 0, str, strlen(str), NULL, 0);

		wchar_t* pContent = new wchar_t[szLen + 1];//ΪĿ�괮����ռ䣬Ҫ�洢\0

		//�ڶ��ε��ã�����������ת��
		::MultiByteToWideChar(CP_UTF8, NULL, str, strlen(str),pContent, szLen);
		pContent[szLen] = '\0';
		CDuiString s(pContent);
		delete[]pContent;
		return s;
	}
	bool IsValidTime(CDuiString strTime)//�жϸ���ʱ���Ƿ���Ч
	{
		//"00:40:07"---ʱ���ʽ
		if (strTime.GetLength() != 8)
		{
			return false;
		}
		for (int i = 0; i < strTime.GetLength(); ++i)
		{
			if (strTime[i] == ':')
				continue;
			if (!(strTime[i] >= '0' && strTime[i] <= '9'))//��������
			{
				return false;
			}
		}
		return true;
	}
	string UnicodeToANSI(CDuiString str)
	{
		int szLen = ::WideCharToMultiByte(CP_ACP, 0, str.GetData(), -1, NULL, 0, NULL, FALSE);
		char* pBuff = new char[szLen + 1];

		::WideCharToMultiByte(CP_ACP, 0, str.GetData(), str.GetLength(), pBuff, szLen, NULL, FALSE);//һ��ʼ���ĸ������������ĸ�Ӧ����str.GetLength(),����д����������ģ��ĳ�-1�Ϳ���������ʾ
		pBuff[szLen] = '\0';
		string s(pBuff);
		delete[] pBuff;
		return s;
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	//��Ӧxml�ļ�����ΪҪ�ҵ�·��
	CPaintManagerUI::SetInstance(hInstance);//����ʵ��
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�
	framWnd.Create(NULL, _T("MakeGif"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	framWnd.CenterWindow();
	//��ʾ���ڣ�������Ϣѭ��
	framWnd.ShowModal();
	return 0;
}

