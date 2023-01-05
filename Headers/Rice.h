//
// Created by tadas on 7/19/22.
//

#ifndef RICEIFY_RICE_H
#define RICEIFY_RICE_H

#include <cstdlib>
#include <list>
#include <string>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <unistd.h>
#include <sys/statvfs.h>
#include <vector>
#include <chrono>
#include "Colors.h"

class Rice{
#pragma err
private:
    unsigned long memSize;
    std::string riceName;
    std::list<std::string> riceNames;
    const time_t *creationDate;
    std::vector<Rice> rices;
    std::vector<std::filesystem::path> files;
    int choice;
protected:
    std::string homedir = getenv("HOME");
    std::string dbDir = homedir + "/Riceify/db.rcf";
public:
    Rice(unsigned long _memSize, const std::string& _riceName, std::vector<Rice> _files, const time_t *_creationDate) {
        memSize = _memSize;
        riceName = _riceName;
        _files.push_back(*this);
        creationDate = _creationDate;
    }
    void CheckForDatabase(){
        if(!std::filesystem::exists(dbDir)) {
            try{
                system("mkdir ~/Riceify && cd ~/Riceify/ && touch db.rcf");
                std::cout << "[" << KRED << "!" << RST << "] Created database at " << dbDir << std::endl;
            }
            catch(...){
                throw std::exception();
            }
        }
        else std::cout << "[" << KGRN << "*" << RST << "] Database exists ;) "<< std::endl;
    }
    //PRELIST
    void GetRiceList(){
        std::cout << "Rices : [" << rices.size() << "]" << std::endl;
        if(!rices.empty()){
            for (int i = 0; i < rices.size(); ++i) {
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
            exit(-1);
        }

        // the available size is f_bsize * f_bavail
        auto end = std::chrono::system_clock::now();
        std::time_t _time = std::chrono::system_clock::to_time_t(end);
        creationDate = &_time;
        char* ct = ctime(creationDate);
        GetHomeFilesAndSubfolders();
        try {
            std::cout << "Please input the rice name :" << std::endl;
            std::cin >> riceName;
            std::cout << "Rice name : " << riceName << std::endl;
            memSize = std::filesystem::space("/").available;
            Rice *r = new Rice(memSize, riceName, rices, creationDate);
        }
        catch(std::exception &exception){
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
        CreateFolder(riceName, homedir + "/Riceify/rices/");
        auto r = new Rice(memSize, riceName, rices, creationDate);
        rices.push_back(*r);
        CopyFiles(riceName);
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
        std::cout << "6. Exit" << std::endl;
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
                //std::cout << p << std::endl;
            }
        }catch(std::exception &ex){
            std::cout << KRED << &ex << std::endl; // WITHOUT IT THROWS what(): filesystem error: cannot increment recursive directory iterator: Permission denied
        }
        std::cout << "Found " << KMAG << files.size() << RST << " files." << std::endl;
    }

    static void CreateFolder(std::string folderName, std::string path) {
        if(!std::filesystem::exists(path + folderName)) {
            try{
                system(("mkdir " + path + folderName).c_str());
                //system(("mkdir " + path + folderName + "/.config").c_str());
                std::cout << "[" << KRED << "!" << RST << "] Created folder at " << path + folderName << std::endl;
            }
            catch(...){
                //throw std::exception();
            }
        }
        else std::cout << "[" << KGRN << "*" << RST << "] Folder exists ;) "<< std::endl;
    }
    void CopyFiles(const std::string& riceName){
        std::string fontDir = "/usr/share/fonts";
        std::cout << "Copying files..." << std::endl;
        CopyConfigFolder();
        CopyHomeFiles();
        DisplayMenu();
    }
    void CopyConfigFolder(){
        std::string cmd = "cp -r ~/.config ~/Riceify/rices/"
                          + riceName + "/.config/";//+"&& sudo cp -r" + fontDir + " ~/Riceify/rices/" + riceName;
        try{
            system(cmd.c_str());
        }
        catch(std::exception &e){
            std::cout << "Error : " << KRED << &e << std::endl;
        }
        std::cout << "[" << KGRN << "+" << RST << "] Copied config file successfully." << std::endl;
    }
    void CopyHomeFiles(){
      std::string cmd = "rsync -v ~/.??* ~/Riceify/rices/"
                          + riceName + "/";
        try{
            system(cmd.c_str());
        }
        catch(std::exception &e){
            std::cout << "Error : " << KRED << &e << std::endl;
        }
        std::cout << "[" << KGRN << "+" << RST << "]" << "Copied home files" << std::endl;  
    }
};
#endif //RICEIFY_RICE_H