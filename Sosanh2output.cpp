#include<iostream>
#include<time.h>
#include<fstream>
#include<string>
#include<sstream>
#include <iostream>
#include<fstream>
using namespace std;

void compareOutput(){
	int dem = 0;
	int a[50];
	// so luong phan tu cua dap an
	ifstream in1;
	in1.open("outputda1.txt");
	while (!in1.eof())
	{
		string temp1 = "";
		getline(in1, temp1);
		dem = dem + 1;
	}
	in1.close();
	/////////////////////////////
	ifstream in1_1;
	in1_1.open("outputda1.txt");///doc DA giam khao
	ifstream in2;
	in2.open("output1.txt");//doc DA thi sinh
	int dung = 0;
	for (int k = 0; k <= dem - 2; k++)
	{
		string temp1 = "";
		getline(in1_1, temp1);
		string temp2 = "";
		getline(in2, temp2);
		if (temp1 == temp2)//so DA
		{
			dung = dung + 1;///// dem so luong kq dung
		}
	}
	in1_1.close();
	in2.close();
	double diem =  (dung)/(double) (dem-1);//tan so dung
	ofstream out;
	out.open("scoreofPercent.txt", ios_base::app);
	out << "so diem dung:" << diem;/// luu tan so dung vao file output
	out.close();
	//print(a, dem);
	return;
}