#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;

static void showImgPara(Mat &img);
static bool pictureTest(Mat &img, string name);
static bool matTest(Mat &img);
static bool picCreat(Mat &img, string name);
static bool pic2digit(Mat &img, string picName, string fileName);
static bool tran2singleChannel(Mat &src, Mat &tar);

int main()
{
	Mat pic = imread("girl.jpg");
	pictureTest(pic, "girl");

	Mat m1 = Mat::zeros(200, 400, CV_8UC3);
	picCreat(m1, "White");

	Mat m2(3, 4, CV_8UC2, Scalar_<uchar>(1, 2));
	matTest(m2);

	Mat pic1 = imread("check-circle.png");
	pic2digit(pic1, "check", "check-circle.txt");

	waitKey(0);
	return 0;
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
	cout <<	"step[1]: " << img.step[1] << " (Bytes per element only when 2 dims)" << endl;
	cout << "step1(0): " << img.step1(0) << ", step1(1): " << img.step1(1) << " (step / elemSize1)" << endl;
}

static bool pictureTest(Mat &img, string name)
{
	if (img.empty())
	{
		cout << "Load picture fail!" << endl;
		return false;
	}
	cout << endl << "/******************pictureTest******************/" << endl;
	showImgPara(img);

	namedWindow(name);
	imshow(name, img);
	return true;
}

static bool matTest(Mat &img)
{
	uchar* pTemp = NULL;
	cout << endl << "/******************matTest******************/" << endl;
	cout << img << endl;
	showImgPara(img);

	for (int i = 0; i < img.rows; i++)
	{
		for(int j = 0; j < img.cols; j++)
		{
			cout << "[";
			for (int k = 0; k < img.step[1]; k++)
			{
				pTemp = img.data + img.step[0] * i + img.step[1] * j + k;
				cout << (int)*pTemp << " ";
			}
			cout << "] ";
		}
		cout << endl;
	}

	return true;
}

static bool picCreat(Mat &img, string name)
{
	for (int i = 0; i < img.rows; i++)
	{
		uchar *p = img.ptr<uchar>(i);
		for (int j = 0; j < img.cols * img.channels(); j += img.channels())
		{
			if (i > img.rows/4 && i < img.rows*3/4 && j > img.cols*img.channels()/4 && j < img.cols*img.channels()*3/4)
			{
				p[j] = 0;
				p[j + 1] = 0;
				p[j + 2] = 0;

			}
			else
			{
				p[j] = 255;
				p[j + 1] = 255;
				p[j + 2] = 255;
			}
		}
	}

	cout << endl << "/******************picCreat******************/" << endl;
	showImgPara(img);

	namedWindow(name);
	imshow(name, img);

	return true;
}

static bool pic2digit(Mat &img, string picName, string fileName)
{
	uchar*		pTemp = NULL;
	Mat			signalChannel;
	ofstream	output;

	if (img.empty())
	{
		cout << "Load picture fail!" << endl;
		return false;
	}

	cout << endl << "/******************pic2digit******************/" << endl;
	//showImgPara(img);

	//namedWindow(picName);
	//imshow(picName, img);


	tran2singleChannel(img, signalChannel);

	//namedWindow(picName + "_signalChannel");
	//imshow(picName + "_signalChannel", signalChannel);

	showImgPara(signalChannel);

	output.open(fileName);
	for (int i = 0; i < signalChannel.rows; i++)
	{
		for (int j = 0; j < signalChannel.cols; j++)
		{
			for (int k = 0; k < signalChannel.step[1]; k++)
			{
				pTemp = signalChannel.data + signalChannel.step[0] * i + signalChannel.step[1] * j + k;
				output << setw(2) << setfill('0') << (int)*pTemp << " ";
			}
		}
		output << endl;
	}

	output.close();

	imwrite(picName + "_signalChannel.jpg", signalChannel);

	return true;
}

static bool tran2singleChannel(Mat &src, Mat &tar)
{
	vector<Mat> channels;

	split(src, channels);
	tar = channels.at(0);
	
	return true;
}