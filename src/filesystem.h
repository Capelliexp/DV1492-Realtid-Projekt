#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <string>
#include <iostream>
#include <fstream>

class FileSystem
{
private:
    MemBlockDevice mMemblockDevice;
	

    // Here you can add your own data structures

    class File {
    public:
        std::string name;
        int id;
		MemBlockDevice* memCopy;

        File(std::string fileName = "New File", int blockNr = -1, MemBlockDevice* pointer= nullptr) {
            name = fileName;    //OBS! måste kasta error om namnet innehåller /
            id = blockNr;
			//memCopy = pointer;
        }
        ~File(){
			//memCopy->freeBlockArr[id] = 0;
        }

        bool operator == (const std::string &filename)
        {
            return this->name == filename;
        }
    };

    class Folder {
    public:
        Folder* parent;

        std::string myFolderName;
        std::vector<Folder> folderVec;
        std::vector<File> fileVec;

        Folder(std::string name = "New Folder", Folder* folderParent = nullptr)
        {
            this->myFolderName = name;
            this->parent = folderParent;
        }

        ~Folder(){

        }

        bool operator==(const Folder &other)
        {
          return this->myFolderName==other.myFolderName;
        }
    };

    Folder root;
    Folder * currentDir;
	
public:
    FileSystem();
    ~FileSystem();

    /* These commands needs to implemented
     *
     * However, you are free to change the parameterlist and specify your own returntype for each function below.
     */

    /* This function creates a file in the filesystem */
    int createFile(std::string fileName, std::string content);

    // /* Removes a file in the filesystem */
    void removeFile(std::string fileName);

    // /* Creates a folder in the filesystem */
    void CreateFolder(std::string folderName);

    //  Removes a folder in the filesystem
    void RemoveFolder(std::string folderName);

    // /* Function will move the current location to a specified location in the filesystem */
    bool GoToFolder(std::string folderName);

    void listDir();

    // /* This function will get all the files and folders in the specified folder */
    std::string ListContent();

    std::string PrintFileContent(std::string fileName);

    bool CopyFile(std::string file1, std::string file2);

    // Folder GetCurrentFolder(std::string currentDir);   //har jag skapat

	void restoreImage(std::string filePath);
	//void recursiveFunction(std::string name, int nrOfFolders, int nrOfFiles);
	void recursiveFunction(Folder * dir, std::ifstream &file);

	void CreateImage(std::string sysName);

	void recursiveCreate(Folder * dir, std::ofstream &outputFile);

	
};

#endif // FILESYSTEM_H
