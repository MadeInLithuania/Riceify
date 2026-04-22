//
// Created by tadas on 7/19/22.
//

#ifndef RICEIFY_RICE_H
#define RICEIFY_RICE_H

#include <cstdlib>
#include <list>
#include <ostream>
#include <string>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <sys/statvfs.h>
#include <vector>
#include <chrono>
#include <dirent.h>
#include <cstddef>
#include "Banner.h"
#include "Colors.h"
#include <dirent.h>

class Rice{
#pragma err
private:
    unsigned long memSize;
    std::string riceName;
    std::list<std::string> riceNames;
    const time_t *creationDate;
    std::vector<Rice> rices;
    std::vector<std::filesystem::path> files;
    std::vector<std::string> dirs;
    int choice;
protected:
    std::string homedir = getenv("HOME");
    std::string riceDir = homedir + "/Riceify/rices/";
public:
    Rice(unsigned long _memSize, const std::string& _riceName, std::vector<Rice> _files, const time_t *_creationDate) {
        memSize = _memSize;
        riceName = _riceName;
        _files.push_back(*this);
        creationDate = _creationDate;
    }

    //PRELIST
    int GetRiceList(){
        /*if(std::filesystem::exists(homedir + "/Riceify/rices/"))
        {*/
            dirs.clear();
            for (auto &p : std::filesystem::directory_iterator(riceDir)) {
                if(p.is_directory()){
                    dirs.push_back(p.path().string());
                }
            }
            std::cout << "Rices : [" << dirs.size() << "]" << std::endl;
            if(dirs.size() == 0){
                std::cout << "The list is empty." << std::endl;
            }
            else{
                for (int i = 0; i < dirs.size(); i++) {
                    std::cout << "[" << i+1 << "] - " << dirs[i] << std::endl;
                }
            }
        /*}
        else{
            std::cerr << "[" << KRED << "!" << RST << "] Directory " << homedir << "/Riceify/rices/ not found." << std::endl; 
        }*/
        return dirs.size();      
    }
    int GetRiceListWithLen(){
        for (auto &p : std::filesystem::directory_iterator(riceDir)) {
            if(p.is_directory()){
                dirs.push_back(p.path().string());
            }
        }
        std::cout << "Rices : [" << dirs.size() << "]" << std::endl;
        if(dirs.size() == 0){
            std::cout << "The list is empty." << std::endl;
        }
        else{
            for (int i = 0; i < dirs.size(); i++) {
                std::cout << "[" << i+1 << "] - " << dirs[i] << std::endl;
            }
        }
        return dirs.size();
    }
    //NUMBER 1
    void ListRice() {
        GetRiceList();
        std::cout << KRED << "You will be redirected soon." << RST << std::endl;
        sleep(3);
        dirs.clear();
        DisplayMenu();
    }

    //NUMBER 2
    void AddRice(){
        try {
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

        std::cout << "Please input the rice name :" << RST << std::endl;
        std::cin >> riceName;
        rtrim(riceName);
        std::cout << "Rice name : " << riceName << std::endl;
        std::cout << "Success ! \n" <<
                  "\nRice name : " << KGRN << riceName << RST <<
                  "\nCreation date : " << KGRN << ct << RST << std::endl;
        std::cout << KRED << "You will be redirected soon." << RST << std::endl;
        if(!std::filesystem::exists(homedir + "/Riceify/rices/")){
            system("mkdir ~/Riceify/rices/");
        }
        CreateFolder(riceName, homedir + "/Riceify/rices/");
        auto rice = new Rice(memSize, riceName, rices, creationDate);
        rices.push_back(*rice);
        CopyFiles(riceName);
        }
        catch(std::exception &exception){
            throw std::exception(exception);
        }
    }
    //NUMBER 3
    void RemoveRice(){
        int toRemove = NULL;
        if(std::filesystem::exists(homedir + "/Riceify/rices/"))
        {
            if(dirs.size() == 0){
                std::cerr << "[" << KRED << "!" << RST << "] No rices found." << std::endl;
                std::cout << KRED << "You will be redirected soon." << RST << std::endl; 
                sleep(3);
                DisplayMenu();
            }
            else{
                GetRiceList();
                std::cout << "Please input a rice to delete :" << std::endl;
                std::cin >> toRemove;
                std::cout << "Test : " << dirs[toRemove] << std::endl;
                if(std::cin.fail()){
                    std::cerr << "Not a valid choice !" << std::endl;
                    RemoveRice();
                }
                else {
                    //rices.erase(rices.begin()+toRemove);
                    std::string delCmd = "rm -rf " + dirs[toRemove];
                    system(delCmd.c_str());
                }
                std::cout << "[" << KGRN << "*" << RST << "] Rice deleted successfully." << std::endl;
                std::cout << KRED << "You will be redirected soon." << RST << std::endl;
                sleep(3);
                DisplayMenu();                
            }
        }
        else{
            std::cerr << "[" << KRED << "!" << RST << "] Directory " << homedir << "/Riceify/rices/ not found." << std::endl; 
            std::cout << KRED << "You will be redirected soon." << RST << std::endl;
            sleep(3);
            DisplayMenu();
        }
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
                AddRice();
                break;
            case 3:
                RemoveRice();
                break;
            case 4://edit rice
                EditRice();
                break;
            case 5:
                SwitchRice();
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
        files.clear();
        std::string excludedFolderPath = homedir + "Riceify/rices/";
        std::filesystem::path home = homedir;
        std::cout << "[" << KCYN << "*" << RST << "] Getting files and subfolders from " << KMAG << home << std::endl;
        try{
            if(!std::filesystem::exists(excludedFolderPath)){
                excludedFolderPath = "";
            }
            for (auto &fd : std::filesystem::directory_iterator(home))
            {
                if(fd.is_directory()){
                    std::string filePath = fd.path().generic_string();
                    std::string homePath = home.generic_string();
                    std::string shortenedPath = filePath.replace(0, homePath.length(), "~");
                    //std::cout << shortenedPath << std::endl;
                }
            }
            for (auto &p : std::filesystem::recursive_directory_iterator(home)){
                if(p.path() != excludedFolderPath)
                files.push_back(p);
            }
        }catch(std::exception &ex){
            std::cout << KRED << &ex << std::endl; // WITHOUT IT THROWS what(): filesystem error: cannot increment recursive directory iterator: Permission denied
        }
        //std::cout << "Found " << KMAG << files.size() << RST << " files." << std::endl; //USELESS FROM NOW
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
        try{
            std::string fontDir = "/usr/share/fonts";
            if(!CopyFolder("mkdir ~/Riceify/rices/" + riceName + "/.config", "Creating config folder ...", "Created config folder successfully.")) throw new std::exception;
            std::cout<<"test2"<<std::endl;
            //CopyFolder("cp -r ~/.config ~/Riceify/rices/" + riceName + "/.config/","Copying config files ...", "Copied config files successfully.");
            if(!CopyFolder("rsync -a ~/.??* ~/Riceify/rices/" + riceName + "/", "Copying home files ...", "Copied home files")) throw new std::exception;
            std::cout<<"test"<<std::endl;
            DisplayMenu();
        }
        catch(std::exception &ex){
            std::cout << "[" << KRED << "!" << RST << "] Error while copying folder : " << KRED << &ex << RST << std::endl;
        }
    }
    bool CopyFolder(const std::string& cmd, const std::string& onCopyStr = "", const std::string & onSuccess = ""){
        try{
            std::cout << onCopyStr << std::endl;
            if(cmd.size() != 0){
                system(cmd.c_str());
            }
            std::cout << "[" KGRN << "+" << RST << "]" << onSuccess << std::endl;
            return true;
        }
        catch(std::exception &e){            
            std::cout << "Error : " << KRED << &e << std::endl;
            return false;
        }
    }
    //change de config
    void SwitchRice(){
        int index;
        int len = GetRiceListWithLen();
        if(len == 1){
            std::cout << "Can't switch with one rice." << std::endl;
            sleep(2);
            DisplayMenu();
        }
        else if (len == 0){
            std::cout << "No rices found." << std::endl;
            std::cout << KRED << "\nYou will be redirected soon." << RST << std::endl;
            sleep(3);
            DisplayMenu();
        }
        else
        {
            std::cout << "Please select the rice to switch : " << std::endl;
            std::cin >> index;
            std::string chosenRice = dirs[index];
            sleep(1);
            std::cout << "Chosen the rice : " << dirs[index - 1] << std::endl;
            if(!std::filesystem::exists(dirs[index - 1])){
                std::cout << "Directory doesn't exist. Operation halted." << std::endl;
                sleep(2);
                DisplayMenu();
            }
            else{
                std::cout << KRED << "This step requires sudo authentification." << RST << std::endl; 
                std::string finalCMD = "sudo cp -rT " + dirs[index - 1] + "/ ~";
                std::cout << finalCMD << std::endl;
                bool a = system(finalCMD.c_str());
                if(a)
                    std::cerr << "[" << KRED << "!" << RST << "]" << "Error occured." << std::endl;
                else
                    std::cout << "Please reboot your device to finalize the changes." << std::endl;
            }
        }
    }
    void EditRice(){
        int choice;
        int len = GetRiceListWithLen();
        if(len == 0){
            std::cout << "No rices found." << std::endl;
            std::cout << KRED << "\nYou will be redirected soon." << RST << std::endl;
            sleep(3);
            DisplayMenu();
        }
        std::cout << "Please choose the rice to edit :" << std::endl;
        std::cin >> choice;
        if(choice > dirs.size() && choice < 1){
            std::cerr << "Index is out of range." << std::endl;
            EditRice();
        }
        else{
            std::cout << "Starting copying from " << KMAG << homedir << RST << " to " << KMAG << dirs[choice-1] << RST << std::endl;
            std::string cmd = "sudo cp -rT ~ " + dirs[choice-1];
            std::cout << KRED << "This step requires sudo authentification." << RST << std::endl; 
            bool isSuccess = system(cmd.c_str());
            if(isSuccess)
                std::cerr << "Error while copying." << std::endl;
            else{
                std::cout << "[" << KGRN << "*" << RST << "] Task completed successfully." << std::endl;
            }
            std::cout << KRED << "\nYou will be redirected soon." << RST << std::endl;
            sleep(3);
            dirs.clear();
            DisplayMenu();
        }
    }
    inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
~Rice(){
    
}
};
#endif //RICEIFY_RICE_H