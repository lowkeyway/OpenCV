#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class MatchTemp
{
public:
	bool matchTest();
	void showImgPara(Mat img);
	bool showImg();
	bool loadOriImg(string fileName);
	bool loadTarImg(string fileName);

public:
	Mat imgOri;
	Mat imgTar;
	Mat imgOut;
};

void MatchTemp::showImgPara(Mat img)
{
	cout << "sizeof(img) is: " << sizeof(img) << ", img size is: " << img.size << endl;
	cout << "rows x cols: (" << img.rows << " x " << img.cols << ")" << endl;
	cout << "dims: " << img.dims << endl;
	cout << "channels: " << img.channels() << endl;
	cout << "type: " << img.type() << endl;
	cout << "depth:" << img.depth() << endl;
	cout << "elemSize:" << img.elemSize() << " (Bytes per element)" << endl;
	cout << "elemSize1:" << img.elemSize1() << "(Bytes per channel)" << endl;
	cout << "step[0]: " << img.step[0] << " (Bytes per cows only when 2 dims)" << endl;
	cout << "step[1]: " << img.step[1] << " (Bytes per element only when 2 dims)" << endl;
	cout << "step1(0): " << img.step1(0) << ", step1(1): " << img.step1(1) << " (step / elemSize1)" << endl;

	cout << endl;
}

bool MatchTemp::matchTest()
{
	double minVal = 0, maxVal = 0;
	Point minLoc, maxLoc, tarLoc;
	if (imgOri.empty() || imgTar.empty())
	{
		cout << "Please input correct image!" << endl;
		return false;
	}

	matchTemplate(imgOri, imgTar, imgOut, TM_SQDIFF);
	minMaxLoc(imgOut, &minVal, &maxVal, &minLoc, &maxLoc);

	showImgPara(imgOri);
	showImgPara(imgTar);
	showImgPara(imgOut);

	tarLoc = minLoc;

	rectangle(imgOri, tarLoc, Point(tarLoc.x + imgTar.cols, tarLoc.y + imgTar.rows), Scalar(0, 0, 255));
	imshow("imgMatch", imgOri);
	

	return true;
}

bool MatchTemp::showImg()
{
	if (!imgOri.empty())
	{
		imshow("ImgOri", imgOri);
	}

	if (!imgTar.empty())
	{
		imshow("imgTar", imgTar);
	}

	if (!imgOut.empty())
	{
		imshow("imgOut", imgOut);
	}

	return true;
}


bool MatchTemp::loadOriImg(string fileName)
{
	imgOri = imread(fileName);
	if (imgOri.empty())
	{
		cout << "loadOriImg Fail!" << endl;
		return false;
	}

	return true;
}

bool MatchTemp::loadTarImg(string fileName)
{
	imgTar = imread(fileName);
	if (imgTar.empty())
	{
		cout << "loadOriImg Fail!" << endl;
		return false;
	}

	return true;
}

int main()
{
	MatchTemp M;
	M.loadOriImg("Gril.jpg");
	M.loadTarImg("eye.png");
	M.matchTest();
	M.showImg();

	waitKey(0);
	return true;
}

