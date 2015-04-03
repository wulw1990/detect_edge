#include "Tester.h"

#include <time.h>
#include <fstream>

#include "SubDetectorBase.h"
#include "Show.h"
#include "./system_dependent/FileDealer.h"


void Tester::TestFileList(
	std::string base_name, std::string list_name,
	SubDetectorBase* detector,
	std::string output_base_name)
{
	std::vector<std::string> name_list = ReadNameList(list_name);
	TestFileList(base_name, name_list, detector, output_base_name);
}

void Tester::TestFileList(
	std::string base_name, std::vector<std::string> name_list,
	SubDetectorBase* detector,
	std::string output_base_name)
{
	const int n_images = (int)name_list.size();
	for (int i = 0; i < n_images; ++i)
	{
		std::string full_name = output_base_name + name_list[i];
		size_t location = full_name.find_last_of("\\");
		if (location == std::string::npos)
			location = full_name.find_last_of("/");
		if (location != std::string::npos)
			FileDealer::CreateDirectoryRecursive(full_name.substr(0, location));
	}
	
	clock_t start_time_all = clock();
	for (int i = 0; i < n_images; ++i)
	{
		printf("\n%d/%d Begin...\n", i+1, n_images);
		clock_t start_time = clock();
		std::string full_name = base_name + name_list[i];
		bool is_success = TestOneImage(full_name, detector, output_base_name + name_list[i]);
		if (!is_success)
		{
			printf("\t%s\n\tfiled!\n", (output_base_name + name_list[i]).c_str());
		}
		//std::cout << output_base_name + name_list[i] << std::endl;
		printf("Time : %d ms\n", clock() - start_time);
	}
	printf("Total Time : %d\n", clock() - start_time_all);
	printf("Saved To : %s\n", output_base_name.c_str());

}

bool Tester::TestOneImage(
	std::string full_name,
	SubDetectorBase* detector,
	std::string output_full_name)
{
	//read image
	clock_t start_time = clock();
	printf("\tReading...");
	cv::Mat img = cv::imread(full_name, 0);//gray image;
	if (img.empty())
	{
		return false;
	}
	printf(".OK. Time : %d ms\n", clock() - start_time);


	std::string suffix = output_full_name.substr(output_full_name.length() - 4, 4);
	std::string prefix = output_full_name.substr(0, output_full_name.length() - 4);
	const int scale = 5;
	const int grid_rows = 5;
	const int grid_cols = 5;
	bool grid_mask[grid_rows][grid_cols] =
	{
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	};
	const int patch_rows = img.rows / grid_rows;
	const int patch_cols = img.cols / grid_cols;
	int count_grid = 0;
	for (int grid_r = 0; grid_r < grid_rows; ++grid_r)
	{
		for (int grid_c = 0; grid_c < grid_rows; ++grid_c)
		{
			printf("\tGrid_c=%d, Grid_c=%d\t", grid_r+1, grid_c+1);
			if (!grid_mask[grid_r][grid_c])
			{
				printf("ignore\n");
				continue;
			}
			else
				printf("Dealing...\n");
			//detect
			start_time = clock();
			printf("\t\tStep 1 : Detect Edge...");
			cv::Rect rect(grid_c*patch_cols, grid_r*patch_rows, patch_cols, patch_rows);
			std::vector<cv::Point> points;
			std::vector<cv::Point2f> subpixel_points;
			std::vector<float> thetas;
			detector->Detect(img(rect), points, subpixel_points, thetas);
			printf(".OK. Time = %d ms\n", clock() - start_time);

			//save to disk
			start_time = clock();
			printf("\t\tStep 2 : Save To Disk...");
			char buf[1024];
			sprintf(buf, "%s_%dr_%dc_%dR_%dC%s", 
				prefix.c_str(), grid_r + 1, grid_c + 1, grid_rows, grid_cols, suffix.c_str());
			//std::cout << output_new_name << std::endl;
			Show::SaveEdgeDirection(img(rect), subpixel_points, thetas, scale, buf);
			sprintf(buf, "%s_%dr_%dc_%dR_%dC_pixel%s",
				prefix.c_str(), grid_r + 1, grid_c + 1, grid_rows, grid_cols, suffix.c_str());
			Show::SavePixelEdge(img(rect), points, buf);
			printf(".OK. Time = %d ms\n", clock() - start_time);
		}
	}
	return true;
}

std::vector<std::string> Tester::ReadNameList(std::string txt_name)
{
	std::ifstream fin(txt_name.c_str());
	std::vector<std::string> name_list;
	std::string name;
	while (fin >> name)
		name_list.push_back(name);
	return name_list;
}