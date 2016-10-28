#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <cstdlib>
#include <direct.h>
#include "tools.h"
#include "WeChatAutoReplyDlg.h"


//const unsigned int Max = 1024;
extern  CString Rebecca_exec_path;
//Create content in query.bat, notice that path is surrounded by ""
//And paragram cmd does not with the prefix word "rs-admin "
//Rebecca_exec_path is enternal variable that indicate the path of "rs-admin.exe", surrounded by "" as well
bool createBat(char* path, CString cmd)
{
	FILE* fp;
	//char* Rebecca_exec_path;
	fopen_s(&fp, "query.bat", "w");
	if (fp == NULL)
	{
		printf("Create File Failed.\n");
		return false;
	}
	if (Rebecca_exec_path.IsEmpty())
	{
		printf("Cannot find Rebecca AIML execuating path.\n");
		return false;
	}
	fprintf(fp, "@echo off\n%C%C\ncd \"%ls\"\n", Rebecca_exec_path[0], Rebecca_exec_path[1], Rebecca_exec_path);
	fprintf(fp, "rs-admin %ls\n", cmd);
	fprintf(fp, "@echo on\n");
	fclose(fp);
	return true;
}

bool shell(CString cmd, char* response, int MaxSize)
{
	char result[1024 * 4] = "";                   //定义存放结果的字符串数组 
	char path[1024] = "";
	char *path_1 = &path[1];
	if (cmd.IsEmpty())//Not to response invalid query words
		return false;

	//Obtain query.bat path and connect to a whole filename
	_getcwd(path_1, 1023);
	strcat_s(path_1, strlen(path_1) + 12, "\\query.bat\"");
	path[0] = '\"';
	createBat(path, cmd);

	//Execuate query.bat using cmd and get feedback with pipe
	if (1 == execmd(path, result, MaxSize)) {
		printf("Query Result:%s\n",result);
		result[min(strlen(result), (unsigned)MaxSize)] = '\0';
		if (result[strlen(result) - 1] == '\n')
			result[strlen(result) - 1] = '\0';
		strcpy_s(response, strlen(result)+1, result);
		return true;
	}
	else
		return false;

	return true;
}

bool query(CString query_word, char* response, int MaxSize)
{
	wchar_t cmd[MSG_SIZE] = L"";
	wsprintf(cmd, L"-gr \"%ls\"", query_word);
	return shell(cmd, response, MaxSize);
}