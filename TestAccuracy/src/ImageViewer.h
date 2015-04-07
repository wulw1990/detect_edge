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
		Rect rect_in_image;//ÿ�θ���rect_in_image����update_flag�������߳�
		bool update_flag;
		double scale;//ʵ�ʳߴ�*scale == ��ʾ�ߴ磬��show�Զ��Լ��������ֶ��޸�
		Point p_down;//������µ�����
	};
	ViewInfo m_view_info;

	void show();
	void setScale();

	static void onMouse(int event, int x, int y, int, void* data);
	static Rect transformRect(Rect src, Rect rect_in_win, double scale);

};

#endif