#ifndef _IMAGE_VIEWER_H_
#define _IMAGE_VIEWER_H_

#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class ImageViewer
{
public:
	ImageViewer(int win_width, int win_height, string win_name);
	void view(Mat img, string save_name, Mat& shot);//save_name--> .bmp
	
private:
	int m_win_width_max;
	int m_win_height_max;
	string m_win_name;
	Mat m_image;
	
	struct ViewInfo
	{
		Rect rect_in_image;//每次更新rect_in_image，用update_flag提醒主线程
		bool update_flag;
		double scale;//实际尺寸*scale == 显示尺寸，由show自动自己算无需手动修改
		Point p_down;//左键按下的坐标
	};
	ViewInfo m_view_info;

	void show();
	void setScale();

	static void onMouse(int event, int x, int y, int, void* data);
	static Rect transformRect(Rect src, Rect rect_in_win, double scale);

};

#endif