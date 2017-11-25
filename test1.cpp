#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include "tinyxml.h"
#include "BKavltree.h"
#include <ctime>
using namespace std;
using namespace boost::filesystem;
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
	create_directory(temp);
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
				if (i == desFileName.string().length()) writable[i] = '\0';
				else writable[i] = desFileName.string()[i];
				
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
bool CompilethroughXML(const char* link,path workingDir,string ID,string sub) {
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
bool checkID(avlTree &dataID, path submitFolder,path workingDir) {
	//duyet tuan tu file submitFolder
	for (directory_iterator file(submitFolder); file != directory_iterator(); ++file) {
		string IDNameFolder = file->path().filename().string();//lay ra ten ID
		//ID bo vao database neu chua co
		if (!dataID.search(IDNameFolder)) dataID.AVLInsert(dataID.root, new node(IDNameFolder, 0), dataID.taller);	
		int count = 0;
		//duyet tuan tu file sub
		for (directory_iterator filesub(submitFolder/IDNameFolder); filesub != directory_iterator(); ++filesub) {
			bool error = false;
			count++;
				if (count > dataID.search(IDNameFolder)->numberSub) {
					dataID.search(IDNameFolder)->numberSub++;
					string ID = IDNameFolder;
					string sub = "sub" + to_string(dataID.search(IDNameFolder)->numberSub);
					double starttime = clock();
					//copy den khi nao du file thi thoi nho` vao quantity trong xml
					while (1) {
						if (Copyfile(workingDir, submitFolder, ID, sub)==true) break;
						double deltatime = (clock()-starttime)/ double(CLOCKS_PER_SEC);
						//neu 20s ma khong du file thi xoa het ,and then,ID nay` dua xuong cuoi ,den luot nhung ID tiep theo.
						if (deltatime > 20) {
							DeleteSubFolder(dataID,workingDir, ID, sub);
							error = true;
							break;
						}
					}
					if (error) break;
					//-----compile nho file xml-----
					path xml = workingDir / ID / sub / "pro.xml";
					string xmlStr = xml.string();
					char * writable = new char[xmlStr.length() + 1];
					for (int i = 0; i < xmlStr.length() + 1; i++) {
						if (i == xmlStr.length()) writable[i] = '\0';
						else writable[i] = xmlStr[i];
					}
					CompilethroughXML(writable, workingDir, ID, sub);
					//--------------------------------
			}
		}
	}
	return 1;
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
	}

	system("pause");
}