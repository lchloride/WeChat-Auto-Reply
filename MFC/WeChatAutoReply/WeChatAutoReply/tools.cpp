#include "stdafx.h"
#include "tools.h"
#include "shell.h"
#include "WeChatAutoReplyDlg.h"


//using namespace std;
CString Rebecca_exec_path=L"";
int zh_jp_ratio =6;
CString log_path = L"";
bool date_diff = true;
CString ini_name = L"\\property.ini";

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

// ����:execmd����ִ�������������洢��result�ַ��������� 
// ����:cmd��ʾҪִ�е�����
// result��ִ�еĽ���洢���ַ�������
// ����ִ�гɹ�����1��ʧ�ܷ���0  
int execmd(char* cmd, char* result, int MaxSize) {
	char buffer[128];                         //���建����   
	int count = 0;
	FILE* pipe = _popen(cmd, "r");            //�򿪹ܵ�����ִ������ 
	if (!pipe)
		return 0;                      //����0��ʾ����ʧ�� 

	while (!feof(pipe)&&count*128<MaxSize) {
		if (fgets(buffer, 128, pipe)) {             //���ܵ������result�� 
			strcat_s(result, strlen(result) + strlen(buffer) + 1, buffer);
		}
	}
	_pclose(pipe);                            //�رչܵ� 
	return 1;                                 //����1��ʾ���гɹ� 
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
	result = L"";
	CString buff = L"";
	if (str.GetLength() >= MaxSize / 5)
		return false;
	for (int i = 0; i < str.GetLength(); i++)
	{
		buff.Format(L"%4x ", str[i]);
		result += buff;
	}
	MessageBoxW(NULL, result, L"", 0);
	return true;
}

//�ظ�ʱ�������ASCII�ַ���һ��ת��Unicode��
bool char2Unicode(char* str, CString& result, int MaxSize)
{
	char* temp = new char[MaxSize];
	trim(str, temp,  MaxSize);
	for (int i = 0; i < strlen(temp); i += 4)
	{
		result.SetAt(i / 4, (temp[i] >= 'a' ? temp[i] - 'a' + 10 : temp[i] - '0') * 4096 +
			(temp[i + 1] >= 'a' ? temp[i + 1] - 'a' + 10 : temp[i + 1] - '0') * 256 +
			(temp[i + 2] >= 'a' ? temp[i + 2] - 'a' + 10 : temp[i + 2] - '0') * 16 +
			(temp[i + 3] >= 'a' ? temp[i + 3] - 'a' + 10 : temp[i + 3] - '0'));

	}
	printf("UnicodeChar: %ws\n", result);
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

void readProperty()
{
	CString exe_full_path = L"";
	int ini_len = wcslen(ini_name);
	wchar_t tmp[MAX_PATH] = L"";
	int len = GetModuleFileNameW(NULL,
		tmp, //Ӧ�ó����ȫ·����ŵ�ַ
		MAX_PATH);
	for (int i = len - 1; i >= 0; i--)
		if (tmp[i] != '\\')
			tmp[i] = L'\0';
		else
			break;
	exe_full_path.Format(L"%ls", tmp);
	refinePathEnd(exe_full_path);
	exe_full_path = exe_full_path+ini_name;
	//wcscat_s(exe_full_path, wcslen(exe_full_path) + ini_len + 1, ini_name);
	
	memset(tmp, 0, sizeof(wchar_t));
	GetPrivateProfileStringW(
		L"Rebecca", // ָ����� Section ���Ƶ��ַ�����ַ 
		L"Rebecca_exec_path", // ָ����� Key ���Ƶ��ַ�����ַ 
		L"", // ��� Key ֵû���ҵ����򷵻�ȱʡ���ַ����ĵ�ַ 
		tmp,// �����ַ����Ļ�������ַ 
		MAX_PATH, // �������ĳ��� 
		exe_full_path // ini �ļ����ļ��� 
		);
	Rebecca_exec_path.Format(L"%ls", tmp);
	refinePathEnd(Rebecca_exec_path);

	zh_jp_ratio = GetPrivateProfileIntW(
		L"language", // ָ����� Section ���Ƶ��ַ�����ַ 
		L"zh_jp_ratio", // ָ����� Key ���Ƶ��ַ�����ַ 
		6, // ��� Key ֵû���ҵ����򷵻�ȱʡ��ֵ�Ƕ��� 
		exe_full_path // ini �ļ����ļ��� 
		);

	memset(tmp, 0, sizeof(wchar_t));
	GetPrivateProfileStringW(
		L"log", // ָ����� Section ���Ƶ��ַ�����ַ 
		L"path", // ָ����� Key ���Ƶ��ַ�����ַ 
		L"", // ��� Key ֵû���ҵ����򷵻�ȱʡ���ַ����ĵ�ַ 
		tmp, // �����ַ����Ļ�������ַ 
		MAX_PATH, // �������ĳ��� 
		exe_full_path // ini �ļ����ļ��� 
		);
	log_path.Format(L"%ls", tmp);
	refinePathEnd(log_path);

	date_diff = (bool)GetPrivateProfileIntW(
		L"log", // ָ����� Section ���Ƶ��ַ�����ַ 
		L"date_diff", // ָ����� Key ���Ƶ��ַ�����ַ 
		1, // ��� Key ֵû���ҵ����򷵻�ȱʡ��ֵ�Ƕ��� 
		exe_full_path // ini �ļ����ļ��� 
		);
	//wprintf(L"%ls %d\n", Rebecca_exec_path, zh_jp_ratio);
}

bool loadRebecca()
{
	wchar_t cmd[MSG_SIZE] = L"";
	char response[MSG_SIZE] = "";
	wsprintf(cmd, L"-rd \"%ls\\..\\..\\aiml\\annotated_alice\"", Rebecca_exec_path);
	shell(cmd, response, MSG_SIZE);

	wsprintf(cmd, L"-rg");
	shell(cmd, response, MSG_SIZE);

	wsprintf(cmd, L"-aduaa \"%ls\\..\\..\\aiml\\annotated_alice\"", Rebecca_exec_path);
	shell(cmd, response, MSG_SIZE);
	wsprintf(cmd, L"-cg");
	shell(cmd, response, MSG_SIZE);

	wsprintf(cmd, L"-gafls");
	shell(cmd, response, MSG_SIZE);
	if (strcmp(response, "0") == 0)
	{
		printf("Cannot load Rebecca AIML files\n");
		return false;
	}
	return true;
}