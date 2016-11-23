#include "filesystem.h"
#include <string>
#include <cstring>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

FileSystem::FileSystem() {
	this->root = Folder("/");
	this->currentDir = &root;
	this->root.parent = &root;


	// Blocket under ger ett enkelt filsystem 
	/*this->root.folderVec.push_back(Folder("A", &this->root));
	this->root.folderVec.push_back(Folder("B", &this->root));
	this->root.folderVec.push_back(Folder("C", &this->root));
	this->root.folderVec.push_back(Folder("D", &this->root));

	createFile("Apa","Apan ater banan");

	this->root.folderVec[0].folderVec.push_back(Folder("A1", &this->root.folderVec[0]));
	this->root.folderVec[0].folderVec.push_back(Folder("A2", &this->root.folderVec[0]));
	this->currentDir = &this->root.folderVec[0].folderVec[1];
	createFile("hest", "hester eter gres");
	this->currentDir = &root;
	this->root.folderVec[0].folderVec[0].folderVec.push_back(Folder("A11", &this->root.folderVec[0].folderVec[0]));
	this->root.folderVec[0].folderVec[0].folderVec.push_back(Folder("A12", &this->root.folderVec[0].folderVec[0]));*/

	
}

FileSystem::~FileSystem() {
}

int FileSystem::createFile(std::string fileName, std::string content) {
	char * finalContent = new char[512];
	std::copy(content.begin(), content.end(), finalContent);
	finalContent[content.size()] = '$';
	
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

	if (it != currentDir->fileVec.end()) {
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
	char * entireDir = new char [length+1];
  	std::strcpy(entireDir, folderName.c_str());

  	for(int i=0;i<=length;i++){
  		if((entireDir[i]=='/' && i != 0) || (i == length && name!="")){
			if(name == ".."){
				if(currentDir == &root)
				{
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
		std::size_t split = rtn.find("$");
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

void FileSystem::CreateImage(std::string sysName)
{
	currentDir = &root;
	std::ofstream file;
	file.open(sysName);
	//std::ofstream file(sysName);
	//std::fstream file;
	//file.open(sysName);
	recursiveCreate(currentDir, file);
	file.close();
	
}

void FileSystem::recursiveCreate(Folder * dir, std::ofstream &file)
{
	currentDir = dir;
	int tmp = currentDir->folderVec.size();
	int tmp2 = currentDir->fileVec.size();
	file << tmp << std::endl;
	file << tmp2 << std::endl;
	for (int i = 0; i < tmp2; i++)
	{
		file << currentDir->fileVec[i].name << "=" << PrintFileContent(currentDir->fileVec[i].name) << std::endl;
	}
	for (int i = 0; i < tmp; i++)
	{
		file << currentDir->folderVec[i].myFolderName << std::endl;
		recursiveCreate(&currentDir->folderVec[i], file);
	}
	currentDir = currentDir->parent;
}

void FileSystem::restoreImage(std::string filePath) {
	currentDir = &root;
	std::string line;
	std::cout<<filePath<<std::endl;
	std::ifstream myfile(filePath);
	//std::fstream myfile(filePath);
	if (myfile.is_open())
	{
		recursiveFunction(currentDir, myfile);
	}
	else
		std::cout<<"Can't file"<<std::endl;
	myfile.close();
	currentDir=&root;
}

void FileSystem::recursiveFunction(Folder * dir, std::ifstream &file)
{
	currentDir = dir;
	std::string line;
	int tmp;
	int tmp2;
	std::cout<<"error check 1"<<std::endl;

	
	std::getline(file, line);
	tmp = std::stoi(line);
	std::cout<<"error check 2: "<< tmp <<std::endl;
	std::getline(file, line);
	tmp2 = std::stoi(line);
	std::cout<<"error check 2: "<< tmp2 <<std::endl<<std::endl;
	for (int i = 0; i < tmp2; i++)
	{
		std::getline(file, line);
		std::string name, content;
		size_t pos = line.find("=");
		name = line.substr(0, pos);
		content = line.substr(pos+1);
		int error=createFile(name, content);
		std::cout<<"error check 3: "<< name << " " << content << "error: " << error <<std::endl;
		std::cout<<PrintFileContent(name)<<std::endl;
	}
	for (int j = 0; j < tmp; j++)
	{
		std::getline(file, line);
		std::cout<<"error check 4: "<< line <<std::endl;
		CreateFolder(line);
		recursiveFunction(&currentDir->folderVec[j], file);
	}
	
	currentDir = currentDir->parent;
}