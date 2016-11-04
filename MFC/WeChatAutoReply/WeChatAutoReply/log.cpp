#include "stdafx.h"
#include "log.h"
#include "tools.h"
#include <locale.h>


//using namespace std;

extern CString log_path;
extern bool date_diff;

bool writeLogFileCtrl(
	wchar_t* filename, //文件名
	CWeChatDlg* dlg, //对话框的句柄
	int type, //消息的分级
	CString date, //日期
	CString time, //时间
	CString label, //标签
	CString content, //消息内容
	CString source) //来源
{
	FILE *log_file = NULL;
	_wfopen_s(&log_file, filename, L"a");
	if (!log_file)
	{
		wprintf(L"Cannot open log file.\n");
		return false;
	}
	if (printf("%ls", content) < 0 && type == MESSAGE)
		type = WARNING;
	CString typeStr = L"";
	switch (type)
	{
	case MESSAGE:  typeStr = L"消息"; break;
	case WARNING: typeStr = L"警告"; break;
	case ERR: typeStr = L"错误"; break;
	}

	if (label == L"Start")
		fprintf(log_file, "=====START=====\n");
	if (fprintf(log_file, "%ls,%ls,%ls,%ls,\"%ls\",\"%ls\"\n", typeStr, date, time, label, content, source) < 0)
	{
		fprintf(log_file, "Unreadable Character Cannot be Printed.");
		CString tmp = L"";
		UnicodeStr2wchar(content, tmp, MSG_SIZE);
		fprintf(log_file, "UNICODE:%ls\", \"%ls\"\n", tmp, source);
		content += L"(具有不可读字符, Unicode: " + tmp + L")";
	}
	dlg->m_logDisplay.InsertItem(0, typeStr);
	dlg->m_logDisplay.SetItemText(0, 1, date);
	dlg->m_logDisplay.SetItemText(0, 2, time);
	dlg->m_logDisplay.SetItemText(0, 3, label);
	dlg->m_logDisplay.SetItemText(0, 4, content);
	dlg->m_logDisplay.SetItemText(0, 5, source);

	fclose(log_file);
	return true;
}


bool writeLog(CWeChatDlg* dlg, CString content, CString source, int label)
{
	time_t tt;
	tm timeinfo;
	setlocale(LC_ALL, "chs");
	time(&tt);
	localtime_s(&timeinfo, &tt);
	char filename[MAX_PATH] = "";
	strftime(filename, MAX_PATH, "log%Y%m%d.log", &timeinfo);
	CString filename_wstr = L"";
	ANSIToUnicode(filename, filename_wstr, MAX_PATH);
	wchar_t file_tmp[MAX_PATH] = L"";
	wsprintf(file_tmp, L"%ls\\%ls", log_path, filename_wstr);
	wprintf(L"log filename:%ls\n", file_tmp);

	//char date_time[MSG_SIZE] = "";
	char time[MSG_SIZE] = "";
	char date[MSG_SIZE] = "";
	CString timeStr, dateStr;
	strftime(date, MSG_SIZE, "%F", &timeinfo);
	strftime(time, MSG_SIZE, "(%z) %T", &timeinfo);
	ANSIToUnicode(date, dateStr, MSG_SIZE);
	ANSIToUnicode(time, timeStr, MSG_SIZE);
	int flag = true;
	switch (label)
	{
	case START:
		flag = writeLogFileCtrl(file_tmp, dlg, MESSAGE, dateStr, timeStr, L"Start", content, source);
		break;
	case RECVMSG:
		flag = writeLogFileCtrl(file_tmp, dlg, MESSAGE, dateStr, timeStr, L"Receive Message", content, source);
		break;
	case SENDMSG:
		flag = writeLogFileCtrl(file_tmp, dlg, MESSAGE, dateStr, timeStr, L"Send Message", content, source);
		break;

	case OPERATION: 		
		flag = writeLogFileCtrl(file_tmp, dlg, MESSAGE, dateStr, timeStr, L"Operation Result", content, source);
		break;
	case ERR: 
		flag = writeLogFileCtrl(file_tmp, dlg, ERR, dateStr, timeStr, L"Error Message", content, source);
		break;
	case WARNING:
		flag = writeLogFileCtrl(file_tmp, dlg, WARNING, dateStr, timeStr, L"Warning Message", content, source);
		break;
	default:
		flag = writeLogFileCtrl(file_tmp, dlg, WARNING, dateStr, timeStr, L"No Matched Label", content, source);
		break;
	}
	return flag;
}