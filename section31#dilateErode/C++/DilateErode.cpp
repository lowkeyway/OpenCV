#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

static bool DilateErodeTest(Mat imgOri);
static void showImgPara(Mat &img);
static bool pic2digit(Mat &img, string fileName);
static bool tran2singleChannel(Mat &src, Mat &tar);

int main()
{
	Mat imgOri = imread("fingerprint.png");
	if (imgOri.empty())
	{
		cout << "Cannot load this picture!" << endl;
		return false;
	}
	DilateErodeTest(imgOri);

	waitKey(0);

	return true;
}


static bool DilateErodeTest(Mat imgOri)
{
	Mat imgDilate, imgErode, imgBin;
	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}
	imshow("imgOri", imgOri);

	double th = threshold(imgOri, imgBin, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("imgBin", imgBin);
	pic2digit(imgBin, "imgBin.txt");

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(imgBin, imgDilate, kernel);
	pic2digit(imgDilate, "imgDilate.txt");

	erode(imgOri, imgErode, kernel, Point(-1, -1), 1, 0);
	pic2digit(imgErode, "imgErode.txt");

	imshow("imgDilate", imgDilate);
	imshow("imgErode", imgErode);

	return true;
}


static bool pic2digit(Mat &img, string fileName)
{
	uchar*		pTemp = NULL;
	Mat			signalChannel;
	ofstream	output;

	if (img.empty())
	{
		cout << "Load picture fail!" << endl;
		return false;
	}

	output.open(fileName, ios::in);
	if (output)
	{
		cout << "File Exist!" << endl;
		return false;
	}
	else
	{
		output.open(fileName);
	}

	cout << endl << "/******************pic2digit******************/" << endl;
	//showImgPara(img);

	//namedWindow(picName);
	//imshow(picName, img);

	if (1 != img.channels())
	{
		tran2singleChannel(img, signalChannel);
	}
	else
	{
		signalChannel = img.clone();
	}

	//namedWindow(picName + "_signalChannel");
	//imshow(picName + "_signalChannel", signalChannel);

	showImgPara(signalChannel);


	for (int i = 0; i < signalChannel.rows; i++)
	{
		for (int j = 0; j < signalChannel.cols; j++)
		{
			for (int k = 0; k < signalChannel.step[1]; k++)
			{
				pTemp = signalChannel.data + signalChannel.step[0] * i + signalChannel.step[1] * j + k;
				output << setw(3) << setfill('0') << (int)*pTemp << " ";
			}
		}
		output << endl;
	}

	output.close();

	return true;
}

static bool tran2singleChannel(Mat &src, Mat &tar)
{
	vector<Mat> channels;

	split(src, channels);
	tar = channels.at(0);
	convertScaleAbs(tar, tar);

	return true;
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