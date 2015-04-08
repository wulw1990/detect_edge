#include "Sorter.h"
#include <algorithm>
#include <iostream>

void Sorter::sortFileNamesByNum(vector<string>& names)
{
	sort(names.begin(), names.end(), lessFileNameByNum);
}



int Sorter::getNum(string& str, int* p)
{
	const int n = (int)str.length();
	int result = 0;
	while (*p<n && isdigit(str[*p])){
		result = result * 10 + str[*p] - 48;
		++(*p);
	}
	return result;
}
bool Sorter::lessFileNameByNum(string& a, string& b)
{
	//remove suffix
	//const int na = (int)a.length();
	//const int nb = (int)b.length();
	int na = (int)a.length() - 1;
	for (; na >= 0 && a[na]!='.'; --na);
	int nb = (int)b.length() - 1;
	for (; nb >= 0 && b[nb] != '.'; --nb);

	int pa = 0;
	int pb = 0;
	while (pa < na && pb < nb){
		if (!isdigit(a[pa]) || !isdigit(b[pb])){
			if (a[pa] < b[pb])
				return true;
			else if (a[pa] > b[pb])
				return false;
			++pa;
			++pb;
		}
		else{
			int numa = getNum(a, &pa);
			int numb = getNum(b, &pb);
			//cout << numa << "\t" << numb <<endl;
			if (numa < numb)
				return true;
			else if (numa > numb)
				return false;
		}
	}
	if (pa == na)
	{
		//cout << endl;
		//cout << a << endl;
		//cout << b << endl;
		return true;
	}
	else
		return false;
}