//#include <stdafx.h>
//#include <afxwin.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <Psapi.h>
#include <string.h>
#include <assert.h>
#include "shell.h"
#include "tools.h"
#include "divide.h"
#pragma comment ( lib, "Psapi.lib" )

using namespace std;
const int OFFSET_SET_X = 120;
const int OFFSET_SET_Y = 80;
const int EDIT_HEIGHT = 145;
const int INIT_HEIGHT = 35;
const int STEP_HEIGHT = 30;
const int MSG_SIZE = 2048;
const bool SENDING = 0;
const bool RECEIVING = 1;

//const wchar_t DEFAULT_MSG[] = L"主人现在不在家，等他回来我会通知他的~ from 可愛いメイドさん";
const wchar_t DEFAULT_MSG[] = L"现在有事不在，请稍等";
const char* property_path = "E:\\computer science\\wechat\\wechat\\conf\\";
HWND lmshwnd = NULL;
wchar_t LastSendMsg[MSG_SIZE + 1] = { 0 };//上次发出的消息
bool state = RECEIVING;//初始状态为接收消息
wchar_t TranSend[100][MSG_SIZE+1];
wchar_t TranRecv[100][MSG_SIZE + 1];
int item_num = 0;



BOOL CALLBACK EnumWindowsProc(HWND hwnd,DWORD lParam)
{ 
	DWORD mpid;
	GetWindowThreadProcessId(hwnd,&mpid);

	if (mpid==lParam)
	{
		int i=GetWindowTextLength(hwnd);
		TCHAR szhello[255];  
		char lmschar[255]="name";
		GetWindowText(hwnd,szhello,i+1);
		if (strstr((char*)szhello,lmschar))
		{
			printf("%s\n",szhello);
			lmshwnd=hwnd;
		}

	}
	return TRUE;
}



void PrintProcessNameAndID( DWORD processID )
{
	TCHAR szProcessName[MAX_PATH] = TEXT("unknow");
	//根据进程ID,打开进程返回进程的句柄
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,processID );
	//得到进程名
	if (NULL!=hProcess )
	{
		HMODULE hMod;
		DWORD cbNeeded;
		//EnumProcessModules根据句柄获取实例，保存到hMod中
		if (EnumProcessModules( hProcess,&hMod,sizeof(hMod),&cbNeeded) )
		{
			//获取根据进程句柄获取进程名称 szProcessName
			GetModuleBaseName( hProcess, hMod, szProcessName,sizeof(szProcessName)/sizeof(TCHAR) );
		}
	}
	//打印进程名和进程ID,其进程句柄为hProcess
	/*TCHAR *lmsname=TEXT("LMSVirtualLab.EXE");
	printf("%s %s, %s %s\n", (char*)szProcessName,szProcessName,(char*)lmsname,lmsname);

	if (!strcmp((char*)szProcessName,(char*)lmsname))
	{  */
		_tprintf( TEXT("%s  (PID: %d)\n"),szProcessName,processID);
		EnumWindows((WNDENUMPROC)EnumWindowsProc,processID);
		//HDESK hDesk=OpenDesktop(_T(""),0,FALSE,DESKTOP_ENUMERATE);   
		//EnumDesktopWindows(hDesk,(WNDENUMPROC)EnumWindowsProc,processID); 
	//}

	CloseHandle( hProcess );
}
// 模拟一个键的动作。vKey 是键的虚拟码 Orientation 表示按下还是释放 , 1按下 0释放
void KeyAction(int vKey, int Orientation)
{
	INPUT Inputs888;
	SecureZeroMemory(&Inputs888, sizeof(INPUT));

	Inputs888.type = INPUT_KEYBOARD;

	// ki 共 5 个成员，仅3个需要赋值
	if (Orientation)
		Inputs888.ki.dwFlags = 0;
	else
		Inputs888.ki.dwFlags = KEYEVENTF_KEYUP;

	Inputs888.ki.wScan = MapVirtualKey(vKey, 0);
	if (Inputs888.ki.wScan == 0) // 失败就放弃这个
	{

		return;
	}
	Inputs888.ki.wVk = vKey;

	UINT ret77 = SendInput(1, &Inputs888, sizeof(INPUT));

	assert(1 == ret77);
}
BOOL GetTextFromClipboard(wchar_t text[], int& text_len)//得到剪贴板的内容，用于获取对方发送的内容
{
	if (::OpenClipboard(NULL))
	{
		text_len = 0;
		//获得剪贴板数据
		HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
		if (NULL != hMem)
		{
			wchar_t* wGlobal = (wchar_t*)GlobalLock(hMem);

			// Allocate memory from the clipboard
			text_len = GlobalSize(hMem);
			wchar_t* wstr = new wchar_t[text_len + 1];
			lstrcpy(wstr, wGlobal);
			if (text_len > MSG_SIZE)
			{
				text_len = MSG_SIZE;
				wstr[MSG_SIZE] = L'\0';//截尾
			}
			//ZeroMemory(text, text_len);
			cout << "length:" << text_len << endl;
			// Make a copy of the text in the clipboard
			lstrcpy(text, wstr);
			delete[] wstr;

			//char* lpStr = (char*)::GlobalLock(hMem);
			if (NULL != text)
			{
				cout << "text:"<<text << endl;
				//MessageBox(NULL, text, NULL, 0);
				::GlobalUnlock(hMem);
			}
			else
				cout << "text=NULL" << endl;
		}
		else
		{
			cout << "cannot get anything" << endl;
			::CloseClipboard();
			return FALSE;
		}
		::CloseClipboard();
		return TRUE;
	}
	else
		cout << "cannot open Clipboard" << endl;
	return FALSE;
}

BOOL openWnd(const wchar_t* path)
{
	HINSTANCE hRet = 0;
	HWND hwnd;
	hRet = ShellExecute(lmshwnd, NULL, path, NULL, NULL, SW_HIDE);
	if ((int)hRet <= 32)
	{
		MessageBox(NULL, L"打开WeChat失败！", L"", 0);
		return FALSE;
	}
	Sleep(2000);
	return TRUE;
}

BOOL getMsg(wchar_t msg[], int& len)
{
	openWnd(L"E:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe");

	for (int k = 0; k < 60 && lmshwnd == NULL; k++)
	{
		if (k>0) Sleep(1000);
		lmshwnd = FindWindowW(L"WeChatMainWndForPC", NULL);
		if (lmshwnd != NULL)
			printf("Get WeChat window\n");
		else
			printf("Not get WeChat window\n");
	}

	RECT rect;
	GetClientRect(lmshwnd, &rect);
	cout << "rect:" << rect.bottom << "," << rect.top << endl;
	POINT point;
	point.x = 385;
	point.y = abs(rect.bottom - rect.top) - EDIT_HEIGHT;// -INIT_HEIGHT;
	//SendMessage(lmshwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);	
	//ShowWindow(lmshwnd, SW_SHOWMINIMIZED);
	BOOL flag = true;
	do
	{
		cout << "point:" << point.y << endl;

		if (OpenClipboard(NULL))
		{
			EmptyClipboard();                            //将剪贴板内容清空
			CloseClipboard();
		}
		PostMessageW(lmshwnd, WM_LBUTTONDBLCLK, 0x1, MAKELPARAM(point.x, point.y));
		PostMessageW(lmshwnd, WM_LBUTTONUP, 0x0, MAKELPARAM(point.x, point.y));

		//keybd_event(VK_CONTROL, 0, 0, 0);
		////PostMessage(lmshwnd, WM_KEYDOWN, 0x11, 0x001D0001);
		//PostMessage(lmshwnd, WM_KEYDOWN, 0x43, 0x002E0001);
		//PostMessage(lmshwnd, WM_CHAR, 0x3, 0x002E0001);
		//PostMessage(lmshwnd, WM_KEYUP, 0x43, 0xC02E0001);
		////PostMessage(lmshwnd, WM_KEYUP, 0x11, 0xC01D0001);
		//keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		KeyAction(0x11, 1);
		KeyAction(0x43, 1);
		KeyAction(0x43, 0);
		KeyAction(0x11, 0);
		Sleep(50);
		flag = GetTextFromClipboard(msg, len);
		//setlocale(LC_ALL, "chs");
		//wprintf(msg,"msg: %s\n");
		//cout << wstr << " " << len << endl; 

		//WM_LBUTTONDBLCLK
		point.y -= STEP_HEIGHT;
	} while (!flag && point.y > 50);

	cout << "get point:"<<point.x <<","<<point.y<< endl;
	//wprintf(L"last:%ws, now:%ws\n", LastSendMsg, msg);
	if (wcscmp(msg, LastSendMsg) == 0)//最后一条是上一次我发送的，也即没有接收到新消息
		return FALSE;
	else
		return TRUE;
}

BOOL setMsg(const wchar_t* msg)
{
	openWnd(L"E:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe");

	//CWnd* pWnd = NULL;
	//可能程序启动较慢，需要尝试多次才能找到目标窗口。最多尝试60次，每隔一秒尝试一次，尝试失败则退出
	for (int k = 0; k < 60 && lmshwnd == NULL; k++)
	{
		if (k>0) Sleep(1000);
		lmshwnd = FindWindowW(L"WeChatMainWndForPC", NULL);
		if (lmshwnd != NULL)
			printf("Get WeChat window\n");
		else
			printf("Not get WeChat window\n");
	}

	//PostMessageW(lmshwnd, WM_KEYDOWN, 0x30, 0xB0001);
	//确定输入框的相对位置
	RECT rect;
	GetClientRect(lmshwnd, &rect);
	POINT point;
	point.x = abs(rect.right - rect.left)-OFFSET_SET_X;
	point.y = abs(rect.bottom - rect.top)-OFFSET_SET_Y;
	cout << "set point:" << point.x << "," << point.y << endl;
	//wchar_t wstr[255] = { 0 };
	//wsprintf(wstr, L"%d, %d", point.x, point.y);
	//MessageBox(NULL, wstr, L"", 0);
	PostMessageW(lmshwnd, WM_LBUTTONDOWN, 0x1, MAKELPARAM(point.x, point.y));
	PostMessageW(lmshwnd, WM_LBUTTONUP, 0x1, MAKELPARAM(point.x, point.y)/*0x021501A3*/);

	//向输入框写内容
	for (int i = 0; i<lstrlen(msg); i++)
	{
		//printf("%d ", msg[i]);
		PostMessageW(lmshwnd, WM_CHAR, msg[i], 0x1);
	}
	putchar('\n');

	//向输入框写回车键，以发送消息
	PostMessageW(lmshwnd, WM_KEYDOWN, 0xD, 0x1C0001);
	PostMessageW(lmshwnd, WM_CHAR, 0xD, 0x1C0001);
	PostMessageW(lmshwnd, WM_KEYUP, 0xD, 0x1C0001);
	wcscpy_s(LastSendMsg, wcslen(msg) + 1, msg);
	return TRUE;
}
BOOL getResponse(wchar_t GetMsg[], wchar_t SendMsg[], bool first)
{
	memset(SendMsg, 0, MSG_SIZE + 1);
	char GetMsg_ansi[MSG_SIZE+1] = { 0 };
	char SendMsg_ansi[MSG_SIZE+1] = { 0 };
	char query_property[MSG_SIZE+1] = { 0 };
	//printf("%ws\n", GetMsg);
	//for (int i = 0; i < wcslen(GetMsg); i++)
	//	printf("%wc-0x%x, ", GetMsg[i], GetMsg[i]);
	int lang = checkLang(GetMsg);
	vector<wstring> div_word;
	char temp[MAXCHARSIZE] = { 0 };
	switch (lang)
	{
		case 0: printf("Input language: English\n"); 
					UnicodeToANSI(GetMsg, GetMsg_ansi, MSG_SIZE);
					sprintf(query_property, "-ppf \"%sproperties.xml\"", property_path);
					shell(query_property, temp, MAXCHARSIZE);
					break;
		case 1: printf("入力言語:日本語\n"); 		
					wcscpy_s(SendMsg, 68, L"Sorry, I can only understand English. Talk with me in English. THX~");
					setMsg(SendMsg);
					memset(SendMsg, 0, MSG_SIZE + 1);
					unicode2char(GetMsg, GetMsg_ansi, MSG_SIZE);
					sprintf(query_property, "-ppf \"%sproperties_jp.xml\"", property_path);
					shell(query_property, temp, MAXCHARSIZE);
					break;
		case 2: printf("输入语言：中文\n");
					wcscpy_s(SendMsg, 68, L"Sorry, I can only understand English. Talk with me in English. THX~");
					setMsg(SendMsg);
					memset(SendMsg, 0, MSG_SIZE + 1);
					unicode2char(GetMsg, GetMsg_ansi, MSG_SIZE);//这里的转换函数包含分字，每个字一次分离
					GetMsg_ansi[strlen(GetMsg_ansi)] = '#';//末尾加上结束符‘#’便于匹配AIML中的‘*’
					sprintf(query_property, "-ppf \"%sproperties_zh.xml\"", property_path);
					shell(query_property, temp, MAXCHARSIZE);
					break;
		default:
			printf("The language cannot be analyzed\n"); break;
	}
	printf("GetMsg_ansi:%s\n", GetMsg_ansi);
	if (GetMsg_ansi != NULL)
		if (!query(GetMsg_ansi, SendMsg_ansi, MSG_SIZE, first))
			return FALSE;
	if (lang == 0)
		ANSIToUnicode(SendMsg_ansi, SendMsg, MSG_SIZE);
	else
		char2Unicode(SendMsg_ansi, SendMsg, MSG_SIZE);
	wprintf(L"Input:%ws, Output:%ws\n", GetMsg, SendMsg);
	return TRUE;
}
void readFile()
{
	FILE* fp;
	fp = fopen("shibie.txt", "r");        //打开文件
	setlocale(LC_ALL, "chs");
	fscanf(fp, "%d", &item_num);
	for (int i = 0; i < item_num; i++)
		fwscanf(fp, L"%ls %ls", TranRecv[i], TranSend[i]);    //输出到文件

	wprintf(L"tranrecv:%ls\n", TranRecv[0]);
	fclose(fp);                        //关闭文件
}

int _tmain(int argc, _TCHAR* argv[])
{
	//readFile();
	bool first_flag = true;
	// 得到进程ID的列表
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	//列举所有进程的ID，返回到aProcesses数组中
	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		return 0;
	//计算一共返回了多少个进程ID
	cProcesses = cbNeeded / sizeof(DWORD);
	//printf("1-%d\n", cProcesses);
	//打印每个进程的名称和对应的ID
	for ( i = 0; i < cProcesses; i++ )
	{
		printf("%d, ", i);
		if( aProcesses[i] != 0 )
		{
			PrintProcessNameAndID(aProcesses[i]);
		}
		putchar('\n');
	}
	printf("%x\n",lmshwnd);
	ShowWindow(lmshwnd,SW_MAXIMIZE);
	wchar_t GetMsg[MSG_SIZE + 1] = { 0 };
	wchar_t SendMsg[MSG_SIZE + 1] = { 0 };
	int GetMsgLen = 0;
	//wcout.imbue(locale("chs"));
	setlocale(LC_ALL, "chs");

	//char readLastMsgFlag[MSG_SIZE+1];
	bool readLastMsgFlag = false;
	char input[MSG_SIZE + 1];
	printf("是否得到最后一次你发送的内容(y or n): ");
	scanf("%s", input); 
	bool flag = false;
	int len = 0;
	if (strcmp(input, "y") == 0)
	{
		/*wprintf(L"使用Ctrl+C将最后一次发送的内容复制到剪贴板，复制后按任意键继续\n");
		system("pause");
		flag = GetTextFromClipboard(LastSendMsg, len);*/
		getMsg(GetMsg, GetMsgLen);
		if (GetMsgLen > 0)
			wcscpy_s(LastSendMsg, GetMsgLen, GetMsg);
	}
	wprintf(L"lastmsg:%ls\n", LastSendMsg);
	bool send_flag = false;
	while (1)
	{
		printf("===========================\n");
		if (getMsg(GetMsg, GetMsgLen))
		{
			send_flag = getResponse(GetMsg, SendMsg, first_flag);
			if (first_flag)
			{
				setMsg(L"Hello, I am Alice, the intelligent AI chatting with English. At the moment, my master is busy so I will help him reply the requests. Besides, you can leave messages and I will inform him.");
				int count = 0;
				for (int i = 0; i < wcslen(SendMsg); i++)
					if (SendMsg[i] == L' ')
						count++;
				if (count >= 3 && send_flag)
					setMsg(SendMsg);
			}			
			else if(send_flag)
				setMsg(SendMsg);

			first_flag = false;
		}
		Sleep(1000);
	}
	return 0;
}
