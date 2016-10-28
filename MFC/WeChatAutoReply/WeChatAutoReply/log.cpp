#include "stdafx.h"
#include "log.h"
#include "tools.h"

//using namespace std;

extern CString log_path;
extern bool date_diff;

bool writeLog(CString content, char* source, int label)
{
	time_t tt;
	tm timeinfo;
	FILE *log_file=NULL;
	time(&tt);
	localtime_s(&timeinfo, &tt);
	char filename[MAX_PATH] = "";
	strftime(filename, MAX_PATH, "log%Y%m%d.log", &timeinfo);
	CString filename_wstr = L"";
	ANSIToUnicode(filename, filename_wstr, MAX_PATH);
	wchar_t file_tmp[MAX_PATH] = L"";
	wsprintf(file_tmp, L"%ls\\%ls", log_path, filename_wstr);
	wprintf(L"log filename:%ls\n", file_tmp);
	_wfopen_s(&log_file, file_tmp, L"a");
	if (!log_file)
	{
		wprintf(L"Cannot open log file.\n");
		return false;
	}
	char date_time[MSG_SIZE] = "";
	switch (label)
	{
	case START:
		strftime(date_time, MSG_SIZE, "%F,%Z:%T", &timeinfo);
		fprintf(log_file, "=====START=====\n%s,Start Auto Reply,\"%ls\",\"%s\"\n", date_time, content, source);
		break;
	case RECVMSG:
		strftime(date_time, MSG_SIZE, "%F,%Z:%T", &timeinfo);
		if (fprintf(log_file, "%s,Receive Message,\"%ls\",\"%s\"\n", date_time, content, source) < 0)
		{
			fprintf(log_file, "Unreadable Character Cannot be Printed.");
			CString tmp = L"";
			UnicodeStr2wchar(content, tmp, MSG_SIZE);
			fprintf(log_file, "UNICODE:%ls\", \"%s\"\n", tmp, source);
		}
		break;
	case SENDMSG:
		strftime(date_time, MSG_SIZE, "%F,%Z:%T", &timeinfo);
		if (fprintf(log_file, "%s,Send Message,\"%ls\",\"%s\"\n", date_time, content, source) < 0)
		{
			fprintf(log_file, "Unreadable Character Cannot be Printed.");
			CString tmp = L"";
			UnicodeStr2wchar(content, tmp, MSG_SIZE);
			fprintf(log_file, "UNICODE:%ls\", \"%s\"\n",tmp, source);
		}
		break;

	case OPERATION: 		
		strftime(date_time, MSG_SIZE, "%F,%Z:%T", &timeinfo);
		fprintf(log_file, "%s,Operation Result,%ls,%s\n", date_time, content, source);
		break;
	case ERR: 
		strftime(date_time, MSG_SIZE, "%F,%Z:%T", &timeinfo);
		fprintf(log_file, "%s,Error Message,%ls,%s\n", date_time, content, source);
		break;
	default:
		strftime(date_time, MSG_SIZE, "%F,%Z:%T", &timeinfo);
		fprintf(log_file, "%s,No Matched Label,%ls,%s\n", date_time, content, source);
		break;
	}
	fclose(log_file);
	return true;
}