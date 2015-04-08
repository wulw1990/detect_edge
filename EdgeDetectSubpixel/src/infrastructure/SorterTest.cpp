#if 1
#include "Sorter.h"
#include <iostream>
#include <fstream>

bool ReadNameList(const std::string& txt_name, std::vector<std::string>& list);
int main()
{
	vector<string> names;
	//names.push_back("abc101");
	//names.push_back("abc21");
	if (!ReadNameList("./src/infrastructure/SorterTestList.txt", names))
	{
		cerr << "File not exist! " << endl;
	}

	(new Sorter())->sortFileNamesByNum(names);

	for (int i = 0; i < (int)names.size(); ++i)
		cout << names[i] << endl;

	system("pause");
}

bool ReadNameList(const std::string& txt_name, std::vector<std::string>& list)
{
	//check input output
	std::ifstream fin(txt_name.c_str());
	if (!fin.is_open())
		return false;
	list.clear();

	std::string name;
	while (fin >> name)
		list.push_back(name);
	return true;
}

#endif