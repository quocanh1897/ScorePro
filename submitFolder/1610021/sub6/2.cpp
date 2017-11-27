#include<iostream>
#include<time.h>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;
int main()
{
	
	int dem = 0;
	int a[50];
	ifstream in;
	in.open("input2.txt");
	while (!in.eof())
	{
		string temp = "";
		int t = 0;
		getline(in, temp);
		stringstream ss(temp);
		ss >> a[dem];
		dem = dem + 1;
	}
	in.close();
	for (int k = 0; k <= dem - 1; k++)
	{
		cout << a[k]+1 << endl;
	}
	//cout << endl;
	ofstream out;
	out.open("output2.txt")
	for (int k = 0; k <= dem - 1; k++)
	{
		int tich = 1;
		if (a[k] == 0) tich = 0;
		else
		{
			for (int m = 1; m <= a[k]; m++)
			{
				tich = tich*m;
			}
		}
	//	cout << tich << endl;
		out << tich << endl;
	}
	out.close();
	//system("pause");
	return 0;
}