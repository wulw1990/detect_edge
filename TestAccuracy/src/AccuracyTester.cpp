#include  "AccuracyTester.h"

#include <fstream>
#include "../../EdgeDetectSubpixel/src/PixelEdgeDetector.h"
#include "../../EdgeDetectSubpixel/src/system_dependent/FileDealer.h"
#include "ImageViewer.h"
#include "PointGetter.h"
#include "Visualiser.h"

void AccuracyTester::test(Mat gray, vector<SubDetectorBase*> sub_detectors, string output_path)
{
	//check input, output
	assert(gray.type() == CV_8UC1);
	assert(!gray.empty());
	size_t location = output_path.find_last_of("\\");
	if (location == std::string::npos)
		location = output_path.find_last_of("/");
	if (location != std::string::npos)
		FileDealer::CreateDirectoryRecursive(output_path.substr(0, location));

	const string full_name_shot(output_path + "shot.bmp");
	const string full_name_txt(output_path + "result.txt");
	const string full_name_points(output_path + "two_points.bmp");
	const string full_name_pixel(output_path + "pixel.bmp");
	const string full_prefix_subpixel(output_path + "subpixel_");

	//take roi
	Mat shot;
	ImageViewer* viewer = new ImageViewer(1000, 700, "ImageViewer");
	viewer->view(gray, full_name_shot, shot);

	//get 2 points
	PointGetter* point_getter = new PointGetter(1000, 700, "PointGetter");
	vector<Point> points;
	if (!point_getter->getPoints(shot, 2, points))
	{
		cerr << "point_getter quit by user" << endl;
		system("pause");
		return;
	}

	//measure the distance between 2 points, save result to disk
	ofstream fout(full_name_txt);

	points[0] = GetNearestValidPoint(shot, points[0]);
	points[1] = GetNearestValidPoint(shot, points[1]);

	cout << endl;
	fout << endl;
	cout << "实际取得边缘点是：" << points[0] << "\t" << points[1] << endl;
	fout << "实际取得边缘点是：" << points[0] << "\t" << points[1] << endl;

	float distance_pixel = getDistance(points[0], points[1]);
	cout << endl;
	fout << endl;
	cout << "测试结果如下：" << endl;
	fout << "测试结果如下：" << endl;
	cout << "像素精度距离 (单位像素) : " << endl;
	fout << "像素精度距离 (单位像素) : " << endl;
	cout << distance_pixel << endl;
	fout << distance_pixel << endl;

	savePixelEdgeResult(shot, full_name_pixel);
	saveTwoPoints(shot, points[0], points[1], full_name_points);

	for (int i = 0; i < sub_detectors.size(); ++i){
		float distance_subpixel = getDistance(shot, points[0], points[1], sub_detectors[i]);

		cout << endl;
		fout << endl;
		cout << sub_detectors[i]->getName() << ":" << endl;
		fout << sub_detectors[i]->getName() << ":" << endl;
		cout << distance_subpixel  << endl;
		fout << distance_subpixel << endl;

		saveSubPixelEdgeResult(shot, sub_detectors[i], full_prefix_subpixel + sub_detectors[i]->getName() + ".bmp");
	}
	fout.close();
	cout << endl;
	cout << "输出目录：" << output_path << endl;
}

float AccuracyTester::getDistance(
	Mat gray,
	Point p1, Point p2,
	SubDetectorBase* sub_detector)
{
	Point2f sub_p1;
	float theta1;
	if (!sub_detector->Detect(gray, p1, sub_p1, theta1)){
		cerr << "AccuracyTester : sub_detector failed!" << endl;
		system("pause");
		exit(-1);
	}
	Point2f sub_p2;
	float theta2;
	if (!sub_detector->Detect(gray, p2, sub_p2, theta2)){
		cerr << "AccuracyTester : sub_detector failed!" << endl;
		system("pause");
		exit(-1);
	}
	return getDistance(sub_p1, sub_p2);
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
	return nearest;
}

void AccuracyTester::saveSubPixelEdgeResult(Mat gray, SubDetectorBase* sub_detector, string name)
{
	std::vector<cv::Point> points;
	std::vector<cv::Point2f> subpixel_points;
	std::vector<float> thetas;
	sub_detector->Detect(gray, points, subpixel_points, thetas);

	Visualiser* visualiser = new Visualiser();
	const int scale = 5;
	visualiser->SaveEdgeDirection(gray, subpixel_points, thetas, scale, name);
}
void AccuracyTester::saveTwoPoints(Mat gray, Point2f p1, Point2f p2, string name)
{
	PixelEdgeDetector* detector = new PixelEdgeDetector();
	Mat bw;
	detector->Detect(gray, bw);
	cvtColor(bw, bw, CV_GRAY2BGR);
	bw.at<Vec3b>(p1.y, p1.x) = Vec3b(0, 0, 255);
	bw.at<Vec3b>(p2.y, p2.x) = Vec3b(0, 0, 255);
	imwrite(name, bw);
}

void AccuracyTester::savePixelEdgeResult(Mat gray, string name)
{
	PixelEdgeDetector* detector = new PixelEdgeDetector();
	Mat bw;
	detector->Detect(gray, bw);
	imwrite(name, bw);
}