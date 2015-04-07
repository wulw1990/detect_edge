#include "PointGetter.h"
#include "../../EdgeDetectSubpixel/src/PixelEdgeDetector.h"


PointGetter::PointGetter(int win_width, int win_height, string win_name)
{
	m_win_width_max = win_width;
	m_win_height_max = win_height;
	m_win_name = win_name;
}

bool PointGetter::getPoints(Mat img, int n, vector<Point>& points)
{
	points.clear();
	destroyWindow(m_win_name);
	Mat img_show = img.clone();

	//canny
	PixelEdgeDetector* detector = new PixelEdgeDetector();
	detector->Detect(img, img_show);

	//resize to show
	double scale = getScale(img.size());
	Size size_show;
	size_show.width = (double)img_show.cols * scale;
	size_show.height = (double)img_show.rows * scale;
	resize(img_show, img_show, size_show);
	cvtColor(img_show, img_show, CV_GRAY2BGR);

	imshow(m_win_name, img_show);
	cv::setMouseCallback(m_win_name, onMouse, &m_view_info);
	m_view_info.update_flag = false;

	while (1)
	{
		if (m_view_info.update_flag){
			m_view_info.update_flag = false;
			for (int i = 0; i < (int)m_view_info.p_vec.size(); ++i){
				circle(img_show, m_view_info.p_vec[i], 5, Scalar(0, 0, 255), -1);
				imshow(m_win_name, img_show);
			}
			if (m_view_info.p_vec.size() == n)
				break;
		}
		char key = waitKey(30);
		if (key == 'q')
			break;
	}
	points.resize(m_view_info.p_vec.size());
	for (int i = 0; i < (int)m_view_info.p_vec.size(); ++i){
		points[i].x = (double)m_view_info.p_vec[i].x / scale;
		points[i].y = (double)m_view_info.p_vec[i].y / scale;
	}
	waitKey(1000);
	destroyWindow(m_win_name);
	return (m_view_info.p_vec.size() == n);
}

double PointGetter::getScale(Size size)
{
	double scale_x = (double)m_win_width_max / (double)size.width;
	double scale_y = (double)m_win_height_max / (double)size.height;
	return min(scale_x, scale_y);
}

void PointGetter::onMouse(int event, int x, int y, int, void* data)
{
	if (event != cv::EVENT_LBUTTONDOWN
		&& event != cv::EVENT_LBUTTONUP)
		//&& event != cv::EVENT_LBUTTONDBLCLK
		//&& event != cv::EVENT_RBUTTONDBLCLK)
		return;

	ViewInfo* info = (ViewInfo*)data;
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		info->p_down.x = x;
		info->p_down.y = y;
		break;
	case EVENT_LBUTTONUP:
		const int thresh = 3;
		if (x != info->p_down.x || y != info->p_down.y)
			break;
		else{
			info->p_vec.push_back(Point(x, y));
			info->update_flag = true;
		}
		break;
	}

}