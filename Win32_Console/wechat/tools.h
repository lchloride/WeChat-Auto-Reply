#pragma once
#include <cstring>
#include <stdio.h>
#include <windows.h>
;
//const int MaxPathLeng = 1024;This constant does not need to be defined since in Windows OS, the longest path length is 260 and stored in Macro MAX_PATH
const int MSG_SIZE = 2048;//It defines the maxium of message length

void ANSIToUnicode(const char* str, wchar_t* result, int MaxSize);
int execmd(char* cmd, char* result, int MaxSize);
void UnicodeToANSI(const wchar_t* str, char* result, int MaxSize);
int checkLang(wchar_t *str);
bool char2Unicode(char* str, wchar_t *result, int MaxSize);
bool unicode2wchar(wchar_t * str, wchar_t* result, int MaxSize);
void readProperty();
bool loadRebecca();