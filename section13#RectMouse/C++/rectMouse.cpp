#include "pch.h"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//宏定义
#define NOT_WINDOWS	"NOT"
#define AND_WINDOWS	"AND"
#define OR_WINDOWS	"OR"

//全局变量
Rect gNotRectangle;
Rect gOrRectangle;
Rect gAndRectanglePre;
Rect gAndRectangleNew;
Rect gAndRectangle;
bool gbNotDrawingBox = false;
bool gbOrDrawingBox = false;
bool gbAndDrawingBox = false;

//Functions List
static void showRectPara(String name, Rect rect);
static void notMouseCallBall(int event, int x, int y, int flags, void* param);
static void orMouseCallBall(int event, int x, int y, int flags, void* param);
static void andMouseCallBall(int event, int x, int y, int flags, void* param);
static void drawRectWindow(Mat img, Rect rect);
static void rectTest();

int main()
{
	Mat imgBackGround = Mat::zeros(600, 800, CV_8UC1);
	Mat notImgTemp = imgBackGround.clone();
	Mat orImgTemp = imgBackGround.clone();
	Mat andImgTemp = imgBackGround.clone();

	gNotRectangle = Rect(-1, -1, 0, 0);
	gOrRectangle = Rect(-1, -1, 0, 0);
	gAndRectanglePre = Rect(-1, -1, 0, 0);
	gAndRectangleNew = Rect(-1, -1, 0, 0);
	gAndRectangle = Rect(-1, -1, 0, 0);

	namedWindow(NOT_WINDOWS);
	namedWindow(OR_WINDOWS);
	namedWindow(AND_WINDOWS);


	imshow(NOT_WINDOWS, imgBackGround);
	imshow(OR_WINDOWS, imgBackGround);
	imshow(AND_WINDOWS, imgBackGround);

	setMouseCallback(NOT_WINDOWS, notMouseCallBall, (void*)&notImgTemp);
	setMouseCallback(OR_WINDOWS, orMouseCallBall, (void*)&orImgTemp);
	setMouseCallback(AND_WINDOWS, andMouseCallBall, (void*)&andImgTemp);


	//rectTest();


	while (true)
	{
		if (gbNotDrawingBox)
		{
			drawRectWindow(notImgTemp, gNotRectangle);
			imshow(NOT_WINDOWS, notImgTemp);
		}
		if (gbOrDrawingBox)
		{
			drawRectWindow(orImgTemp, gOrRectangle);
			imshow(OR_WINDOWS, orImgTemp);
		}
		if (gbAndDrawingBox)
		{
			drawRectWindow(andImgTemp, gAndRectangleNew);
			imshow(AND_WINDOWS, andImgTemp);
		}
		if (waitKey(10) == 27)
		{
			break;
		}
	}
	
	waitKey(0);

	return true;
}


static void notMouseCallBall(int event, int x, int y, int flags, void* param)
{
	Mat& img = *(Mat*)param;
	Mat notImgTemp = img.clone();

	int deltaX, deltaY;
	static int oriX = 0;
	static int oriY = 0;

	switch (event)
	{
	case EVENT_MOUSEMOVE:
		if (gbNotDrawingBox)
		{
			deltaX = x - oriX;
			deltaY = y - oriY;

			if (deltaX >= 0 && deltaY >= 0)
			{
				gNotRectangle = Rect(oriX, oriY, deltaX, deltaY);
			}
			else if (deltaX >= 0 && deltaY < 0)
			{
				gNotRectangle = Rect(oriX, y, deltaX, -1*deltaY);
			}
			else if (deltaX < 0 && deltaY >= 0)
			{
				gNotRectangle = Rect(x, oriY, -1*deltaX, deltaY);
			}
			else if (deltaX < 0 && deltaY < 0)
			{
				gNotRectangle = Rect(x, y, -1*deltaX, -1 * deltaY);
			}
		}
		break;

	case EVENT_LBUTTONDOWN:
		gbNotDrawingBox = true;
		oriX = x;
		oriY = y;
		img = 0;
		//gNotRectangle = Rect(x, y, 0, 0);

		break;

	case EVENT_LBUTTONUP:
		gbNotDrawingBox = false;
		bitwise_not(img, notImgTemp);
		//notImgTemp.copyTo(img);
		imshow(NOT_WINDOWS, notImgTemp);
		break;

	default:
		break;
	}

}

static void andMouseCallBall(int event, int x, int y, int flags, void* param)
{
	int deltaX, deltaY;
	static int oriX = 0;
	static int oriY = 0;
	static bool isInit = false;
	Mat& img = *(Mat*)param;

	switch (event)
	{
	case EVENT_MOUSEMOVE:
		if (gbAndDrawingBox)
		{
			deltaX = x - oriX;
			deltaY = y - oriY;

			if (deltaX >= 0 && deltaY >= 0)
			{
				gAndRectangleNew = Rect(oriX, oriY, deltaX, deltaY);
			}
			else if (deltaX >= 0 && deltaY < 0)
			{
				gAndRectangleNew = Rect(oriX, y, deltaX, -1 * deltaY);
			}
			else if (deltaX < 0 && deltaY >= 0)
			{
				gAndRectangleNew = Rect(x, oriY, -1 * deltaX, deltaY);
			}
			else if (deltaX < 0 && deltaY < 0)
			{
				gAndRectangleNew = Rect(x, y, -1 * deltaX, -1 * deltaY);
			}
		}
		break;

	case EVENT_LBUTTONDOWN:
		gbAndDrawingBox = true;
		oriX = x;
		oriY = y;
		gAndRectangleNew = Rect(x, y, 0, 0);
		break;

	case EVENT_LBUTTONUP:
		cout << "isInit: " << isInit << endl;
		if (false == isInit)
		{
			gAndRectanglePre = gAndRectangleNew;
			isInit = true;
		}
		gbAndDrawingBox = false;

		gAndRectangle = gAndRectangleNew & gAndRectanglePre;
		showRectPara("Pre", gAndRectanglePre);
		showRectPara("New", gAndRectangleNew);
		showRectPara("Rect", gAndRectangle);


		gAndRectanglePre = gAndRectangle;
		img = 0;
		drawRectWindow(img, gAndRectangle);
		imshow(AND_WINDOWS, img);

		break;

	default:
		break;
	}

}
static void orMouseCallBall(int event, int x, int y, int flags, void* param)
{
	int deltaX, deltaY;
	static int oriX = 0;
	static int oriY = 0;

	switch (event)
	{
	case EVENT_MOUSEMOVE:
		if (gbOrDrawingBox)
		{
			deltaX = x - oriX;
			deltaY = y - oriY;

			if (deltaX >= 0 && deltaY >= 0)
			{
				gOrRectangle = Rect(oriX, oriY, deltaX, deltaY);
			}
			else if (deltaX >= 0 && deltaY < 0)
			{
				gOrRectangle = Rect(oriX, y, deltaX, -1 * deltaY);
			}
			else if (deltaX < 0 && deltaY >= 0)
			{
				gOrRectangle = Rect(x, oriY, -1 * deltaX, deltaY);
			}
			else if (deltaX < 0 && deltaY < 0)
			{
				gOrRectangle = Rect(x, y, -1 * deltaX, -1 * deltaY);
			}
		}
		break;

	case EVENT_LBUTTONDOWN:
		gbOrDrawingBox = true;
		oriX = x;
		oriY = y;
		//gOrRectangle = Rect(x, y, 0, 0);
		break;

	case EVENT_LBUTTONUP:
		gbOrDrawingBox = false;
		break;

	default:
		break;
	}

}

static void drawRectWindow(Mat img, Rect rect)
{
	rectangle(img, rect, Scalar(255), -1);
}


static void rectTest()
{
	Mat imgBackGround = Mat::zeros(600, 800, CV_8UC1);
	Rect test1(100, 100, 200, 200);
	Rect test2(200, 200, 200, 200);
	Rect test3 = test1 | test2;
	Rect test4 = test1 & test2;
	showRectPara("test1", test1);
	showRectPara("test2", test2);
	showRectPara("test1 | test2", test3);
	showRectPara("test1 & test2", test4);
	drawRectWindow(imgBackGround, test4);
	namedWindow("Test");
	imshow("Test", imgBackGround);
}


static void showRectPara(String name, Rect rect)
{
	static uint number = 0;
	cout << "\n-------------------" << name << "-" << number << "--------------------" << endl;
	cout << "area: " << rect.area() << endl;
	cout << "size: " << rect.size() << endl;
	cout << "TopLeft: " << rect.tl() << endl;
	cout << "BottomRight: " << rect.br() << endl;
	cout << "width: " << rect.width << endl;
	cout << "height " << rect.height << endl;
	number++;
 }