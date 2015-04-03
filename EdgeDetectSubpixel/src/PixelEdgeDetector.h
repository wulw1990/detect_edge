#ifndef _PIXEL_EDGE_DETECTOR_H_
#define _PIXEL_EDGE_DETECTOR_H_

#include "opencv2/opencv.hpp"

class PixelEdgeDetector
{
public:
	/*
	* input : 
	*	src -- gray image (CV_8UC1)
	* output : 
	*	points -- edge points
	*/
	void Detect(
		const cv::Mat& src,
		std::vector<cv::Point>& points);
	void Detect(
		const cv::Mat& src,
		cv::Mat& bw);

private:
	void ConnectDomainTwoPass(const cv::Mat& _binImg, cv::Mat& _labelImg);
	void ConnectDomainSeedFilling(const cv::Mat& _binImg, cv::Mat& _labelImg);
	cv::Mat ShowLabelResult(cv::Mat& _lableImg);
	void RemoveSmallConnectDomain(const cv::Mat& labelImg, cv::Mat cannyImg, int min_size);

};


#endif