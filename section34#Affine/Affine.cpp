#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


#define IMG_ORI		"imgOri"
#define IMG_AFF		"imgAff"
#define IMG_ROT_ORI	"imgRotOri"
#define IMG_ROT_DST	"imgRotDst"

using namespace std;
using namespace cv;

static bool affineTest(Mat imgOri);
static bool rotationTest(Mat imgOri);

int main(int *arg, int**argv)
{
	Mat imgOri = imread("starry_night.jpg");
	if (imgOri.empty())
	{
		cout << "Cannot load this picture!" << endl;
		return false;
	}
	affineTest(imgOri);
	waitKey(0);
	return true;
}


static bool affineTest(Mat imgOri)
{
	imshow(IMG_ORI, imgOri);

	Point2f pointSrc[3];
	Point2f pointDst[3];

	Mat imgSrc, imgDst;
	Mat warpMat;

	imgSrc = imgOri.clone();
	imgDst = Mat::zeros(imgOri.rows, imgOri.cols, imgOri.type());

	pointSrc[0] = Point2f(0, 0);
	pointSrc[1] = Point2f(static_cast<float>(imgSrc.cols - 1), 0);
	pointSrc[2] = Point2f(0, static_cast<float>(imgSrc.rows - 1));

	pointDst[0] = Point2f(static_cast<float>(imgOri.cols * 0.0), static_cast<float>(imgOri.rows * 0.33));
	pointDst[1] = Point2f(static_cast<float>(imgOri.cols * 0.8), static_cast<float>(imgOri.rows * 0.33));
	pointDst[2] = Point2f(static_cast<float>(imgOri.cols * 0.15), static_cast<float>(imgOri.rows * 0.78));

	warpMat = getAffineTransform(pointSrc, pointDst);
	warpAffine(imgSrc, imgDst, warpMat, imgDst.size());

	imshow(IMG_AFF, imgDst);

	rotationTest(imgDst);

	return true;
}

static bool rotationTest(Mat imgOri)
{
	imshow(IMG_ROT_ORI, imgOri);

	Point center = Point(imgOri.cols / 2, imgOri.rows / 2);
	Mat imgRotation = Mat::zeros(imgOri.cols, imgOri.rows, imgOri.type());
	double angle = -30.0;
	double scale = 0.8;

	Mat rotMat = getRotationMatrix2D(center, angle, scale);
	warpAffine(imgOri, imgRotation, rotMat, imgOri.size());

	imshow(IMG_ROT_DST, imgRotation);

	return true;
}