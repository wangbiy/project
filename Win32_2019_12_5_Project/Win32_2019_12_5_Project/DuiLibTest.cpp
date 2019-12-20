#include "UIlib.h"

using namespace DuiLib;
#include <fstream>
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
		return _T("gifMake.xml");//一定要与目录放的xml文件文件名称相同

	}
	virtual LPCTSTR GetWindowClassName(void)const
	{
		return _T("GIFMakeWnd");
	}
	virtual void Notify(TNotifyUI& msg)//响应
	{
		CDuiString strName = msg.pSender->GetName();//获取控件的名字
		if (msg.sType == _T("click"))//如果是鼠标点击
		{		
			if (strName == _T("btn_close"))//如果是关闭按钮
			{
				Close();
			}
			else if (strName == _T("btn_min"))//如果是最小化按钮
			{
				MessageBox(NULL, _T("最小化"), _T("测试"), IDOK);
			}
			else if (strName == _T("btn_load"))
				LoadFile();
			else if (strName == _T("btn_cut"))//截取视频
			{
				Cutview();
			}
			else if (strName == _T("btn_get_srt"))//提取字幕
			{
				GetSRTFile();//提取
				LoadSRT();//加载
			}
			else if (strName == _T("btn_commit"))//提交按钮
			{
				CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("edit_word"));
				CDuiString strWord=pEdit->GetText();//获取这个edit对应的文本

				//将该文本写回到list中，此时已经在edit中编辑过了
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));//先获取list控件
				CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel());//将当前list的这一行内容选中

				pListItem->SetText(1,strWord);//下标为1
			}
			else if (strName == _T("btn_write_srt"))//写入字幕
			{
				WriteSRT();
			}
			else if (strName == _T("btn_view"))//提取视频
			{
				GenerateView();
			}
			else if (strName == _T("btn_bron"))//烧录
			{
				BornSRTtoView();
			}
			else if (strName == _T("btn_generate"))//生成gif
			{
				//根据CComboBox控件的name拿到该控件，也就是绘画管理器中来进行管理的
				CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
				if (0 == pCombo->GetCurSel())//如果当前选中是0
				{
					GenerateGifWithPic();//使用图片生成
				}
				else
				{
					GenerateGifWithView();//使用视频生成
				}
			}
		}
		//在窗口初始化期间，由于默认的是图片生成，我们需要让视频生成的相关控件无效
		else if (msg.sType == _T("windowinit"))
		{
			SetControlEnable(false);
		}
		else if (msg.sType == _T("itemselect"))//如果下拉框Combo是选择改变了
		{
			if (strName == _T("List_srt"))//表示选择改变，将改变后的内容放在右侧的edit中即可
			{
				//获取list控件
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));
				//拿出list中某一项的内容
				CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel());
				//将list选中行中的对应文本信息增加到edit中
				CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("edit_word"));
				pEdit->SetText(pListItem->GetText(1));//获取第一项,并且设置进edit框中
			}
			if (strName == _T("combo_select"))
			{		
				CComboBoxUI* pComboUI = (CComboBoxUI*)m_PaintManager.FindControl(_T("combo_select"));
				if (0 == pComboUI->GetCurSel())
				{
					//选择图片方式生成，让以下控件无效
					SetControlEnable(false);
				}
				else//选择视频方式生成，让以下控件有效
				{
					SetControlEnable(true);
				}
			}
		}
	}
	void SetControlEnable(bool IsValid)//将一些控件设置为有效或者无效
	{
		((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->SetEnabled(IsValid);//start编辑框设置
		((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->SetEnabled(IsValid);//end编辑框设置
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_cut")))->SetEnabled(IsValid);//截取按钮设置
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_get_srt")))->SetEnabled(IsValid);//提取SRT按钮设置
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_write_srt")))->SetEnabled(IsValid);//写入SRT按钮设置
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_view")))->SetEnabled(IsValid);//提取视频按钮设置
		((CButtonUI*)m_PaintManager.FindControl(_T("btn_bron")))->SetEnabled(IsValid);//烧录按钮设置

	}
	void SendMessage(CDuiString strCMD)//向控制台发命令
	{
		//1.初始化结构体
		SHELLEXECUTEINFO strSEInfo;
		memset(&strSEInfo, 0, sizeof(SHELLEXECUTEINFO));
		strSEInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		strSEInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		strSEInfo.lpFile = _T("C:\\Windows\\System32\\cmd.exe");
		//windows命令行cmd所在的路径，就是本机的cmd所在的路径，一定要给对

		strSEInfo.lpParameters = strCMD;//打开程序的参数
		strSEInfo.nShow = SW_SHOW;//SW_HIDE：隐藏

		//调用命令行窗口，给命令行发消息，
		//在该函数中，会新创建一个进程，来负责调用命令行窗口执行命令
		ShellExecuteEx(&strSEInfo);
		
		//等待命令响应完成
		WaitForSingleObject(strSEInfo.hProcess, INFINITE);
		MessageBox(NULL, _T("命令操作完成"), _T("MakeGif"), IDOK);
	}
	//1、使用图片生成gif动态图
	void GenerateGifWithPic()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程的目录，刚好与exe文件在同一个目录
		strPath += _T("ffmpeg\\");
		//1.构造命令
		CDuiString strCMD;
		strCMD += _T("/c ");//构造命令期间第一条必须是'/c'，要加上/c参数
		strCMD += strPath;
		strCMD+=_T("ffmpeg -r 3 -i ");
		strCMD += strPath;
		strCMD += _T(".\\Picture\\%d.jpg ");//这里是图片的路径，注意要进行转义字符的转义
		strCMD += strPath;
		strCMD += _T("output.gif -y");//生成动态图，-y表示如果该目录下存在output.gif，就覆盖

		//2.给cmd发命令
		SendMessage(strCMD);
	}

	//2、使用视频生成gif动态图
	//(1)截取所需要的片段
	void Cutview()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");
		//此时我们有两种方法，因为加载按钮响应后，我们可以从加载编辑框中获取路径
		CDuiString strViewPath = ((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->GetText();//获取到编辑框中内容的路径，也就是拿到了视频的完整路径
		//1.构造命令
		CDuiString strCMD;
		strCMD += _T("/c ");//构造命令期间第一条必须是'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//要加上\c参数
		//优先通过界面中编辑框来加载视频路径
		if (!strViewPath.IsEmpty())
		{
			strCMD += strViewPath;
		}
		else//再到默认路径下获取文件
		{
			strCMD += strPath;
			strCMD += _T("input.mkv ");//视频文件的路径
		}	
		strCMD += _T("-vcodec copy -acodec copy ");
		strCMD += _T("-ss "); 

		//获取起始时间和结尾时间
		CDuiString strStartTime=((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->GetText();
		if (!IsValidTime(strStartTime))
		{
			MessageBox(NULL, _T("起始时间有误"), _T("MakeGif"), IDOK);
		}
		CDuiString strEndTime=((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->GetText();
		if (!IsValidTime(strEndTime))
		{
			MessageBox(NULL, _T("终止时间有误"), _T("MakeGif"), IDOK);
		}
		strCMD += strStartTime;
		strCMD += _T(" -to ");
		strCMD += strEndTime;
		strCMD += _T(" ");

		//输出文件的路径
		strCMD += strPath;
		strCMD += _T("11.mkv -y");

		//2.向cmd发送命令
		SendMessage(strCMD);
	}
	void LoadFile()//加载视频文件的路径
	{
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(OPENFILENAME));
		//设置参数
		TCHAR strPath[MAX_PATH] = { 0 };//MAX_PATH--->260
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = strPath;
		ofn.nMaxFile = sizeof(strPath);
		ofn.lpstrFilter = _T("All(*.*)\0 *.*\0mkv(*.mkv)\0 *.mkv\0");//使用这个过滤串，我们这里是过滤出mkv文件
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&ofn))
		{
			//将文件的路径设置到edit
			((CEditUI*)m_PaintManager.FindControl(_T("edit_path")))->SetText(strPath);//将路径设置进编辑框中
		}

	}
	//(2)从截取的视频中提取字幕（必须是外挂字幕）
	void GetSRTFile()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");
		//1.构造命令
		CDuiString strCMD;
		strCMD += _T("/c ");//构造命令期间第一条必须是'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//要加上\c参数
		strCMD += strPath;
		strCMD += _T("11.mkv ");//视频文件的路径
		strCMD += strPath;
		strCMD += _T("input.srt -y");//输出的字幕文件

		//2.向cmd发送命令
		SendMessage(strCMD);
	}

	//(3)编辑字幕，在窗口中编辑
	void LoadSRT()
	{
		//将srt格式的字幕文件，加载到界面中的list控件
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//得到文件的路径
		strPath += _T("ffmpeg\\input.srt");
		std::ifstream fIn(strPath.GetData());//获取字符串类型的文件

		char strSRTCon[512] = { 0 };
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));//控件都是通过绘画管理器画出来的，拿到list控件

		//给list中添加每一行
		pList->RemoveAll();//第二次获取字幕时要清掉第一次获取的字幕
		while (!fIn.eof())//文件指针有没有在文件结尾
		{
			//读取字幕序号
			fIn.getline(strSRTCon,512);

			//给出list中的文本元素
			CListTextElementUI* pListItem = new CListTextElementUI;
			pList->Add(pListItem);

			//读取时间轴
			fIn.getline(strSRTCon,512);
			pListItem->SetText(0, UTF8ToUniCode(strSRTCon));
			//从0开始，将文本设置进去，但是此时strSRTCon是LPCTSTR类型的，
			//win32项目是基于Unicode的，不是ASCII形式的，因此要进行转化（UTF-8--->Unicode)

			//读取字幕
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(1, UTF8ToUniCode(strSRTCon));

			//读取空行
			fIn.getline(strSRTCon, 512);
		}
		fIn.close();
	}
	void WriteSRT()
	{
		//拿到srt文件的路径
		CDuiString strPath = CPaintManagerUI::GetInstancePath();
		strPath += _T("ffmpeg\\input.srt");
		std::ofstream fOut(strPath.GetData());//写到fout中

		//1.从list控件中获取文本内容
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_srt"));
		int szCount = pList->GetCount();//得到总共的行
		for (int i = 0; i < szCount; ++i)
		{
			CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(i);

			//序号
			CDuiString strNo;
			strNo.Format(_T("%d"), i + 1);//因为srt文件标号是从1开始的，而i是从0开始的，所以要+1
			//时间轴
			CDuiString strTime = pListItem->GetText(0);
			//文本内容

			CDuiString strWord = pListItem->GetText(1);

			//2.将获取到的内容写回到srt文件中

			string strNewLine = UnicodeToANSI(_T("\n"));
			//写行号
			string itemNo = UnicodeToANSI(strNo);//但是我们拿到的字符属于Unicode格式，而ofstream又是基于ASCII码的格式，所以需要转化
			fOut.write(itemNo.c_str(),itemNo.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());//加一个换行
			//写时间轴
			string itemTime = UnicodeToANSI(strTime);
			fOut.write(itemTime.c_str(), itemTime.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());//加一个换行

			//写文本
			string itemWord = UnicodeToANSI(strWord);
			fOut.write(itemWord.c_str(), itemWord.size());
			fOut.write(strNewLine.c_str(), strNewLine.size());//加一个换行

			//最后再加一个换行
			fOut.write(strNewLine.c_str(), strNewLine.size());//加一个换行
		}
		fOut.close();
	}

	//(4)从截取的片段中提取视频裸流
	void GenerateView()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");
		//1.构造命令
		CDuiString strCMD;
		strCMD += _T("/c ");//构造命令期间第一条必须是'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//要加上\c参数
		strCMD += strPath;
		strCMD += _T("11.mkv -vcodec copy -an -sn ");
		strCMD += strPath;
		strCMD += _T("22.mkv -y");

		SendMessage(strCMD);
	}
	void BornSRTtoView()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");
		//1.构造命令
		CDuiString strCMD;
		strCMD += _T("/c ");//构造命令期间第一条必须是'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//要加上\c参数
		strCMD += strPath;
		strCMD += _T("22.mkv -vf subtitles=input.srt ");
		strCMD += strPath;
		strCMD += _T("33.mkv -y");

		SendMessage(strCMD);
	}
	void GenerateGifWithView()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取路径
		strPath += _T("ffmpeg\\");
		//1.构造命令
		CDuiString strCMD;
		strCMD += _T("/c ");//构造命令期间第一条必须是'/c'
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");//要加上\c参数
		strCMD += strPath;
		strCMD += _T("33.mkv -vf scale=iw/2:ih/2 -f gif ");
		strCMD += strPath;
		strCMD += _T("output.gif -y");

		SendMessage(strCMD);
	}
	CDuiString UTF8ToUniCode(const char* str)//UTF-8--->Unicode 
	{
		//第一次调用：获取转化之后的目标串的长度
		int szLen=::MultiByteToWideChar(CP_UTF8, 0, str, strlen(str), NULL, 0);

		wchar_t* pContent = new wchar_t[szLen + 1];//为目标串申请空间，要存储\0

		//第二次调用：进行真正的转化
		::MultiByteToWideChar(CP_UTF8, NULL, str, strlen(str),pContent, szLen);
		pContent[szLen] = '\0';
		CDuiString s(pContent);
		delete[]pContent;
		return s;
	}
	bool IsValidTime(CDuiString strTime)//判断给的时间是否有效
	{
		//"00:40:07"---时间格式
		if (strTime.GetLength() != 8)
		{
			return false;
		}
		for (int i = 0; i < strTime.GetLength(); ++i)
		{
			if (strTime[i] == ':')
				continue;
			if (!(strTime[i] >= '0' && strTime[i] <= '9'))//不是数字
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

		::WideCharToMultiByte(CP_ACP, 0, str.GetData(), str.GetLength(), pBuff, szLen, NULL, FALSE);//一开始第四个参数理论上哪个应该是str.GetLength(),但是写出来是乱码的，改成-1就可以正常显示
		pBuff[szLen] = '\0';
		string s(pBuff);
		delete[] pBuff;
		return s;
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	//对应xml文件，因为要找到路径
	CPaintManagerUI::SetInstance(hInstance);//设置实例
	// 设置资源的默认路径（此处设置为和exe在同一目录）
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框
	framWnd.Create(NULL, _T("MakeGif"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	framWnd.CenterWindow();
	//显示窗口，激活消息循环
	framWnd.ShowModal();
	return 0;
}

