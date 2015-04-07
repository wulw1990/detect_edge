#ifndef _SUB_DETECTOR_H_
#define _SUB_DETECTOR_H_

#include "opencv2/opencv.hpp"

class SubDetectorBase
{
public:
	virtual void Detect(
		const cv::Mat src,
		std::vector<cv::Point>& pixel_edge_points,
		std::vector<cv::Point2f>& subpixel_edge_points,
		std::vector<float>& thetas)=0; //���麯�����������ʵ��

	/*
	* ���룺�Ҷ�ͼ�� ���ؾ��ȵĵ�
	* ����������ؾ��ȵĵ㣬 ����
	* return : �Ƿ�ɹ�
	*/
	virtual bool Detect(
		const cv::Mat src,
		cv::Point pixel_edge_point,
		cv::Point2f& subpixel_edge_point,
		float& theta) = 0; //���麯�����������ʵ��

	std::string getName(){
		return m_method_name;
	}

protected:
	std::string m_method_name;
};


#endif