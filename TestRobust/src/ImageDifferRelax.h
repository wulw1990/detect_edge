#ifndef _IMGAE_DIFFER_RELAX_H_
#define _IMGAE_DIFFER_RELEX_H_

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

#include "ImageDifferBase.h"

class ImageDifferRelax : public ImageDifferBase
{
public:
	ImageDifferRelax(string name){ method_name = name; }
	~ImageDifferRelax(){};

	void diff(
		Mat gt,
		Mat src,
		TestResultInfo& info,
		Mat& dst) override;

private:
};

#endif