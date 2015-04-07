#ifndef _IMGAE_DIFFER_BASE_H_
#define _IMGAE_DIFFER_BASE_H_

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

class ImageDifferBase
{
public:
	struct TestResultInfo
	{
		int pos_count;
		int pos_error_count;
		int neg_count;
		int neg_error_count;

		TestResultInfo(){ Reset(); }
		void Reset()
		{
			pos_count = 0;
			pos_error_count = 0;
			neg_count = 0;
			neg_error_count = 0;
		}
	};
	ImageDifferBase(){};
	virtual ~ImageDifferBase(){};
	string GetName(){ return method_name; }

	virtual void diff(
		Mat gt, 
		Mat src,
		TestResultInfo& info,
		Mat& dst) = 0;

protected:
	string method_name;
};

#endif