
// WeChatAutoReplyDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CWeChatDlg �Ի���
class CWeChatDlg : public CDialogEx
{
// ����
public:
	CWeChatDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WECHATAUTOREPLY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	const int OFFSET_SET_X = 120;
	const int OFFSET_SET_Y = 80;
	const int EDIT_HEIGHT = 145;
	const int INIT_HEIGHT = 5;
	const int STEP_HEIGHT = 30;
	const bool SENDING = 0;
	const bool RECEIVING = 1;

// ʵ��
protected:
	HICON m_hIcon;
	CString LastSendMsg;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedStart();
	//BOOL EnumWindowsProc(HWND hwnd, DWORD IParam);
	CWnd *lmshwnd;

	// ��ȡ���ظ���Ϣ״̬��ѡ��Ŀ��Ʊ���
	int m_replyStateRadioGroup;
	afx_msg void OnClickedReplyStateRadioGroup();
protected:
	//BOOL getMsg(wchar_t msg[], int& len);
	// ģ��һ�����Ķ�����vKey �Ǽ��������� Orientation ��ʾ���»����ͷ� , 1���� 0�ͷ�
	void KeyAction(int vKey, int Orientation);
	bool getClipboard(CString & text, int & text_len);
	BOOL getMsg(CString & msg, int & len);
	BOOL openWnd(const wchar_t* path);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
//	bool getResponse();
	bool getResponse(CString GetMsg, CString& SendMsg, bool first);
//	CString Rebecca_exec_path;
	// ��΢�ŵ�ǰ�Ự������Ϣ
	BOOL sendMsg(CString msg);
private:
	// ���յ���Ϣ�ַ���
	CString GetMsg;
	// ���͵��ַ���
	CString SendMsg;
	// �����Ƿ�Ϊ��һ�λ����Ϣ
	bool first_flag;
	// �����Ƿ�����Զ��ظ��ı�־
	bool process_flag;
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	// ��ʾ��־���ݵĿؼ�
	CListCtrl m_logDisplay;
private:
	// ���΢�������Ĵ���
	bool findWeChatWnd();
public:
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedReloadAiml();
protected:
	afx_msg void OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult);
public:
	afx_msg void OnDblclkLogDisplay(NMHDR *pNMHDR, LRESULT *pResult);
};
