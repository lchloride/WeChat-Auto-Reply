#include "divide.h"
#include "stdafx.h"
using namespace std;

//vector<wstring> word;
int word_num = 0;

void divideWord(wchar_t sent[], vector<wstring> & word)
{
	int i = 0, j = 1;
	int len = wcslen(sent);
	bool word_exist = 0;
	while (i < len)
	{
		for (j = len - i; j > 0; j--)
		{
			wchar_t Char[MAXCHARSIZE] = { 0 };
			wcsncpy_s(Char, sent + i, j);
			//wprintf(L"Char Divide: %ls\n", Char);
			word_exist = getType(Char);
			//printf("word exist:%d\n", word_exist);
			if (word_exist != 0)
			{
				//wcscpy_s(word[word_num++], Char);
				word.push_back(Char);
				break;
			}
		}
		if (j == 0)
		{
			wchar_t temp[MAXCHARSIZE] = { 0 };
			wcsncpy_s(temp, sent+i, 1);
			word.push_back(temp);
			i++;
		}
		else
			i = i + j;
	}
}
//int main()
//void divideWords(vector<wstring>& word)
//{
//	time_t t, tt;
//	t = time(&t);
//	wchar_t sent[] = L"俞敏洪词缀记忆法废物录入整理版";
//	setlocale(LC_ALL, "chs");
//	wcout.imbue(locale("chs"));
//	divideWord(sent);
//	for (int i = 0; i < word.size(); i++)
//		wcout << "word: " << word.at(i) << endl;
//	tt= time(&tt);
//	printf("%d\n", tt - t);

	//wchar_t wstr[50] = L"你好";
	//vector<wstring> words;
	//words.push_back(wstr);
	//wcscpy_s(wstr, L"谢谢");
	//words.push_back(wstr);
	//wcout.imbue(locale("chs"));
	//for (int i = 0; i<words.size(); i++)
	//	wcout << words.at(i) << endl;
//	return 0;
//}