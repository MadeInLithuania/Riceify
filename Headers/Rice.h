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
#include "Colors.h"
#include "Navigation.h"

class Rice {
    private:
    unsigned long memSize;
    std::string riceName;
    std::list<std::string> riceNames;
    std::time_t creationDate;
    std::vector<Rice> rices;
    std::vector<std::filesystem::path> files;
    std::string homedir = getenv("HOME");
    std::string dbDir = homedir + "/Riceify/db.rcf";
public:
    Rice(unsigned long _memSize, const std::string& _riceName, std::vector<Rice> _files, time_t _creationDate) {
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
    void addRice(){
        struct statvfs stat{};

        if (statvfs("/", &stat) != 0) {
            std::cerr << "Error (can't read the dir !)";
            exit(-1);
        }

        // the available size is f_bsize * f_bavail
        creationDate = time(nullptr);
        char* ct = ctime(&creationDate);
        GetHomeFilesAndSubfolders();
        try {
            std::cout << "Please input the rice name :" << std::endl;
            std::cin >> riceName;
            std::cout << "Creation date : " <<  ct << std::endl;
            std::cout << "Rice name : " << riceName << std::endl;
            memSize = stat.f_frsize;
            Rice *r = new Rice(memSize, riceName, rices, creationDate);
        }
        catch(std::exception &exception){
            throw std::exception(exception);
        }
        std::cout << "Success ! \n" <<
                  "Disk size : " << KGRN << memSize << RST <<
                  "\nRice name : " << KGRN << riceName << RST <<
                  "\nCreation date : " << KGRN << creationDate << RST << std::endl;
        std::cout << KRED << "You will be redirected in 3 seconds." << RST << std::endl;
        sleep(3);
    }
    void ListRice() {
        std::cout << "Rices : [" << rices.size() << "]" << std::endl;
        if(!rices.empty()){
            for (int i = 0; i < rices.size(); ++i) {
                std::cout << "[" << i + 1 << "]" << "\nDisk size : " <<
                          rices.at(i).memSize << "\nRice name :" <<
                          rices.at(i).riceName << "Creation date :" <<
                          rices.at(i).creationDate << std::endl;
            }
        }
        std::cout << "The array is empty." << std::endl;
        std::cout << KRED << "You will be redirected in 3 seconds." << RST << std::endl;
        sleep(3);
    }
    void GetHomeFilesAndSubfolders() {
        std::filesystem::path home = homedir;
        for (auto &p : std::filesystem::recursive_directory_iterator(home)) {
                files.push_back(p);
                std::cout << p << std::endl;
        }
        std::cout << "Found " << KMAG << files.size() << RST << " files." << std::endl;
    }

    void CreateFolder(std::string folderName, std::string path) {
        std::string command = "mkdir " + path + folderName;
        system(command.c_str());
    }
};ç
#endif //RICEIFY_RICE_H
