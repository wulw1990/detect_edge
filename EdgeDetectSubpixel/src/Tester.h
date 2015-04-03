#ifndef _TESTER_H_
#define _TESTER_H_

#include <vector>
#include <string>
class SubDetectorBase;

class Tester
{
public:
	void TestFileList(
		std::string base_name, std::string list_name,
		SubDetectorBase* detector,
		std::string output_base_name);

	void TestFileList(
		std::string base_name,
		std::vector<std::string> name_list,
		SubDetectorBase* detector,
		std::string output_base_name);

private:
	
	bool TestOneImage(
		std::string full_name, 
		SubDetectorBase* detector,
		std::string output_full_name);

	std::vector<std::string> ReadNameList(std::string txt_name);
	
};

#endif
