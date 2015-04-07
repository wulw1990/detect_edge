#include "SubDetectorAreaEffect.h"
#include "PixelEdgeDetector.h"

#define PRINT_INFO 0

void SubDetectorAreaEffect::Detect(
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
		if (theta >= 0.0 && theta <= PI / 4.0)
		{
			double A = 0;
			double B = 0;
			A += src.at<unsigned char>(row - 2, col);
			A += src.at<unsigned char>(row - 2, col + 1);
			A += src.at<unsigned char>(row - 1, col + 1);
			A /= 3.0;
			B += src.at<unsigned char>(row + 1, col - 1);
			B += src.at<unsigned char>(row + 2, col - 1);
			B += src.at<unsigned char>(row + 2, col);
			B /= 3.0;

			double SL = 0;
			double SM = 0;
			double SR = 0;
			for (int r = row - 2; r <= row + 2; r++)
			{
				SL += src.at<unsigned char>(r, col - 1);
				SM += src.at<unsigned char>(r, col);
				SR += src.at<unsigned char>(r, col + 1);
			}
			double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
			double b = (SR - SL) / (2 * (A - B));
			double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

			double offset_x = 0.0;
			double offset_y = a;

			double new_theta = std::atan(b / 1.0) + PI / 2;

			pixel_edge_points.push_back(cv::Point(col, row));
			subpixel_edge_points.push_back(cv::Point2f((float)(offset_x + col), (float)(offset_y + row)));
			//m_strength.push_back(strength);
			thetas.push_back((float)new_theta);
#if PRINT_INFO
			std::cout <<"1 "<< (int)(new_theta / 3.14159 * 180) << std::endl;
#endif
		}//if( g_theta >= 0.0 && g_theta <= PI/4.0 )
		else if (theta >= -PI / 4.0 && theta <= -0.0)
		{
			double A = 0;
			double B = 0;
			A += src.at<unsigned char>(row - 2, col - 1);
			A += src.at<unsigned char>(row - 1, col - 1);
			A += src.at<unsigned char>(row - 2, col);
			A /= 3.0;
			B += src.at<unsigned char>(row + 1, col + 1);
			B += src.at<unsigned char>(row + 2, col + 1);
			B += src.at<unsigned char>(row + 2, col);
			B /= 3.0;

			double SL = 0;
			double SM = 0;
			double SR = 0;
			for (int r = row - 2; r <= row + 2; r++)
			{
				SL += src.at<unsigned char>(r, col - 1);
				SM += src.at<unsigned char>(r, col);
				SR += src.at<unsigned char>(r, col + 1);
			}
			double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
			double b = (SR - SL) / (2 * (A - B));
			double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

			double offset_x = 0.0;
			double offset_y = a;

			double new_theta = std::atan(b / 1.0) + PI / 2;

			pixel_edge_points.push_back(cv::Point(col, row));
			subpixel_edge_points.push_back(cv::Point2f((float)(offset_x + col), (float)(offset_y + row)));
			//_m_strength.push_back(strength);
			thetas.push_back((float)new_theta);
#if PRINT_INFO
			std::cout <<"4 "<<(int)(new_theta / 3.14159 * 180) << std::endl;
#endif
		}//if( theta >= 3.0*PI/4.0 && theta <= PI )
		else if (theta >= PI / 4.0 && theta <= 2.0*PI / 4.0)
		{
			double A = 0;
			double B = 0;
			A += src.at<unsigned char>(row, col - 2);
			A += src.at<unsigned char>(row + 1, col - 1);
			A += src.at<unsigned char>(row + 1, col - 2);
			A /= 3.0;
			B += src.at<unsigned char>(row - 1, col + 1);
			B += src.at<unsigned char>(row - 1, col + 2);
			B += src.at<unsigned char>(row, col + 2);
			B /= 3.0;

			double SL = 0;
			double SM = 0;
			double SR = 0;
			for (int c = col - 2; c <= col + 2; c++)
			{
				SL += src.at<unsigned char>(row - 1, c);
				SM += src.at<unsigned char>(row, c);
				SR += src.at<unsigned char>(row + 1, c);
			}
			double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
			double b = (SR - SL) / (2 * (A - B));
			double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

			double offset_x = a;
			double offset_y = 0.0;

			if (std::abs(b) < 1e-8)
				b = 1e-8;
			double new_theta = std::atan(1.0 / b) + PI / 2;

			pixel_edge_points.push_back(cv::Point(col, row));
			subpixel_edge_points.push_back(cv::Point2f((float)(offset_x + col), (float)(offset_y + row)));
			//m_strength.push_back(strength);
			thetas.push_back((float)new_theta);
#if PRINT_INFO
			std::cout <<"2 "<< (int)(new_theta / 3.14159 * 180) << std::endl;
#endif
		}//if( theta >= PI/4.0 && theta <= 2.0*PI/4.0 )
		else if (theta >= -2.0*PI / 4.0 && theta < -PI / 4.0)
		{
			double A = 0;
			double B = 0;
			A += src.at<unsigned char>(row - 1, col - 2);
			A += src.at<unsigned char>(row - 1, col - 1);
			A += src.at<unsigned char>(row, col - 2);
			A /= 3.0;
			B += src.at<unsigned char>(row + 1, col + 1);
			B += src.at<unsigned char>(row + 1, col + 2);
			B += src.at<unsigned char>(row, col + 2);
			B /= 3.0;

			double SL = 0;
			double SM = 0;
			double SR = 0;
			for (int c = col - 2; c <= col + 2; c++)
			{
				SL += src.at<unsigned char>(row - 1, c);
				SM += src.at<unsigned char>(row, c);
				SR += src.at<unsigned char>(row + 1, c);
			}
			double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
			double b = (SR - SL) / (2 * (A - B));
			double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

			double offset_x = a;
			double offset_y = 0.0;

			if (std::abs(b) < 1e-8)
				b = 1e-8;
			double new_theta = std::atan(1.0 / b) + PI / 2;

			pixel_edge_points.push_back(cv::Point(col, row));
			subpixel_edge_points.push_back(cv::Point2f((float)(offset_x + col), (float)(offset_y + row)));
			//m_strength.push_back(strength);
			thetas.push_back((float)new_theta);
#if PRINT_INFO
			std::cout <<"3 "<< (int)(new_theta / 3.14159 * 180) << std::endl;
#endif
		}//if( theta >= 2.0*PI/4.0 && theta <= 3.0*PI/4.0 )
		else
			;
#if PRINT_INFO
		std::cout << "---" <<(int)(theta / 3.14159 * 180) << std::endl;
#endif
	}

}

//---------------------------------------------------------------------------
void SubDetectorAreaEffect::SobelPoint(cv::Mat img, cv::Point p, double* Gx, double* Gy,
	double* strength, double* theta)
{
	cv::Mat sobel_maskx = cv::Mat(3, 3, CV_64FC1);
	sobel_maskx.at<double>(0, 0) = 1;
	sobel_maskx.at<double>(0, 1) = 0;
	sobel_maskx.at<double>(0, 2) = -1;
	sobel_maskx.at<double>(1, 0) = 2;
	sobel_maskx.at<double>(1, 1) = 0;
	sobel_maskx.at<double>(1, 2) = -2;
	sobel_maskx.at<double>(2, 0) = 1;
	sobel_maskx.at<double>(2, 1) = 0;
	sobel_maskx.at<double>(2, 2) = -1;

	cv::Mat sobel_masky = cv::Mat(3, 3, CV_64FC1);
	sobel_masky.at<double>(0, 0) = -1;
	sobel_masky.at<double>(0, 1) = -2;
	sobel_masky.at<double>(0, 2) = -1;
	sobel_masky.at<double>(1, 0) = 0;
	sobel_masky.at<double>(1, 1) = 0;
	sobel_masky.at<double>(1, 2) = 0;
	sobel_masky.at<double>(2, 0) = 1;
	sobel_masky.at<double>(2, 1) = 2;

	*Gx = 0;
	*Gy = 0;

	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			double I = img.at<unsigned char>(p.y - 1 + r, p.x - 1 + c);
			*Gx += sobel_maskx.at<double>(r, c) * I;
			*Gy += sobel_masky.at<double>(r, c) * I;
		}
	}

	*strength = std::sqrt((*Gx)*(*Gx) + (*Gy)*(*Gy));
	if (std::abs(*Gy) < 1e-8)
		*Gy = 1e-8;
	*theta = std::atan((*Gx) / (*Gy));
}

bool SubDetectorAreaEffect::Detect(
	const cv::Mat src,
	cv::Point pixel_edge_point,
	cv::Point2f& subpixel_edge_point,
	float& theta_output)
{
	//-- step 1 : pixel_edge_detector--------------------
	//no need (input : pixel_edge_point)

	const int r = 2;


	const int col = pixel_edge_point.x;
	const int row = pixel_edge_point.y;

	//check x, y
	if (col < r || col >= src.cols - r || row < r || row >= src.rows - r)
		return false;

	double Gx, Gy, strength, theta;
	SobelPoint(src, cv::Point(col, row), &Gx, &Gy, &strength, &theta);

	const double PI = 3.1415926;
	if (theta >= 0.0 && theta <= PI / 4.0)
	{
		double A = 0;
		double B = 0;
		A += src.at<unsigned char>(row - 2, col);
		A += src.at<unsigned char>(row - 2, col + 1);
		A += src.at<unsigned char>(row - 1, col + 1);
		A /= 3.0;
		B += src.at<unsigned char>(row + 1, col - 1);
		B += src.at<unsigned char>(row + 2, col - 1);
		B += src.at<unsigned char>(row + 2, col);
		B /= 3.0;

		double SL = 0;
		double SM = 0;
		double SR = 0;
		for (int r = row - 2; r <= row + 2; r++)
		{
			SL += src.at<unsigned char>(r, col - 1);
			SM += src.at<unsigned char>(r, col);
			SR += src.at<unsigned char>(r, col + 1);
		}
		double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
		double b = (SR - SL) / (2 * (A - B));
		double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

		double offset_x = 0.0;
		double offset_y = a;

		double new_theta = std::atan(b / 1.0) + PI / 2;

		subpixel_edge_point = cv::Point2f((float)(offset_x + col), (float)(offset_y + row));
		theta_output = (float)new_theta;
#if PRINT_INFO
		std::cout << "1 " << (int)(new_theta / 3.14159 * 180) << std::endl;
#endif
	}//if( g_theta >= 0.0 && g_theta <= PI/4.0 )
	else if (theta >= -PI / 4.0 && theta <= -0.0)
	{
		double A = 0;
		double B = 0;
		A += src.at<unsigned char>(row - 2, col - 1);
		A += src.at<unsigned char>(row - 1, col - 1);
		A += src.at<unsigned char>(row - 2, col);
		A /= 3.0;
		B += src.at<unsigned char>(row + 1, col + 1);
		B += src.at<unsigned char>(row + 2, col + 1);
		B += src.at<unsigned char>(row + 2, col);
		B /= 3.0;

		double SL = 0;
		double SM = 0;
		double SR = 0;
		for (int r = row - 2; r <= row + 2; r++)
		{
			SL += src.at<unsigned char>(r, col - 1);
			SM += src.at<unsigned char>(r, col);
			SR += src.at<unsigned char>(r, col + 1);
		}
		double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
		double b = (SR - SL) / (2 * (A - B));
		double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

		double offset_x = 0.0;
		double offset_y = a;

		double new_theta = std::atan(b / 1.0) + PI / 2;

		subpixel_edge_point = cv::Point2f((float)(offset_x + col), (float)(offset_y + row));
		theta_output = (float)new_theta;
#if PRINT_INFO
		std::cout << "4 " << (int)(new_theta / 3.14159 * 180) << std::endl;
#endif
	}//if( theta >= 3.0*PI/4.0 && theta <= PI )
	else if (theta >= PI / 4.0 && theta <= 2.0*PI / 4.0)
	{
		double A = 0;
		double B = 0;
		A += src.at<unsigned char>(row, col - 2);
		A += src.at<unsigned char>(row + 1, col - 1);
		A += src.at<unsigned char>(row + 1, col - 2);
		A /= 3.0;
		B += src.at<unsigned char>(row - 1, col + 1);
		B += src.at<unsigned char>(row - 1, col + 2);
		B += src.at<unsigned char>(row, col + 2);
		B /= 3.0;

		double SL = 0;
		double SM = 0;
		double SR = 0;
		for (int c = col - 2; c <= col + 2; c++)
		{
			SL += src.at<unsigned char>(row - 1, c);
			SM += src.at<unsigned char>(row, c);
			SR += src.at<unsigned char>(row + 1, c);
		}
		double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
		double b = (SR - SL) / (2 * (A - B));
		double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

		double offset_x = a;
		double offset_y = 0.0;

		if (std::abs(b) < 1e-8)
			b = 1e-8;
		double new_theta = std::atan(1.0 / b) + PI / 2;

		subpixel_edge_point = cv::Point2f((float)(offset_x + col), (float)(offset_y + row));
		theta_output = (float)new_theta;
#if PRINT_INFO
		std::cout << "2 " << (int)(new_theta / 3.14159 * 180) << std::endl;
#endif
	}//if( theta >= PI/4.0 && theta <= 2.0*PI/4.0 )
	else if (theta >= -2.0*PI / 4.0 && theta < -PI / 4.0)
	{
		double A = 0;
		double B = 0;
		A += src.at<unsigned char>(row - 1, col - 2);
		A += src.at<unsigned char>(row - 1, col - 1);
		A += src.at<unsigned char>(row, col - 2);
		A /= 3.0;
		B += src.at<unsigned char>(row + 1, col + 1);
		B += src.at<unsigned char>(row + 1, col + 2);
		B += src.at<unsigned char>(row, col + 2);
		B /= 3.0;

		double SL = 0;
		double SM = 0;
		double SR = 0;
		for (int c = col - 2; c <= col + 2; c++)
		{
			SL += src.at<unsigned char>(row - 1, c);
			SM += src.at<unsigned char>(row, c);
			SR += src.at<unsigned char>(row + 1, c);
		}
		double c = (SL + SR - 2.0*SM) / (2.0*(A - B));
		double b = (SR - SL) / (2 * (A - B));
		double a = (2.0*SM - 5 * (A + B)) / (2.0*(A - B)) - 1.0 / 12.0*c;

		double offset_x = a;
		double offset_y = 0.0;

		if (std::abs(b) < 1e-8)
			b = 1e-8;
		double new_theta = std::atan(1.0 / b) + PI / 2;

		subpixel_edge_point = cv::Point2f((float)(offset_x + col), (float)(offset_y + row));
		theta_output = (float)new_theta;
#if PRINT_INFO
		std::cout << "3 " << (int)(new_theta / 3.14159 * 180) << std::endl;
#endif
	}//if( theta >= 2.0*PI/4.0 && theta <= 3.0*PI/4.0 )
	else
		;

	return true;
}