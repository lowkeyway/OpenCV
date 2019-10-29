#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#define ORIGINAL_WIN	"Original"
#define	ERO_DIL_WIN		"ErodeDilate"
#define OPEN_CLOSE_WIN	"OpenClose"
#define TOP_BLACK_WIN	"TopBlack"

#define	ERO_DIL_BAR		"ErodeDilateBar"
#define OPEN_CLOSE_BAR	"OpenCloseBar"
#define TOP_BLACK_BAR	"TopBlackBar"

#define TH_MAX			10

using namespace std;
using namespace cv;

static bool morphologyTest(Mat imgOri);
static void OpenCloseCallBack(int, void*);
static void TopBlackCallBack(int, void*);
static void ErodeDilateCallBack(int, void*);
static bool changeEleShape();

Mat imgOri;
int g_iStructuringElement = MORPH_RECT;

int g_iOpenCloseTh = 5;
int g_iTopBlackTh = 5;
int g_iErodeDilateTh = 5;

int main()
{
	imgOri = imread("HappyFish.jpg");
	if (imgOri.empty())
	{
		cout << "Cannot load this picture!" << endl;
		return false;
	}

	morphologyTest(imgOri);

	//waitKey(0);
	return true;
}

bool morphologyTest(Mat imgOri)
{
	namedWindow(ORIGINAL_WIN);
	namedWindow(ERO_DIL_WIN);
	namedWindow(OPEN_CLOSE_WIN);
	namedWindow(TOP_BLACK_WIN);
	imshow(ORIGINAL_WIN, imgOri);

	createTrackbar(ERO_DIL_BAR, ERO_DIL_WIN, &g_iErodeDilateTh, TH_MAX, ErodeDilateCallBack);
	createTrackbar(OPEN_CLOSE_BAR, OPEN_CLOSE_WIN, &g_iOpenCloseTh, TH_MAX, OpenCloseCallBack);
	createTrackbar(TOP_BLACK_BAR, TOP_BLACK_WIN, &g_iTopBlackTh, TH_MAX, TopBlackCallBack);

	do {
		OpenCloseCallBack(g_iOpenCloseTh, nullptr);
		TopBlackCallBack(g_iTopBlackTh, nullptr);
		ErodeDilateCallBack(g_iErodeDilateTh, nullptr);
	} while (changeEleShape());

	return true;
}


static void OpenCloseCallBack(int, void*)
{
	Mat imgOpenClose;
	int diff = g_iOpenCloseTh - TH_MAX / 2;
	int size = diff < 0 ? -diff : diff;
	Mat kernel = getStructuringElement(g_iStructuringElement, Size(2 * size + 1, 2 * size + 1));
	if (diff < 0)
	{
		morphologyEx(imgOri, imgOpenClose, MORPH_OPEN, kernel);
	}
	else
	{
		morphologyEx(imgOri, imgOpenClose, MORPH_CLOSE, kernel);
	}

	imshow(OPEN_CLOSE_WIN, imgOpenClose);
}

static void TopBlackCallBack(int, void*)
{
	Mat imgTopBlack;
	int diff = g_iTopBlackTh - TH_MAX / 2;
	int size = diff < 0 ? -diff : diff;
	Mat kernel = getStructuringElement(g_iStructuringElement, Size(2*size+1, 2 * size + 1));
	if (diff < 0)
	{
		morphologyEx(imgOri, imgTopBlack, MORPH_TOPHAT, kernel);
	}
	else
	{
		morphologyEx(imgOri, imgTopBlack, MORPH_BLACKHAT, kernel);
	}

	imshow(TOP_BLACK_WIN, imgTopBlack);
}

static void ErodeDilateCallBack(int, void*)
{
	Mat imgErodeDilate;
	int diff = g_iErodeDilateTh - TH_MAX/2;
	int size = diff < 0 ? -diff : diff;
	Mat kernel = getStructuringElement(g_iStructuringElement, Size(2 * size + 1, 2 * size + 1));
	if (diff < 0)
	{
		morphologyEx(imgOri, imgErodeDilate, MORPH_DILATE, kernel);
	}
	else
	{
		morphologyEx(imgOri, imgErodeDilate, MORPH_ERODE, kernel);
	}

	imshow(ERO_DIL_WIN, imgErodeDilate);
}

static bool changeEleShape()
{
	int ch;

	while (true)
	{
		ch = waitKey(0);

		if (ch == 27)
		{
			return false;
		}

		if ((char)ch == ' ')
		{
			g_iStructuringElement = (g_iStructuringElement + 1) % 3;
			switch (g_iStructuringElement)
			{
			case MORPH_RECT:
				cout << "MORPH_RECT" << endl;
				break;
			case MORPH_CROSS:
				cout << "MORPH_CROSS" << endl;
				break;
			case MORPH_ELLIPSE:
				cout << "MORPH_ELLIPSE" << endl;
				break;
			default:
				break;
			}
			return true;
		}
	}

	return false;
}