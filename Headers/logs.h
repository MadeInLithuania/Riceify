//
// Created by tadas on 15/09/22.
//

#ifndef RICEIFY_LOGS_H
#define RICEIFY_LOGS_H

#include "Rice.h"

class Logs: public Rice{
    std::string logFile = "logs.log";
    std::string dirFile = homedir + "/Riceify/" + logFile;
    std::string createCmd = "touch " + dirFile;

    void CreateLogFile(){
        try{
            system(createCmd.c_str());
        }
        catch(std::exception &e){
            std::cerr << KRED << &e << std::endl;
        }
    }
};
#endif //RICEIFY_LOGS_H
