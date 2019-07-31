#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define TOOTH_PIC		"Fig0230(a)(dental_xray).tif"
#define MASK_PIC		"Fig0230(b)(dental_xray_mask).tif"
#define TUNGWIRE_PIC	"Fig0229(a)(tungsten_filament_shaded).tif"
#define SHADOWN_PIC		"Fig0229(b)(tungsten_sensor_shading).tif"

static Mat coverMaskPic(Mat imgOri);
static void showImgPara(Mat &img);
static void mutiplyTest();
static void divideTest();


int main()
{
	//mutiplyTest();
	divideTest();

	waitKey(0);

	//system("pause");
	return true;
}


static Mat coverMaskPic(Mat imgOri)
{
	Mat imgOneZeroMask = imgOri.clone();
	uchar* pTemp = NULL;

	for (int row = 0; row < imgOri.rows; row++)
	{
		pTemp = imgOneZeroMask.ptr<uchar>(row);
		for (int col = 0; col < imgOri.cols * imgOri.channels(); col++)
		{
			if (pTemp[col] != 0)
				pTemp[col] = 1;
		}
	}

	return imgOneZeroMask;
}

static void showImgPara(Mat &img)
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
}

static void mutiplyTest()
{
	Mat imgTooth = imread(TOOTH_PIC);
	Mat imgMask = imread(MASK_PIC);
	Mat imgMaskZeroOne = coverMaskPic(imgMask);
	Mat imgOut = Mat::zeros(imgTooth.size(), imgTooth.type());

	multiply(imgTooth, imgMaskZeroOne, imgOut);

	imshow("tooth", imgTooth);
	imshow("mask", imgMask);
	imshow("outPic", imgOut);
}


static void divideTest()
{
	Mat imgTungWire = imread(TUNGWIRE_PIC);
	Mat imgShadow = imread(SHADOWN_PIC);
	Mat imgOut = Mat::zeros(imgTungWire.size(), CV_32FC3);
	Mat imgTungWireTemp = Mat::zeros(imgTungWire.size(), CV_32FC3);
	Mat imgShadowTemp = Mat::zeros(imgTungWire.size(), CV_32FC3);

	imgTungWire.convertTo(imgTungWireTemp, CV_32FC3);
	imgShadow.convertTo(imgShadowTemp, CV_32FC3);

	divide(imgTungWireTemp, imgShadowTemp, imgOut);

	showImgPara(imgTungWireTemp);

	imshow("TungWire", imgTungWire);
	imshow("Shadow", imgShadow);
	imshow("OutPut", imgOut);
}