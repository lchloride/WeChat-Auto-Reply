#include <stdio.h>
#include <Windows.h>
#include <cstdlib>
#include <direct.h>
#include "tools.h"



bool createBat(char* path, char* query_word)
{
	FILE* fp;

	fopen_s(&fp, "query.bat", "w");
	if (fp == NULL)
	{
		printf("Create File Failed.\n");
		return false;
	}
	fprintf(fp, "@echo off\n%c%c\ncd \"%s\\bin\\cpp\"\n", path[0], path[1], path);
	fprintf(fp, "rs-admin %s\n", query_word);
	fprintf(fp, "@echo on\n");
	fclose(fp);
	return true;
}

bool query(char* query_word, char* response, int MaxSize, bool first) {
	char result[1024 * 4] = "";                   //定义存放结果的字符串数组 
	char path[1024] = "";
	char* command = new char[MaxSize + 100];
	char *path_1 = &path[1];
	if (query_word == NULL)//Not to response invalid query words
		return false;
	_getcwd(path_1, 1023);
	sprintf(command, "-gr \"%s\"", query_word);
	printf("command:%s\n", command);
	createBat(path_1, command);

	strcat_s(path_1, strlen(path_1)+12, "\\query.bat\"");
	path[0] = '\"';

	delete[] command;
	if (1 == execmd(path, result, MaxSize)) {
		printf(result);
		if (result[strlen(result) - 1] == '\n')
			result[strlen(result) - 1] = '\0';

		strcpy_s(response, min(strlen(result)+1, MaxSize+1), result);
		return true;
	}
	else
		return false;
	//system("pause");                          //暂停以查看结果 
}

bool shell(char* query, char* response, int MaxSize)
{
	char result[1024 * 4] = "";                   //定义存放结果的字符串数组 
	char path[1024] = "";
	char *path_1 = &path[1];
	if (query == NULL)//Not to response invalid query words
		return false;
	_getcwd(path_1, 1023);
	createBat(path_1, query);
	strcat_s(path_1, strlen(path_1) + 12, "\\query.bat\"");
	path[0] = '\"';
	if (1 == execmd(path, result, MaxSize)) {
		printf("Query Result:%s\n",result);
		strcpy_s(response, min(strlen(result) + 1, MaxSize + 1), result);
		return true;
	}
	else
		return false;

	return true;
}