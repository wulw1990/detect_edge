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

using namespace std;
using namespace cv;


#include "../../EdgeDetectSubpixel/src/SubDetectorBase.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorAreaEffect.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorBlurModel.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorFacetModel.h"
#include "../../EdgeDetectSubpixel/src/SubDetectorSobelProjection.h"
#include "AccuracyTester.h"

bool errorFileNotExist(string name)
{
	cerr << "File not exist : " << name << endl;
	return false;
}

void GetNearestValidPoint(vector<Point> src, Mat gray, vector<Point>& dst);

int main()
{
	string path("D:\\AutoSync\\MyProject\\BBNC凌云——重大仪器项目\\项目1——亚像素边缘检测\\精度验证\\边缘算法精度验证2015PCB机台图\\2015.01.07-PCB机台图\\");
	string name("内层\\10um\\内层板-对焦清晰-10um-109-73-73-向上400脉冲-500um.bmp");
	
	Mat gray = imread(path + name, 0);
	assert(!gray.empty());

	AccuracyTester* tester = new AccuracyTester();

	vector<SubDetectorBase*> sub_detectors;
	sub_detectors.push_back(new SubDetectorAreaEffect());
	sub_detectors.push_back(new SubDetectorBlurModel());
	sub_detectors.push_back(new SubDetectorFacetModel());
	sub_detectors.push_back(new SubDetectorSobelProjection());

	float distance_gt = 0;
	string output_prefix = "hh\\";

	tester->test(gray, distance_gt, sub_detectors, output_prefix);

	system("pause");
}