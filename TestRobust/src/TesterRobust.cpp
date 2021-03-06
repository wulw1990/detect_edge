#include "TesterRobust.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include <iomanip>

#include "opencv2/opencv.hpp"
#include "../../EdgeDetectSubpixel/src/PixelEdgeDetector.h"
#include "../../EdgeDetectSubpixel/src/system_dependent/FileDealer.h"
#include "../../EdgeDetectSubpixel/src/infrastructure/Sorter.h"

#include "ImageDifferStrict.h"
#include "ImageDifferRelax.h"

using namespace std;
using namespace cv;


bool TesterRobust::TestMultiMethod(
	const std::string& path,
	const std::string& reference_name,
	const std::vector<std::string>& list,
	const std::string& output_path)
{
#if 1
	cout << "清晰图像(reference_name):" << endl;
	cout << reference_name << endl;
	cout << endl;
	cout << "测试图像列表(list):" << endl;
	for (int i = 0; i < (int)list.size(); ++i)
		cout << list[i] << endl;
	cout << endl;
	cout << "输出路径(output_path):" << endl;
	cout << output_path << endl;
	cout << endl;
	cout << "注意：用文件夹中的第一个图像作为清晰图像(reference_name)" << endl;
	cout << "请核对清晰图像(reference_name)的文件名是否有误" << endl;
	cout << "如果确认，请按任意键继续；否则，请直接关闭窗口" << endl;
	system("pause");
#endif

	//check input output
	FileDealer::CreateDirectoryRecursive(output_path);

	saveNames(reference_name, list, output_path + "name_list.txt");
	return TestRelax(path, reference_name, list, true, output_path)	&& TestStrict(path, reference_name, list, true, output_path);
}

bool TesterRobust::TestStrict(
	const std::string& path,
	const std::string& reference_name,
	const std::vector<std::string>& list,
	bool save_img,
	const std::string& output_path)
{
	ImageDifferBase* differ = new ImageDifferStrict("diff_strict");
	return Test(path, reference_name, list, save_img, output_path, differ);
}
bool TesterRobust::TestRelax(
	const std::string& path,
	const std::string& reference_name,
	const std::vector<std::string>& list,
	bool save_img,
	const std::string& output_path)
{
	ImageDifferBase* differ = new ImageDifferRelax("diff_relax");
	return Test(path, reference_name, list, save_img, output_path, differ);
}


bool TesterRobust::Test(
	const std::string& path,
	const std::string& reference_name,
	const std::vector<std::string>& list,
	bool save_img,
	const std::string& output_path,
	ImageDifferBase* differ)
{
	PixelEdgeDetector* detector = new PixelEdgeDetector();

	cout << "Deal with Reference Image ..." << endl;
	//cout << "\tName : " << reference_name << endl;
	//system("pause");
	clock_t start_time = clock();
	Mat gray_reference;
	Mat bw_reference;
	gray_reference = imread(path+reference_name, 0);
	if (gray_reference.empty()) return errorFileNotExist(path + reference_name);
	detector->Detect(gray_reference, bw_reference);
	gray_reference.release();
	if (save_img)
		imwrite(output_path + reference_name, bw_reference);
	cout << "\tDeal with Reference Image OK. Time = " << clock() - start_time << " ms" << endl;

	const int n_files = (int)list.size();
	vector<int> pos_error_count(n_files), pos_count(n_files);
	vector<int> neg_error_count(n_files), neg_count(n_files);
	clock_t start_time_total = clock();

	vector<TestResultInfo> infos(n_files);
	bool success = true;
	for (int i = 0; i < n_files; ++i)
	{
		cout << endl;
		cout << setfill(' ');
		cout << setw(4) << i + 1 << "/" << setw(4) << n_files << " " << differ->GetName()<< " Testing.. " << endl;
		//cout << "\tName : " << list[i] << endl;
		clock_t start_time = clock();
		Mat bw, gray;
		gray = imread(path + list[i], 0);
		if (gray.empty()) return errorFileNotExist(path + list[i]);
		detector->Detect(gray, bw);
		Mat diff_mat;
		differ->diff(bw_reference, bw, infos[i], diff_mat);
		if (save_img)
		{
			imwrite(output_path + list[i], bw);
			imwrite(output_path + list[i]+"."+differ->GetName() + ".bmp", diff_mat);
		}
		cout << "\t" << setw(4) << i + 1 << " OK. Time = " << clock() - start_time << " ms" << endl;
		//system("pause");
	}
	WriteTestResultInfo(infos, output_path + "test_result_" + differ->GetName()+".txt");
	return success;
}


bool TesterRobust::WriteTestResultInfo(std::vector<TestResultInfo> infos, std::string name)
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
		fprintf(file, "%10ld \t %10ld \t %.5lf", info.pos_error_count, info.pos_count, FPR);
		fprintf(file, "%10ld \t %10ld \t %.5lf\n", info.neg_error_count, info.neg_count, FNR);
	}
	fclose(file);
	return true;
}

void TesterRobust::saveNames(
	const std::string& reference_name,
	const std::vector<std::string>& list,
	string output_name)
{
	ofstream ofs(output_name);
	ofs << reference_name << endl;
	for (int i = 0; i < (int)list.size(); ++i)
		ofs << list[i] << endl;
	ofs.clear();
}

bool TesterRobust::errorFileNotExist(string name)
{
	cerr << "File not exist : " << name << endl;
	return false;
}