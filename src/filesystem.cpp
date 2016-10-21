#include "filesystem.h"
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>

FileSystem::FileSystem() {
	this->root = Folder("/");
	this->currentDir = &root;


	this->root.folderVec.push_back(Folder("A", &this->root));
	this->root.folderVec.push_back(Folder("B", &this->root));
	this->root.folderVec.push_back(Folder("C", &this->root));
	this->root.folderVec.push_back(Folder("D", &this->root));

	createFile("Apa","Apan ater banan");

	this->root.folderVec[0].folderVec.push_back(Folder("A1", &this->root.folderVec[0]));
	this->root.folderVec[0].folderVec[0].folderVec.push_back(Folder("A11", &this->root.folderVec[0].folderVec[0]));
	this->root.folderVec[0].folderVec[0].folderVec.push_back(Folder("A12", &this->root.folderVec[0].folderVec[0]));

	//removeFolder("A");
	//GoToFolder("A/A1");

	/*for (const auto & item : this->currentDir->folderVec)	//skriver ut allt innehåll i nuvarande mappen
			std::cout << item.myFolderName << std::endl;
*/

	/*for (const auto & item : this->root.folderVec[0].folderVec)	//skriver ut allt innehåll i A
		std::cout << item.myFolderName << std::endl;
	for (const auto & item : this->root.folderVec)	//skriver ut allt innehåll i root
			std::cout << item.myFolderName << std::endl;
*/
}

FileSystem::~FileSystem() {
}

int FileSystem::createFile(std::string fileName, std::string content) {
	char * finalContent = new char[512];
	std::copy(content.begin(), content.end(), finalContent);
	finalContent[content.size()] = '§';
	
	for(int i = 0; i < 250; i++){
		if(mMemblockDevice.freeBlockArr[i] == 0){
			if(mMemblockDevice.writeBlock(i, finalContent) == 1){
				mMemblockDevice.freeBlockArr[i] = 1;
				currentDir->fileVec.push_back(File(fileName, i, &mMemblockDevice));
				delete[] finalContent;
				return 1;
			}
			else{
				delete[] finalContent;
				return -1;
			}
		}
	}
	delete[] finalContent;
	return -2;
}

void FileSystem::removeFile(std::string fileName){
	auto it = std::find(currentDir->fileVec.begin(), currentDir->fileVec.end(), fileName);

	if (it != currentDir->fileVec.end()){
		int i = std::distance(currentDir->fileVec.begin(), it);
		mMemblockDevice.freeBlockArr[currentDir->fileVec[i].id] = 0;
		currentDir->fileVec.erase(it);
	}
}

void FileSystem::CreateFolder(std::string folderName){
	currentDir->folderVec.push_back(Folder(folderName , currentDir));
}

void FileSystem::RemoveFolder(std::string folderName){
	const Folder tmp(folderName,nullptr);
	auto it = std::find(currentDir->folderVec.begin(), currentDir->folderVec.end(), tmp);

	if (it != currentDir->folderVec.end())
		currentDir->folderVec.erase(it);
 }

bool FileSystem::GoToFolder(std::string folderName){
	std::string name="";
	int length=folderName.length();
	//std::cout << length << std::endl;
	char * entireDir = new char [length+1];
  	std::strcpy(entireDir, folderName.c_str());

  	for(int i=0;i<=length;i++){
  		if((entireDir[i]=='/' && i != 0) || (i == length && name!="")){
			if(name == ".."){
				if(currentDir == &root)
				{
					//std::cout<<"I am Groot"<<std::endl;
					return false;
				}
				else
					currentDir = currentDir->parent;
				}
				else{
					const Folder tmp(name,nullptr);
		 			auto it = std::find(currentDir->folderVec.begin(), currentDir->folderVec.end(), tmp);

					if (it != currentDir->folderVec.end()) {
						int index = std::distance(currentDir->folderVec.begin(), it);
						currentDir = &currentDir->folderVec[index];
						name = "";
						i++;
					}
					else
						return false;
				}
  		}
			//name = name + entireDir[i];

  		name += entireDir[i];
  	}
		return true;
}

void FileSystem::listDir(){

}

std::string FileSystem::ListContent(){
	std::string rtn;
	for (const auto & item : this->currentDir->folderVec) {
		rtn += item.myFolderName + "\n";
	}


	for (const auto & thing : this->currentDir->fileVec) {
		rtn += thing.name + "\n";
	}
	return rtn;
}

std::string FileSystem::PrintFileContent(std::string fileName){
	auto it = std::find(currentDir->fileVec.begin(), currentDir->fileVec.end(), fileName);

	if (it != currentDir->fileVec.end()){
		int i = std::distance(currentDir->fileVec.begin(), it );
		int block = currentDir->fileVec[i].id;
		std::string rtn = mMemblockDevice.readBlock(block).toString();
		std::size_t split = rtn.find("§");
		rtn=rtn.substr(0,split);
		return rtn;
	}
	return "error";
}

bool FileSystem::CopyFile(std::string file1, std::string file2)
{
	auto it = std::find(currentDir->fileVec.begin(), currentDir->fileVec.end(), file1);

	if (it != currentDir->fileVec.end()) {
		createFile(file2, PrintFileContent(file1));
	}
	else
		return false;
		
	return true;
}

char * fp;
FILE * file;
char rowC[1024];
int rowI;

void FileSystem::restoreImage(std::string filePath) {
	fp = new char[filePath.length() + 1];
	FILE * file = fopen(fp, "r");

	fscanf(file, "%i \n", rowI);
	int nrOfFolders = rowI;
	fscanf(file, "%i \n", rowI);
	int nrOfFiles = rowI;

	for (int i = 0; i < nrOfFiles; i++) {
		std::string filename, content;
		fscanf(file, "%s %s \n", filename, content);
		createFile(filename, content);
	}

	for (int i = 0; i < nrOfFolders; i++) {
		fscanf(file, "%i \n", rowI);
		int childNrOfFolders = rowI;
		fscanf(file, "%i \n", rowI);
		int childNrOfFiles = rowI;

		fscanf(file, "%s \n", rowC);
		std::string name = rowC;
		CreateFolder(name);

		currentDir = &currentDir->folderVec[i];
		recursiveFunction(name, childNrOfFolders, childNrOfFiles);
	}
	currentDir = currentDir->parent;
}

void FileSystem::recursiveFunction(std::string name, int nrOfFolders, int nrOfFiles) {

	for (int i = 0; i < nrOfFiles; i++) {
		std::string filename, content;
		fscanf(file, "%s %s \n", filename, content);
		createFile(filename, content);
	}

	for (int i = 0; i < nrOfFolders; i++) {
		fscanf(file, "%i \n", rowI);
		int childNrOfFolders = rowI;
		fscanf(file, "%i \n", rowI);
		int childNrOfFiles = rowI;

		fscanf(file, "%s \n", rowC);
		std::string name = rowC;
		CreateFolder(name);

		currentDir = &currentDir->folderVec[i];
		recursiveFunction(name, childNrOfFolders, childNrOfFiles);
	}
	currentDir = currentDir->parent;
}