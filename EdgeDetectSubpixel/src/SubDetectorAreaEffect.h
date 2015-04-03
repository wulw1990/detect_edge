#ifndef _SUB_DETECTOR_AREA_EFFECT_H_WULIWEI_BBNC_TSINGHUA_
#define _SUB_DETECTOR_AREA_EFFECT_H_WULIWEI_BBNC_TAINGHUA_

#include "opencv2/opencv.hpp"
#include <vector>
#include "SubDetectorBase.h"

class SubDetectorAreaEffect : public SubDetectorBase
{
public:
	void Detect(
		const cv::Mat src,
		std::vector<cv::Point>& pixel_edge_points,
		std::vector<cv::Point2f>& subpixel_edge_points,
		std::vector<float>& thetas);

private:
	void SobelPoint(cv::Mat img, cv::Point p, double* Gx, double* Gy,
				double* strength, double* theta);
};
#endif