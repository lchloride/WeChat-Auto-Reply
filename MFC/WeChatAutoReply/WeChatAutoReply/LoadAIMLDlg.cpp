// LoadAIMLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WeChatAutoReply.h"
#include "LoadAIMLDlg.h"
#include "afxdialogex.h"


// CLoadAIMLDlg 对话框

IMPLEMENT_DYNAMIC(CLoadAIMLDlg, CDialogEx)

CLoadAIMLDlg::CLoadAIMLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOAD_AIML_DIALOG, pParent)
	, m_loadFilename(_T(""))
	, progress(0)
{

}

CLoadAIMLDlg::~CLoadAIMLDlg()
{
}

void CLoadAIMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOAD_FILE, m_loadFilename);
	DDX_Control(pDX, IDC_LOAD_PROGRESS, m_loadProgress);
}


BEGIN_MESSAGE_MAP(CLoadAIMLDlg, CDialogEx)
	ON_WM_INITMENUPOPUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLoadAIMLDlg 消息处理程序


void CLoadAIMLDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码

}


void CLoadAIMLDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(2);
}



void CLoadAIMLDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 2)
	{
		m_loadProgress.SetPos(progress);
		m_loadFilename = cmdList;
		UpdateData(FALSE);
		if (progress == 7)
		{
			Sleep(1000);
			PostMessageW(WM_CLOSE, NULL, NULL);//最好是用PostMessage这样也是为了让线程自然终止
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CLoadAIMLDlg::setProgress(int progress)
{
	this->progress = progress;
}



void CLoadAIMLDlg::setCmd(CString cmd)
{
	//lstrcpyW((LPWSTR)(LPCTSTR)cmdList, cmd);
	cmdList = L"执行:" + cmd;
}


BOOL CLoadAIMLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_loadProgress.SetRange(0, 7);//设置进度条的显示范围
	m_loadProgress.SetPos(0);//进度条初始状态为显示零
	m_loadFilename = L"Loading AIML";
	UpdateData(FALSE);
	SetTimer(2, 1000, NULL);//每隔0.5秒触发ontimer事件使其前进

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
