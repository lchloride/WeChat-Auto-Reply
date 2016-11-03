// LoadAIMLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WeChatAutoReply.h"
#include "LoadAIMLDlg.h"
#include "afxdialogex.h"
#include "log.h"

extern bool load_flag;
extern int load_result;
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
	ON_BN_CLICKED(IDCANCEL, &CLoadAIMLDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
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
		if (m_loadProgress.GetPos() < progress*2)
			m_loadProgress.SetPos(m_loadProgress.GetPos() + 1);
		else
			m_loadProgress.SetPos(progress*2);
		m_loadFilename = cmdList;
		UpdateData(FALSE);
		if (progress == 21)
		{
			m_loadProgress.SetPos(21*2);
			progress++;
			while (load_result == 0)
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
	m_loadProgress.SetRange(0, 21*2);//设置进度条的显示范围
	m_loadProgress.SetPos(0);//进度条初始状态为显示零
	m_loadFilename = L"Loading AIML";
	UpdateData(FALSE);
	SetTimer(2, 500, NULL);//每隔0.5秒触发ontimer事件使其前进

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLoadAIMLDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(L"如果退出可能导致AIML数据库失效，从而导致无法正常自动回复。确定退出吗？", L"WeChat Auto Reply", MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		load_flag = false;
		writeLog((CWeChatDlg*)this->GetParent(), L"终止加载AIML文件，已执行到：" + m_loadFilename, L"LoadAIML-OnBnClickedCancel()", ERR);
		m_loadFilename = L"终止加载AIML文件，" + m_loadFilename + L"中，请稍后";
		UpdateData(FALSE);
	}
	while (load_result == 0)
		Sleep(1000);
	CDialogEx::OnCancel();
}


void CLoadAIMLDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//OnClose()函数会自动调用Cancel按钮的处理函数，这样会执行一些无用的代码。所以要直接调用OnCancel()跳过cancel按钮响应函数的执行
	CDialogEx::OnCancel();
	//CDialogEx::OnClose();
}
