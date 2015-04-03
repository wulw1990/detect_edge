#include "SubDetectorSobelProjection.h"
#include "PixelEdgeDetector.h"
#define PRINT_INFO 0
//--------------------------------------------------------------------
void SubDetectorSobelProjection::Detect(
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

	//-- step 1 : pixel_edge_detector--------------------
	PixelEdgeDetector * pixel_edge_detector = new PixelEdgeDetector();
	std::vector<cv::Point> pixel_edge_points_init;
	pixel_edge_detector->Detect(src, pixel_edge_points_init);

	cv::Mat bw;
	cv::Canny(src, bw, 120, 100, 3);

	const int r = 2;
	for (int p = 0, p_end = (int)pixel_edge_points_init.size(); p < p_end; ++p)
	{
		const int col = pixel_edge_points_init[p].x;
		const int row = pixel_edge_points_init[p].y;

		//check x, y
		if (col < r || col >= src.cols - r || row < r || row >= src.rows - r)
			continue;

		double Gx, Gy, strength, theta;
		SobelPoint(src, cv::Point(col, row), &Gx, &Gy, &strength, &theta);

		const double PI = 3.1415926;
		double g_theta = theta + PI / 2.0;
		if ((g_theta >= 0.0 && g_theta <= PI / 8.0) || (g_theta >= 7.0*PI / 8.0 && g_theta <= PI))
		{
			double A, B, C;
			{
				double Gx_tmp, Gy_tmp, theta_tmp;
				SobelPoint(src, cv::Point(col - 1, row), &Gx_tmp, &Gy_tmp, &A, &theta_tmp);
				SobelPoint(src, cv::Point(col, row), &Gx_tmp, &Gy_tmp, &B, &theta_tmp);
				SobelPoint(src, cv::Point(col + 1, row), &Gx_tmp, &Gy_tmp, &C, &theta_tmp);
			}

			double a = (A + C - 2 * B) / 2.0;
			double b = (C - A) / 2.0;
			double c = B;

			double k = std::tan(g_theta);
			double l = -b / (2.0*a);
			//y=kx
			//(x,kx)*(l-x,0-kx) = l*x - x*x - k^2*x^2 = 0
			//x = l / (k^2 + 1)
			double offset_x = l / (k*k + 1);
			double offset_y = k*offset_x;

			pixel_edge_points.push_back(cv::Point( col, row));
			subpixel_edge_points.push_back(cv::Point2f(float(offset_x + col), float(offset_y + row)));
			//m_strength.push_back(strength);
			thetas.push_back((float)g_theta);
#if PRINT_INFO
			std::cout << (int)((g_theta-PI/2.0) / 3.14159 * 180) << "\t";
			//std::cout << a << "\t";
			//std::cout << b << "\t";
			//std::cout << c << "\t";
			//std::cout << k << "\t";
			std::cout << l << "\t";
			std::cout << offset_x << "\t";
			std::cout << offset_y << std::endl;
#endif
		}//if( (g_theta >= 0.0 && g_theta <= PI/8.0) || (g_theta >= 7.0*PI/8.0 && g_theta <= PI) )
		if (g_theta >= PI / 8.0 && g_theta <= 3.0*PI / 8.0)
		{
			double A, B, C;
			{
				double Gx_tmp, Gy_tmp, theta_tmp;
				SobelPoint(src, cv::Point(col - 1, row - 1), &Gx_tmp, &Gy_tmp, &A, &theta_tmp);
				SobelPoint(src, cv::Point(col, row), &Gx_tmp, &Gy_tmp, &B, &theta_tmp);
				SobelPoint(src, cv::Point(col + 1, row + 1), &Gx_tmp, &Gy_tmp, &C, &theta_tmp);
			}

			double a = (A + C - 2 * B) / 4.0;
			double b = (C - A) / 2.0*std::sqrt(2.0);
			double c = B;

			double k = std::tan(g_theta);
			double l = -b / (2.0*a);

			double offset_x = (l + k*l) / std::sqrt(2.0) / (k*k + 1);
			double offset_y = k*offset_x;

			pixel_edge_points.push_back(cv::Point(col, row));
			subpixel_edge_points.push_back(cv::Point2f(float(offset_x + col), float(offset_y + row)));
			//m_strength.push_back(strength);
			thetas.push_back((float)g_theta);
#if PRINT_INFO
			std::cout << (int)((g_theta-PI/2.0) / 3.14159 * 180) << "\t";
			//std::cout << a << "\t";
			//std::cout << b << "\t";
			//std::cout << c << "\t";
			//std::cout << k << "\t";
			std::cout << l << "\t";
			std::cout << offset_x << "\t";
			std::cout << offset_y << std::endl;
#endif
		}//if( g_theta >= PI/8.0 && g_theta <= 3.0*PI/8.0 )
		if (g_theta >= 3.0*PI / 8.0 && g_theta <= 5.0*PI / 8.0)
		{
			double A, B, C;
			{
				double Gx_tmp, Gy_tmp, theta_tmp;
				SobelPoint(src, cv::Point(col, row - 1), &Gx_tmp, &Gy_tmp, &A, &theta_tmp);
				SobelPoint(src, cv::Point(col, row), &Gx_tmp, &Gy_tmp, &B, &theta_tmp);
				SobelPoint(src, cv::Point(col, row + 1), &Gx_tmp, &Gy_tmp, &C, &theta_tmp);
			}

			double a = (A + C - 2 * B) / 2.0;
			double b = (C - A) / 2.0;
			double c = B;

			double k = std::tan(g_theta);
			double l = -b / (2.0*a);

			double offset_x = k*l / (k*k + 1);
			double offset_y = k*offset_x;

			pixel_edge_points.push_back(cv::Point( col, row));
			subpixel_edge_points.push_back(cv::Point2f(float(offset_x + col), float(offset_y + row)));
			//m_strength.push_back(strength);
			thetas.push_back((float)g_theta);
#if PRINT_INFO
			std::cout << (int)((g_theta-PI/2.0) / 3.14159 * 180) << "\t";
			//std::cout << a << "\t";
			//std::cout << b << "\t";
			//std::cout << c << "\t";
			//std::cout << k << "\t";
			std::cout << l << "\t";
			std::cout << offset_x << "\t";
			std::cout << offset_y << std::endl;
#endif
		}//if( g_theta >= 3.0*PI/8.0 && g_theta <= 5.0*PI/8.0 )

		if (g_theta >= 5.0*PI / 8.0 && g_theta <= 7.0*PI / 8.0)
		{
			double A, B, C;
			{
				double Gx_tmp, Gy_tmp, theta_tmp;
				SobelPoint(src, cv::Point(col - 1, row + 1), &Gx_tmp, &Gy_tmp, &A, &theta_tmp);
				SobelPoint(src, cv::Point(col, row), &Gx_tmp, &Gy_tmp, &B, &theta_tmp);
				SobelPoint(src, cv::Point(col + 1, row - 1), &Gx_tmp, &Gy_tmp, &C, &theta_tmp);
			}

			double a = (A + C - 2 * B) / 4.0;
			double b = (C - A) / 2.0*std::sqrt(2.0);
			double c = B;

			double k = std::tan(g_theta);
			double l = -b / (2.0*a);

			double offset_x = (l - k*l) / std::sqrt(2.0) / (k*k + 1);
			double offset_y = k*offset_x;

			pixel_edge_points.push_back(cv::Point( col, row));
			subpixel_edge_points.push_back(cv::Point2f(float(offset_x + col), float(offset_y + row)));
			//m_strength.push_back(strength);
			thetas.push_back((float)g_theta);
#if PRINT_INFO
			std::cout << (int)((g_theta-PI/2.0) / 3.14159 * 180) << "\t";
			//std::cout << a << "\t";
			//std::cout << b << "\t";
			//std::cout << c << "\t";
			//std::cout << k << "\t";
			std::cout << l << "\t";
			std::cout << offset_x << "\t";
			std::cout << offset_y << std::endl;
#endif
		}//if( g_theta >= 5.0*PI/8.0 && g_theta <= 7.0*PI/8.0 )
	}
}

SubDetectorSobelProjection::SubDetectorSobelProjection()
{
	mask_x_ = cv::Mat(3, 3, CV_64FC1);
	mask_x_.at<double>(0, 0) = 1;
	mask_x_.at<double>(0, 1) = 0;
	mask_x_.at<double>(0, 2) = -1;
	mask_x_.at<double>(1, 0) = 2;
	mask_x_.at<double>(1, 1) = 0;
	mask_x_.at<double>(1, 2) = -2;
	mask_x_.at<double>(2, 0) = 1;
	mask_x_.at<double>(2, 1) = 0;
	mask_x_.at<double>(2, 2) = -1;

	mask_y_ = cv::Mat(3, 3, CV_64FC1);
	mask_y_.at<double>(0, 0) = -1;
	mask_y_.at<double>(0, 1) = -2;
	mask_y_.at<double>(0, 2) = -1;
	mask_y_.at<double>(1, 0) = 0;
	mask_y_.at<double>(1, 1) = 0;
	mask_y_.at<double>(1, 2) = 0;
	mask_y_.at<double>(2, 0) = 1;
	mask_y_.at<double>(2, 1) = 2;
	mask_y_.at<double>(2, 2) = 1;
}

void SubDetectorSobelProjection::SobelPoint(cv::Mat img, cv::Point p, double* Gx, double* Gy,
	double* strength, double* theta)
{
	*Gx = 0;
	*Gy = 0;

	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			double I = img.at<unsigned char>(p.y - 1 + r, p.x - 1 + c);
			*Gx += mask_x_.at<double>(r, c) * I;
			*Gy += mask_y_.at<double>(r, c) * I;
		}
	}

	*strength = std::sqrt((*Gx)*(*Gx) + (*Gy)*(*Gy));
	if (std::abs(*Gy) < 1e-8)
		*Gy = 1e-8;
	*theta = std::atan((*Gx) / (*Gy));
}