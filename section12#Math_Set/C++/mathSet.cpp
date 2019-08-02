#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


#define BONE_PIC	"Fig0232(a)(partial_body_scan).tif"

using namespace std;
using namespace cv;

static bool complementSetTest(Mat imgOri);

int main()
{
	Mat imgBoneOri = imread(BONE_PIC);
	if (imgBoneOri.empty())
	{
		cout << "Cannot find this picture!" << endl;
		return false;
	}
	imshow("boneOri", imgBoneOri);


	complementSetTest(imgBoneOri);

	waitKey(0);
	return true;
}

static bool complementSetTest(Mat imgOri)
{
	Mat imgOut(imgOri.size(), imgOri.type(), Scalar(255, 255, 255));
	if (CV_8UC3 != imgOri.type())
	{
		cout << "Can not match this picture type!" << endl;
		return false;
	}

	//imgOut = imgOut - imgOri;

	bitwise_not(imgOri, imgOut);

	imshow("imgBoneCom", imgOut);
	imwrite("BoneCom.png", imgOut);

	return true;
}