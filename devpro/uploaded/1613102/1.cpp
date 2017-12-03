
#include<iostream>
#include<time.h>
#include<fstream>
#include<string>
#include<sstream>
#include <iostream>
#include<fstream>
#include "1.h"
using namespace std;


void print(int *a, int n) {
	for (int i = 0; i<n; i++)
		cout << a[i] << " ";
	cout << endl;
}


int main()
{

	int dem = 0;
	int a[50];
	ifstream in;
	in.open("input1.txt");
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
		cout << a[k]<< endl;
	}
	cout << endl;
	ofstream out;
	out.open("output1.txt");
	BubbleSort(a,dem);
	for (int k = 0; k <= dem - 1; k++)
	{
		out << a[k] << endl;
	}
	
	out<<endl<<endl;

	out.close();
	//print(a, dem);
	//system("pause");
	return 0;
}