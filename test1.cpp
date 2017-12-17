#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include "tinyxml.h"
#include "BKavltree.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <thread>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include<queue>

using namespace boost::filesystem;

struct threadCount {
	int count = 0;
};

struct Checker {
	bool* startRun = new bool(false);
	string* nameExe = new string("");
	bool* signal = new bool(false);
	void Reset() { *startRun = false; *nameExe = ""; *signal = false; }
};
boost::mutex mutex1;
boost::mutex mutex2;
boost::mutex mutex3;


//------------->>>>  PROTOTYPE  <<<<-------------//
bool whenToStop();
bool DeleteSubFolder(avlTree &Data, path workingDir, string ID, string sub);
bool CopyfileStoW(path workingDir, path submitFol, nodeHeap ID, int sub);

bool checkID(avlTree* dataID, path submitFolder, path workingDir, Checker* checkErrorExe,Heap* Priority, int* count,AvlSubject* Manager);
void exportScore(path workingDir, avlTree* dataIN, string ID,string subjectName);
int compileFile(path FolderWD, string ID, string sub);
void scoreOutput(path workingDir, string ID, string fileToScore, int subNumber, int numTestcase,int Alltest);
void runThenScoreFileSub(path workingDir, string ID, avlTree* dataID, int numOfSubIn,int count, Checker* checkErrorExe,string subjectName);
void scoreSub(path workingDir, string ID, int subNumber, avlTree* dataIn,string subjectName);
void AddPriority(Heap* Priority, string gettime, string ID,string subject);
void ThreadCheckErrorExe(Checker* ErrorExe);


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

bool CopyfileStoW(path workingDir, path submitFol, nodeHeap IDnode,int subnum ) {
	bool flag = false;//da tim thay quantity chua
	int quantity = 0;//so luong file

	string ID = IDnode.ID;
	string Subjectsub;
	string sub = "sub" + to_string(subnum);
	path IDFolderInWorkingDir = workingDir /ID/sub;
	path subFolderInSubmitFol="";
	int countsub = 0;//bien dem folder sub theo subject
	for (directory_iterator filesub(submitFol / ID); filesub != directory_iterator(); ++filesub) {
		subFolderInSubmitFol = filesub->path();
		string Subject = "";
		string curFileName = "pro.xml";
		path desFileName = filesub->path() / curFileName;
		//chuyen string ve char* de dung ham doc
		char * writable = new char[desFileName.string().size() + 1];
		for (int i = 0; i < desFileName.string().length() + 1; i++) {
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
			string SubjectID = "SubjectID";
			if (!SubjectID.compare(child1->ValueTStr().c_str())) {
				Subject = child1->GetText();
			}
			child1 = child1->NextSiblingElement();
		}
		if (!Subject.compare(IDnode.subject)) countsub++;
		if (countsub == subnum) break;
	}
		//this is a brand new IDFolder
	create_directory(workingDir / ID);
	create_directory(workingDir / ID/sub);
	int count = 0;//bien dem file trong sub
	for (directory_iterator file(subFolderInSubmitFol);file != directory_iterator(); ++file) {
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
	while (1) {
		//quet file trong folder ID
		for (directory_iterator file(IDFolder); file != directory_iterator(); ++file) {
			string curFileName = file->path().filename().string();
			path desFileName = newsub / curFileName;
			//dem file
			if (is_directory(file->path())) continue;
			count++;

			if (exists(newsub / curFileName)) return true;
			if (!exists(file->path())) return false;
			copy_file(file->path(), newsub / curFileName); //copy file sang file sub
			//neu la file pro.xml lay ra quantity
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
	}
	return true;
}
void ReadXML(path IDFolder, string sub, Heap* Priority) {
	string gettime;
	string subject;
	string curFileName = "pro.xml";
	path desFileName = IDFolder / sub/curFileName;
	//chuyen string ve char* de dung ham doc
	char * writable = new char[desFileName.string().size() + 1];
	for (int i = 0; i < desFileName.string().length() + 1; i++) {
		if (i == desFileName.string().length())
			writable[i] = '\0';
		else
			writable[i] = desFileName.string()[i];

	}
	TiXmlDocument doc(writable);
	if (!doc.LoadFile())
		{
		//printf("%s", doc.ErrorDesc());
		return;
		}
	TiXmlElement* root = doc.RootElement();
	TiXmlElement* child1 = root->FirstChildElement();
	while (child1) {
		string time = "time";
		string SubjectID = "SubjectID";
		if (!time.compare(child1->ValueTStr().c_str()))
			{
				gettime = child1->GetText();
			}
		else if (!SubjectID.compare(child1->ValueTStr().c_str())) {
			subject = child1->GetText();
		}
		child1 = child1->NextSiblingElement();
	}
	AddPriority(Priority, gettime, IDFolder.filename().string(),subject);
}

bool checkID(avlTree* dataID, path submitFolder, path workingDir,Checker* checkErrorExe,Heap* Priority, 
	int* countThread, AvlSubject* ManagerSubject) {

		if (Priority->isEmpty()) return 0;
		nodeHeap temp;
		//pop đỉnh heap
		Priority->heapDelete(temp);
		string IDNameFolder = temp.ID;//lay ra ten ID
		node* IDNode = dataID->search(IDNameFolder);	
		int count = 1;
		if (!IDNode)
			return 0;
		path IDfolder = workingDir/temp.subject/ IDNameFolder;
		mutex1.lock();
		if (!exists(IDfolder)) {}
		else {			
			for (directory_iterator file(IDfolder); file != directory_iterator(); ++file)
				if (is_directory(file->path())) count++;
		}
		if (IDNode->isLoading) {
			mutex1.unlock(); 
			return 0;
		}//neu fileID dang load thi di ra ngoai
		string ID = IDNameFolder;
		int numOfSub = count;
		string sub = "sub" + to_string(count);
		//copy den khi nao du file thi thoi nho` vao quantity trong xml
				
		while (1) {
			if (CopyfileStoW(workingDir/ temp.subject, submitFolder, temp, count)) break;
		}
		mutex1.unlock();
		if (exists(workingDir/ temp.subject / ID / sub / "build")) return 0;
		//Phan loai
		avlTree* Phanloai = ManagerSubject->GetavlData(temp.subject);
		node* nodePhanLoai = Phanloai->search(temp.ID);
		if (!nodePhanLoai) { 
			Phanloai->AVLInsert(Phanloai->root, new node(temp.ID, 0), Phanloai->taller); 
			nodePhanLoai = Phanloai->search(temp.ID); 
		}

		nodePhanLoai->numberSub++;
		

		int countTest=compileFile(workingDir/ temp.subject, ID, sub);
		//--------------------------------//
		mutex2.lock();
		nodePhanLoai->timeQueue.push(temp.time);
		runThenScoreFileSub(workingDir/ temp.subject, ID, Phanloai, numOfSub, countTest, checkErrorExe, temp.subject);
		mutex2.unlock();
		*countThread-= 1;
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

void scoreOutput(path workingDir, string ID, string fileToScore, int subNumber, int numTestcase,int Alltest) {
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
	if (numTestcase == Alltest) {
		out << "testcase " << numTestcase << ": " << diem * 10 << endl;
		out.close();
		
		//open file to calculate total score
		out.open(scoreFile, ios_base::in);
		float *score = new float[Alltest];
		for (int i = 0; i < Alltest; i++) {
			string line = "";
			int num;
			getline(out, line);
			
			stringstream tempLine(line);
			tempLine >> line >> num >> line >> score[i];

		}
		out.close();
		out.open(scoreFile, ios_base::app);
		float sum = 0;
		for (int i = 0; i < Alltest; i++) {
			sum += score[i];
		}
		out << "Total: " << sum / Alltest;
		out.close();
		return;
	}

	out << "testcase " << numTestcase << ": " << diem * 10 << endl;// luu tan so dung vao file output
	out.close();
	return;
}

void exportScore(path workingDir, avlTree* dataIN, string ID,string subjectName) {
	if (dataIN->root == NULL)
		return;

	path listSV = workingDir / "listSV.csv";
	path listAllSubSV = workingDir / "listAllSubSV.csv";
	path scoreOfSV = workingDir / ID / "scoreOfSV.csv";
	path maxScoreSV = workingDir / ID / "maxScore.csv";

	string pathListSV = listSV.string();
	string pathListAllSubSV = listAllSubSV.string(); 
	string pathScoreOfSV = scoreOfSV.string();
	string pathMaxScore = maxScoreSV.string();

	std::ofstream myfile;
	std::ofstream myfileAS;
	std::ofstream myfileSV;
	std::ofstream myfileMax;

	//<-----print to scoreOfSV.csv all of subs __ PUT 1 FILE/FOLDER ID----->//
	if (!exists(scoreOfSV)) {
		myfileSV.open(pathScoreOfSV);
		myfileSV << "MSSV, Lan nop, Diem,Thoi gian nop\n";
		myfileSV.close();
	}
	myfileSV.open(pathScoreOfSV, ios_base::app);
	node *sv = dataIN->search(ID);
	string cmdTempSV = sv->key + "," + to_string(sv->numberSub) + "," + to_string(sv->scoreStack.top()) + "," + sv->timeQueue.front()+ "\n";
	myfileSV << cmdTempSV;
	if (sv->scoreStack.top() >= sv->scoreHeap->getMax()) sv->HighScoreTime = sv->timeQueue.front();
	//<-----print to maxScore.csv __ PUT 1 FILE/FOLDER ID----->//
	myfileMax.open(pathMaxScore);
	myfileMax << "MSSV, So lan nop, Diem cao nhat, Thoi gian nop\n";

	string cmdTempMax = sv->key + "," + to_string(sv->numberSub) + "," + to_string(sv->scoreHeap->getMax()) + "," +sv->HighScoreTime+ "\n";
	myfileMax << cmdTempMax;

	//<-----print to listAllSubSV.csv all of subs __ 1 FILE----->//
	if (!exists(listAllSubSV)) {
		myfileAS.open(pathListAllSubSV);
		myfileAS << "MSSV, Lan nop, Diem,Thoi Gian Nop\n";
		myfileAS.close();
	}
	myfileAS.open(pathListAllSubSV, ios_base::app);


	string cmdTempAS = sv->key + "," + to_string(sv->numberSub) + "," + to_string(sv->scoreStack.top()) + "," +sv->timeQueue.front() + "\n";
	myfileAS << cmdTempAS;

	//<-----print to listSV.csv max score of subs __ 1 FILE----->//
	myfile.open(pathListSV);
	myfile << "MSSV, So lan nop, Diem cao nhat, Thoi gian nop\n";
	//traverse avlTree data to find all MSSV
	node *current = dataIN->root;
	sv->timeQueue.pop();
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
				string process = "";
				//process here
				if (current->scoreHeap->getMax() < 0)
					process = "processing";
				else process = to_string(current->scoreHeap->getMax());

				string cmdTemp = current->key + "," + to_string(current->numberSub) + "," + process + "," + sv->HighScoreTime + "\n" ;
				myfile << cmdTemp;

				current = current->right;
			}
			else
				done = 1;
		}
	} /* end of while */
	myfile.close();
	myfileAS.close();
	myfileSV.close();
	myfileMax.close();
	
	std::ofstream outTree(subjectName+".dat");
	dataIN->saveAVLTime(dataIN->root, outTree);
	outTree.close();

}
 
 void scoreSub(path workingDir, string ID, int subNumber, avlTree* dataIn, string subjectName) {
 
	int dem = 0;
	string s = "sub" + to_string(subNumber);
	path pathDaFile = workingDir / ID / s / "build";
	queue<string> scoreOfx; int countscore = 0;
	//duyet toan thu muc build, dem so luong file scoreOfx
	for (directory_iterator fileScore(pathDaFile); fileScore != directory_iterator(); ++fileScore) {
		string t = fileScore->path().filename().string(); 
		int f=t.find("scoreOf");
		if (f >= 0) {
			string a = "scoreOf";
			a = t.substr(a.length(), 1);
			scoreOfx.push(a);//lay ra chu so trong scoreOf...text sang queue
		}
	}
	int sum = 0;
	string totalScore = pathDaFile.string() + "\\score.txt";
	std::fstream in;
	string line;
	while (!scoreOfx.empty()) {
		string tenfile = pathDaFile.string()+"\\scoreOf" + scoreOfx.front()+".txt";
		in.open(tenfile, ios_base::in);
		while (1) {
			getline(in, line);
			int t = line.find("Total");
			if ( t>= 0) break;
		}
		stringstream tempLine(line);
		float scoreElement = 0;
		tempLine >> line >> scoreElement;
		in.close();
		std::fstream gt;
		string gtFolder = workingDir.string() + "\\testcase\\gt" + scoreOfx.front() + ".txt";
		gt.open(gtFolder, ios_base::in);
		getline(gt, line);
		stringstream tempLinegt(line);
		float hs = 0;
		tempLinegt >> hs;
		sum += hs*scoreElement;
		gt.close();
		scoreOfx.pop();
	}
	std::fstream out;
	out.open(totalScore, ios_base::app);
	out << sum;
	out.close();

	//luu diem vao heap & stack cua rieng tung sinh vien
 
	node *SV = dataIn->search(ID);
 
	SV->scoreStack.push(sum);
	SV->scoreHeap->heapInsert(nodeHeap(sum));
	exportScore(workingDir, dataIn, ID,subjectName);
	return;
}

void runThenScoreFileSub(path workingDir, string ID, avlTree* dataID, int numOfSubIn,int count,Checker* checkErrorExe,string subjectName) {
	queue<int> ntest;
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
			string tempName = "";
			string cdDirectory = "pushd " + (workingDir / ID / s).string();
			string cmdCreateExe= cdDirectory + " && make -f makefile" + to_string(j) + " createexe";
			system(cmdCreateExe.c_str());
			//dem fileinput
			int numAlltestcase = 0;
			path testcasefolder{ workingDir / "testcase" };
			string inputName = "input" + to_string(j);
			for (directory_iterator file(testcasefolder); file != directory_iterator(); ++file) {
				tempName = file->path().filename().string();
				tempName = tempName.substr(0, inputName.length());
				if (!tempName.compare(inputName)) numAlltestcase++;
			}
			ntest.push(numAlltestcase);
			//
			for (int numTestcase = 1; numTestcase <= numAlltestcase; numTestcase++) {
				//copy testcase\input1.txt -> build\input.txt
				string fileChange = to_string(j);
				string inputNum = "testcase\\input" + fileChange + '_' + to_string(numTestcase) + ".txt";
				path inputFile = workingDir / inputNum;
				string tempname = "input" + fileChange + ".txt";
				path reNameInputFile = build / tempname;
				copy_file(inputFile, reNameInputFile);

				thread t3(ThreadCheckErrorExe, checkErrorExe);
				t3.detach();
				string cmdRunFileToScore = cdDirectory + " && cd build && " + fileChange + ".exe";
				//tin hie.u startRun =true
				*(checkErrorExe->nameExe) = fileChange; *(checkErrorExe->startRun) = true;
				system(cmdRunFileToScore.c_str());
				//neu on thi reset startRun
				*(checkErrorExe->startRun) = false;
				//signal tra ve true khi file exe co van de
				if (*(checkErrorExe->signal)) {
					checkErrorExe->Reset();//reset checker
					break;
				}
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
				scoreOutput(workingDir, ID, fileChange, i+1, numTestcase,numAlltestcase);
				remove(reNameDAFile);
			}
			//xoa obj exe
			string cmdClean = cdDirectory + " && make -f makefile" + to_string(j) + " clean";
			system(cmdClean.c_str());
			
		}
		scoreSub(workingDir, ID, i+1, dataID,subjectName);
		return;
	}
}
 
void ThreadCompile(avlTree* DataID, path submitFolder, path workingDir,Checker* checkErrorExe,Heap* Priority, AvlSubject* ManagerSubject) {
	int* count = new int(0);

	while (1) {
		if (!whenToStop())return;
		if (Priority->isEmpty()) continue;
		if (*count > 3) continue;
		*count += 1;
		boost::thread t1(checkID, DataID, submitFolder, workingDir, checkErrorExe, Priority, count, ManagerSubject);
		t1.detach();
	}
}
void Traverse(path submitFolder, Heap* Priority,avlTree* DataID) {
	for (directory_iterator fileID(submitFolder); fileID != directory_iterator(); ++fileID) {
		int count = 0;
		node* IDNode = DataID->search(fileID->path().filename().string());
		if (!IDNode) continue;
		if (IDNode->isLoading) continue;
		//dem so luong submit file co trong ID folder
		for (directory_iterator file(fileID->path()); file != directory_iterator(); ++file) 
			if (!is_directory(file->path())) {
				continue;
			}
			else count++;
		if (count > IDNode->numberSub) {
				IDNode->numberSub++;
				//doc pro.xml va cap nhat lan submit nay vao Priority
				ReadXML(fileID->path(), "sub" + to_string(IDNode->numberSub), Priority);
		}
	}
 }
void PrepareCompile(avlTree* DataID,path submitFolder,Heap* Priority) {
 
	for (directory_iterator fileID(submitFolder); fileID != directory_iterator(); ++fileID) {
		node* IDNode = DataID->search(fileID->path().filename().string());
		if (!IDNode)
			DataID->AVLInsert(DataID->root, IDNode = new node(fileID->path().filename().string(), 0), DataID->taller);
		//dem co bao nhieu sub
		int count = 0;
		//co su thay doi gi khong-> co can sinh file khong
		bool CanCreateSub = 0;
		for (directory_iterator file(fileID ->path()); file != directory_iterator(); ++file) {
			if (is_directory(file->path())) { 
				count++;
			}
			else CanCreateSub = 1;
		}

		//neu khong can tao folder thi continue
		if (!CanCreateSub) {
			continue;
		}
			IDNode->isLoading = true;
			//tao sub moi
			path newsub = fileID->path() / ("sub" + to_string(count + 1)).c_str();
			create_directory(newsub);
			Replacefile(fileID->path(), newsub.string());
			IDNode->isLoading = false;
	}
}
 
void ThreadPrepareCompile(avlTree* DataID,path submitFolder,Heap* Priority) {
 
	while (1) {
		Traverse(submitFolder, Priority, DataID);
		PrepareCompile(DataID,submitFolder,Priority);
		if (!whenToStop()) return;
	}
}

void settingConfig(path &SF, path &WD, path &uploadedFolder) {
	std::ifstream ifs("settings.config");
	string line, sf, wd, uploaded, stringntest;

	getline(ifs, line);
	sf = line.substr(line.find("\"") + 1, line.find("\"") );
	path t(sf);
	SF = t;

	getline(ifs, line);
	wd = line.substr(line.find("\"") + 1, line.length());
	wd.erase(wd.find("\""), 1);
	path t2(wd);
	WD = t2;

	getline(ifs, line);
	uploaded = line.substr(line.find("\"") + 1, line.length() - 1);
	uploaded.erase(uploaded.find("\""),1);
	path t3(uploaded);
	uploadedFolder = t3;

	getline(ifs, line); getline(ifs, line); getline(ifs, line); getline(ifs, line);
	char token='\0';
	int temp;
	line = line.substr(line.find(":") + 1, line.length());
	stringstream tp(line);
	//while(token != '.') {
	//	tp >> stringntest >> token;
	//	stringstream convert(stringntest);
	//	convert >> temp;
	//	ntestcase.push(temp);
	//	tp << line.substr(stringntest.length(), line.length());
	//}
}
//checkerError
bool checkErrorExe(Checker* checkErrorExe) {
	//signal dung de phat hien exe co van de, va se khong thuc thi tiep
	//bat dau dem thoi gian
	std::ifstream ifs("settings.config");
	string line;
	getline(ifs, line);	getline(ifs, line);	getline(ifs, line); getline(ifs, line);

	int star1 = line.find("\"");
	int star2 = star1 + line.substr(star1 + 1, line.length() - 1).find("\"");
	string t = line.substr(star1 + 1, star2);
	string temp = t.substr(0, t.find("\""));
	stringstream ss(temp);
	float time = 5;
	ss >> time;

	if (!*(checkErrorExe->startRun)) return false;
	double startTime = clock();
	while (1) {
		if (!*(checkErrorExe->startRun)) break;
		double deltaTime = (clock() - startTime) / (double)CLOCKS_PER_SEC;
		//co hoi cuoi cung la x sec cho sv
		if (deltaTime > time) {
			*(checkErrorExe->signal) = true;
			string cmd = "pushd C:\\Windows\\System32 && Taskkill /F /IM " + *(checkErrorExe->nameExe) + ".exe";
			system(cmd.c_str());

			break; 
		}
	}
	*(checkErrorExe->nameExe) = "";
	*(checkErrorExe->startRun) = false;
	return true;
}
void ThreadCheckErrorExe(Checker* ErrorExe) {
	while (1) {
		if(checkErrorExe(ErrorExe)) return;
	}
}
void AddPriority(Heap* Priority, string gettime, string ID,string subject) {
	stringstream sstr;
	sstr << gettime;
	boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
	int month, day, hour, min, second;
	char a[4];
	sstr >> month >> a[0] >> day >> a[1] >> hour >> a[2] >> min >> a[3] >> second;
	//create key priority
	double timevalue = (timeLocal.date().month().as_enum() - month) * 44640 + (timeLocal.date().day() - day) * 1400 + (timeLocal.time_of_day().hours() - hour) * 60 + (timeLocal.time_of_day().minutes() - min) + (timeLocal.time_of_day().seconds() - second) / (double)60;
	double key = timevalue;
	string timesub = to_string(day) + "-" + to_string(month)+" " + to_string(hour) + ":" + to_string(min) + ":" + to_string(second);
	Priority->heapInsert(nodeHeap(key, ID,subject,timesub));
}

bool whenToStop() {
	string line;
	std::ifstream ifs("settings.config");
	for (int i = 0; i < 9; i++) {
		getline(ifs, line);
	}
	string running = line.substr(line.find("=") + 1, line.length());
	stringstream r(running);
	r >> running;
	if (running != "true" && running != "1") 
		return false;
}

int main() {
	Checker* checkErrorExe = new Checker();
	path submitFolder, workingDir, uploadedFolder;
	settingConfig(submitFolder, workingDir, uploadedFolder);
	Heap* Priority = new Heap(1000);
	avlTree* DataID = new avlTree(); // quanly toan bo submit cua sv
	//loading data
	AvlSubject* SubjectManage = new AvlSubject();
	SubjectManage->BuildSubject();//4 mon
	SubjectManage->LoadData();
	SubjectManage->loadToOtherAVL(DataID);
	thread t1(ThreadPrepareCompile, DataID, submitFolder, Priority);
	t1.detach();
	ThreadCompile(DataID, submitFolder, workingDir, checkErrorExe, Priority,SubjectManage);
}