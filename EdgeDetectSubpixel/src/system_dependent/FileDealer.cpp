#include "FileDealer.h"

#include <iostream>
#include <fstream>
#include <direct.h>
#include <io.h>
#include "dirent_ls.h"
#include <time.h>
#include <sstream>
#include <windows.h>

/*
* make the dir recursively
* if _dir = "a/b/c"
* than a, a/b, a/b/c will be made
*/
int FileDealer::CreateDirectoryRecursive(std::string _dir)
{
	const char *pDir = _dir.c_str();
	int i = 0;
	int iRet;
	int iLen;
	char* pszDir;

	if (NULL == pDir)
		return 0;

	pszDir = _strdup(pDir);
	iLen = (int)strlen(pszDir);

	// 创建中间目录
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';
			//如果不存在,创建
			iRet = _access(pszDir, 0);
			if (iRet != 0)
			{
				iRet = _mkdir(pszDir);
				if (iRet != 0)
					return -1;
			}
			//支持linux,将所有\换成/
			pszDir[i] = '/';
		}
	}
	iRet = _mkdir(pszDir);
	free(pszDir);
	return iRet;
}

bool FileDealer::CreateDirectoryMy(const std::string& name)
{
	const char *pDir = name.c_str();
	int iRet;
	iRet = _access(pDir, 0);
	if (iRet == 0)
		return true;
	return (_mkdir(pDir)==0);
}

/*
* Function : Get all the file names in the folder and is sub folders recursively
* Input : 
*	folder name
* Output : 
*	file names, output path is ralative to the input _dir
*	is success, return ture; otherwise, return false
*/
bool FileDealer::GetFileList(std::string _dir, std::vector<std::string>& _file_names)
{
	_file_names.clear();
	DIR *p;
	struct dirent *dirp;
	if ((p = opendir(_dir.c_str())) == NULL)
		return false;

	while ((dirp = readdir(p)) != NULL)
	{
		if (dirp->d_type != DT_DIR)
		//std::cout << "dir: " << dirp->d_name << std::endl;
			_file_names.push_back(dirp->d_name);
	}

	closedir(p);

	return true;
}

void FileDealer::PrintStringList(const std::vector<std::string>& _file_names)
{
	for (int i = 0, i_end = (int)_file_names.size(); i < i_end; ++i)
		std::cout << _file_names[i] << std::endl;
}

//TODO
//not very good, exe may has no authority to write
bool FileDealer::GetFileListChinese(std::string dir, std::vector<std::string>& list)
{
	list.clear();
	string cmd;

	stringstream ss;
	ss << "dir /B " << dir;
	cmd = ss.str();

	//cout << cmd << endl;
	string result = exec(cmd);
	//cout << result << endl;

	ss.clear(); ss.str("");
	ss << result;
	string str;
	while (ss >> str)
		list.push_back(str);

	return true;
}

bool FileDealer::ReadList(string name, vector<string> list)
{
	list.clear();
	ifstream ifs(name);
	if (!ifs.is_open())
		return false;
	string str;
	while (ifs >> str)
		list.push_back(str);
	return true;
}



#include <string>
#include <iostream>
#include <stdio.h>

//Replace popen and pclose with _popen and _pclose for Windows.
std::string FileDealer::exec(string cmd) {
	FILE* pipe = _popen(cmd.c_str(), "r");
	if (!pipe) return "ERROR";
	char buffer[128];
	std::string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	_pclose(pipe);
	return result;
}
