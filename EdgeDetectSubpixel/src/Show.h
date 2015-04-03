#ifndef _SHOW_SUB_PIXEL_WILL_TSINGHUA_
#define _SHOW_SUB_PIXEL_WILL_TSINGHUA_

#include "opencv2/opencv.hpp"
#include <vector>
#include <string>

namespace Show
{
	cv::Mat DrawEdgeDirection(
		const cv::Mat& img,
		const std::vector<cv::Point2f>& points, const std::vector<float>& thetas,
		int scale);

	void SaveEdgeDirection(
		const cv::Mat& img, 
		const std::vector<cv::Point2f>& points, const std::vector<float>& thetas, 
		int scale, const std::string& output_name);

	void SaveGradDirection(
		const cv::Mat& img,
		const std::vector<cv::Point2f>& points, const std::vector<float>& thetas,
		int scale, const std::string& output_name);

	void SavePixelEdge(
		const cv::Mat& img,
		const std::vector<cv::Point>& points,
		const std::string& output_name);
}


#endif