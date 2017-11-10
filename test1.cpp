#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
namespace fs = boost::filesystem;

void compileFile(fs::path input) {
	string thisFile = input.string();
	cout << input << " need compile" << endl;
}

bool checkFolderNotExistOrChange(fs::path workingDir, fs::path submitFol,
	string IDStudent) {
	fs::path IDFolderInWorkingDir = workingDir / IDStudent;
	fs::path IDFolderInSubmitFol = submitFol / IDStudent;
	if (!fs::exists(IDFolderInWorkingDir)) {//this is a brand new IDFolder
		fs::copy(IDFolderInSubmitFol, IDFolderInWorkingDir);	
	}
		
	//iterate from first file in submitFolder to end, copy all of them to
	//the corresponse folder in workingDir 
	for (fs::directory_iterator file(IDFolderInSubmitFol);
		file != fs::directory_iterator(); ++file) {
		
		string curFileName = file->path().filename().string();
		fs::path desFileName = workingDir /IDStudent/curFileName;
		if (!fs::exists(desFileName) ||
			fs::exists(desFileName) &&
			fs::file_size(desFileName) != fs::file_size(file->path())) {

			compileFile(file->path());
			fs::copy_file(file->path(), workingDir / IDStudent / curFileName,fs::copy_option::overwrite_if_exists);
		}
		
	}
	cout << "success \n";


	return true;
}

int main() {
	fs::path workingDir("D:\\workingDir");
	fs::path submitFolder("D:\\submitFolder");
	string ID = "1610081";
	fs::path IDFolder = submitFolder / ID;

	checkFolderNotExistOrChange(workingDir, submitFolder, ID);
	system("pause");
}