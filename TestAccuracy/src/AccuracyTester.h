#ifndef _ACCURACY_TESTER_H_
#define _ACCURACY_TESTER_H_

#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#include "../../EdgeDetectSubpixel/src/SubDetectorBase.h"

class AccuracyTester
{
public:
	void test(Mat gray, vector<SubDetectorBase*> sub_detectors, string output_path);
private:
	float getDistance(
		Mat gray,
		Point p1, Point p2,
		SubDetectorBase* sub_detector);

	float getDistance(Point2f p1, Point2f p2);
	float getDistance(Point p1, Point p2);

	Point GetNearestValidPoint(Mat gray, Point p);

	void saveSubPixelEdgeResult(Mat gray, SubDetectorBase* sub_detector, string name);
	void savePixelEdgeResult(Mat gray, string name);
	void saveTwoPoints(Mat gray, Point2f p1, Point2f p2, string name);
};

#endif