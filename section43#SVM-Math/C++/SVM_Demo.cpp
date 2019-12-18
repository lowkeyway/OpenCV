#include <stdio.h>  
#include <time.h>  
#include <iostream> 
#include <io.h>
#include <direct.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

#define ORIGINAL_PIC_NAME	"digits.png"
#define	SPLIT_BLOCK_SIZE	20
#define ROOT_PATH			"./data/"
#define TRAIN_PATH			"train/"
#define TEST_PATH			"test/"
#define SVM_XML				"svm.xml"

#define NEED_SPLIT			1
#define NEED_TRAIN			1
#define NEED_PREDICT		1

static int	createDirectory(string path);
static bool splitPic(string rootPath, Mat imgOri);
static void getFiles(string path, vector<string>& files);
static bool svmTrain(string dataPath, string saveFile);
static bool svmPredict(string dataPath, string loadFile, int expectVaule);

int main()
{
	bool ret = true;
	string rootPath = ROOT_PATH;

	Mat imgOri = imread(ORIGINAL_PIC_NAME);
	if (imgOri.empty())
	{
		cout << "Cannot load this picture!" << endl;
		ret = false;
		goto out;
	}
	if (NEED_SPLIT)
	{
		cout << "Begain split pictures..." << endl;
		ret = splitPic(rootPath, imgOri);
		if (true != ret)
		{
			cout << "Split Image Fail!" << endl;
			goto out;
		}
	}

	if (NEED_TRAIN)
	{
		cout << "Begain train pictures..." << endl;
		ret = svmTrain("./data/train/", SVM_XML);
		if (true != ret)
		{
			cout << "SVM Train Fail!" << endl;
			goto out;
		}
	}

	if (NEED_PREDICT)
	{
		cout << "Begain predict pictures..." << endl;
		ret = svmPredict("./data/test/", SVM_XML, 0);
		if (true != ret)
		{
			cout << "SVM Predict Fail!" << endl;
			goto out;
		}
	}
	
out:
	getchar();
	return ret;
}



static int createDirectory(string path)
{
	int len = path.length();
	char tmpDirPath[256] = { 0 };
	for (int i = 0; i < len; i++)
	{
		tmpDirPath[i] = path[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			if (_access(tmpDirPath, 0) == -1)
			{
				int ret = _mkdir(tmpDirPath);
				if (ret == -1) return ret;
			}
		}
	}
	return true;
}

static bool splitPic(string rootPath, Mat imgOri)
{
	string trainFilePath, testFilePath;
	string trainFileName, testFileName;
	int  filename = 0, filenum = 0;

	Mat gray;
	cvtColor(imgOri, gray, COLOR_BGR2GRAY);

	int b = SPLIT_BLOCK_SIZE;
	int m = gray.rows / b;   //Oroginal Picture size 1000*2000
	int n = gray.cols / b;   //Split to m * n blocks, size of every block is SPLIT_BLOCK_SIZE * SPLIT_BLOCK_SIZE
	bool ret = 1;

	for (int i = 0; i < m; i++)
	{
		int offsetRow = i * b;  //offset of Row
		if (i % 5 == 0 && i != 0)
		{
			filename++;
			filenum = 0;
		}
		for (int j = 0; j < n; j++)
		{
			int offsetCol = j * b; //offset of Column
			trainFilePath = rootPath + TRAIN_PATH + to_string(filename) + "/";
			testFilePath  = rootPath + TEST_PATH + to_string(filename) + "/";
			ret = createDirectory(trainFilePath);
			if (true != ret)
			{
				cout << "Cannot creat trainFilePath!" << endl;
				return ret;
			}
			ret = createDirectory(testFilePath);
			if (true != ret)
			{
				cout << "Cannot creat testFilePath!" << endl;
				return ret;
			}
			if (filenum < 400)
			{
				trainFileName = trainFilePath + to_string(filenum++) + ".jpg";
				Mat tmp;
				gray(Range(offsetRow, offsetRow + b), Range(offsetCol, offsetCol + b)).copyTo(tmp);
				ret = imwrite(trainFileName, tmp);
				if (true != ret)
				{
					cout << "Cannot write train image!" << endl;
					return ret;
				}
			}
			else
			{
				testFileName = testFilePath + to_string(filenum++) + ".jpg";
				Mat tmp;
				gray(Range(offsetRow, offsetRow + b), Range(offsetCol, offsetCol + b)).copyTo(tmp);
				ret = imwrite(testFileName, tmp);
				if (true != ret)
				{
					cout << "Cannot write test image!" << endl;
					return ret;
				}
			}

		}
	}
	cout << "SVM Split Finish!!!" << endl;

	return true;
}

static void getFiles(string path, vector<string>& files)
{
	__int64   hFile = 0;
	struct __finddata64_t fileinfo;
	string p;
	if ((hFile = _findfirst64(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("/").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext64(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

static bool svmSetTrainLabel(Mat& trainingImages, vector<int>& trainingLabels, string trainDataPath, int trainLabel)
{
	if (_access(trainDataPath.c_str(), 0) == -1)
	{
		cout << "Cannot find this folder!" << endl;
		return false;
	}

	vector<string> files;
	getFiles(trainDataPath, files);
	int number = files.size();
	for (int i = 0; i < number; i++)
	{
		Mat  SrcImage = imread(files[i].c_str());
		SrcImage = SrcImage.reshape(1, 1);
		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(trainLabel);
	}

	return true;
}

static bool svmTrain(string dataPath, string saveFile)
{
	Mat classes;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;
	int ret = 1, i = 0;
	cout << "setting label..." << endl;

	for (i = 0; i < 10; i++)
	{
		ret = svmSetTrainLabel(trainingImages, trainingLabels, dataPath + to_string(i), i);
		if (true != ret)
		{
			printf("svmSetTrainLabel: %d Fail!", i);
			return ret;
		}
	}

	Mat(trainingImages).copyTo(trainingData);
	trainingData.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);

	Ptr<ml::SVM> svm;
	cout << "training SVM..." << endl;
	svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::LINEAR);
	svm->setDegree(0);
	svm->setGamma(1);
	svm->setCoef0(0);
	svm->setC(1);
	svm->setNu(0);
	svm->setP(0);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.01));

	svm->train(trainingData, ROW_SAMPLE, classes);
	svm->save(saveFile);
	cout << "SVM Train Finish!!!" << endl;

	return true;
}


static bool svmPredict(string dataPath, string loadFile, int expectVaule)
{
	int result = 0;
	string filePath = dataPath + to_string(expectVaule);

	if (_access(filePath.c_str(), 0) == -1)
	{
		cout << "Cannot find this folder!" << endl;
		return false;
	}

	vector<string> files;
	getFiles(filePath, files);
	int number = files.size();
	Ptr<ml::SVM> svm;

	FileStorage svm_fs(loadFile, FileStorage::READ);
	if (svm_fs.isOpened())
	{
		svm = StatModel::load<SVM>(loadFile);
	}
	else
	{
		cout << "Cannot find this file!" << endl;
		return false;
	}
	for (int i = 0; i < number; i++)
	{
		Mat inMat = imread(files[i].c_str());
		Mat p = inMat.reshape(1, 1);
		p.convertTo(p, CV_32FC1);
		int response = (int)svm->predict(p);
		if (response == expectVaule)
		{
			result++;
		}
		else
		{
			cout << "The " << i << "-th image predict fail, expect: [" << expectVaule << "], predict: [" << response << "]" << endl;
			return false;
		}

	}
	cout << "All files predict Success!!!" << result << endl;

	return  true;
}