#include "SubDetectorBlurModel.h"
#include <time.h>
#include "PixelEdgeDetector.h"

#define SHOW 0

const double SubDetectorBlurModel::PI = 3.14159265354;

void SubDetectorBlurModel::Detect(
	const cv::Mat src,
	std::vector<cv::Point>& pixel_edge_points,
	std::vector<cv::Point2f>& subpixel_edge_points,
	std::vector<float>& thetas)
{
	//check input and output
	assert(src.type() == CV_8UC1);
	pixel_edge_points.resize(0);
	subpixel_edge_points.resize(0);
	thetas.resize(0);

#if SHOW
	cv::cvtColor(src, img_show, CV_GRAY2BGR);
#endif
	const int a = 2;
	const int n_rows = src.rows;
	const int n_cols = src.cols;

	//-- step 1 : pixel_edge_detector--------------------
	PixelEdgeDetector * pixel_edge_detector = new PixelEdgeDetector();
	std::vector<cv::Point> pixel_edge_points_init;
	pixel_edge_detector->Detect(src, pixel_edge_points_init);

	//-- step 2 : calculate the first derivatives Ex, Ey, M(x,y)--------------------
	cv::Mat kernel_x = (cv::Mat_<short>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	cv::Mat kernel_y = (cv::Mat_<short>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

	cv::Mat gradient_x, gradient_y;
	clock_t start_time = clock();
	cv::filter2D(src, gradient_x, CV_16S, kernel_x);
	cv::filter2D(src, gradient_y, CV_16S, kernel_y);

	//-- step 3 : calculate the sx, sy, rho------------------------------------
	for (int p = 0, p_end = (int)pixel_edge_points_init.size(); p < p_end; ++p)
	{
		const int x = pixel_edge_points_init[p].x;
		const int y = pixel_edge_points_init[p].y;

		//check x, y
		if (x < a || x >= n_cols - a || y < a || y >= n_rows - a)
			continue;

		double theta = GetTheta(gradient_x.at<short>(y, x), gradient_y.at<short>(y, x));

		double bl, br, M0, M1, M2;
		double sx, sy, rho;
		std::vector<double> f;
		//-- calculate sx-------------------------------
		bl = gradient_x.at<short>(y, x - a); // bl=f(-a);
		br = gradient_x.at<short>(y, x + a); // br=f(+a);
		f.clear();
		for (int i = -a; i <= a; ++i)
			f.push_back(gradient_x.at<short>(y, x + i));
		M0 = cal_M(f, 0);
		M1 = cal_M(f, 1);
		M2 = cal_M(f, 2);
		sx = a*(bl + br)*M1 - M0*M1 + (bl - br)*M2;
		sx /= a*(bl + br)*M0 - M0*M0 + (bl - br)*M1;

		//-- calculate sy----------------------------------
		bl = gradient_y.at<short>(y - a, x); // bl=f(-a);
		br = gradient_y.at<short>(y + a, x); // br=f(+a);
		f.clear();
		for (int i = -a; i <= a; ++i)
			f.push_back(gradient_y.at<short>(y + i, x));
		M0 = cal_M(f, 0);
		M1 = cal_M(f, 1);
		M2 = cal_M(f, 2);
		sy = a*(bl + br)*M1 - M0*M1 + (bl - br)*M2;
		sy /= a*(bl + br)*M0 - M0*M0 + (bl - br)*M1;

		//-- calculate rho---------------------------
		rho = std::abs(sx*sy) / std::sqrt(sx*sx + sy*sy);

		//-- calculate the subpixel location
		double x_new = (double)x;
		double y_new = (double)y;
		const double PI = 3.14159265354;
		if (ThetaNearX(theta))
		{
			if (std::abs(sx)>2.0)
				sx = 0;
			x_new += sx;
		}
		else
		{
			if (std::abs(sy)>2.0)
				sy = 0;
			y_new += sy;
		}
		pixel_edge_points.push_back(cv::Point(x, y));
		subpixel_edge_points.push_back(cv::Point2f((float)x_new, (float)y_new));
		thetas.push_back((float)theta);
	}
#if SHOW
	cv::imwrite("./output/img.bmp", img_show);
	system("pause");
#endif
}