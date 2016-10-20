#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <string>

class FileSystem
{
private:
    MemBlockDevice mMemblockDevice;

    // Here you can add your own data structures

    class File {
    public:
        std::string name;
        int id;

        File(std::string fileName = "New File", int blockNr = -1) {
            name = fileName;    //OBS! måste kasta error om namnet innehåller /
            id = blockNr;
        }
        ~File(){

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
    void ListContent();

    std::string PrintFileContent(std::string fileName);

    void CopyFile(std::string file1, std::string file2);

    // Folder GetCurrentFolder(std::string currentDir);   //har jag skapat
};

#endif // FILESYSTEM_H
