#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 15;
bool test;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
std::string help();

int main(void) {

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output

    bool bRun = true;

    FileSystem f; // <<<--------


    //return 0;

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

            case 0: // quit
                bRun = false;
                std::cout << "Exiting\n";
                break;

            case 1:{ // format
				f = FileSystem();
				break;
			}
                

			case 2: { // ls
				std::string returnValue = f.ListContent();
				std::cout << returnValue << std::endl;
				break;
			}

            case 3:{ // create
                userCommand.erase (0,7);
                std::string name = userCommand;
                std::cout << "write content:" << std::endl;
                std::string content;
                //std::cin >> content;
				getline(std::cin, content);
                int rtn = f.createFile(userCommand, content);
				if (rtn == -1) {
					std::cout << "error writing to block" << std::endl;
				}
				else if (rtn == -2) {
					std::cout << "out of memory" << std::endl;
				}
                break;
            }

            case 4:{ // cat
                userCommand.erase (0,4);
                std::string rtn = f.PrintFileContent(userCommand);
                std::cout << rtn << std::endl;
                break;
            }

            case 5: // createImage
                break;

			case 6: { // restoreImage
				f = FileSystem();
				userCommand.erase(0, 13);
				f.restoreImage(userCommand);

				break;
			}
            case 7:{ // rm
                userCommand.erase (0,3);
                f.removeFile(userCommand);
                break;
            }

            case 8:{ // cp
                userCommand.erase (0,3);
                std::size_t split = userCommand.find(" ");
                std::string file1, file2;
                //userCommand.copy(file1.c_str(), 0, split-1);
                //userCommand.copy(*file2, split+1, userCommand.length());
				file1 = userCommand.substr(0, split);
				file2 = userCommand.substr(split + 1, userCommand.length());

				if (f.CopyFile(file1, file2) != true)
					std::cout << "Error in copy operation" << std::endl;
               break; 
            }
                
            case 9: // append - behövs ej
                break;

            case 10: // mv - behövs ej
                break;

            case 11: // mkdir
                userCommand.erase (0,6);
                f.CreateFolder(userCommand);
                break;

            case 12:{ // cd
                userCommand.erase (0,3);
                test = f.GoToFolder(userCommand);

				if (test == true && currentDir == "/") {
					currentDir = currentDir + userCommand;
				}
                else if(test == true && userCommand==".."){
                  //auto it = std::find(currentDir.rbegin(), currentDir.rend(), "/");
                  //currentDir.erase(it,currentDir.end());
                  size_t pos = currentDir.rfind("/");
				  if (pos != 0)
					  currentDir = currentDir.substr(0, pos);
				  else
					  currentDir = currentDir.substr(0, 1);
                }
                else if(test == true){
                  currentDir = currentDir + "/" + userCommand;
                }
				else
				{
					break;
				}
                break;
            }

            case 13: // pwd
			{
				std::cout << currentDir << std::endl;
				break;
			}
                

            case 14: // help
                std::cout << help() << std::endl;
                break;

            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);

    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}
