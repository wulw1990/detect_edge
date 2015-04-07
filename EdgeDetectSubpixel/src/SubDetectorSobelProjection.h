#ifndef _SUB_DETCTOR_SOBEL_PROJECTION_H_WILL_BBNC_TSINGHUA_
#define _SUB_DETCTOR_SOBEL_PROJECTION_H_WILL_BBNC_TSINGHUA_

#include "opencv2/opencv.hpp"
#include <vector>
#include "SubDetectorBase.h"

class SubDetectorSobelProjection : public SubDetectorBase
{
public:
	void Detect(
		const cv::Mat src,
		std::vector<cv::Point>& pixel_edge_points,
		std::vector<cv::Point2f>& subpixel_edge_points,
		std::vector<float>& thetas);

	bool Detect(
		const cv::Mat src,
		cv::Point pixel_edge_point,
		cv::Point2f& subpixel_edge_point,
		float& theta) override;

	SubDetectorSobelProjection();

private:
	cv::Mat mask_x_;
	cv::Mat mask_y_;
	void SobelPoint(cv::Mat img, cv::Point p, double* Gx, double* Gy,
		double* strength, double* theta);
};
#endif
