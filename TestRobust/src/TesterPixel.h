#ifndef _TESTER_PIXEL_H_WULIWEI_BBNC_TSINGHUA_
#define _TESTER_PIXEL_H_WULIWEI_BBNC_TSINGHUA_

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

#include "ImageDifferBase.h"
using TestResultInfo = ImageDifferBase::TestResultInfo;

class TesterPixel
{
public:
	bool TestMultiMethod(
		const std::string& path,
		const std::string& list_name,
		const std::string& output_path
		);
	bool TestMultiMethod(
		const std::string& path,
		const std::string& reference_name,
		const std::vector<std::string>& list,
		const std::string& output_path);

private:
	bool TestStrict(
		const std::string& path,
		const std::string& list_name,
		bool save_img,
		const std::string& output_path);
	bool TestStrict(
		const std::string& path,
		const std::string& reference_name,
		const std::vector<std::string>& list,
		bool save_img,
		const std::string& output_path);

	bool TestRelax(
		const std::string& path,
		const std::string& list_name,
		bool save_img,
		const std::string& output_path);
	bool TestRelax(
		const std::string& path,
		const std::string& reference_name,
		const std::vector<std::string>& list,
		bool save_img,
		const std::string& output_path);

	bool GerRefrenceAndList(
		const std::string& path,
		const std::string& list_name,
		std::string& reference_name,
		std::vector<std::string>& list);
	/*
	* real test worker
	*/
	bool Test(
		const std::string& path,
		const std::string& reference_name,
		const std::vector<std::string>& list,
		bool save_img,
		const std::string& output_path,
		ImageDifferBase* differ);

	static bool ReadNameList(const std::string& txt_name, std::vector<std::string>& list);
	static void PrintList(std::vector<std::string>& list);
	

	static bool WriteTestResultInfo(std::vector<TestResultInfo> infos, std::string name);

	void saveNames(
		const std::string& reference_name,
		const std::vector<std::string>& list,
		string output_name);

	bool errorFileNotExist(string name);
};

#endif