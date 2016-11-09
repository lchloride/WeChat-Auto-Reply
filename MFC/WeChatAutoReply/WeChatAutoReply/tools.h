#pragma once
#include <cstring>
#include <stdio.h>
#include <windows.h>
#include "WeChatAutoReplyDlg.h"

//const int MaxPathLeng = 1024;This constant does not need to be defined since in Windows OS, the longest path length is 260 and stored in Macro MAX_PATH
const int MSG_SIZE = 2048;//It defines the maxium of message length

void ANSIToUnicode(const char* str, CString& result, int MaxSize);
//int execmd(char* cmd, char* result, int MaxSize);
BOOL execmd(wchar_t* CommandLine, char* result, int MaxSize);
void UnicodeToANSI(const wchar_t* str, char* result, int MaxSize);
int checkLang(CString str);
bool char2Unicode(char* str, CString& result, int MaxSize);
bool UnicodeStr2wchar(CString str, CString& result, int MaxSize);
void refinePathEnd(CString& src);
bool loadRebecca(CWeChatDlg *parent);
