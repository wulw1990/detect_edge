#include  "AccuracyTester.h"

#include <fstream>
#include "../../EdgeDetectSubpixel/src/PixelEdgeDetector.h"
#include "ImageViewer.h"
#include "PointGetter.h"

void AccuracyTester::test(Mat gray, float distance_gt, vector<SubDetectorBase*> sub_detectors, string output_prefix)
{
	Mat shot;
	ImageViewer* viewer = new ImageViewer(1000, 700, "ImageViewer");
	viewer->view(gray, output_prefix + "_roi.bmp", shot);

	PointGetter* point_getter = new PointGetter(1000, 700, "PointGetter");
	vector<Point> points;
	if (!point_getter->getPoints(shot, 2, points))
	{
		cerr << "point_getter quit by user" << endl;
		system("pause");
		return;
	}

	string distance_txt = output_prefix + "_distance.txt";
	ofstream ofs(distance_txt);
	for (int i = 0; i < sub_detectors.size(); ++i){
		float distance_pixel, distance_subpixel;
		getDistance(shot, points[0], points[1], sub_detectors[i], distance_pixel, distance_subpixel);

		cout << endl;
		cout << sub_detectors[i]->getName() << endl;
		cout << "distance pixel : " << distance_pixel << " pixel" << endl;
		cout << "distance subpixel : " << distance_subpixel << " pixel" << endl;
		cout << "distance gt : " << distance_gt << " pixel" << endl;

		ofs << endl;
		ofs << sub_detectors[i]->getName() << endl;
		ofs << "distance pixel : " << distance_pixel << " pixel" << endl;
		ofs << "distance subpixel : " << distance_subpixel << " pixel" << endl;
		ofs << "distance gt : " << distance_gt << " pixel" << endl;
	}
	ofs.close();
	cout << endl;
	cout << "Distance info saved to : " << distance_txt << endl;
}

void AccuracyTester::getDistance(
	Mat gray,
	Point p1, Point p2,
	SubDetectorBase* sub_detector,
	float& distance_pixel,
	float& distance_subpixel)
{
	Point2f sub_p1;
	float theta1;
	if (!sub_detector->Detect(gray, p1, sub_p1, theta1)){
		cerr << "AccuracyTester : sub_detector failed!" << endl;
		return;
	}
	Point2f sub_p2;
	float theta2;
	if (!sub_detector->Detect(gray, p2, sub_p2, theta2)){
		cerr << "AccuracyTester : sub_detector failed!" << endl;
		return;
	}
	distance_pixel = getDistance(p1, p2);
	distance_subpixel = getDistance(sub_p1, sub_p2);
}

float AccuracyTester::getDistance(Point2f p1, Point2f p2)
{
	float result = 0;
	result = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	result = sqrt(result);
	return result;
}
float AccuracyTester::getDistance(Point p1, Point p2)
{
	float result = 0;
	result = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	result = sqrt(result);
	return result;
}

Point AccuracyTester::GetNearestValidPoint(Mat gray, Point p)
{
	PixelEdgeDetector* detector = new PixelEdgeDetector();
	Mat bw;
	detector->Detect(gray, bw);
	double dis_min = 1e6;
	Point nearest;
	for (int r = 0; r < bw.rows; ++r){
		for (int c = 0; c < bw.cols; ++c){
			if (bw.at<unsigned char>(r, c) == 0)
				continue;
			double dis = (r - p.y)*(r - p.y) + (c - p.x)*(c - p.x);
			if (dis < dis_min){
				dis_min = dis;
				nearest.x = c;
				nearest.y = r;
			}
		}
	}
	return p;
}