#if 1
/*
* For lingyun test
*/
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include "SubDetectorBlurModel.h"
#include "SubDetectorAreaEffect.h"
#include "SubDetectorFacetModel.h"
#include "SubDetectorSobelProjection.h"
#include "Tester.h"
#include "./system_dependent/FileDealer.h"

using namespace std;

int main()
{
	string path = "./image/";
	std::vector<std::string> list;
	FileDealer::GetFileList(path, list);
	//FileDealer::PrintStringList(list);

	SubDetectorBase* detector = NULL;
	std::string output_path;

	for (int i = 1; i <= 4; ++i)//all in one 
	{
		switch (i)
		{
		case 1:
			detector = new SubDetectorBlurModel();
			output_path = string("./output_detector_blur_model/");
			break;
		case 2:
			detector = new SubDetectorAreaEffect();
			output_path = string("./output_detector_area_effect/");
			break;
		case 3:
			detector = new SubDetectorFacetModel();
			output_path = string("./output_detector_facet_model/");
			break;
		case 4:
			detector = new SubDetectorSobelProjection();
			output_path = string("./output_detector_sobel_projection/");
			break;
		default:
			cerr << "wrong detector" << endl;
			system("pause");
		}
		Tester* tester = new Tester();
		tester->TestFileList(path, list, detector, output_path);
	}

	cout << "Program END" << endl;
	system("pause");
}


#endif