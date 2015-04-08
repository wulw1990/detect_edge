#include "Visualiser.h"

cv::Mat Visualiser::DrawEdgeDirection(
	const cv::Mat& img,
	const std::vector<cv::Point2f>& points, 
	const std::vector<float>& thetas,
	int scale)
{
	const int n_points = (int)points.size();
	const int circle_radius = 1;
	const int line_length = 3;
	cv::Mat mat(img.rows * scale, img.cols * scale, CV_8UC3);
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			cv::Rect rect = cv::Rect(c*scale, r*scale, scale, scale);
			unsigned char gray = img.at<unsigned char>(r, c);
			cv::Scalar color(gray, gray, gray);
			mat(rect).setTo(color);
		}
	}
	//printf("copy ok\n");
	for (int p = 0; p < n_points; p++)
	{
		int x = int(points[p].x + 0.5);
		int y = int(points[p].y + 0.5);
		x = std::max(x, 0);
		x = std::min(x, img.cols - 1);
		y = std::max(y, 0);
		y = std::min(y, img.rows - 1);
		cv::Rect rect = cv::Rect(x*scale, y*scale, scale, scale);
		mat(rect).setTo(cv::Scalar(255, 0 , 0));
	}
	//printf("pixel ok\n");
	for (int p = 0; p < n_points; p++)
	{
		int x = int(points[p].x * scale + scale / 2);
		int y = int(points[p].y * scale + scale / 2);
		int x2 = x + int(3.0 * std::cos(thetas[p]));
		int y2 = y + int(3.0 * std::sin(thetas[p]));

		cv::line(mat, cv::Point(x, y), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 1);
		cv::circle(mat, cv::Point(x, y), circle_radius, cv::Scalar(0, 0, 255), -1);
	}
	return mat;
}

void Visualiser::SaveEdgeDirection(
	const cv::Mat& img,
	const std::vector<cv::Point2f>& points, 
	const std::vector<float>& thetas,
	int scale, const std::string& output_name)
{
	const float PI_2 = 3.1415926f / 2.0f;
	const int n_points = (int)thetas.size();
	std::vector<float> thetas_grad(n_points);
	
	for (int i = 0; i < n_points; i++)
		thetas_grad[i] = thetas[i] + PI_2;
	SaveGradDirection(img, points, thetas_grad, scale, output_name);
}


void Visualiser::SaveGradDirection(
	const cv::Mat& img,
	const std::vector<cv::Point2f>& points, 
	const std::vector<float>& thetas,
	int scale, const std::string& output_name)
{
	cv::Mat mat = DrawEdgeDirection(img, points, thetas, scale);
	cv::imwrite(output_name.c_str(), mat);
}

void Visualiser::SavePixelEdge(
	const cv::Mat& img,
	const std::vector<cv::Point>& points,
	const std::string& output_name)
{
	//check input, output
	assert(img.type() == CV_8UC3 || img.type() == CV_8UC1);
	cv::Mat img_show;
	if (img.type() == CV_8UC3)
		img_show = img.clone();
	else
		cv::cvtColor(img, img_show, cv::COLOR_GRAY2BGR);

	//do something
	for (int p = 0, p_end = (int)points.size(); p < p_end; ++p)
		if (points[p].x >= 0 && points[p].x < img.cols && points[p].y >= 0 && points[p].y < img.rows)
			img_show.at<cv::Vec3b>(points[p].y, points[p].x) = cv::Vec3b(0, 0, 255);

	//output
	cv::imwrite(output_name.c_str(), img_show);
}