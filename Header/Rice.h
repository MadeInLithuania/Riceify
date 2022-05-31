//
// Created by tadas on 5/13/22.
//
#ifndef RICEIFY_RICE_H
#define RICEIFY_RICE_H

#include <vector>
#include <list>
#include <string>
#include <ctime>
#include <sys/statvfs.h>
#include <iostream>
#include <unistd.h>
#include "Colors.h"
#include "Nav.h"
class Rice{
private:
    int packageNumbers;
    unsigned long memSize;
    std::string riceName;
    std::vector<std::string> riceNames;
    std::vector<std::string> packageNames;
    std::time_t creationDate;
    std::vector<Rice> rices;
public:
    Rice(int _packageNumbers, unsigned long _memSize, const std::string& _riceNames, std::time_t _creationDate){
        packageNumbers = _packageNumbers;
        memSize = _memSize;
        riceName = _riceNames;
        //packageNames = _packageNames;
        creationDate = _creationDate;
    }

    ~Rice()= default;

    int getPackageNumbers() const{
        return packageNumbers;
    }
    int getMemSize() const{
        return memSize;
    }
    std::vector<std::string> getRiceNames(){
        return riceNames;
    }
    std::vector<std::string> getPackageNames(){
        return packageNames;
    }
    std::time_t getCreationDate() const{
        return creationDate;
    }
    void setMemSize(int _mem) const{
        _mem = memSize;
    }
    void setRiceName(std::string name) const{
        name = riceName;
    }
    void setCreationName(std::time_t date) const{
        date = creationDate;
    }
    //FINALLY, THE FUN BEGINS
    void addRice(){
        struct statvfs stat{};

        if (statvfs("/", &stat) != 0) {
            std::cerr << "Error (can't read the dir !)";
            exit(-1);
        }

        // the available size is f_bsize * f_bavail
        unsigned long gigabytes = stat.f_bsize * stat.f_bavail;
        creationDate = time(nullptr);
        char* ct = ctime(&creationDate);

        try {
            std::cout << "Please input the rice name :" << std::endl;
            std::cin >> riceName;
            std::cout << "Creation date : " <<  ct << std::endl;
            std::cout << "Rice name : " << riceName << std::endl;
            memSize = stat.f_frsize;
            Rice *r = new Rice(0, memSize, riceName, creationDate);
            rices.push_back(*r);
        }
        catch(std::exception exception){
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
};
#endif //RICEIFY_RICE_H
