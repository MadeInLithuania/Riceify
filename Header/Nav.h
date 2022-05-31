//
// Created by tadas on 5/17/22.
//

#ifndef RICEIFY_NAV_H
#define RICEIFY_NAV_H

#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif

#include "Rice.h"
#include "GetDirectories.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string>
#include <fstream>
namespace fs = std::experimental::filesystem;

class Nav{
private:
    GetDirectories getDirectories = *new GetDirectories();
    Rice *rice = new Rice(0, 0, " ", NULL);
    int choice{};
    std::ifstream fs;
    std::string homedir = getenv("HOME");
    std::string fullDir = homedir + "/";
public:
    int Print(){
        menu:
        printf("Made by <3 by ZukiDev#0001\n\nPlease make a choice :\n1 - Scan files\n2 - Save rice\n3 - Rice list\n4 - Switch to\n5 - Delete rice\n");
        std::cin >> choice;
        ChooseActions(choice);
        return choice;
    }
    void ChooseActions(int cho){
        switch (cho) {
            case 1: //scan files
                getDirectories.GetOS();
                getDirectories.FileExists(".bashrc");
                getDirectories.FileExists(".zshrc");
                getDirectories.FileExists(".cshrc");
                getDirectories.FileExists(".kshrc");
                for (const auto & entry : fs::directory_iterator(fullDir))
                    std::cout << KYEL << "File detected :" << entry.path() << RST << std::endl;
                break;
            case 2: //save rice
                rice->addRice();
                break;
            case 3: // display rices
                rice->ListRice();
                break;
            case 4:// switch riceFileExists
                exit(0);
                break;
            case 5:// delete rice
                exit(0);
                break;
            default:
                std::cerr << "Unknown choice, aborting.";
                exit(-1);
                break;
        }
    }
};

#endif //RICEIFY_NAV_H
