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
#include "TesterPixel.h"
#include "../../EdgeDetectSubpixel/src/system_dependent/FileDealer.h"
#include "../../EdgeDetectSubpixel/src/infrastructure/Sorter.h"

using namespace std;

int main(void)
{
	string path_list("image\\");
	string path_output("output\\");

	string refer;
	vector<string> list;
	FileDealer::GetFileList(path_list, list);
	if (list.size() < 2)
	{
		cerr << "There must be 2 files at least in " << path_list << endl;
		system("pause");
		return -1;
	}
	(new Sorter())->sortFileNamesByNum(list);
	refer = list[0];
	list.erase(list.begin());

	TesterPixel* tester_pixel = new TesterPixel();
	tester_pixel->TestMultiMethod(path_list, refer, list, path_output);

	system("pause");
}
