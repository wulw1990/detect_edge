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

#if 0
#include "SubDetectorBlurModel.h"
#include "SubDetectorAreaEffect.h"
#include "SubDetectorFacetModel.h"
#include "SubDetectorSobelProjection.h"
#include "Tester.h"
#include "./system_dependent/FileDealer.h"

#include "TesterPixel.h"

int main(void)
{
	//TODO : make name list(chinese) automatically, not use python
	//CMD -- ls /B
#define TEST_SUB 0
#if TEST_SUB
	std::string base_name("D:\\No_Backup_BBNC\\LingYun\\");
	std::string list_name("list.txt");
	std::string output_base_name("D:\\No_Backup_BBNC\\LingYun\\output_edge\\");

	SubDetectorBase* detector;

	detector = new SubDetectorBlurModel();
	//detector = new SubDetectorAreaEffect();
	//detector = new SubDetectorFacetModel();
	//detector = new SubDetectorSobelProjection();

	Tester* tester = new Tester();
	tester->TestFileList(base_name, list_name, detector, output_base_name);
#else
	std::string base_name("D:\\No_Backup_BBNC\\LingYun\\PCB³���Լ��\\��Ե�㷨³����ʵ��-ͼ���뽹�˻�-�ڲ��8΢�׷ֱ���\\�齹�Ա�ͼ��\\");
	std::string list_name("D:\\No_Backup_BBNC\\LingYun\\PCB³���Լ��\\��Ե�㷨³����ʵ��-ͼ���뽹�˻�-�ڲ��8΢�׷ֱ���.txt");
	std::string output_path("D:\\No_Backup_BBNC\\LingYun\\PCB³���Լ��\\��Ե�㷨³����ʵ��-ͼ���뽹�˻�-�ڲ��8΢�׷ֱ���-result\\");

	TesterPixel* tester_pixel = new TesterPixel();
	tester_pixel->TestStrict(base_name, list_name, output_path);

#endif



	system("pause");
}

#endif