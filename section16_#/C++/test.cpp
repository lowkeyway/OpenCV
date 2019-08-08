#include <iostream>
#include <string>
#include <vector>

using namespace std;

static void showVector(vector<string> vFoo)
{
	for (vector<string>::iterator it = vFoo.begin(); it != vFoo.end(); it++)
	{
		cout << *it << endl;
	}
}

int test()
{
	string foo[] = { "B", "G", "R" };
	vector<string> vFoo;
	vector<string> vFoo2;
	for (int i = 0; i < 3; i++)
	{
		vFoo.push_back(foo[i]);
	}

	vFoo2 = vFoo;
	showVector(vFoo);

	vFoo[0] = "H";
	showVector(vFoo);

	showVector(vFoo2);

	system("pause");
	return true;
}