#include "opencv2/opencv.hpp"

#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif

#pragma comment( lib, cvLIB("core"))
#pragma comment( lib, cvLIB("imgproc"))
#pragma comment( lib, cvLIB("highgui"))

#include <string>
#include "TesterPixel.h"

int main(void)
{

	std::string base_name("D:\\No_Backup_BBNC\\LingYun\\PCBÂ³°ôÐÔ¼ì²â\\±ßÔµËã·¨Â³°ôÐÔÊµÑé-Í¼ÏñÀë½¹ÍË»¯-ÄÚ²ã°å8Î¢Ã×·Ö±æÂÊ\\Ðé½¹¶Ô±ÈÍ¼Ïñ\\");
	std::string list_name("D:\\No_Backup_BBNC\\LingYun\\PCBÂ³°ôÐÔ¼ì²â\\±ßÔµËã·¨Â³°ôÐÔÊµÑé-Í¼ÏñÀë½¹ÍË»¯-ÄÚ²ã°å8Î¢Ã×·Ö±æÂÊdebug.txt");
	std::string output_path("D:\\No_Backup_BBNC\\LingYun\\PCBÂ³°ôÐÔ¼ì²â\\±ßÔµËã·¨Â³°ôÐÔÊµÑé-Í¼ÏñÀë½¹ÍË»¯-ÄÚ²ã°å8Î¢Ã×·Ö±æÂÊ-result\\");

	TesterPixel* tester_pixel = new TesterPixel();
	tester_pixel->TestMultiMethod(base_name, list_name, output_path);

	system("pause");
}