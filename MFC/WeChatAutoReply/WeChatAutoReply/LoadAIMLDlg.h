#pragma once
#include "afxcmn.h"


// CLoadAIMLDlg 对话框

class CLoadAIMLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadAIMLDlg)

public:
	CLoadAIMLDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoadAIMLDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOAD_AIML_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 正在加载的文件名
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
