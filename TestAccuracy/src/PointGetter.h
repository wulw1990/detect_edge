#ifndef _POINT_GETTER_H_
#define _POINT_GETTER_H_

#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class PointGetter
{
public:
	PointGetter(int win_width, int win_height, string win_name);
	bool getPoints(Mat img, int n, vector<Point>& points);
	
private:
	int m_win_width_max;
	int m_win_height_max;
	string m_win_name;

	struct ViewInfo
	{
		vector<Point> p_vec;
		Point p_down;//左键按下的坐标
		bool update_flag;
	};
	ViewInfo m_view_info;

	double getScale(Size size);

	static void onMouse(int event, int x, int y, int, void* data);
};

#endif