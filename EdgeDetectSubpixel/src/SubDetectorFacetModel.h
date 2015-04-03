#ifndef _SUB_DETECTOR_FACET_MODEL_H_WULIWEI_BBNC_TSINGHUA_
#define _SUB_DETECTOR_FACET_MODEL_H_WULIWEI_BBNC_TSINGHUA_

#include "opencv2/opencv.hpp"
#include <vector>
#include "SubDetectorBase.h"

class SubDetectorFacetModel : public SubDetectorBase
{
public:
	void Detect(
		const cv::Mat src,
		std::vector<cv::Point>& pixel_edge_points,
		std::vector<cv::Point2f>& subpixel_edge_points,
		std::vector<float>& thetas);

	SubDetectorFacetModel();

private:
	std::vector<std::vector<float>> masks;

	//--read the mask of of facet model from txt
	std::vector<std::vector<float>> ReadMasksFromFile(std::string file_name);

	//--calculate the coefs of each facet
	std::vector<float> SolveCoefs(std::vector<float> &facet,
		std::vector<std::vector<float>> &masks);

	//--get sub pixel from coefs
	cv::Point2f GetSubPixelFromCoefs(std::vector<float>& coefs, int row, int col);
};
#endif