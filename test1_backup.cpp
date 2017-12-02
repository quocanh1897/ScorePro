#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include "tinyxml.h"
#include "BKavltree.h"
#include <ctime>
#include<fstream>
#include<sstream>
#include<stack>

using namespace boost::filesystem;

//------------->>>>  PROTOTYPE  <<<<-------------//

bool DeleteSubFolder(avlTree &Data, path workingDir, string ID, string sub);
bool Copyfile(path workingDir, path submitFol, string ID, string sub);
bool CompilethroughXML(const char* link, path workingDir, string ID, string sub);
bool checkID(avlTree &dataID, path submitFolder, path workingDir);
bool CreateXML(path submitfolder, string ID, string sub);

void exportScore(path workingDir, avlTree dataIN);
void compileFile(path FolderWD, string ID, string fileName, string sub);
void scoreOutput(path workingDir, string ID, string fileToScore, int subNumber, int numTestcase);
void runThenScoreFileSub(path workingDir, string ID, avlTree &dataID, int numOfSubIn);
void scoreSub(path workingDir, string ID, int subNumber, avlTree &dataIn);

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

void compileFile(path FolderWD, string ID, string fileName,string sub) {
	string objName = fileName.substr(0,fileName.find(".")) + ".obj";
	//tao thu muc build
	path temp{ FolderWD /ID/sub/ "build" };
	//create_directory(temp);
	//noi chua file .obj
	path objPath = temp / objName;
	//noi chua file .cpp
	path cppPath = FolderWD /ID/sub/ fileName;
	string compileCmd = "g++ -o " + objPath.string() + " " + cppPath.string();

	system(compileCmd.c_str());
}

bool Copyfile(path workingDir, path submitFol,string ID,string sub) {
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
		if (quantity == count+1&&flag) return true;
	}
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

bool CompilethroughXML(const char* link,path workingDir,string ID,string sub) {
	create_directory(workingDir / ID / sub / "build");
	TiXmlDocument doc(link);
	if (!doc.LoadFile())
	{
		//printf("%s", doc.ErrorDesc());
		return 0;
	}
	TiXmlElement* root = doc.RootElement();
	TiXmlElement* child1 = root->FirstChildElement();
	//cout << child1->ValueTStr().c_str();
	while (child1) {
		string path = child1->GetText();
		if (path.find(".") < path.length()) {
			string cpp = path.substr(path.find(".") + 1, cpp.length() - 1);
			if (!cpp.compare("cpp"))
			{
				compileFile(workingDir, ID, path,sub);
			}
		}
		child1 = child1->NextSiblingElement();
	}
	return 1;
}

bool checkID(avlTree &dataID, path submitFolder, path workingDir) {
	//duyet tuan tu file submitFolder
	for (directory_iterator file(submitFolder); file != directory_iterator(); ++file) {
		string IDNameFolder = file->path().filename().string();//lay ra ten ID
															   //ID bo vao database neu chua co
		if (!dataID.search(IDNameFolder))
			dataID.AVLInsert(dataID.root, new node(IDNameFolder, 0), dataID.taller);
		int count = 0;

		//duyet tuan tu file sub
		for (directory_iterator filesub(submitFolder / IDNameFolder); 
			filesub != directory_iterator(); ++filesub) {
			bool error = false;
			count++;
			if (count > dataID.search(IDNameFolder)->numberSub) {
				dataID.search(IDNameFolder)->numberSub++;
				string ID = IDNameFolder;
				int numOfSub = dataID.search(IDNameFolder)->numberSub;
				string sub = "sub" + to_string(dataID.search(IDNameFolder)->numberSub);
				//double starttime = clock();
				CreateXML(submitFolder, ID, sub);
				//copy den khi nao du file thi thoi nho` vao quantity trong xml
				while (1) {
					if (Copyfile(workingDir, submitFolder, ID, sub) == true) break;
				}
				//if (error) break;


				//-----compile nho file xml-----
				path xml = workingDir / ID / sub / "pro.xml";
				string xmlStr = xml.string();
				char * writable = new char[xmlStr.length() + 1];
				for (int i = 0; i < xmlStr.length() + 1; i++) {
					if (i == xmlStr.length()) writable[i] = '\0';
					else writable[i] = xmlStr[i];
				}
				CompilethroughXML(writable, workingDir, ID, sub);
				//--------------------------------//

				runThenScoreFileSub(workingDir, ID, dataID, numOfSub);

				exportScore(workingDir, dataID);
			}
		}
	}

	return 1;
}

void scoreOutput(path workingDir, string ID, string fileToScore, int subNumber, int numTestcase) {
	int dem = 0;
	int a[50];
	string fileChange = fileToScore.substr(0, fileToScore.find("."));
	//node *numofSub = dataID.search(ID);
	string s = "sub" + to_string(subNumber);
	path pathDaFile = workingDir / ID / s / "build" ;
	string daFile = pathDaFile.string() + "\\outputda" + fileChange + "_" + to_string(numTestcase) + ".txt";
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

void exportScore(path workingDir, avlTree dataIN) {
	path listSV = workingDir / "listSV.csv";
	string pathListSV = listSV.string();
	std::ofstream myfile;

	/*if (!exists(listSV)) {
		myfile.open(pathListSV);
		myfile << "MSSV, So lan nop, Diem cao nhat\n";
	}
	else {
		myfile.open(pathListSV);
	}
*/
	myfile.open(pathListSV);
	myfile << "MSSV, So lan nop, Diem cao nhat\n";

	if (dataIN.root == NULL) 
		return;
	
	//traverse avlTree data to find all MSSV
	node *current = dataIN.root;
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
				node *t1 = dataIN.search(tempstack.top());
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

void scoreSub(path workingDir, string ID, int subNumber, avlTree &dataIn) {
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
	node *SV = dataIn.search(ID);
	SV->scoreHeap->heapInsert(score1*0.3 + score2*0.7);
	return;
}

void runThenScoreFileSub(path workingDir, string ID, avlTree &dataID, int numOfSubIn) {
	node *numofSub = dataID.search(ID);
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

		list<string> objFile;
		for (directory_iterator fileObj(build);
			fileObj != directory_iterator(); ++fileObj) {
			string t = fileObj->path().filename().string();
			objFile.push_back(t);
		}

		//run code per objFile
		for each (string file in objFile){
			for (int numTestcase = 1; numTestcase <= 5; numTestcase++) {
				//copy testcase\input1.txt -> build\input.txt
				string fileChange = file.substr(0, file.find(".") );
				string inputNum = "testcase\\input" + fileChange + '_' + to_string(numTestcase) + ".txt";
				path inputFile = workingDir / inputNum;
				string tempname = "input" + fileChange + ".txt";
				path reNameInputFile = build / tempname;
				copy_file(inputFile, reNameInputFile);

				string cdDirectory = "pushd " + build.string();
				string cmdRunFileToScore = cdDirectory + " && " + file;

				system(cmdRunFileToScore.c_str());

				//double begin = clock();
				//while (true) {
				//	
				//	system(cmdRunFileToScore.c_str());
				//	double deltat = (clock() - begin) / double(CLOCKS_PER_SEC);
				//	//xay ra loi code chay vo han
				//	if (deltat > 40) {
				//		break;
				//	}
				//} 
				


				//rename ouput1.txt -> output1_1.txt
				string outputNum = "output" + fileChange + '_' + to_string(numTestcase) + ".txt";
				string tempnameout = "output" + fileChange + ".txt";
				path outputFile = build / tempnameout;
				path reNameOutputFile = build / outputNum;
				rename(outputFile, reNameOutputFile);

				remove(reNameInputFile);
				
			}
			
		}
		
		//score output in subx
		for each (string file in objFile) {
			for (int numTestcase = 1; numTestcase <= 5; numTestcase++) {
				//copy testcase\input1.txt -> build\input.txt
				string nameFileDapAn = file.substr(0, file.find("."));
				string daNum = "outputda" + nameFileDapAn + '_' + to_string(numTestcase) + ".txt";
				path daFile = workingDir / "testcase" / daNum;
				path reNameDAFile = build / daNum;
				copy_file(daFile, reNameDAFile);


				scoreOutput(workingDir, ID, file, i + 1 , numTestcase);
				remove(reNameDAFile);

			}
		}
		
		scoreSub(workingDir, ID, i + 1, dataID);
	}
	
}

int main() {

	//path lam vc
	path workingDir("D:\\workingDir");
	path submitFolder("D:\\submitFolder");
	//data base
	avlTree DataID;
	//check file
	//thu tu uu tien la flie ID ben trong co file sub dau tien
	
	while (1) {
		checkID(DataID, submitFolder, workingDir);
		//exportScore(workingDir, DataID);
		
	}

	//checkID(DataID, submitFolder, workingDir);
	//runThenScoreFileSub(workingDir, "1610081", DataID);
	//system("pause");
}