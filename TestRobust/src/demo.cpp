#include "opencv2/opencv.hpp"

#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif

#pragma comment( lib, cvLIB("core"))
#pragma comment( lib, cvLIB("imgproc"))
#pragma comment( lib, cvLIB("highgui"))

#include <string>
#include <time.h>
#include <sstream>
#include <iomanip>
#include "TesterRobust.h"
#include "../../EdgeDetectSubpixel/src/system_dependent/FileDealer.h"
#include "../../EdgeDetectSubpixel/src/infrastructure/Sorter.h"

using namespace std;

string getOutputPath()
{
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	stringstream ss;
	ss << setfill('0');
	ss << "output_" 
		<< setw(4) << t->tm_year + 1900 << "_"
		<< setw(2) << t->tm_mon + 1 << "_"
		<< setw(2) << t->tm_mday << "__"
		<< setw(2) << t->tm_hour << "_"
		<< setw(2) << t->tm_min << "_"
		<< setw(2) << t->tm_sec << "\\";
	return ss.str();
}

int main(void)
{
	string path_list("image\\");
	string path_output = getOutputPath();

	string refer;
	vector<string> list;
	FileDealer::GetFileListChinese(path_list, list);
	(new Sorter())->sortFileNamesByNum(list);
	if (list.size() < 2)
	{
		cerr << "There must be 2 files at least in " << path_list << endl;
		system("pause");
		return -1;
	}
	
	refer = list[0];
	list.erase(list.begin());

	TesterRobust* tester = new TesterRobust();
	tester->TestMultiMethod(path_list, refer, list, path_output);

	system("pause");
}
