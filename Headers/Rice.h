//
// Created by tadas on 7/19/22.
//

#ifndef RICEIFY_RICE_H
#define RICEIFY_RICE_H

#include <list>
#include <string>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <unistd.h>
#include <sys/statvfs.h>
#include <vector>
#include <chrono>
#include <fstream>
#include "Colors.h"
class Rice{
private:
    unsigned long memSize;
    std::string riceName;
    std::list<std::string> riceNames;
    const time_t *creationDate;
    std::vector<Rice> rices;
    std::string logFile = "logs.log";
    std::vector<std::filesystem::path> files;
    int choice{};
protected:
    std::string homedir = getenv("HOME");
    std::string dbDir = homedir + "/Riceify/data.yaml";
    std::string dirFile = homedir + "/Riceify/" + logFile;
    std::string createCmd = "touch " + dirFile;
    std::string createDB = "mkdir ~/Riceify && cd ~/Riceify/ && touch data.yaml";
public:
    Rice(unsigned long _memSize, const std::string& _riceName, std::vector<Rice> _files, const time_t *_creationDate) {
        memSize = _memSize;
        riceName = _riceName;
        _files.push_back(*this);
        creationDate = _creationDate;
    }
    ~Rice() = default;
    //GETTERS
    std::string GetRiceName(){
        return riceName;
    }
    unsigned long GetMemSize() const{
        return memSize;
    }
    time_t GetCreationDate(){
        return *creationDate;
    }
    std::string GetDatabaseFile(){
        return dbDir;
    }

    void CheckForDatabase(){
        if(!std::filesystem::exists(dbDir)) {
            try{
                system("cd ~/Riceify/ && touch data.yaml");
                std::cout << "[" << KRED << "!" << RST << "] Created database at " << dbDir << std::endl;
                WriteToLog("[" + GetCurrentTime() + "] Created database at " + dbDir);
            }
            catch(std::exception &e){
                WriteToLog("[" + GetCurrentTime() + "] Failed to create database at " + dbDir + ": " + e.what());
                throw std::exception();
            }
        }
        else std::cout << "[" << KGRN << "*" << RST << "] Database exists ;) "<< std::endl;
    }
    //PRELIST
    void GetRiceList(){
        std::cout << "Rices : [" << rices.size() << "]" << std::endl;
        if(!rices.empty()){
            for (long unsigned int i = 0; i < rices.size(); ++i) { //Experimental
                std::cout << "[" << i + 1 << "]" << "\nDisk size : " <<
                          rices.at(i).memSize << "\nRice name :" <<
                          rices.at(i).riceName << "\nCreation date :" <<
                          ctime(rices.at(i).creationDate) <<
                          "=================" << std::endl;
            }
        }
        else{
            std::cout << "No rices found" << std::endl;
        }
    }
    //NUMBER 1
    void ListRice() {
        GetRiceList();
        std::cout << KRED << "You will be redirected soon." << RST << std::endl;
        sleep(3);
        DisplayMenu();
    }
    //NUMBER 2
    void addRice(){
        struct statvfs stat{};
        if (statvfs("/", &stat) != 0) {
            std::cerr << "Error (can't read the dir !)";
            WriteToLog("[" + GetCurrentTime() + "] Error (can't read the dir !)");
            exit(-1);
        }

        // the available size is f_bsize * f_bavail
        auto end = std::chrono::system_clock::now();
        std::time_t _time = std::chrono::system_clock::to_time_t(end);
        creationDate = &_time;
        GetHomeFilesAndSubfolders();
        try {
            std::cout << "Please input the rice name :" << std::endl;
            std::cin >> riceName;
            std::cout << "Rice name : " << riceName << std::endl;
            memSize = std::filesystem::space("/").available;
        }
        catch(std::exception &exception){
            WriteToLog("[" + GetCurrentTime() + "]" + exception.what());
            throw std::exception(exception);
        }
        std::cout << "Success ! \n" <<
                  "Disk size : " << KGRN << memSize << RST <<
                  "\nRice name : " << KGRN << riceName << RST <<
                  "\nCreation date : " << KGRN << std::ctime(creationDate) << RST << std::endl;
        std::cout << KRED << "You will be redirected soon." << RST << std::endl;
        if(!std::filesystem::exists(homedir + "/Riceify/rices/")){
            system("mkdir ~/Riceify/rices/");
        }
        try{
            CreateFolder(riceName, homedir + "/Riceify/rices/");
            WriteToLog("[" + GetCurrentTime() + "] Created rice folder at " + homedir + "/Riceify/rices/" + riceName);
        }catch (std::exception &e){
            WriteToLog("[" + GetCurrentTime() + "] Failed to create folder : " + e.what());
            throw std::exception(e);
        }
    }
    //NUMBER 3
    void RemoveRice(){
        int toRemove;
        GetRiceList();
        std::cout << "Please input a rice to delete :" << std::endl;
        std::cin >> toRemove;
        if(!std::cin.fail()){
            std::cerr << "Not a valid choice !" << std::endl;
            RemoveRice();
        }
        else rices.erase(rices.begin()+toRemove);
    }

    //----

    void DisplayMenu(){
        std::cout << "1. List all the rices" << std::endl;
        std::cout << "2. Add a rice" << std::endl;
        std::cout << "3. Remove a rice" << std::endl;
        std::cout << "4. Edit a rice" << std::endl;
        std::cout << "5. Switch rices" << std::endl;
        std::cout << "6. Clear logs" << std::endl;
        std::cout << "7. Exit" << std::endl;
        GetChoice();
    }
    void GetChoice() {
        std::cout << "Please enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                ListRice();
                break;
            case 2:
                addRice();
                break;
            case 3:
                RemoveRice();
                break;
            case 4:
                std::cout << "SOON" << std::endl;
                break;
            case 5:
                std::cout << "Not yet !" << std::endl;
                break;
            case 6:
                ClearLog();
                break;
            case 7:
                exit(1);
            default:
                std::cout << "Invalid choice" << std::endl;
                break;
        }
    }
    //FINDS ONLY FILES, NOT COPYING THEM
    void GetHomeFilesAndSubfolders() {
        std::filesystem::path home = homedir;
        std::cout << "[" << KCYN << "*" << RST << "] Getting files and subfolders from " << KMAG << home << std::endl;
        try{
            for (auto &p : std::filesystem::recursive_directory_iterator(home)) {
                files.push_back(p);
            }
        }catch(std::exception &ex){
            std::cout << KRED << &ex << std::endl; // WITHOUT IT THROWS what(): filesystem error: cannot increment recursive directory iterator: Permission denied
        }
        std::cout << "Found " << KMAG << files.size() << RST << " files." << std::endl;
    }

    void CreateFolder(const std::string& folderName, const std::string& path) {
        if(!std::filesystem::exists(path + folderName)) {
            try{
                system(("mkdir " + path + folderName).c_str());
                std::cout << "[" << KRED << "!" << RST << "] Created folder at " << path + folderName << std::endl;
            }
            catch(std::exception &ex){
                std::cout << KRED << &ex << std::endl;
                WriteToLog("[" + GetCurrentTime() + "] Error : " + ex.what());
            }
        }
        else std::cout << "[" << KGRN << "*" << RST << "] Folder exists ;) "<< std::endl;
        auto r = new Rice(memSize, riceName, rices, creationDate);
        rices.push_back(*r);
        CopyFiles(riceName);
    }
    void CopyFiles(const std::string &_riceName){
        std::string fontDir = "/usr/share/fonts/";
        std::cout << "Copying files..." << std::endl;
        //USING COMMANDS
        std::string cmd = "cp -r ~/.config ~/Riceify/rices/" + _riceName; //GOING EAT, CREATES FOLDERS SEPARATELY
        std::string fonts = "cd ~ && sudo -S mkdir ~/Riceify/rices/" + riceName + "/usr/ && cd ~/Riceify/rices/" + riceName + "/usr/" +
                            " && sudo -S mkdir ~/Riceify/rices/" + riceName + "/share/ && cd ~/Riceify/rices/" + riceName + "/share/"+
                            //"&& sudo -S mkdir ~/Riceify/Rices/" + riceName + "/fonts/ " +
                            " && sudo -S mkdir ~/Riceify/rices/" + riceName + fontDir + " && cd fonts/ && sudo -S cp -r " + fontDir + " ~/Riceify/rices/" + riceName;
        try{
            system(cmd.c_str());
            std::cout << "[" << KGRN << "*" << RST << "] Copied config files." << std::endl;
            system(fonts.c_str());
            std::cout << "[" << KGRN << "*" << RST << "] Copied fonts." << std::endl;
        }
        catch(std::exception &e){
            WriteToLog("[" + GetCurrentTime() + "] Error : " + e.what());
            std::cout << "Error : " << KRED << &e << std::endl;
        }
        DisplayMenu();
    }

    std::string GetDirLogFile()
    {
        return dirFile;
    }
    std::string GetCmdLog(){
        return createCmd;
    }

    static std::string GetCurrentTime(){
        time_t now = time(nullptr);
        char* dt = ctime(&now);
        return dt;
    }

    void WriteToLog(const std::string& message){
        std::ofstream log(dirFile, std::ios::app);
        log << message << std::endl;
        log.close();
    }

    void ClearLog(){
        char _choice;
        std::cout << KRED << "WARNING: " << RST << "This action cannot be undone" << std::endl << "Confirm? [y/n]: ";
        std::cin >> _choice;
        switch (_choice) {
            case 'y':
                try{
                    std::ofstream log(dirFile, std::ios::trunc);
                    log.close();
                }catch(std::exception &e){
                    std::cerr << KRED << &e << std::endl;
                    WriteToLog("[" + GetCurrentTime() + "] " + e.what());
                }
                std::cout << KGRN << "Log file cleared" << RST << std::endl;
                std::cout << KRED << "You will be redirected to the main menu" << RST << std::endl;
                sleep(3);
                DisplayMenu();
                break;
            case 'n':
                std::cout << KRED << "You will be redirected to the main menu" << RST << std::endl;
                sleep(3);
                DisplayMenu();
                break;
            default:
                std::cout << KRED << "Invalid choice" << RST << std::endl;
                std::cout << KRED << "You will be redirected to the main menu" << RST << std::endl;
                sleep(3);
                DisplayMenu();
        }
    }
};
//RICE CLASS
#endif //RICEIFY_RICE_H