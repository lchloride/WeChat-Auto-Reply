
// WeChatAutoReplyDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CWeChatDlg 对话框
class CWeChatDlg : public CDialogEx
{
// 构造
public:
	CWeChatDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WECHATAUTOREPLY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	const int OFFSET_SET_X = 120;
	const int OFFSET_SET_Y = 80;
	const int EDIT_HEIGHT = 145;
	const int INIT_HEIGHT = 5;
	const int STEP_HEIGHT = 30;
	const bool SENDING = 0;
	const bool RECEIVING = 1;

// 实现
protected:
	HICON m_hIcon;
	CString LastSendMsg;

	// 生成的消息映射函数
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

	// 获取最后回复消息状态单选组的控制变量
	int m_replyStateRadioGroup;
	afx_msg void OnClickedReplyStateRadioGroup();
protected:
	//BOOL getMsg(wchar_t msg[], int& len);
	// 模拟一个键的动作。vKey 是键的虚拟码 Orientation 表示按下还是释放 , 1按下 0释放
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
	// 向微信当前会话发送消息
	BOOL sendMsg(CString msg);
private:
	// 接收的消息字符串
	CString GetMsg;
	// 发送的字符串
	CString SendMsg;
	// 标明是否为第一次获得消息
	bool first_flag;
	// 控制是否进行自动回复的标志
	bool process_flag;
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	// 显示日志内容的控件
	CListCtrl m_logDisplay;
private:
	// 获得微信桌面版的窗口
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
