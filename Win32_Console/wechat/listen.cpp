
#include <cstdio>
#include <windows.h>
#include <wininet.h>
#include <tchar.h>
#include <shlwapi.h>
#include <locale.h>

#define MAXSIZE 1024
#define MAXCHARSIZE 50
#define INIT 0
#define TRANS 1
#define UL 2

#pragma comment(lib, "Wininet.lib") 
#pragma comment(lib, "shlwapi.lib") 

const int BUFF_LEN = 23;

void urlopen(_TCHAR* url);
int substr(char* str1, char*str2, int pos);//从str1的pos位置开始寻找是否存在str2，存在
										   //则返回第一次出现之后的位置，不存在返回-1
bool getMeaning(wchar_t*);//从网页的脚本中获取单词的含义，获取成功返回true，失败返回false


//int _tmain(int argc, _TCHAR* argv[])
int getType(wchar_t* Char)
{
	setlocale(LC_ALL, "chs");
	wchar_t urlAddr[MAXSIZE] = { L"http://dict.youdao.com/w/eng/" };

	DWORD len = 6 * MAXCHARSIZE;
	//wchar_t Char1[MAXCHARSIZE] = L"";
	wcscpy_s(urlAddr, 30, L"http://dict.youdao.com/w/eng/");

	//wprintf(L"%ls\n", Char);
	wcscat_s(urlAddr, Char);
	wcscat_s(urlAddr, L"/#keyfrom=dict2.index");

	//wprintf(L"url-trans:%ls\n", CharEncode);
	urlopen(urlAddr);

	return getMeaning(Char);
}

void urlopen(_TCHAR* url)
{
	FILE *fp;
	fopen_s(&fp, "script.txt", "w");
	HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

		if (hHttp != NULL)
		{
			//wprintf_s(_T("%s\n"), url);

			char Temp[MAXSIZE];
			ULONG Number = 1;
			while (Number > 0)
			{
				InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);
				Temp[Number] = '\0';
				fprintf(fp, "%s", Temp);
				//wprintf(L"%ls", Temp);
			}
			InternetCloseHandle(hHttp);
			hHttp = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
	fclose(fp);
}
bool getMeaning(wchar_t* word)
{
	FILE *fp=NULL;
	static int state = INIT;
	char buff[2 * BUFF_LEN] = { '\0' };
	memset(buff, 0, sizeof(char));
	fopen_s(&fp, "script.txt", "r");
	if (!fp)
	{
		printf("Cannot open the script from websites. Check the file script.txt\n");
		return false;
	}
	else
	{
		for (int i = 0; i < BUFF_LEN; i++)
			buff[i] = fgetc(fp);
		int pos = 0;
		while (!feof(fp))
		{
			for (int i = 0; i < BUFF_LEN; i++)
				buff[BUFF_LEN + i] = fgetc(fp);
			//printf("%d %s\n", state, buff);
			if ((pos = substr(buff, "error-typo", 0)) >= 0)
			{				
				fclose(fp);
				return false;
			}
			if (state == INIT && (pos = substr(buff, "<span class=\"keyword\">", 0)) >= 0)
				state = TRANS;
			if (state == TRANS && (pos = substr(buff, "<ul>", 0)) >= 0)
				state = UL;
			if (state == UL && (pos = substr(buff, "<p class=\"wordGroup\">", 0)) >= 0)
			{
				fclose(fp);
				return true;
				for (int j = 0; j < BUFF_LEN; j++)
					buff[j] = fgetc(fp);
				continue;
			}
			if (state == UL && ((pos = substr(buff, "</ul>", 0)) >= 0)|| (pos = substr(buff, "</div>", 0)) >= 0)
			{
				state = INIT;
				//break;
			}
			for (int i = 0; i < BUFF_LEN; i++)
				buff[i] = buff[BUFF_LEN + i];
		}
	}
	fclose(fp);
	return false;
}
int substr(char* str1, char*str2, int pos)
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	int i = 0, j = 0;
	while (i < len1 && j < len2)
	{
		if (str1[i] == str2[j])
		{
			i++;
			j++;
		}
		else
		{
			i++;
			j = 0;
		}
	}
	if (j == len2)
		return i;
	else
		return -1;
}
