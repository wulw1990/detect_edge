#ifndef _TESTER_PIXEL_H_WULIWEI_BBNC_TSINGHUA_
#define _TESTER_PIXEL_H_WULIWEI_BBNC_TSINGHUA_

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"

class TesterPixel
{
	struct TestResultInfo
	{
		std::string file_name;
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
public:
	bool TestStrict(
		const std::string& path,
		const std::string& list_name,
		const std::string& output_path);
	bool TestStrict(
		const std::string& path,
		const std::string& reference_name,
		const std::vector<std::string>& list,
		const std::string& output_path);
	bool TestRelax(
		const std::string& path,
		const std::string& list_name,
		const std::string& output_path);
	bool TestRelax(
		const std::string& path,
		const std::string& reference_name,
		const std::vector<std::string>& list,
		const std::string& output_path);
private:
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
		const std::string& output_path,
		void CompareEdge(cv::Mat, cv::Mat, TestResultInfo&, std::string ));

	static bool ReadNameList(const std::string& txt_name, std::vector<std::string>& list);
	static void PrintList(std::vector<std::string>& list);
	
	static void CompareEdgeStrict(cv::Mat bw_base, cv::Mat bw,
		TestResultInfo& result, std::string diff_img_name);
	static void CompareEdgeRelax(cv::Mat bw_base, cv::Mat bw,
		TestResultInfo& result, std::string diff_img_name);

	static bool WriteTestResultInfo(std::vector<TestResultInfo> infos, std::string name);
};

#endif