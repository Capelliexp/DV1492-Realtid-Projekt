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
	this->root.parent = currentDir;


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
				this->currentDir->fileVec.push_back(File(fileName, i, &mMemblockDevice));
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
	auto it = std::find(this->currentDir->fileVec.begin(), this->currentDir->fileVec.end(), fileName);

	if (it != this->currentDir->fileVec.end()) {
		int i = std::distance(this->currentDir->fileVec.begin(), it);
		mMemblockDevice.freeBlockArr[this->currentDir->fileVec[i].id] = 0;
		this->currentDir->fileVec.erase(it);
	}
}

void FileSystem::CreateFolder(std::string folderName){
	this->currentDir->folderVec.push_back(Folder(folderName , this->currentDir));
}

void FileSystem::RemoveFolder(std::string folderName){
	const Folder tmp(folderName,nullptr);
	auto it = std::find(this->currentDir->folderVec.begin(), this->currentDir->folderVec.end(), tmp);

	if (it != this->currentDir->folderVec.end())
		this->currentDir->folderVec.erase(it);
 }

/* bool FileSystem::GoToFolder(std::string folderName){
 	std::string name="";
 	int length=folderName.length();
 	char * entireDir = new char [length+1];
   	std::strcpy(entireDir, folderName.c_str());

   	for(int i=0;i<=length;i++){
   		if((entireDir[i]=='/' && i != 0) || (i == length && name!="")){
 			if(name == ".."){
 				if(this->currentDir == &root)
 				{
 					this->currentDir = this->currentDir->parent;
 					return false;
 				}
 				else
 					this->currentDir = this->currentDir->parent;
 				}
 			else{
 				const Folder tmp(name,nullptr);
 		 		auto it = std::find(this->currentDir->folderVec.begin(), this->currentDir->folderVec.end(), tmp);

 				if (it != this->currentDir->folderVec.end()) {
 					int index = std::distance(this->currentDir->folderVec.begin(), it);
 					this->currentDir = &this->currentDir->folderVec[index];
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
 }*/

std::string FileSystem::GoToFolder(std::string folderName)
{
	std::string tmpName = folderName;
	std::string roadRunner = "";
	if (folderName==".." && this->currentDir->myFolderName=="/")
	{
		this->currentDir = this->currentDir->parent;
		return "";
	}
	else if (folderName=="..")
	{
		this->currentDir = this->currentDir->parent;
		return "";
	}
	else
	{
		bool tmp = true;
		do
		{
			size_t pos = tmpName.find("/");
			if (pos != std::string::npos)
			{
				const Folder tmpFolder(tmpName.substr(0, pos), nullptr);
				auto it = std::find(this->currentDir->folderVec.begin(), this->currentDir->folderVec.end(), tmpFolder);

				if (it != this->currentDir->folderVec.end())
				{
					int index = std::distance(this->currentDir->folderVec.begin(), it);
					this->currentDir = &this->currentDir->folderVec[index];
				}
				else
				{
					std::cout << "you ended with a slash, lel" << std::endl;
					tmp = true;
				}
			}
			else
			{
				const Folder tmpFolder(tmpName, nullptr);
				auto it = std::find(this->currentDir->folderVec.begin(), this->currentDir->folderVec.end(), tmpFolder);

				if (it != this->currentDir->folderVec.end())
				{
					int index = std::distance(this->currentDir->folderVec.begin(), it);
					this->currentDir = &this->currentDir->folderVec[index];
				}
				else
				{
					std::cout << "No such folder" << std::endl;
					return false;
				}
				tmp = false;
			}


			//tmp = false;
		} while (tmp);

	}

	return true; need to fix this shiet!
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
	auto it = std::find(this->currentDir->fileVec.begin(), this->currentDir->fileVec.end(), fileName);

	if (it != this->currentDir->fileVec.end()){
		int i = std::distance(this->currentDir->fileVec.begin(), it );
		int block = this->currentDir->fileVec[i].id;
		std::string rtn = mMemblockDevice.readBlock(block).toString();
		std::size_t split = rtn.find("$");
		rtn=rtn.substr(0,split);
		return rtn;
	}
	return "error";
}

bool FileSystem::CopyFile(std::string file1, std::string file2)
{
	auto it = std::find(this->currentDir->fileVec.begin(), this->currentDir->fileVec.end(), file1);

	if (it != this->currentDir->fileVec.end()) {
		createFile(file2, PrintFileContent(file1));
	}
	else
		return false;

	return true;
}

void FileSystem::CreateImage(std::string sysName)
{
	this->currentDir = &root;
	std::ofstream file;
	file.open(sysName);
	//std::ofstream file(sysName);
	//std::fstream file;
	//file.open(sysName);
	recursiveCreate(this->currentDir, file);
	file.close();

}

void FileSystem::recursiveCreate(Folder * dir, std::ofstream &file)
{
	this->currentDir = dir;
	int tmp = this->currentDir->folderVec.size();
	int tmp2 = this->currentDir->fileVec.size();
	file << tmp << std::endl;
	file << tmp2 << std::endl;
	for (int i = 0; i < tmp2; i++)
	{
		file << this->currentDir->fileVec[i].name << "=" << PrintFileContent(this->currentDir->fileVec[i].name) << std::endl;
	}
	for (int i = 0; i < tmp; i++)
	{
		file << this->currentDir->folderVec[i].myFolderName << std::endl;
		recursiveCreate(&this->currentDir->folderVec[i], file);
	}
	this->currentDir = this->currentDir->parent;
}

void FileSystem::restoreImage(std::string filePath) {
	//this->currentDir = &root;
	//std::string line;
	std::cout<<filePath<<std::endl;
	std::ifstream myfile(filePath);
	//std::fstream myfile(filePath);
	if (myfile.is_open())
	{
		recursiveFunction(this->currentDir, myfile);
	}
	else
		std::cout<<"Can't file"<<std::endl;
	myfile.close();
	//this->currentDir=&root;
	std::cout<<std::endl<<"this->currentDir pekar på: "<<this->currentDir->myFolderName<<" som har "<<this->currentDir->folderVec.size()<<" mappar."<<std::endl;
	for (int h = 0; h < this->currentDir->folderVec.size(); h++)
	{
		std::cout<<this->currentDir->folderVec[h].myFolderName<<std::endl;
	}
	//this->currentDir=this->&root; //spöke
	std::cout<<std::endl<<"this->currentDir pekar på: "<<this->currentDir->myFolderName<<" som har "<<this->currentDir->folderVec.size()<<" mappar."<<std::endl;
	for (int h = 0; h < this->currentDir->folderVec.size(); h++)
	{
		std::cout<<this->currentDir->folderVec[h].myFolderName<<std::endl;
	}
}

void FileSystem::recursiveFunction(Folder * dir, std::ifstream &file)
{
	this->currentDir = dir;
	std::cout<<"current folder is: "<<this->currentDir->myFolderName<<std::endl;
	std::cout<<"and my parent is: "<<this->currentDir->parent->myFolderName<<std::endl;
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
		std::cout<<"error check 3: "<< name << " " << content << " error: " << error <<std::endl;
		std::cout<<PrintFileContent(name)<<std::endl;
	}
	for (int j = 0; j < tmp; j++)
	{
		std::getline(file, line);
		std::cout<<"error check 4: "<< line <<std::endl;
		CreateFolder(line);
		recursiveFunction(&this->currentDir->folderVec[j], file);
	}

	this->currentDir = this->currentDir->parent;
	std::cout<<"i went back to: "<<this->currentDir->myFolderName<<std::endl;
}

void FileSystem::test()
{
	std::cout<<std::endl<<"this->currentDir pekar på: "<<this->currentDir->myFolderName<<" som har "<<this->currentDir->folderVec.size()<<" mappar."<<std::endl;
	std::cout<<"min pappa heter: "<<this->currentDir->parent->myFolderName<<std::endl;
	for (int h = 0; h < this->currentDir->folderVec.size(); h++)
	{
		std::cout<<this->currentDir->folderVec[h].myFolderName<<std::endl;
	}
}
