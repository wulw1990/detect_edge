#ifndef _FILE_DEALER_H_WULIWEI_BBNC_TSINGHUA_
#define _FILE_DEALER_H_WULIWEI_BBNC_TSINGHUA_

#include <string>
#include <vector>

using namespace std;

namespace FileDealer
{
	int CreateDirectoryRecursive(std::string _dir);
	bool CreateDirectoryMy(const std::string& name);
	bool GetFileList(std::string _dir, std::vector<std::string>& _file_names);
	bool GetFileListChinese(std::string _dir, std::vector<std::string>& _file_names);
	void PrintStringList(const std::vector<std::string>& _file_names);
	bool ReadList(string name, vector<string> list);

	std::string exec(string  cmd);
}

#endif