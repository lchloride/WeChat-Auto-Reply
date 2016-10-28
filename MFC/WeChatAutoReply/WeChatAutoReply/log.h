#pragma once
#include <cstdio>
#include <ctime>

#define START 0
#define RECVMSG 1
#define SENDMSG 2
#define OPERATION 3
#define ERR 4

bool writeLog(CString content, char* source, int label);