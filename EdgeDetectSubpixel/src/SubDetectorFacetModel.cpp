#include "SubDetectorFacetModel.h"

#include "PixelEdgeDetector.h"
#include <iostream>
#include <fstream>
#include "SubDetectorFacetModel.model.h"

//----------------------------------------------------------------------------
void SubDetectorFacetModel::Detect(
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

	//check masks(model)
	if ((int)masks.size() == 0)
	{
		std::cerr << "model does not exists, FacetModelDetector failed!" << std::endl;
		return;
	}

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

		std::vector<float> facet;
		for (int i = row - 2; i <= row + 2; i++)
		{
			for (int j = col - 2; j <= col + 2; j++)
			{
				facet.push_back(src.at<unsigned char>(i, j));
			}
		}
		std::vector<float> coefs = SolveCoefs(facet, masks);

		pixel_edge_points.push_back(cv::Point(col, row));
		subpixel_edge_points.push_back(GetSubPixelFromCoefs(coefs, row, col));
		float g = std::sqrt(coefs[1] * coefs[1] + coefs[2] * coefs[2]);
		//m_strenth.push_back(g);
		float sin_theta = coefs[1] / g;
		float cos_theta = coefs[2] / g;
		//m_sin_theta.push_back(coefs[1] / g);
		//m_cos_theta.push_back(coefs[2] / g);
		float theta = std::atan(sin_theta / cos_theta);
		thetas.push_back(theta);

	}

}

SubDetectorFacetModel::SubDetectorFacetModel()
{
	masks = g_GetMasks();
	m_method_name = "FacetModel";
}
//----------------------------------------------------------------------------
std::vector<std::vector<float>> SubDetectorFacetModel::ReadMasksFromFile(std::string file_name)
{
	std::vector<std::vector<float>> masks;
	std::ifstream ifs(file_name.c_str());
	if (!ifs.is_open())
	{
		std::cerr << file_name << " can not open!" << std::endl;
		system("pause");
		exit(-1);
	}
	for (int i = 0; i < 10; i++)
	{
		float numerator, denominator, tmp;
		ifs >> tmp;
		ifs >> numerator;
		ifs >> denominator;

		std::vector<float> mask;
		for (int j = 0; j < 25; j++)
		{
			ifs >> tmp;
			mask.push_back(tmp * numerator / denominator);
		}
		masks.push_back(mask);
	}
	ifs.close();
	return masks;
}
//---------------------------------------------------------------------------
std::vector<float> SubDetectorFacetModel::SolveCoefs(std::vector<float> &facet,
	std::vector<std::vector<float>> &masks)
{
	std::vector<float> coefs;
	for (int i = 0; i < 10; i++)
	{
		float sum = 0;
		for (int j = 0; j < 25; j++)
		{
			sum += facet[j] * masks[i][j];
		}
		coefs.push_back(sum);
	}
	return coefs;
}
//---------------------------------------------------------------------------
cv::Point2f SubDetectorFacetModel::GetSubPixelFromCoefs(std::vector<float>& coefs, int row, int col)
{
	float sin_theta = coefs[2] / std::sqrt(coefs[1] * coefs[1] + coefs[2] * coefs[2]);
	float cos_theta = coefs[1] / std::sqrt(coefs[1] * coefs[1] + coefs[2] * coefs[2]);

	float A = coefs[9] * sin_theta*sin_theta*sin_theta +
		coefs[8] * sin_theta*sin_theta*cos_theta +
		coefs[7] * sin_theta*cos_theta*cos_theta +
		coefs[6] * cos_theta*cos_theta*cos_theta;
	A *= 6;
	float B = coefs[5] * sin_theta*sin_theta +
		coefs[4] * sin_theta*cos_theta +
		coefs[3] * cos_theta*cos_theta;
	B *= 2;
	float rho = -B / A;

	float x = rho * cos_theta;
	float y = rho * sin_theta;

	return cv::Point2f(x + (float)col, -y + (float)row);
}

bool SubDetectorFacetModel::Detect(
	const cv::Mat src,
	cv::Point pixel_edge_point,
	cv::Point2f& subpixel_edge_point,
	float& theta)
{
	//check masks(model)
	if ((int)masks.size() == 0)
	{
		std::cerr << "model does not exists, FacetModelDetector failed!" << std::endl;
		return false;
	}

	//-- step 1 : pixel_edge_detector--------------------
	//no need (input : pixel_edge_point)

	const int r = 2;

	const int col = pixel_edge_point.x;
	const int row = pixel_edge_point.y;

	//check x, y
	if (col < r || col >= src.cols - r || row < r || row >= src.rows - r)
		return false;

	std::vector<float> facet;
	for (int i = row - 2; i <= row + 2; i++)
	{
		for (int j = col - 2; j <= col + 2; j++)
		{
			facet.push_back(src.at<unsigned char>(i, j));
		}
	}
	std::vector<float> coefs = SolveCoefs(facet, masks);

	subpixel_edge_point = GetSubPixelFromCoefs(coefs, row, col);
	float g = std::sqrt(coefs[1] * coefs[1] + coefs[2] * coefs[2]);
	//m_strenth.push_back(g);
	float sin_theta = coefs[1] / g;
	float cos_theta = coefs[2] / g;
	//m_sin_theta.push_back(coefs[1] / g);
	//m_cos_theta.push_back(coefs[2] / g);
	float theta_tmp = std::atan(sin_theta / cos_theta);
	theta = theta_tmp;

	return true;
}