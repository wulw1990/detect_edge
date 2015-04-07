#ifndef _SUB_DETECTOR_H_
#define _SUB_DETECTOR_H_

#include "opencv2/opencv.hpp"

class SubDetectorBase
{
public:
	virtual void Detect(
		const cv::Mat src,
		std::vector<cv::Point>& pixel_edge_points,
		std::vector<cv::Point2f>& subpixel_edge_points,
		std::vector<float>& thetas)=0; //纯虚函数，子类必须实现

	/*
	* 输入：灰度图像， 像素精度的点
	* 输出：亚像素精度的点， 方向
	* return : 是否成功
	*/
	virtual bool Detect(
		const cv::Mat src,
		cv::Point pixel_edge_point,
		cv::Point2f& subpixel_edge_point,
		float& theta) = 0; //纯虚函数，子类必须实现

	std::string getName(){
		return m_method_name;
	}

protected:
	std::string m_method_name;
};


#endif