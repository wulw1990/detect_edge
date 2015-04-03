#include "TesterPixel.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include "opencv2/opencv.hpp"
#include "PixelEdgeDetector.h"
#include "./system_dependent/FileDealer.h"

using namespace std;
using namespace cv;

bool TesterPixel::TestStrict(
	const std::string& path,
	const std::string& list_name,
	const std::string& output_path)
{
	std::vector<std::string> list;
	std::string reference_name;
	if (!GerRefrenceAndList(path, list_name, reference_name, list))
		return false;
	return TestStrict(path, reference_name, list, output_path);
}

bool TesterPixel::TestStrict(
	const std::string& path,
	const std::string& reference_name,
	const std::vector<std::string>& list,
	const std::string& output_path)
{
	return Test(path, reference_name, list, output_path, CompareEdgeStrict);
}



bool TesterPixel::TestRelax(
	const std::string& path,
	const std::string& list_name,
	const std::string& output_path)
{
	std::vector<std::string> list;
	std::string reference_name;
	if (!GerRefrenceAndList(path, list_name, reference_name, list))
		return false;
	return TestRelax(path, reference_name, list, output_path);
}

bool TesterPixel::TestRelax(
	const std::string& path,
	const std::string& reference_name,
	const std::vector<std::string>& list,
	const std::string& output_path)
{
	return Test(path, reference_name, list, output_path, CompareEdgeRelax);
}

bool TesterPixel::GerRefrenceAndList(
	const std::string& path,
	const std::string& list_name,
	std::string& reference_name,
	std::vector<std::string>& list)
{
	//check input output
	if (!ReadNameList(list_name, list))
	{
		fprintf(stderr, "file not exist : %s\n", list_name.c_str());
		return false;
	}

	const int n_files = (int)list.size();
	if (n_files < 2)
	{
		fprintf(stderr, "no enough image\n");
		return false;
	}
	reference_name = list[0];
	list.erase(list.begin());
	return true;
}

bool TesterPixel::Test(
	const std::string& path,
	const std::string& reference_name,
	const std::vector<std::string>& list,
	const std::string& output_path,
	void CompareEdge(cv::Mat, cv::Mat, TestResultInfo&, std::string))
{
	//check input output
	if (!FileDealer::CreateDirectoryMy(output_path))
	{
		fprintf(stderr, "can not mkdir : %s\n", output_path.c_str());
		return false;
	}
	const string result_name("result.txt");

	PixelEdgeDetector* detector = new PixelEdgeDetector();

	Mat gray_reference;
	Mat bw_reference;
	gray_reference = imread(path+reference_name, 0);
	if (gray_reference.empty())
	{
		fprintf(stderr, "image not exist : %s\n", (path + reference_name).c_str());
		return false;
	}
	detector->Detect(gray_reference, bw_reference);
	gray_reference.release();

	const int n_files = (int)list.size();
	vector<int> pos_error_count(n_files), pos_count(n_files);
	vector<int> neg_error_count(n_files), neg_count(n_files);
	clock_t start_time_total = clock();

	vector<TestResultInfo> infos(n_files);
	bool success = true;
#pragma omp parallel for
	for (int i = 0; i < n_files; ++i)
	{
		Mat bw, gray;
		gray = imread(path + list[i], 0);
		if (gray.empty())
		{
			fprintf(stderr, "image not exist : %s\n", (path + list[i]).c_str());
			success = false;
		}
		detector->Detect(gray, bw);
		CompareEdge(bw_reference, bw, infos[i], output_path + list[i]);
	}
	WriteTestResultInfo(infos, path + result_name);
	return success;
}

bool TesterPixel::ReadNameList(const std::string& txt_name, std::vector<std::string>& list)
{
	//check input output
	std::ifstream fin(txt_name.c_str());
	if (!fin.is_open())
		return false;
	list.clear();

	std::string name;
	while (fin >> name)
		list.push_back(name);
	return true;
}

void TesterPixel::PrintList(std::vector<std::string>& list)
{
	const int n = (int)list.size();
	for (int i = 0; i < n; ++i)
		cout << list[i] << endl;
}


void TesterPixel::CompareEdgeStrict(cv::Mat bw_base, cv::Mat bw,
	TestResultInfo& info, std::string diff_img_name)
{
	//check input output
	assert(bw_base.size() == bw.size());
	assert(bw_base.type() == CV_8UC1);
	assert(bw.type() == CV_8UC1);
	const int rows = bw.rows;
	const int cols = bw.cols;

	Mat diff_mat(bw.size(), CV_8UC3);
	diff_mat.setTo(Vec3b(0, 0, 0));

	info.file_name = diff_img_name;
	for (int r = 0; r < rows; ++r){
		for (int c = 0; c < cols; ++c){
			if (bw_base.at<unsigned char>(r, c) != 0){
				info.pos_count++;
				if (bw.at<unsigned char>(r, c) == 0){//该是的不是——漏检
					diff_mat.at<Vec3b>(r, c) = Vec3b(0, 255, 255);//yellow
					info.pos_error_count++;
				}else
					diff_mat.at<Vec3b>(r, c) = Vec3b(255, 0, 0);//blue
			}
			else{
				info.neg_count++;
				if (bw.at<unsigned char>(r, c) != 0){//不该是的是——多检
					info.neg_error_count++;
					diff_mat.at<Vec3b>(r, c) = Vec3b(0, 0, 255);//red
				}
			}
		}
	}
	imwrite(diff_img_name, diff_mat);
}

void TesterPixel::CompareEdgeRelax(cv::Mat bw_base, cv::Mat bw,
	TestResultInfo& info, std::string diff_img_name)
{
	//check input output
	assert(bw_base.size() == bw.size());
	assert(bw_base.type() == CV_8UC1);
	assert(bw.type() == CV_8UC1);
	const int rows = bw.rows;
	const int cols = bw.cols;

	Mat diff_mat(bw.size(), CV_8UC3);
	diff_mat.setTo(Vec3b(0, 0, 0));

	info.file_name = diff_img_name;
	const int radius = 0;
	for (int r = radius; r < rows - radius; ++r){
		for (int c = radius; c < cols - radius; ++c){
			if (bw_base.at<unsigned char>(r, c) != 0){
				info.pos_count++;
				bool flag = false;
				for (int rr = r - radius; rr <= r + radius; ++rr){
					for (int cc = c - radius; cc <= c + radius; ++cc){
						if (bw.at<unsigned char>(rr, cc) != 0){
							flag = true;
							break;
						}
					}
				}
				if (!flag){//该是的不是——漏检
					diff_mat.at<Vec3b>(r, c) = Vec3b(0, 255, 255);//yellow
					info.pos_error_count++;
				}else
					diff_mat.at<Vec3b>(r, c) = Vec3b(255, 0, 0);//blue
			}else
				info.neg_count++;
		}
	}
	for (int r = radius; r < rows - radius; ++r){
		for (int c = radius; c < cols - radius; ++c){
			if (bw.at<unsigned char>(r, c) != 0){//检查出来是边缘
				bool flag = false;
				for (int rr = r - radius; rr <= r + radius; ++rr){
					for (int cc = c - radius; cc <= c + radius; ++cc){
						if (bw_base.at<unsigned char>(rr, cc) != 0){
							flag = true;
							break;
						}
					}
				}
				if (!flag){//不该是的是——多检 ： 假阳性
					info.neg_error_count++;
					diff_mat.at<Vec3b>(r, c) = Vec3b(0, 0, 255);//red
				}
				else{
					diff_mat.at<Vec3b>(r, c) = Vec3b(255, 0, 0);//blue
				}
			}
		}
	}
}

bool TesterPixel::WriteTestResultInfo(std::vector<TestResultInfo> infos, std::string name)
{
	const int n = (int)infos.size();
	FILE* file = fopen(name.c_str(), "w");
	if (!file)
	{
		fprintf(stderr, "can not open : %s\n", name.c_str());
		return false;
	}
	for (int i = 0; i < n; ++i)
	{
		TestResultInfo& info = infos[i];
		double FPR = (double)info.pos_error_count / (double)info.pos_count;
		double FNR = (double)info.neg_error_count / (double)info.neg_count;
		fprintf(file, "%s\t", info.file_name.c_str());
		fprintf(file, "%10ld \t %10ld \t %.5lf", info.pos_error_count, info.pos_count, FPR);
		fprintf(file, "%10ld \t %10ld \t %.5lf\n", info.neg_error_count, info.neg_count, FNR);
	}
	fclose(file);
	return true;
}