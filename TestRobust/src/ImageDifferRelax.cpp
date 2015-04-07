#include "ImageDifferRelax.h"

void ImageDifferRelax::diff(
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

	const int radius = 1;
	for (int r = radius; r < rows - radius; ++r){
		for (int c = radius; c < cols - radius; ++c){
			if (gt.at<unsigned char>(r, c) != 0){
				info.pos_count++;
				bool flag = false;
				for (int rr = r - radius; rr <= r + radius; ++rr){
					for (int cc = c - radius; cc <= c + radius; ++cc){
						if (src.at<unsigned char>(rr, cc) != 0){
							flag = true;
							break;
						}
					}
				}
				if (!flag){//该是的不是——漏检
					dst.at<Vec3b>(r, c) = Vec3b(0, 255, 255);//yellow
					info.pos_error_count++;
				}
				else
					dst.at<Vec3b>(r, c) = Vec3b(255, 0, 0);//blue
			}
			else
				info.neg_count++;
		}
	}
	for (int r = radius; r < rows - radius; ++r){
		for (int c = radius; c < cols - radius; ++c){
			if (src.at<unsigned char>(r, c) != 0){//检查出来是边缘
				bool flag = false;
				for (int rr = r - radius; rr <= r + radius; ++rr){
					for (int cc = c - radius; cc <= c + radius; ++cc){
						if (gt.at<unsigned char>(rr, cc) != 0){
							flag = true;
							break;
						}
					}
				}
				if (!flag){//不该是的是——多检 ： 假阳性
					info.neg_error_count++;
					dst.at<Vec3b>(r, c) = Vec3b(0, 0, 255);//red
				}
				else{
					dst.at<Vec3b>(r, c) = Vec3b(255, 0, 0);//blue
				}
			}
		}
	}
}

