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

	std::string base_name("D:\\No_Backup_BBNC\\LingYun\\PCB³���Լ��\\��Ե�㷨³����ʵ��-ͼ���뽹�˻�-�ڲ��8΢�׷ֱ���\\�齹�Ա�ͼ��\\");
	std::string list_name("D:\\No_Backup_BBNC\\LingYun\\PCB³���Լ��\\��Ե�㷨³����ʵ��-ͼ���뽹�˻�-�ڲ��8΢�׷ֱ���debug.txt");
	std::string output_path("D:\\No_Backup_BBNC\\LingYun\\PCB³���Լ��\\��Ե�㷨³����ʵ��-ͼ���뽹�˻�-�ڲ��8΢�׷ֱ���-result\\");

	TesterPixel* tester_pixel = new TesterPixel();
	tester_pixel->TestMultiMethod(base_name, list_name, output_path);

	system("pause");
}