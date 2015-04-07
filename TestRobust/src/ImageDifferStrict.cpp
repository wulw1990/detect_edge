#include "ImageDifferStrict.h"

void ImageDifferStrict::diff(
	Mat gt,
	Mat src,
	TestResultInfo& info,
	Mat& dst)
{
	//check input output
	assert(gt.size() == src.size());
	assert(gt.type() == CV_8UC1);
	assert(src.type() == CV_8UC1);
	const int rows = src.rows;
	const int cols = src.cols;

	dst = Mat::zeros(src.size(), CV_8UC3);
	dst.setTo(Scalar(0, 0, 0));

	for (int r = 0; r < rows; ++r){
		for (int c = 0; c < cols; ++c){
			if (gt.at<unsigned char>(r, c) != 0){
				info.pos_count++;
				if (src.at<unsigned char>(r, c) == 0){//该是的不是——漏检
					dst.at<Vec3b>(r, c) = Vec3b(0, 255, 255);//yellow
					info.pos_error_count++;
				}
				else
					dst.at<Vec3b>(r, c) = Vec3b(255, 0, 0);//blue
			}
			else{
				info.neg_count++;
				if (src.at<unsigned char>(r, c) != 0){//不该是的是——多检
					info.neg_error_count++;
					dst.at<Vec3b>(r, c) = Vec3b(0, 0, 255);//red
				}
			}
		}
	}
	//imwrite("0.bmp", gt);
	//imwrite("1.bmp", src);
	//imwrite("2.bmp", dst);

}

