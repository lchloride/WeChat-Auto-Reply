// LoadAIMLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WeChatAutoReply.h"
#include "LoadAIMLDlg.h"
#include "afxdialogex.h"
#include "log.h"

extern bool load_flag;
extern int load_result;
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
	ON_BN_CLICKED(IDCANCEL, &CLoadAIMLDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
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
	m_loadProgress.SetRange(0, 21*2);//���ý���������ʾ��Χ
	m_loadProgress.SetPos(0);//��������ʼ״̬Ϊ��ʾ��
	m_loadFilename = L"Loading AIML";
	UpdateData(FALSE);
	SetTimer(2, 500, NULL);//ÿ��0.5�봥��ontimer�¼�ʹ��ǰ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLoadAIMLDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBox(L"����˳����ܵ���AIML���ݿ�ʧЧ���Ӷ������޷������Զ��ظ���ȷ���˳���", L"WeChat Auto Reply", MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		load_flag = false;
		writeLog((CWeChatDlg*)this->GetParent(), L"��ֹ����AIML�ļ�����ִ�е���" + m_loadFilename, L"LoadAIML-OnBnClickedCancel()", ERR);
		m_loadFilename = L"��ֹ����AIML�ļ���" + m_loadFilename + L"�У����Ժ�";
		UpdateData(FALSE);
	}
	while (load_result == 0)
		Sleep(1000);
	CDialogEx::OnCancel();
}


void CLoadAIMLDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//OnClose()�������Զ�����Cancel��ť�Ĵ�������������ִ��һЩ���õĴ��롣����Ҫֱ�ӵ���OnCancel()����cancel��ť��Ӧ������ִ��
	CDialogEx::OnCancel();
	//CDialogEx::OnClose();
}
