#include <iostream>
#include <string>
//#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


static void splitRGBTest(Mat imgOri);
static void cover2HSVTest(Mat imgOri);
static void cover2OthersTest(Mat imgOri);
static void binHSVThreasholdCallBack(int, void*);
static void scaleHCallBack(int, void*);
static void scaleSCallBack(int, void*);
static void scaleVCallBack(int, void*);
static void binGrayThreasholdCallBack(int, void*);
static void showImgPara(Mat &img);

int gBinHSVThreshold = 64;
int gBinGrayThreshold = 100;
int gHScale = 100;
int gSScale = 100;
int gVScale = 100;
Mat gHImTemp, gGrayImTemp;
vector<Mat> gImgRGBChannels;
vector<Mat> gImgHSVChannels;
vector<Mat> gTempImgHSVChannels;


#define BIN_MAX 255
#define HSV_MAX 300

int main()
{
	Mat imgOri = imread("Danxia.jpg");
	if (imgOri.empty())
	{
		cout << "Cannot find this picture!" << endl;
	}

	//splitRGBTest(imgOri);
	cover2HSVTest(imgOri);
	//cover2OthersTest(imgOri);

	waitKey(0);
	return true;
}

static void clearChannel(vector<Mat> &imgChannels)
{
	for (vector<Mat>::iterator channel = imgChannels.begin(); channel != imgChannels.end(); channel++)
	{
		*channel = Scalar(0);
	}
}

static void splitRGBTest(Mat imgOri)
{
	Mat channelB, channelG, channelR;
	split(imgOri, gImgRGBChannels);
	vector<Mat> imgChannels(gImgRGBChannels);

	/* Show original picture */
	imshow("imgOri", imgOri);
	
	/* Blue Vector */
	channelB = imgChannels[0].clone();
	imshow("Blue Channel", channelB);

	/* Green Vector */
	channelG = imgChannels[1].clone();
	imshow("Green Channel", channelG);

	/* Red Vector */
	channelR = imgChannels[2].clone();
	imshow("Red Channel", channelR);

	/* G & R Channel is 0 */
	Mat imgBlue;
	clearChannel(imgChannels);
	imgChannels[0] = channelB;
	merge(imgChannels, imgBlue);
	imshow("imgBlue", imgBlue);

	/* B & R Channel is 0 */
	Mat imgGreen;

	clearChannel(imgChannels);
	imgChannels[1] = channelG;
	merge(imgChannels, imgGreen);
	imshow("imgGreen", imgGreen);

	/* B & G Channel is 0 */
	Mat imgRed;
	clearChannel(imgChannels);
	imgChannels[2] = channelR;
	merge(imgChannels, imgRed);
	imshow("imgRed", imgRed);
}


static void cover2HSVTest(Mat imgOri)
{
	Mat hsv, channelH, channelS, channelV, imgBin;

	/* Show original picture */
	imshow("imgOri", imgOri);

	/* Show HSV picture */
	cvtColor(imgOri, hsv, COLOR_BGR2HSV);
	imshow("HSV", hsv);

	split(hsv, gImgHSVChannels);
	split(hsv, gTempImgHSVChannels);
	vector<Mat> imgChannels(gImgHSVChannels);
	//gTempImgHSVChannels = gImgHSVChannels;

	/* H Vector */
	channelH = imgChannels[0].clone();
	imshow("H Channel", channelH);

	/* S Vector */
	channelS = imgChannels[1].clone();
	imshow("S Channel", channelS);

	/* V Vector */
	channelV = imgChannels[2].clone();
	imshow("V Channel", channelV);


	/* Bin Image */
	gHImTemp = channelH;
	threshold(gHImTemp, imgBin, gBinHSVThreshold, 255, THRESH_BINARY);
	imshow("HSV Bin Channel", imgBin);

	cv::createTrackbar("H", "imgOri", &gHScale, HSV_MAX, scaleHCallBack);
	cv::createTrackbar("S", "imgOri", &gSScale, HSV_MAX, scaleSCallBack);
	cv::createTrackbar("V", "imgOri", &gVScale, HSV_MAX, scaleVCallBack);

	cv::createTrackbar("Threshold", "HSV Bin Channel", &gBinHSVThreshold, BIN_MAX, binHSVThreasholdCallBack);
}

static void cover2OthersTest(Mat imgOri)
{
	Mat imgGray, imgBin;

	cvtColor(imgOri, imgGray, COLOR_BGR2GRAY);
	imshow("GRAY", imgGray);

	/* Bin Image */
	gGrayImTemp = imgGray;
	threshold(gGrayImTemp, imgBin, gBinGrayThreshold, 255, THRESH_BINARY);
	imshow("Bin Gray Channel", imgBin);

	cv::createTrackbar("Threshold", "Bin Gray Channel", &gBinGrayThreshold, BIN_MAX, binGrayThreasholdCallBack);
}


static void binHSVThreasholdCallBack(int, void*)
{
	Mat imgBin;
	threshold(gHImTemp, imgBin, gBinHSVThreshold, 255, THRESH_BINARY);
	imshow("HSV Bin Channel", imgBin);
}

static void binGrayThreasholdCallBack(int, void*)
{
	Mat imgBin;
	threshold(gGrayImTemp, imgBin, gBinGrayThreshold, 255, THRESH_BINARY);
	imshow("Bin Gray Channel", imgBin);
}

static void scaleHCallBack(int, void* )
{
#if 1
	Mat fImgOriHChannel = gImgHSVChannels[0].clone();
	Mat fImgOutHChannel = gImgHSVChannels[0].clone();
	Mat imgOutHSV, imgOutShow;

	fImgOriHChannel.convertTo(fImgOriHChannel, CV_32FC1);
	fImgOutHChannel.convertTo(fImgOutHChannel, CV_32FC1);

	Mat mask(fImgOriHChannel.rows, fImgOriHChannel.cols, CV_32FC1, Scalar(gHScale / 100.0));

	multiply(fImgOriHChannel, mask, fImgOutHChannel);
	fImgOutHChannel.convertTo(gTempImgHSVChannels[0], gTempImgHSVChannels[0].type());
	merge(gTempImgHSVChannels, imgOutHSV);
	cvtColor(imgOutHSV, imgOutShow, COLOR_HSV2BGR);

	imshow("imgOri", imgOutShow);

	//showImgPara(mask);
	//showImgPara(fImgOutHChannel);
	//cout << ">>>>>>>>>>>>>" << gHScale << "<<<<<<<<<<<<<\n" << endl;
#else
	Mat imgHSVOutShow, imgOutShow;
	Mat imgHChannel = gImgHSVChannels[0].clone();
	for (int row = 0; row < imgHChannel.rows; row++)
	{
		uchar* pGImgHSVChannels = imgHChannel.ptr<uchar>(row);
		uchar* pGTempImgHSVChannels = gTempImgHSVChannels[0].ptr<uchar>(row);
		for (int col = 0; col < imgHChannel.cols; col++)
		{
			pGTempImgHSVChannels[col] = saturate_cast<uchar>((float)(pGImgHSVChannels[col]) * (float)gHScale / 100.0);
			//cout << pGTempImgHSVChannels[col];
		}
		//cout << endl;
	}
	//cout << "------------------------------END------------------------------\n" << endl;

	merge(gTempImgHSVChannels, imgHSVOutShow);
	cvtColor(imgHSVOutShow, imgOutShow, COLOR_HSV2BGR);

	imshow("imgOri", imgOutShow);
	   
#endif
}
static void scaleSCallBack(int, void*)
{
	Mat imgHSVOutShow, imgOutShow;
	Mat imgSChannel = gImgHSVChannels[1].clone();
	for (int row = 0; row < imgSChannel.rows; row++)
	{
		uchar* pGImgHSVChannels = imgSChannel.ptr<uchar>(row);
		uchar* pGTempImgHSVChannels = gTempImgHSVChannels[1].ptr<uchar>(row);
		for (int col = 0; col < imgSChannel.cols; col++)
		{
			pGTempImgHSVChannels[col] = saturate_cast<uchar>((float)(pGImgHSVChannels[col]) * (float)gSScale / 100.0);
		}
	}

	merge(gTempImgHSVChannels, imgHSVOutShow);
	cvtColor(imgHSVOutShow, imgOutShow, COLOR_HSV2BGR);

	imshow("imgOri", imgOutShow);
}
static void scaleVCallBack(int, void*)
{
	cout << gVScale << endl;
	Mat fImgOriVChannel = gImgHSVChannels[2].clone();
	Mat fImgOutVChannel = gImgHSVChannels[2].clone();
	Mat imgOutHSV, imgOutShow;

	fImgOriVChannel.convertTo(fImgOriVChannel, CV_32FC1);
	fImgOutVChannel.convertTo(fImgOutVChannel, CV_32FC1);

	Mat mask(fImgOriVChannel.rows, fImgOriVChannel.cols, CV_32FC1, Scalar(gVScale / 100.0));

	multiply(fImgOriVChannel, mask, fImgOutVChannel);
	fImgOutVChannel.convertTo(gTempImgHSVChannels[2], gTempImgHSVChannels[2].type());
	merge(gTempImgHSVChannels, imgOutHSV);
	cvtColor(imgOutHSV, imgOutShow, COLOR_HSV2BGR);

	imshow("imgOri", imgOutShow);

	//showImgPara(mask);
	//showImgPara(fImgOutHChannel);
	//cout << ">>>>>>>>>>>>>" << gHScale << "<<<<<<<<<<<<<\n" << endl;
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
	cout << "--------------	END --------------\n" << endl;
}