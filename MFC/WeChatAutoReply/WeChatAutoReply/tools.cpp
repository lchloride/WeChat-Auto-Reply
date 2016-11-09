#include "stdafx.h"
#include "tools.h"
#include "shell.h"
#include "LoadAIMLDlg.h"

#include "resource.h"
#include<stdexcept>  

//using namespace std;
extern int zh_jp_ratio;
extern CString Rebecca_exec_path;



int load_result = -1;//���صĽ����0����û����ɼ��أ�1��ʾ���سɹ���-1��ʾ����ʧ��
bool load_flag = true;//��Ϊ�ź�������tool.cpp��LoadAIMLDlg.cpp�й�ͬ���Ƽ����̵߳�ִ��

void ANSIToUnicode(const char* str, CString& result, int MaxSize)
{
	int  len = 0;
	len = strlen(str);
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP,
		0,
		str,
		-1,
		NULL,
		0);
	wchar_t *  pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP,
		0,
		str,
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	if (wcslen(pUnicode) <= MaxSize)
		result.Format(L"%ls", pUnicode);
	else
		result = L"";
	delete  pUnicode;
}

//�������������������ʾ�����д���  
BOOL execmd(wchar_t* CommandLine, char* result, int MaxSize)
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return   FALSE;
	}

	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//�ؼ����裬CreateProcess�����������������MSDN     
	if (!CreateProcess(NULL, CommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		return   FALSE;
	}
	CloseHandle(hWrite);

	char   buffer[128] = { 0 };
	DWORD   bytesRead;
	while (true)
	{
		memset(buffer, 0, strlen(buffer));
		if (ReadFile(hRead, buffer, 127, &bytesRead, NULL) == NULL)
			break;
		if (strlen(result) + strlen(buffer) + 1 <= MaxSize)
			strcat_s(result, strlen(result) + strlen(buffer) + 1, buffer);
		//Sleep(100);
	}
	return   TRUE;
}

void UnicodeToANSI(const wchar_t* str, char* result, int MaxSize)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	if (strlen(pElementText) <= MaxSize)
		strcpy_s(result, MaxSize, pElementText);
	else
		result = NULL;
}

int checkLang(CString str)
{
	int lang = 0;//English as default
	int count_char[3] = { 0 };//[0]->English, [1]->kana, [2]->character
	for (int i = 0; i < wcslen(str); i++)
	{
		if ((str[i] >= 0x41 && str[i] <= 0x5A) || (str[i] >= 0x61 && str[i] <= 0x7A))//English character
			count_char[0]++;
		else if (str[i] >= 0x3040 && str[i] <= 0x31FE || str[i] >= 0xFF00 && str[i] <= 0xFFEF)//Japanese Kana
			count_char[1]++;
		else if (str[i] >= 0x2E80 && str[i] <= 0xFE4F)//Chinese character
			count_char[2]++;
	}
	if (count_char[0] >= count_char[1] + count_char[2])
		lang = 0;//English
	else if (count_char[1] / ((double)count_char[2]) >= zh_jp_ratio/10.0)
		lang = 1;//Japanese
	else
		lang = 2;//Chinese
	return lang;
}

void trim(char *str, char* result, int MaxSize)
{
	int idx = 0;
	for (int i = 0; i < strlen(str)&& i < MaxSize-1; i++)
		if (str[i] >= '0' && str[i] <= '9' || str[i] >= 'a' && str[i] <= 'f' || str[i] >= 'A' && str[i] <= 'F')
			result[idx++] = str[i];
	result[idx] = '\0';
}

bool UnicodeStr2wchar(CString str, CString& result, int MaxSize)
{
	result.Empty();
	CString buff = L"";
	if (str.GetLength() >= MaxSize / 5)
		return false;
	for (int i = 0; i < str.GetLength(); i++)
	{
		buff.Format(L"%4x ", str[i]);
		result += buff;
	}
	//MessageBoxW(NULL, result, L"", 0);
	return true;
}

//�ظ�ʱ�������ASCII�ַ���һ��ת��Unicode��
bool char2Unicode(char* str, CString& result, int MaxSize)
{
	char* temp = new char[MaxSize];
	trim(str, temp,  MaxSize);
	result.Empty();
	wchar_t ch = L'\0';
	for (int i = 0; i < strlen(temp); i += 4)
	{
		 ch =  (temp[i] >= 'a' ? temp[i] - 'a' + 10 : temp[i] - '0') * 4096 +
			(temp[i + 1] >= 'a' ? temp[i + 1] - 'a' + 10 : temp[i + 1] - '0') * 256 +
			(temp[i + 2] >= 'a' ? temp[i + 2] - 'a' + 10 : temp[i + 2] - '0') * 16 +
			(temp[i + 3] >= 'a' ? temp[i + 3] - 'a' + 10 : temp[i + 3] - '0');
			result += ch;
	}
	//printf("UnicodeChar: %ws\n", result);
	delete[] temp;
	return true;
}

void refinePathEnd(CString& src)
{
	int i = src.GetLength()-1;
 	while (i >= 0 && src.GetAt(i) == '\\')
		i--;
	src = src.Left(i + 1);
}


UINT ProcessDlgFunc(LPVOID in)
{
	load_result = 0;
	load_flag = true;
	wchar_t cmd[MSG_SIZE] = L"";
	char response[MSG_SIZE] = "";
	bool flag = true;
	CLoadAIMLDlg *dlg = (CLoadAIMLDlg*)in;
	if (load_flag)
	{
		wsprintf(cmd, L"-rd \"%ls\\..\\..\\aiml\\annotated_alice\"", Rebecca_exec_path);
		dlg->setProgress(dlg->rd);
		dlg->setCmd(cmd);
		flag &= shell(cmd, response, MSG_SIZE);
	}
	if (load_flag)
	{
		wsprintf(cmd, L"-rg");
		dlg->setProgress(dlg->rg);
		dlg->setCmd(cmd);
		flag &= shell(cmd, response, MSG_SIZE);
	}
	if (load_flag)
	{
		wsprintf(cmd, L"-aduaa \"%ls\\..\\..\\aiml\\annotated_alice\"", Rebecca_exec_path);
		dlg->setProgress(dlg->aduaa);
		dlg->setCmd(cmd);
		flag &= shell(cmd, response, MSG_SIZE);
	}
	if (load_flag)
	{
		wsprintf(cmd, L"-cg");
		dlg->setProgress(dlg->cg);
		dlg->setCmd(cmd);
		flag &= shell(cmd, response, MSG_SIZE);
	}
	if (load_flag)
	{
		wsprintf(cmd, L"-gafls");
		dlg->setProgress(dlg->gafls);
		dlg->setCmd(cmd);
		flag &= shell(cmd, response, MSG_SIZE);
	}
	if (!load_flag || !flag || strcmp(response, "0") == 0)
	{
		printf("Cannot load Rebecca AIML files\n");
		load_result = -1;
		return 1;
	}
	load_result = 1;
	return 0;

}
bool loadRebecca(CWeChatDlg *parent)
{
	CLoadAIMLDlg *dlg;
	dlg = new CLoadAIMLDlg(parent);
	//dlg->Create(IDD_LOAD_AIML_DIALOG);
	while (load_result == 0)
		Sleep(1000);
	HANDLE hThread = AfxBeginThread(ProcessDlgFunc, dlg);
	dlg->DoModal();
	delete dlg;
	return load_result&&load_flag;
}

