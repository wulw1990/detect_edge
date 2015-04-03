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
		std::vector<float>& thetas)=0; //���麯�����������ʵ�֣���������ͬ�����ܱ�ʵ����
};


#endif