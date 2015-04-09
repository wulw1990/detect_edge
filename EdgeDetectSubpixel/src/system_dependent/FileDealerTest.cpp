#if 1

#include "FileDealer.h"

int main()
{
	string dir("D:\\");
	vector<string> list;
	FileDealer::GetFileListChinese(dir, list);
	FileDealer::PrintStringList(list);

	system("pause");
}

#endif
