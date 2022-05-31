//
// Created by tadas on 4/8/22.
//

#ifndef RICEIFY_GETDIRECTORIES_H
#define RICEIFY_GETDIRECTORIES_H
#include <string>
#include <cstdlib>
#include "Colors.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sys/utsname.h>
#include <iostream>
#include <pwd.h>

class GetDirectories {
private:
    unsigned int myuid;
    passwd *mypasswd;
public:
    inline bool FileExists(const std::string &name);
    void GetOS();

    GetDirectories(){
        myuid = getuid();
        mypasswd = getpwuid(myuid);
    }
};

void GetDirectories::GetOS() {
    struct utsname name{};
    if(uname(&name)) exit(-1);
    std::cout << "Current OS : " << KCYN << name.sysname << name.release << RST << std::endl;
    std::cout << "Current PID :" << KCYN << myuid << RST << std::endl;
}

bool GetDirectories::FileExists(const std::string &name) {
    std::ifstream fs;
    std::string homedir = getenv("HOME");
    std::string fullDir = homedir + "/";

    homedir = getpwuid(getuid())->pw_dir;

    fs.open(fullDir + name);

    if(fs){
        std::cout << "File exists : " << KGRN << fullDir << name << RST << std::endl;
        return true;
    }
    else{
        std::cout << KRED << name << RST << " doesn't exist !" << std::endl;
        return false;
    }
}
#endif //RICEIFY_GETDIRECTORIES_H
