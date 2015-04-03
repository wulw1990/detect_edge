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
		std::vector<float>& thetas)=0; //纯虚函数，子类必须实现，否则子类同样不能被实例化
};


#endif