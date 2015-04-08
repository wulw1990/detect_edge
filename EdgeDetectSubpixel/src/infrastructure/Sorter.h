#ifndef _SORTER_H_
#define _SORTER_H_

#include <string>
#include <vector>
using namespace std;

class Sorter
{
public:
	void sortFileNamesByNum(vector<string>& names);

private:
	static int getNum(string& str, int* p);
	static bool lessFileNameByNum(string& a, string& b);
};

#endif