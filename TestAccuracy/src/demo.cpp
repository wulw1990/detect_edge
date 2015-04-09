#if 1

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
#include <vector>
#include <iomanip>
#include <time.h>
#include <sstream>

using namespace std;
using namespace cv;

#include "../../EdgeDetectSubpixel/src/system_dependent/FileDealer.h"

#include "../../EdgeDetectSubpixel/src/SubDetectorBase.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorAreaEffect.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorBlurModel.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorFacetModel.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorSobelProjection.h"
#include "AccuracyTester.h"


string getOutputPath(string name)
{
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	stringstream ss;
	ss << setfill('0');
	ss << "output_" << name << "_" 
		<< setw(4) << t->tm_year + 1900 << "_"
		<< setw(2) << t->tm_mon + 1 << "_"
		<< setw(2) << t->tm_mday << "__"
		<< setw(2) << t->tm_hour << "_"
		<< setw(2) << t->tm_min << "_"
		<< setw(2) << t->tm_sec << "\\";
	return ss.str();
}

int main()
{
	string path_image("image\\");
	vector<string> list;
	FileDealer::GetFileListChinese(path_image, list);
	//cout << list.size() << endl;
	if (list.size() != 1){
		cerr << "image文件夹中应该有且只有一个图片" << endl;
		system("pause");
		return -1;
	}

	Mat gray = imread(path_image + list[0], 0);
	assert(!gray.empty());

	AccuracyTester* tester = new AccuracyTester();

	vector<SubDetectorBase*> sub_detectors;
	sub_detectors.push_back(new SubDetectorAreaEffect());
	sub_detectors.push_back(new SubDetectorBlurModel());
	sub_detectors.push_back(new SubDetectorFacetModel());
	sub_detectors.push_back(new SubDetectorSobelProjection());

	string path_output = getOutputPath(list[0]);

	tester->test(gray, sub_detectors, path_output);

	system("pause");
}

#endif