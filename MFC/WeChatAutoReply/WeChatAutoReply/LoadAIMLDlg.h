#pragma once
#include "afxcmn.h"


// CLoadAIMLDlg �Ի���

class CLoadAIMLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadAIMLDlg)

public:
	CLoadAIMLDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoadAIMLDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOAD_AIML_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ���ڼ��ص��ļ���
	CString m_loadFilename;
	CProgressCtrl m_loadProgress;
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	const int rd = 1;
	const int rg = 10;
	const int aduaa = 11;
	const int cg = 20;
	const int gafls = 21;
private:
	int progress;
	CString cmdList;
public:
	void setProgress(int progress);
	void setCmd(CString cmd);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
