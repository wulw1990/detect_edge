#include "ImageViewer.h"

ImageViewer::ImageViewer(int win_width, int win_height, string win_name)
{
	m_win_width_max = win_width;
	m_win_height_max = win_height;
	m_win_name = win_name;
}

void ImageViewer::view(Mat img, string save_name, Mat& shot)
{
	assert(!img.empty());
	assert(img.type() == CV_8UC1);
	m_image = img.clone();
	destroyWindow(m_win_name);

	m_view_info.rect_in_image = Rect(0, 0, img.cols, img.rows);
	m_view_info.update_flag = true;
	int count = 0;
	while (1)
	{
		if (m_view_info.update_flag){
			m_view_info.update_flag = false;
			show();
		}
		char key = waitKey(30);
		if ((int)key == 13){
			shot = m_image(m_view_info.rect_in_image).clone();
			imwrite(save_name, shot);
			cout << "½ØÍ¼±£´æÖÁ : " << save_name << endl;
			break;
		}
	}
	destroyWindow(m_win_name);
	cout << "End of View" << endl;
}

void ImageViewer::setScale()
{
	double scale_x = (double)m_win_width_max / (double)m_view_info.rect_in_image.width;
	double scale_y = (double)m_win_height_max / (double)m_view_info.rect_in_image.height;
	m_view_info.scale = min(scale_x, scale_y);
}

void ImageViewer::show()
{
	setScale();
	//cout << "scale : " << m_view_info.scale << endl;
	Size win_size;
	win_size.width = (int)((double)m_view_info.rect_in_image.width * m_view_info.scale);
	win_size.height = (int)((double)m_view_info.rect_in_image.height * m_view_info.scale);
	
	Mat tmp = m_image(m_view_info.rect_in_image).clone();
	resize(tmp, tmp, win_size, 0, 0, INTER_NEAREST);
	imshow(m_win_name, tmp);
	cv::setMouseCallback(m_win_name, onMouse, &m_view_info);
	waitKey(1);
}

void ImageViewer::onMouse(int event, int x, int y, int, void* data)
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
		//cout << "L DOWN" << endl;
		info->p_down.x = x;
		info->p_down.y = y;
		break;
	case EVENT_LBUTTONUP:
		//cout << "L UP" << endl;
		const int thresh = 3;
		if (x < info->p_down.x || y < info->p_down.y)
			break;

		if (abs(x - info->p_down.x) < thresh && abs(y - info->p_down.y) < thresh){//µ¥»÷
			;
		}
		else{
			//printf("x1=%d, y1=%d, x2=%d, y2=%d\n", info->p_down.x, info->p_down.y, x, y);
			Rect rect_in_win(info->p_down.x, info->p_down.y, x - info->p_down.x + 1, y - info->p_down.y + 1);
			//cout << info->rect_in_image << endl;
			info->rect_in_image = transformRect(info->rect_in_image, rect_in_win, info->scale);
			//cout << info->rect_in_image << endl;
			info->update_flag = true;
		}
		break;
	}

}

Rect ImageViewer::transformRect(Rect src, Rect rect_in_win, double scale)
{
	Rect dst;
	dst.x = src.x + rect_in_win.x / scale;
	dst.y = src.y + rect_in_win.y / scale;
	dst.width = rect_in_win.width / scale;
	dst.height = rect_in_win.height / scale;
	return dst;
}