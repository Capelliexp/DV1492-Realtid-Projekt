#include "filesystem.h"
#include <string>
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

	this->root.folderVec[0].folderVec.push_back(Folder("A1", &this->root.folderVec[0]));
	this->root.folderVec[0].folderVec[0].folderVec.push_back(Folder("A11", &this->root.folderVec[0].folderVec[0]));
	this->root.folderVec[0].folderVec[0].folderVec.push_back(Folder("A12", &this->root.folderVec[0].folderVec[0]));

	//removeFolder("A");
	//GoToFolder("A/A1");
	/*for (const auto & item : this->currentDir->folderVec)
			std::cout << item.myFolderName << std::endl;
*/

	/*for (const auto & item : this->root.folderVec[0].folderVec)
		std::cout << item.myFolderName << std::endl;
	for (const auto & item : this->root.folderVec)
			std::cout << item.myFolderName << std::endl;
*/
}

FileSystem::~FileSystem() {
}

int FileSystem::createFile(std::string fileName, std::string content){
	for(int i = 0; i < 255; i++){
		if(mMemblockDevice.freeBlockArr[i] == 0){
			if(mMemblockDevice.writeBlock(i, content) == 1){
				mMemblockDevice.freeBlockArr[i] = 1;
				currentDir->fileVec.push_back(File(fileName, i));
				return 1;
			}
			else{
				return -1;
			}
		}
	}
	return -1;
}

void FileSystem::removeFile(std::string fileName){
	auto it = std::find(currentDir->fileVec.begin(), currentDir->fileVec.end(), fileName);

	if (it != currentDir->fileVec.end()){
		currentDir->fileVec.erase(it);

		int i = std::distance(currentDir->fileVec.begin(), it );
		mMemblockDevice.freeBlockArr[i] = 0;
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
					std::cout<<"I am Groot"<<std::endl;
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

void FileSystem::ListContent(){
	for (const auto & item : this->currentDir->folderVec)
		std::cout << item.myFolderName << std::endl;

	for (const auto & item : this->currentDir->fileVec)
		std::cout << item.name << std::endl;
}

std::string FileSystem::PrintFileContent(std::string fileName){
	auto it = std::find(currentDir->fileVec.begin(), currentDir->fileVec.end(), fileName);

	if (it != currentDir->fileVec.end()){
		int i = std::distance(currentDir->fileVec.begin(), it );
		int block = currentDir->fileVec[i].id;
		std::string rtn = mMemblockDevice.readBlock(block).toString();
		return rtn;
	}
	return "error";
}

 /*GetCurrentFolder(std::string currentDir){
 	std::string entireDir = currentDir;
 	Folder* folderPointer = &root;
 	std::string folderName;
 	std::size_t pos

 	char a[];
 	entireDir.c_str();
 	std::string name=NULL;

 	for(int i=0;i<sizeOf(entireDir);i++)
 	{
 		if(entireDir[i]=='/'&& i!=0)
 		{

 		}

 		name += entireDir[i];

 	}

 	return folderPointer;
}*/
