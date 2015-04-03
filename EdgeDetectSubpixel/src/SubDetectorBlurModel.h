#ifndef _SUB_DETECTOR_BLUR_MODEL_H_WULIWEI_BBNC_TSINGHUA_
#define _SUB_DETECTOR_BLUR_MODEL_H_WULIWEI_BBNC_TSINGHUA_

#include "opencv2/opencv.hpp"
#include "SubDetectorBase.h"

class SubDetectorBlurModel : public SubDetectorBase
{
public:
	void Detect(
		const cv::Mat src, 
		std::vector<cv::Point>& pixel_edge_points,
		std::vector<cv::Point2f>& subpixel_edge_points, 
		std::vector<float>& thetas);

private:
	cv::Mat img_show;
	static const double PI;


	inline double GetTheta(double gx, double gy)
	{
		double theta = 0;
		if (std::abs(gx) < 1e-8)
		{
			if (gy > 0)
				theta = PI / 2;
			else
				theta = 3 * PI / 2;
		}
		else if (std::abs(gy) < 1e-8)
		{
			if (gx > 0)
				theta = 0;
			else
				theta = PI;
		}
		else
		{
			theta = std::atan(gy / gx);
			if (gx < 0 && gy < 0)//第三象限
				theta += PI;
			if (gx < 0 && gy > 0)//第二象限
				theta += PI;
			if (gx > 0 && gy < 0)//第四象限
				theta += 2 * PI;
		}
		return theta;
	}
	
	inline bool ThetaNearX(double theta)
	{
		return (theta >= 0 && theta <= PI / 4 || theta >= 7 * PI / 4 && theta <= 2 * PI
			|| theta >= 3 * PI / 4 && theta <= 5 * PI / 4);
	}



	inline double cal_M(std::vector<double> f, int p)
	{
		int a = (int)f.size() / 2;
		double Mp = 0;
		for (int j = -a; j < a; j++)
		{
			double fj = f[j + a];
			double fj1 = f[j + a + 1];
			Mp += (fj*std::pow(j, p) + fj1 * std::pow(j + 1, p)) / 2.0;
		}
		return Mp;
	}
};

#endif
