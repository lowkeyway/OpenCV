#ifndef __PIC2DIG_H__
#define __PIC2DIG_H__

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class pic2file
{
public:
	pic2file(int rows = 0, int cols = 0);
	~pic2file();
	bool save2file(string fileName);
private:
	unsigned char* picBuffer;
	int m_iRows;
	int m_iCols;
};

pic2file::pic2file(int rows, int cols)
{
	m_iRows = rows;
	m_iCols = cols;
	picBuffer = new unsigned char[m_iCols * m_iRows];
}

pic2file::~pic2file()
{
	m_iRows = 0;
	m_iCols = 0;
	delete[]picBuffer;
	picBuffer = NULL;
}

bool pic2file::save2file(string fileName)
{
	return true;
}

#endif // !__PIC2DIG_H__
