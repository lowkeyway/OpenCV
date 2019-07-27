#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


static void showImgPara(Mat &img);
static void linearTest(string name, float a, float b);


int main()
{
	linearTest("left06.jpg", 1, -50);

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
	cout << "step[1]: " << img.step[1] << " (Bytes per element only when 2 dims)" << endl;
	cout << "step1(0): " << img.step1(0) << ", step1(1): " << img.step1(1) << " (step / elemSize1)" << endl;
}


static void linearTest(string name, float a, float b)
{
	/* Original Picture */
	Mat imgOri = imread(name);
	showImgPara(imgOri);
	namedWindow("original");
	imshow("original", imgOri);

	/* Output Picture */
	Mat imgOut = Mat::ones(imgOri.size(), imgOri.type());
	//namedWindow("zeros");
	//imshow("zeros", imgOut);

	uchar bOri = 0, gOri = 0, rOri = 0;

	for (int i = 0; i < imgOri.cols; i++)
	{
		for (int j = 0; j < imgOri.rows; j++)
		{
			bOri = imgOri.at<Vec3b>(j, i)[0];
			gOri = imgOri.at<Vec3b>(j, i)[1];
			rOri = imgOri.at<Vec3b>(j, i)[2];

			if (a >= 0)
			{
				imgOut.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(a*bOri + b);
				imgOut.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(a*gOri + b);
				imgOut.at<Vec3b>(j, i)[2] = saturate_cast<uchar>(a*rOri + b);
			}
			else
			{
				imgOut.at<Vec3b>(j, i)[0] = saturate_cast<uchar>(255 + a*bOri + b);
				imgOut.at<Vec3b>(j, i)[1] = saturate_cast<uchar>(255 + a*gOri + b);
				imgOut.at<Vec3b>(j, i)[2] = saturate_cast<uchar>(255 + a*rOri + b);
			}

		}
	}

	namedWindow("output");
	imshow("output", imgOut);
}