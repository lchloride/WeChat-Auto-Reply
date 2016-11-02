#pragma once
#include <cstdio>
#include <ctime>
#include "WeChatAutoReplyDlg.h"

#define START 0
#define RECVMSG 1
#define SENDMSG 2
#define OPERATION 3
#define ERR 4
#define WARNING 5

#define MESSAGE 0
//#define WARNING 5
//#define ERR 4

bool writeLog(CWeChatDlg* dlg, CString content, CString source, int label);
