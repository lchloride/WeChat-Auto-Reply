#include "tools.h"
#include "shell.h"

using namespace std;
wchar_t Rebecca_exec_path[MAX_PATH]=L"";
int zh_jp_ratio =6;
wchar_t log_path[MAX_PATH] = L"";
bool date_diff = true;
wchar_t ini_name[] = L"\\property.ini";

void ANSIToUnicode(const char* str, wchar_t* result, int MaxSize)
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
		wcscpy_s(result, wcslen(pUnicode) + 1, pUnicode);
	else
		result = NULL;
	delete  pUnicode;
}

// 描述:execmd函数执行命令，并将结果存储到result字符串数组中 
// 参数:cmd表示要执行的命令
// result是执行的结果存储的字符串数组
// 函数执行成功返回1，失败返回0  
int execmd(char* cmd, char* result, int MaxSize) {
	char buffer[128];                         //定义缓冲区   
	int count = 0;
	FILE* pipe = _popen(cmd, "r");            //打开管道，并执行命令 
	if (!pipe)
		return 0;                      //返回0表示运行失败 

	while (!feof(pipe)&&count*128<MaxSize) {
		if (fgets(buffer, 128, pipe)) {             //将管道输出到result中 
			strcat_s(result, strlen(result) + strlen(buffer) + 1, buffer);
		}
	}
	_pclose(pipe);                            //关闭管道 
	return 1;                                 //返回1表示运行成功 
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

int checkLang(wchar_t *str)
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

bool unicode2wchar(wchar_t * str, wchar_t* result, int MaxSize)
{
	wchar_t buff[6] = { 0 };
	if (wcslen(str) >= MaxSize / 5)
		return false;
	//int idx = 0;
	for (int i = 0; i < wcslen(str); i++)
	{
		//if (str[i] >= 0x2E80 && str[i] <= 0xFE4F)//For non-English Characters, change to unicode format
		//{
			wsprintf(buff, L"%4x ", str[i]);
			for (int j = 0; j < 6; j++)
				//result[idx++] = buff[j];
				result[i * 5 + j] = buff[j];
		//}
		//else//For ASCII characters, remain them
		//	result[idx++] = str[i];
	}
	printf("charArray: %s\n", result);
	return true;
}

//回复时如果出现ASCII字符，一律转成Unicode码
bool char2Unicode(char* str, wchar_t *result, int MaxSize)
{
	char* temp = new char[MaxSize];
	trim(str, temp,  MaxSize);
	for (int i = 0; i < strlen(temp); i += 4)
	{
		result[i / 4] = (temp[i] >= 'a' ? temp[i] - 'a' + 10 : temp[i] - '0') * 4096 +
			(temp[i + 1] >= 'a' ? temp[i + 1] - 'a' + 10 : temp[i + 1] - '0') * 256 +
			(temp[i + 2] >= 'a' ? temp[i + 2] - 'a' + 10 : temp[i + 2] - '0') * 16 +
			(temp[i + 3] >= 'a' ? temp[i + 3] - 'a' + 10 : temp[i + 3] - '0');

	}
	printf("UnicodeChar: %ws\n", result);
	delete[] temp;
	return true;
}

void refinePathEnd(wchar_t* src)
{
	int i = wcslen(src)-1;
	while (i >= 0 && src[i] == '\\')
	{
		src[i] = '\0';
		i--;
	}
}

void readProperty()
{
	wchar_t exe_full_path[MAX_PATH] = L"";
	int ini_len = wcslen(ini_name);
	int len = GetModuleFileNameW(NULL,
		exe_full_path, //应用程序的全路径存放地址
		MAX_PATH);
	for (int i = len - 1; i >= 0; i--)
		if (exe_full_path[i] != '\\')
			exe_full_path[i] = '\0';
		else
			break;
	refinePathEnd(exe_full_path);
	wcscat_s(exe_full_path, wcslen(exe_full_path) + ini_len + 1, ini_name);
	
	GetPrivateProfileStringW(
		L"Rebecca", // 指向包含 Section 名称的字符串地址 
		L"Rebecca_exec_path", // 指向包含 Key 名称的字符串地址 
		L"", // 如果 Key 值没有找到，则返回缺省的字符串的地址 
		Rebecca_exec_path, // 返回字符串的缓冲区地址 
		MAX_PATH, // 缓冲区的长度 
		exe_full_path // ini 文件的文件名 
		);
	refinePathEnd(Rebecca_exec_path);

	zh_jp_ratio = GetPrivateProfileIntW(
		L"language", // 指向包含 Section 名称的字符串地址 
		L"zh_jp_ratio", // 指向包含 Key 名称的字符串地址 
		6, // 如果 Key 值没有找到，则返回缺省的值是多少 
		exe_full_path // ini 文件的文件名 
		);

	GetPrivateProfileStringW(
		L"log", // 指向包含 Section 名称的字符串地址 
		L"path", // 指向包含 Key 名称的字符串地址 
		L"", // 如果 Key 值没有找到，则返回缺省的字符串的地址 
		log_path, // 返回字符串的缓冲区地址 
		MAX_PATH, // 缓冲区的长度 
		exe_full_path // ini 文件的文件名 
		);
	refinePathEnd(Rebecca_exec_path);

	date_diff = (bool)GetPrivateProfileIntW(
		L"log", // 指向包含 Section 名称的字符串地址 
		L"date_diff", // 指向包含 Key 名称的字符串地址 
		1, // 如果 Key 值没有找到，则返回缺省的值是多少 
		exe_full_path // ini 文件的文件名 
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