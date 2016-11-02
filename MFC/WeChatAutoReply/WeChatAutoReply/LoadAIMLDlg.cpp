// LoadAIMLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WeChatAutoReply.h"
#include "LoadAIMLDlg.h"
#include "afxdialogex.h"


// CLoadAIMLDlg �Ի���

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


// CLoadAIMLDlg ��Ϣ�������


void CLoadAIMLDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: �ڴ˴������Ϣ����������

}


void CLoadAIMLDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	KillTimer(2);
}



void CLoadAIMLDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 2)
	{
		m_loadProgress.SetPos(progress);
		m_loadFilename = cmdList;
		UpdateData(FALSE);
		if (progress == 7)
		{
			Sleep(1000);
			PostMessageW(WM_CLOSE, NULL, NULL);//�������PostMessage����Ҳ��Ϊ�����߳���Ȼ��ֹ
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
	cmdList = L"ִ��:" + cmd;
}


BOOL CLoadAIMLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_loadProgress.SetRange(0, 7);//���ý���������ʾ��Χ
	m_loadProgress.SetPos(0);//��������ʼ״̬Ϊ��ʾ��
	m_loadFilename = L"Loading AIML";
	UpdateData(FALSE);
	SetTimer(2, 1000, NULL);//ÿ��0.5�봥��ontimer�¼�ʹ��ǰ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
