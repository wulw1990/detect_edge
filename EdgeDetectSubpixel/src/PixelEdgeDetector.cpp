#include "PixelEdgeDetector.h"


void PixelEdgeDetector::Detect(
	const cv::Mat& src,
	std::vector<cv::Point>& points)
{
	cv::Mat canny_result;
	Detect(src, canny_result);

	//points
	points.resize(0);
	for (int x = 1, x_end = src.cols - 1; x < x_end; ++x)
		for (int y = 1, y_end = src.rows - 1; y < y_end; ++y)
			if (canny_result.at<unsigned char>(y,x) != 0)
				points.push_back(cv::Point(x, y));
}

void PixelEdgeDetector::Detect(
	const cv::Mat& src,
	cv::Mat& bw)
{
	//check input
	assert(src.type() == CV_8UC1);

	//canny
	cv::Canny(src, bw, 100, 150);

#if 0
	//remove small connect domain
	cv::Mat label_result;
	//ConnectDomainTwoPass(canny_result / 255, label_result);
	ConnectDomainSeedFilling(canny_result / 255, label_result);
	const int kEdgeSizeMin = 100;

	//cv::imwrite("./output/canny_result.bmp", canny_result);
	//cv::Mat label_result_show = ShowLabelResult(label_result);
	//cv::imwrite("./output/label_result_show.bmp", label_result_show);
	//system("pause");
	RemoveSmallConnectDomain(label_result, canny_result, kEdgeSizeMin);
#endif
}

/*
* there is still a small bug in this function;
* before fix it, use ConnectDomainSeedFilling ustead;
* bug:
* 001
* 110
*/
void PixelEdgeDetector::ConnectDomainTwoPass(const cv::Mat& binImg, cv::Mat& lableImg)
{
	using std::vector;
	if (binImg.empty() || binImg.type() != CV_8UC1)
		return;

	// 第一个通路
	lableImg.release();
	binImg.convertTo(lableImg, CV_32SC1);

	int label = 1;
	vector<int> labelSet;
	labelSet.push_back(0);
	labelSet.push_back(1);//0和1是已经有的，大于1表示已标记

	const int rows = binImg.rows - 1;
	const int cols = binImg.cols - 1;
	std::vector<cv::Point> equal_pairs;
	for (int row = 1; row < rows; row++)
	{
		int* data_preRow = lableImg.ptr<int>(row - 1);
		int* data_curRow = lableImg.ptr<int>(row);
		for (int col = 1; col < cols; col++)
		{
			if (data_curRow[col] == 1)//mask must be 1
			{
				//八邻域，下一行不用算，当前行下一个点也不用算，所以需要看4个点
				vector<int> neighbor;
				neighbor.clear();
				neighbor.push_back(data_preRow[col - 1]);//左上
				neighbor.push_back(data_preRow[col]);//上
				neighbor.push_back(data_preRow[col + 1]);//右上
				neighbor.push_back(data_curRow[col - 1]);//左

				//检查邻域内有没有已经标记过的点
				vector<int> neighborLabels;
				neighborLabels.clear();
				for (int i = 0; i < (int)neighbor.size(); ++i)
					if (neighbor[i] >= 1)//大于1说明已经标记
						neighborLabels.push_back(neighbor[i]);

				//设置当前标签
				const int n_neignbor_labels = (int)neighborLabels.size();
				if (n_neignbor_labels == 0)
				{
					++label;  // 不连通，标签+1
					labelSet.push_back(label);
					data_curRow[col] = label;
				}
				else
				{
					int min_label = neighborLabels[0];
					for (int i = 0; i < n_neignbor_labels; ++i)
						if (neighborLabels[i] < min_label)
							min_label = neighborLabels[i];
					data_curRow[col] = min_label;

					// 保存最小等价表
					for (int k = 0; k < n_neignbor_labels; k++)
						if (min_label != neighborLabels[k])
							equal_pairs.push_back(cv::Point(min_label, neighborLabels[k]));
				}
			}
		}
	}

	// 更新等价对列表
	// 将最小标号给重复区域
	//std::cout << "equal_pairs size :　" << (int)equal_pairs.size() << std::endl;
	//std::cout << "labelSet size :　" << (int)labelSet.size() << std::endl;
	for (int i = 0; i < (int)equal_pairs.size(); ++i)
	{
		int first = equal_pairs[i].x;
		int second = equal_pairs[i].y;
		const int set_size = (int)labelSet.size();
		// 第4域==第5域
		// 则把labelset中所有与第5域具有相同标签的 换成 第4域的标签
		if (labelSet[second] < labelSet[first])
		{
			for (int i = 0; i < set_size; ++i)
				if (labelSet[i] == labelSet[first])
					labelSet[i] = labelSet[second];
		}
		else
		{
			for (int i = 0; i < set_size; ++i)
				if (labelSet[i] == labelSet[second])
					labelSet[i] = labelSet[first];
		}
	}

	for (int row = 0; row < rows; row++)
	{
		int* data_curRow = lableImg.ptr<int>(row);
		for (int col = 0; col < cols; col++)
			data_curRow[col] = labelSet[data_curRow[col]];
	}
}

#include <stack>  
void PixelEdgeDetector::ConnectDomainSeedFilling(const cv::Mat& binImg, cv::Mat& labelImg)
{
	using std::vector;
	if (binImg.empty() || binImg.type() != CV_8UC1)
		return;

	// 第一个通路
	labelImg.release();
	binImg.convertTo(labelImg, CV_32SC1);

	const int n_rows = binImg.rows;
	const int n_cols = binImg.cols;

	int label = 1;
	for (int y = 0; y < n_rows; y++)
	{
		for (int x = 0; x < n_cols; x++)
		{
			if (labelImg.at<int>(y, x) == 1)
			{
				++label;//a new label
				std::stack<cv::Point> neighborPixels;
				labelImg.at<int>(y, x) = label;
				neighborPixels.push(cv::Point(x, y));
				while (!neighborPixels.empty())
				{
					cv::Point curPixel = neighborPixels.top();
					neighborPixels.pop();
					// push the 8-neighbors (foreground pixels) 
					for (int nei_x = curPixel.x - 1; nei_x <= curPixel.x + 1; ++nei_x)
						for (int nei_y = curPixel.y - 1; nei_y <= curPixel.y + 1; ++nei_y)
							if (nei_x >= 0 && nei_x < n_cols && nei_y >= 0 && nei_y < n_rows)
								if (labelImg.at<int>(nei_y, nei_x) == 1)
								{
							labelImg.at<int>(nei_y, nei_x) = label;
							neighborPixels.push(cv::Point(nei_x, nei_y));
								}
				}
			}
		}
	}
}

cv::Mat PixelEdgeDetector::ShowLabelResult(cv::Mat& _labelImg)
{
	int max_label = 0;
	for (int x = 0; x < _labelImg.cols; ++x)
		for (int y = 0; y < _labelImg.rows; ++y)
			if (_labelImg.at<int>(y, x)>max_label)
				max_label = _labelImg.at<int>(y, x);

	std::vector<cv::Vec3b> color_vec(max_label);
	for (int l = 0; l < max_label; ++l)
	{
		int b = 0, g = 0, r = 0;
		while (b < 100 && g < 100 && r < 100)
		{
			b = rand() % 255;
			g = rand() % 255;
			r = rand() % 255;
		}
		color_vec[l] = cv::Vec3b(b,g,r);
	}
		

	cv::Mat img(_labelImg.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	for (int x = 0; x < _labelImg.cols; ++x)
		for (int y = 0; y < _labelImg.rows; ++y)
			if (_labelImg.at<int>(y, x) != 0)
				img.at<cv::Vec3b>(y, x) = color_vec[_labelImg.at<int>(y, x) - 1];

	return img;
}

void PixelEdgeDetector::RemoveSmallConnectDomain(const cv::Mat& _labelImg, cv::Mat cannyImg, int min_size)
{
	int max_label = 0;
	for (int x = 0; x < _labelImg.cols; ++x)
		for (int y = 0; y < _labelImg.rows; ++y)
			if (_labelImg.at<int>(y, x)>max_label)
				max_label = _labelImg.at<int>(y, x);

	std::vector<int> count_vec(max_label + 1, 0);
	for (int x = 0; x < _labelImg.cols; ++x)
		for (int y = 0; y < _labelImg.rows; ++y)
			if (_labelImg.at<int>(y, x) != 0)
				++count_vec[_labelImg.at<int>(y, x)];

	for (int x = 0; x < _labelImg.cols; ++x)
		for (int y = 0; y < _labelImg.rows; ++y)
			if (cannyImg.at<unsigned char>(y, x))
				if (count_vec[_labelImg.at<int>(y, x)] < min_size)
					cannyImg.at<unsigned char>(y, x) = 0;
}