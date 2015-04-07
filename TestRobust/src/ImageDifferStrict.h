#ifndef _IMGAE_DIFFER_STRICT_H_
#define _IMGAE_DIFFER_STRICT_H_

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

#include "ImageDifferBase.h"

class ImageDifferStrict : public ImageDifferBase
{
public:
	ImageDifferStrict(string name){ method_name = name; }
	~ImageDifferStrict(){};

	void diff(
		Mat gt,
		Mat src,
		TestResultInfo& info,
		Mat& dst) override;

private:
};

#endif