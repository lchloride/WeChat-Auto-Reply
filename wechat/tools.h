#pragma once
#include <cstring>
#include <stdio.h>
#include <windows.h>

void ANSIToUnicode(const char* str, wchar_t* result, int MaxSize);
int execmd(char* cmd, char* result, int MaxSize);
void UnicodeToANSI(const wchar_t* str, char* result, int MaxSize);
int checkLang(wchar_t *str);
bool char2Unicode(char* str, wchar_t *result, int MaxSize);
bool unicode2char(wchar_t * str, char* result, int MaxSize);
