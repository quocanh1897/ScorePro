#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include "tinyxml.h"
#include "BKavltree.h"
#include <ctime>
#include<fstream>
#include<sstream>
#include<stack>
#include<algorithm>
#include<thread>

using namespace boost::filesystem;

//------------->>>>  PROTOTYPE  <<<<-------------//

bool DeleteSubFolder(avlTree &Data, path workingDir, string ID, string sub);
bool CopyfileStoW(path workingDir, path submitFol, string ID, string sub);

bool checkID(avlTree* dataID, path submitFolder, path workingDir);
bool CreateXML(path submitfolder, string ID, string sub);

void exportScore(path workingDir, avlTree* dataIN);
int compileFile(path FolderWD, string ID, string sub);
void scoreOutput(path workingDir, string ID, string fileToScore, int subNumber, int numTestcase);
void runThenScoreFileSub(path workingDir, string ID, avlTree* dataID, int numOfSubIn,int count);
void scoreSub(path workingDir, string ID, int subNumber, avlTree* dataIn);


//------------->>>>  IMPLEMENT <<<<-------------//

bool DeleteSubFolder(avlTree &Data, path workingDir, string ID, string sub) {
	if(Data.search(ID)) Data.search(ID)->numberSub--;
	path subfolder = workingDir / ID / sub;
	for (directory_iterator file(subfolder); file != directory_iterator(); ++file) {
		string filename = file->path().filename().string();//lay ra ten file
		path temp = subfolder / filename;
		remove(temp);
	}
	remove(workingDir / ID / sub);
	return true;
}

int compileFile(path FolderWD, string ID, string sub) {
	create_directory(FolderWD / ID / sub / "build");
	int count = 0;
	while (1) {
		count++;
		string makefile = "makefile" + to_string(count);
		if (exists(FolderWD / "testcase" /makefile)) {
			copy_file(FolderWD / "testcase" / makefile, FolderWD / ID / sub / makefile);
			string cmdstr = "pushd " + (FolderWD / ID / sub).string() + "&& make -f " + makefile + " buildcpp";
			system(cmdstr.c_str());
		}
		else break;
	}
	return count - 1;
}

bool CopyfileStoW(path workingDir, path submitFol,string ID,string sub) {
	bool flag = false;//da tim thay quantity chua
	int quantity = 0;//so luong file
	int count = 0;//dem
	path IDFolderInWorkingDir = workingDir /ID/sub;
	path IDFolderInSubmitFol = submitFol /ID/sub;
		//this is a brand new IDFolder
	create_directory(workingDir / ID);
	create_directory(workingDir / ID/sub);
	for (directory_iterator file(IDFolderInSubmitFol);file != directory_iterator(); ++file) {
		//dem file da duoc doc sang
		count++;
		string curFileName = file->path().filename().string();
		path desFileName = workingDir /ID/sub/curFileName;
		if (!exists(desFileName) ||
			exists(desFileName) &&
			file_size(desFileName) != file_size(file->path())) {
			copy(file->path(), workingDir / ID/sub/ curFileName);
		}


		//tim so luong file
		if (!curFileName.compare("pro.xml")) {
			//chuyen string ve char* de dung ham doc
			char * writable = new char[desFileName.string().size() + 1];
			for (int i = 0; i < desFileName.string().length()+1; i++) {
				if (i == desFileName.string().length()) 
					writable[i] = '\0';
				else 
					writable[i] = desFileName.string()[i];
				
			}
			TiXmlDocument doc(writable);
			if (!doc.LoadFile())
			{
				//printf("%s", doc.ErrorDesc());
				return 0;
			}
			TiXmlElement* root = doc.RootElement();
			TiXmlElement* child1 = root->FirstChildElement();
			while (child1) {
				string temp = "quantity";
				if (!temp.compare(child1->ValueTStr().c_str()))
				{
					quantity = atoi(child1->GetText());
					flag = true;
					if (quantity == count&&flag) return true;
					break;
				}
				child1 = child1->NextSiblingElement();
			}		
		}
		if (quantity == count&&flag) return true;
	}
	return false;
}
bool Replacefile(path IDFolder, string newsub) {
	bool flag = false;//da tim thay quantity chua
	int quantity = 0;//so luong file
	int count = 0;//dem
	//xet xem co phai thu muc hoan toan la folder khong
	int isAllDir = true;
	for (directory_iterator file(IDFolder); file != directory_iterator(); ++file) {
		string curFileName = file->path().filename().string();
		path desFileName =	newsub / curFileName;
		//dem file
		if (is_directory(file->path())) continue;
		else isAllDir = false;//neu k phai folder -> false;
		count++;
		if (exists(newsub / curFileName)) return true;
		copy_file(file->path(),newsub/curFileName);
		
		//tim so luong file
		if (!curFileName.compare("pro.xml")) {
			//chuyen string ve char* de dung ham doc
			char * writable = new char[desFileName.string().size() + 1];
			for (int i = 0; i < desFileName.string().length() + 1; i++) {
				if (i == desFileName.string().length())
					writable[i] = '\0';
				else
					writable[i] = desFileName.string()[i];

			}
			remove(file->path());
			TiXmlDocument doc(writable);
			if (!doc.LoadFile())
			{
				//printf("%s", doc.ErrorDesc());
				return 0;
			}
			TiXmlElement* root = doc.RootElement();
			TiXmlElement* child1 = root->FirstChildElement();
			while (child1) {
				string temp = "quantity";
				if (!temp.compare(child1->ValueTStr().c_str()))
				{
					quantity = atoi(child1->GetText());
					flag = true;
					if (quantity == count&&flag) return true;
					break;
				}
				child1 = child1->NextSiblingElement();
			}
		}
		remove(file->path());
		if (quantity == count&&flag) return true;
	}
	if (isAllDir) return true;
	return false;
}
bool CreateXML(path submitfolder, string ID, string sub) {

	//tao file xml
	path subfile = submitfolder / ID / sub;
	path exist = subfile / "pro.xml";
	if (exists(exist)) return 0;
	TiXmlDocument doc;
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc.LinkEndChild(dec);
	//root
	TiXmlElement* root = new TiXmlElement("Source");
	doc.LinkEndChild(root);

	int i = 0;
	//file

	for (directory_iterator file(subfile); file != directory_iterator(); ++file) {
		string namefile = file->path().filename().string();//lay ra ten ID
		string str = namefile.substr(namefile.find(".") + 1, namefile.length() - 1);
		const char* duoi = str.c_str();
		const char* file_name = namefile.c_str();
		TiXmlElement* child = new TiXmlElement(duoi);
		TiXmlText *file_name_text = new TiXmlText(file_name);
		root->LinkEndChild(child);
		child->LinkEndChild(file_name_text);
		i++;
	}
	{
		TiXmlElement* child = new TiXmlElement("xml");
		TiXmlText *file_name_text = new TiXmlText("pro.xml");
		root->LinkEndChild(child);
		child->LinkEndChild(file_name_text);
	}
	{
		TiXmlElement* quantity = new TiXmlElement("quantity");
		root->LinkEndChild(quantity);
		TiXmlText *quantity_num = new TiXmlText(to_string(i + 1).c_str());
		quantity->LinkEndChild(quantity_num);
	}
	path xmlfile = subfile / "pro.xml";
	string link = xmlfile.string();
	const char* s1 = link.c_str();
	//char* temp = new char(xmlfile.string().length() + 1);
	//memcpy(temp, s0, xmlfile.string().length() + 1);
	doc.SaveFile(s1);

	return 1;
}

bool checkID(avlTree* dataID, path submitFolder, path workingDir) {

	//duyet tuan tu file submitFolder
	for (directory_iterator file(submitFolder); file != directory_iterator(); ++file) {
		string IDNameFolder = file->path().filename().string();//lay ra ten ID
				
															//ID bo vao database neu chua co
		node* IDNode = dataID->search(IDNameFolder);	
		int count = 0;
		if (!IDNode)
			dataID->AVLInsert(dataID->root, IDNode = new node(file->path().filename().string(), 0), dataID->taller);

		//duyet tuan tu file sub
		for (directory_iterator filesub(submitFolder / IDNameFolder); filesub != directory_iterator(); ++filesub) {
			//bool error = false;
			
			//kiem tra xem co phai la folder k
			if (!is_directory(filesub->path())) continue;


			//dem bao nhieu file sub
			count++;
			if (count > dataID->search(IDNameFolder)->numberSub) {
				if (IDNode->isLoading) break;//neu fileID dang load thi di ra ngoai
				dataID->search(IDNameFolder)->numberSub++;
				string ID = IDNameFolder;
				int numOfSub = dataID->search(IDNameFolder)->numberSub;
				string sub = "sub" + to_string(dataID->search(IDNameFolder)->numberSub);
				//double starttime = clock();
				//CreateXML(submitFolder, ID, sub);
				

				//copy den khi nao du file thi thoi nho` vao quantity trong xml
				while (1) {
					if (CopyfileStoW(workingDir, submitFolder, ID, sub) == true) break;
				}
				//if (error) break;
				if (exists(workingDir / ID / sub / "build")) return 0;
				int count=compileFile(workingDir, ID, sub);
				//--------------------------------//

				runThenScoreFileSub(workingDir, ID, dataID, numOfSub,count);

				exportScore(workingDir, dataID);
			}
		}
	}

	return 1;
}

void reFormatTxt(string pathFile) {

	std::ifstream fin(pathFile);
	std::ofstream fout("temp.txt");
	string s;
	while (getline(fin, s)) { // read a line at a time
		stringstream line(s);
		while (line >> s) { // read the words, eliminate all extra blank space
			fout << s << " ";
		}
		fout << endl; // put the newline back
	}
	fin.close();
	fout.close();

	std::ifstream ifs("temp.txt");
	string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();
	//str.erase(std::remove(str.begin(), str.end(), '\n \n'), str.end());
	std::string::size_type i = 0;
	while (i < str.length()) {
		i = str.find("\n\n", i);
		if (str[str.length() - 1] == '\n')
			str.erase(str.length() - 1, 1);
		if (i == std::string::npos) {
			break;
		}
		str.erase(i, 1);
	}
	std::ofstream ofs(pathFile);
	ofs << str;
	ofs.close();

}

void scoreOutput(path workingDir, string ID, string fileToScore, int subNumber, int numTestcase) {
	int dem = 0;
	int a[50];
	string fileChange = fileToScore.substr(0, fileToScore.find("."));
	//node *numofSub = dataID->search(ID);
	string s = "sub" + to_string(subNumber);
	path pathDaFile = workingDir / ID / s / "build" ;
	string daFile = pathDaFile.string() + "\\outputda" + fileChange + "_" + to_string(numTestcase) + ".txt";
	
	//xoa dong trong o testcase
	reFormatTxt(daFile);
	
	// so luong phan tu cua dap an
	std::ifstream in1;
	//in1.open(daFile);
	in1.open(daFile);
	while (!in1.eof())
	{
		string temp1 = "";
		getline(in1, temp1);
		dem = dem + 1;
	}
	in1.close();
	/////////////////////////////
	std::ifstream in1_1;
	in1_1.open(daFile);//doc DA giam khao
	std::ifstream in2;
	string thisinhFile = pathDaFile.string() + "\\output" + fileChange + "_" + to_string(numTestcase) + ".txt";
	
	reFormatTxt(thisinhFile);

	in2.open(thisinhFile);//doc DA thi sinh
	int dung = 0;
	for (int k = 0; k <= dem - 2; k++)
	{
		string temp1 = "";
		getline(in1_1, temp1);
		string temp2 = "";
		getline(in2, temp2);
		if (temp1 == temp2)//so DA
		{
			dung = dung + 1;// dem so luong kq dung
		}
	}
	in1_1.close();
	in2.close();
	double diem = (dung) / (double)(dem - 1);//tan so dung
	std::fstream out;
	string scoreFile = pathDaFile.string() + "\\scoreOf" + fileChange + ".txt";

	out.open(scoreFile, ios_base::app);
	if (numTestcase == 5) {
		out << "testcase " << numTestcase << ": " << diem * 10 << endl;
		out.close();
		
		//open file to calculate total score
		out.open(scoreFile, ios_base::in);
		float *score = new float[5];
		for (int i = 0; i < 5; i++) {
			string line = "";
			int num;
			getline(out, line);
			
			stringstream tempLine(line);
			tempLine >> line >> num >> line >> score[i];

		}
		out.close();
		out.open(scoreFile, ios_base::app);
		float sum = 0;
		for (int i = 0; i < 5; i++) {
			sum += score[i];
		}
		out << "Total: " << sum / 5;
		out.close();
		return;
	}

	out << "testcase " << numTestcase << ": " << diem * 10 << endl;// luu tan so dung vao file output
	out.close();
	return;
}


void exportScore(path workingDir, avlTree* dataIN) {

	path listSV = workingDir / "listSV.csv";
	string pathListSV = listSV.string();
	std::ofstream myfile;
	myfile.open(pathListSV);
	myfile << "MSSV, So lan nop, Diem cao nhat\n";

	if (dataIN->root == NULL) 
		return;
	
	//traverse avlTree data to find all MSSV
	node *current = dataIN->root;
 
	stack<string> tempstack;
	bool done = 0;

	while (!done)
	{
		if (current != NULL){
			tempstack.push(current->key);
			current = current->left;
		}
		else{
			if (!tempstack.empty())	{
 
				node *t1 = dataIN->search(tempstack.top());
 
				current = t1;//node chua MSSV, sub, score
				tempstack.pop();

				//process here


				string cmdTemp = current->key + "," + to_string(current->numberSub) + "," + to_string(current->scoreHeap->getMax()) + "\n" ;
				myfile << cmdTemp;


				/* we have visited the node and its left subtree.
				Now, it's right subtree's turn */
				current = current->right;
			}
			else
				done = 1;
		}
	} /* end of while */
	myfile.close();
}

 
void scoreSub(path workingDir, string ID, int subNumber, avlTree* dataIn) {
 
	int dem = 0;
	string s = "sub" + to_string(subNumber);
	path pathDaFile = workingDir / ID / s / "build";
	string scoreFile1 = pathDaFile.string() + "\\scoreOf1.txt";
	string scoreFile2 = pathDaFile.string() + "\\scoreOf2.txt";
	string totalScore = pathDaFile.string() + "\\score.txt";

	std::fstream in;
	string line;
	float score1 = 0, score2 = 0;
	//open scoreOf1 to save score of program1
	in.open(scoreFile1, ios_base::in);
	for (int i = 0; i < 5; i++) {
		string temp;
		getline(in, temp);
	}
	getline(in, line);
	stringstream tempLine(line);
	tempLine >> line >> score1;
	in.close();

	//open scoreOf2 to save score of program2
	in.open(scoreFile2, ios_base::in);
	for (int i = 0; i < 5; i++) {
		string temp;
		getline(in, temp);
	}
	getline(in, line);
	stringstream tempLine2(line);
	tempLine2 >> line >> score2;
	in.close();

	std::fstream out;
	out.open(totalScore, ios_base::app);
	out << score1*0.3 + score2*0.7;
	out.close();

	//luu diem vao heap cua rieng tung sinh vien
 
	node *SV = dataIn->search(ID);
 
	SV->scoreHeap->heapInsert(score1*0.3 + score2*0.7);
	return;
}

 
void runThenScoreFileSub(path workingDir, string ID, avlTree* dataID, int numOfSubIn,int count) {
	node *numofSub = dataID->search(ID);
 
	if (numofSub == NULL) {
		return;
	}

	//run numofSub times this function
	for (int i = 0; i < numofSub->numberSub; i++) {
		string s = "sub" + to_string(i + 1);
		path build = workingDir / ID / s / "build";

		path scoreFile = build / "score.txt";
		if (!exists(build) || exists(scoreFile)) {
			continue;
		}

		//run code per objFile
		for (int j = 1; j <= count;j++) {
			for (int numTestcase = 1; numTestcase <= 5; numTestcase++) {
				//copy testcase\input1.txt -> build\input.txt
				string fileChange = to_string(j);
				string inputNum = "testcase\\input" + fileChange + '_' + to_string(numTestcase) + ".txt";
				path inputFile = workingDir / inputNum;
				string tempname = "input" + fileChange + ".txt";
				path reNameInputFile = build / tempname;
				copy_file(inputFile, reNameInputFile);

				string cdDirectory = "pushd " + (workingDir / ID / s).string();
				string cmdRunFileToScore = cdDirectory + " && make -f makefile" + fileChange + " run";
				system(cmdRunFileToScore.c_str());
				path objfile{ workingDir / ID / s / "build" / (fileChange + ".obj").c_str() };
				if (!exists(objfile)) break;

				
				string outputNum = "output" + fileChange + '_' + to_string(numTestcase) + ".txt";
				string tempnameout = "output" + fileChange + ".txt";
				path outputFile = build / tempnameout;
				path reNameOutputFile = build / outputNum;
				rename(outputFile, reNameOutputFile);

				remove(reNameInputFile);
				string daNum = "outputda" + fileChange + '_' + to_string(numTestcase) + ".txt";
				path daFile = workingDir / "testcase" / daNum;
				path reNameDAFile = build / daNum;
				copy_file(daFile, reNameDAFile);
				scoreOutput(workingDir, ID, fileChange, i+1, numTestcase);
				remove(reNameDAFile);
			}

		}


		scoreSub(workingDir, ID, i+1, dataID);
	}

}
 
void ThreadCompile(avlTree* DataID, path submitFolder, path workingDir) {
 
	while (1) {
		checkID(DataID, submitFolder, workingDir);
	}
}
 
void PrepareCompile(avlTree* DataID,path submitFolder) {
 
	for (directory_iterator fileID(submitFolder); fileID != directory_iterator(); ++fileID) {
		//dem co bao nhieu sub
		int count = 0;
		//co su thay doi gi khong-> co can sinh file khong
		bool CanCreateSub = 0;
		for (directory_iterator file(fileID->path()); file != directory_iterator(); ++file) {
			if (is_directory(file->path())) count++;
			else CanCreateSub = 1;
		}
		//neu khong can tao folder thi continue
		if (!CanCreateSub) continue;
 
		node* IDNode = DataID->search(fileID->path().filename().string());
		
		if (!IDNode)
			DataID->AVLInsert(DataID->root, IDNode=new node(fileID->path().filename().string(), 0), DataID->taller);
 
		IDNode->isLoading = true;
		//tao sub moi
		path newsub = fileID->path() / ("sub" + to_string(count + 1)).c_str();
		create_directory(newsub);
		while (1) {
			if (Replacefile(fileID->path(), newsub.string())) break;
		}
		IDNode->isLoading = false;
	}
}
 
void ThreadPrepareCompile(avlTree* DataID,path submitFolder) {
 
	while (1) {
		PrepareCompile(DataID,submitFolder);
	}
}
int main() {

	//path lam vc
	path workingDir("D:\\workingDir");
	path submitFolder("D:\\submitFolder");
	//data base
 
	avlTree* DataID=new avlTree();
 

	thread t1(ThreadCompile, DataID, submitFolder, workingDir);
	thread t2(ThreadPrepareCompile, DataID,submitFolder);
	t1.join();
	t2.join();
	//while(1) {
		//PrepareCompile(submitFolder);
	//}
	system("pause");
}