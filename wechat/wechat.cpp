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

//const wchar_t DEFAULT_MSG[] = L"�������ڲ��ڼң����������һ�֪ͨ����~ from �ɐۤ��ᥤ�ɤ���";
const wchar_t DEFAULT_MSG[] = L"�������²��ڣ����Ե�";
const char* property_path = "E:\\computer science\\wechat\\wechat\\conf\\";
HWND lmshwnd = NULL;
wchar_t LastSendMsg[MSG_SIZE + 1] = { 0 };//�ϴη�������Ϣ
bool state = RECEIVING;//��ʼ״̬Ϊ������Ϣ
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
	//���ݽ���ID,�򿪽��̷��ؽ��̵ľ��
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,processID );
	//�õ�������
	if (NULL!=hProcess )
	{
		HMODULE hMod;
		DWORD cbNeeded;
		//EnumProcessModules���ݾ����ȡʵ�������浽hMod��
		if (EnumProcessModules( hProcess,&hMod,sizeof(hMod),&cbNeeded) )
		{
			//��ȡ���ݽ��̾����ȡ�������� szProcessName
			GetModuleBaseName( hProcess, hMod, szProcessName,sizeof(szProcessName)/sizeof(TCHAR) );
		}
	}
	//��ӡ�������ͽ���ID,����̾��ΪhProcess
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
// ģ��һ�����Ķ�����vKey �Ǽ��������� Orientation ��ʾ���»����ͷ� , 1���� 0�ͷ�
void KeyAction(int vKey, int Orientation)
{
	INPUT Inputs888;
	SecureZeroMemory(&Inputs888, sizeof(INPUT));

	Inputs888.type = INPUT_KEYBOARD;

	// ki �� 5 ����Ա����3����Ҫ��ֵ
	if (Orientation)
		Inputs888.ki.dwFlags = 0;
	else
		Inputs888.ki.dwFlags = KEYEVENTF_KEYUP;

	Inputs888.ki.wScan = MapVirtualKey(vKey, 0);
	if (Inputs888.ki.wScan == 0) // ʧ�ܾͷ������
	{

		return;
	}
	Inputs888.ki.wVk = vKey;

	UINT ret77 = SendInput(1, &Inputs888, sizeof(INPUT));

	assert(1 == ret77);
}
BOOL GetTextFromClipboard(wchar_t text[], int& text_len)//�õ�����������ݣ����ڻ�ȡ�Է����͵�����
{
	if (::OpenClipboard(NULL))
	{
		text_len = 0;
		//��ü���������
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
				wstr[MSG_SIZE] = L'\0';//��β
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
		MessageBox(NULL, L"��WeChatʧ�ܣ�", L"", 0);
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
			EmptyClipboard();                            //���������������
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
	if (wcscmp(msg, LastSendMsg) == 0)//���һ������һ���ҷ��͵ģ�Ҳ��û�н��յ�����Ϣ
		return FALSE;
	else
		return TRUE;
}

BOOL setMsg(const wchar_t* msg)
{
	openWnd(L"E:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe");

	//CWnd* pWnd = NULL;
	//���ܳ���������������Ҫ���Զ�β����ҵ�Ŀ�괰�ڡ���ೢ��60�Σ�ÿ��һ�볢��һ�Σ�����ʧ�����˳�
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
	//ȷ�����������λ��
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

	//�������д����
	for (int i = 0; i<lstrlen(msg); i++)
	{
		//printf("%d ", msg[i]);
		PostMessageW(lmshwnd, WM_CHAR, msg[i], 0x1);
	}
	putchar('\n');

	//�������д�س������Է�����Ϣ
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
		case 1: printf("�������Z:�ձ��Z\n"); 		
					wcscpy_s(SendMsg, 68, L"Sorry, I can only understand English. Talk with me in English. THX~");
					setMsg(SendMsg);
					memset(SendMsg, 0, MSG_SIZE + 1);
					unicode2char(GetMsg, GetMsg_ansi, MSG_SIZE);
					sprintf(query_property, "-ppf \"%sproperties_jp.xml\"", property_path);
					shell(query_property, temp, MAXCHARSIZE);
					break;
		case 2: printf("�������ԣ�����\n");
					wcscpy_s(SendMsg, 68, L"Sorry, I can only understand English. Talk with me in English. THX~");
					setMsg(SendMsg);
					memset(SendMsg, 0, MSG_SIZE + 1);
					unicode2char(GetMsg, GetMsg_ansi, MSG_SIZE);//�����ת�������������֣�ÿ����һ�η���
					GetMsg_ansi[strlen(GetMsg_ansi)] = '#';//ĩβ���Ͻ�������#������ƥ��AIML�еġ�*��
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
	fp = fopen("shibie.txt", "r");        //���ļ�
	setlocale(LC_ALL, "chs");
	fscanf(fp, "%d", &item_num);
	for (int i = 0; i < item_num; i++)
		fwscanf(fp, L"%ls %ls", TranRecv[i], TranSend[i]);    //������ļ�

	wprintf(L"tranrecv:%ls\n", TranRecv[0]);
	fclose(fp);                        //�ر��ļ�
}

int _tmain(int argc, _TCHAR* argv[])
{
	//readFile();
	bool first_flag = true;
	// �õ�����ID���б�
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	//�о����н��̵�ID�����ص�aProcesses������
	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		return 0;
	//����һ�������˶��ٸ�����ID
	cProcesses = cbNeeded / sizeof(DWORD);
	//printf("1-%d\n", cProcesses);
	//��ӡÿ�����̵����ƺͶ�Ӧ��ID
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
	printf("�Ƿ�õ����һ���㷢�͵�����(y or n): ");
	scanf("%s", input); 
	bool flag = false;
	int len = 0;
	if (strcmp(input, "y") == 0)
	{
		/*wprintf(L"ʹ��Ctrl+C�����һ�η��͵����ݸ��Ƶ������壬���ƺ����������\n");
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
