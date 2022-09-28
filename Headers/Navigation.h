//
// Created by tadas on 7/20/22.
//

#ifndef RICEIFY_NAVIGATION_H
#define RICEIFY_NAVIGATION_H

#include <iostream>
#include "Rice.h"
#include "logs.h"

class Navigation{
private:
    int choice{};
    Rice *rice = new Rice(0,"",{},0);
    Logs logs = *new Logs();
public:
    void GetHomeDir(){
        if(!std::filesystem::exists(logs.GetDirLogFile()))
        {
            system(logs.GetCmdLog().c_str());
        }
        std::string homedir = getenv("HOME");
        std::cout << "Home directory is " << KMAG << homedir << RST << std::endl;
        std::cout << "The PID of the process is " << KMAG << getpid() << RST << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
    }
    void DisplayMenu(){
        std::cout << "1. List all the rices" << std::endl;
        std::cout << "2. Add a rice" << std::endl;
        std::cout << "3. Remove a rice" << std::endl;
        std::cout << "4. Edit a rice" << std::endl;
        std::cout << "5. Switch rices" << std::endl;
        std::cout << "6. Exit" << std::endl;
        GetChoice();
    }
    void GetChoice(){
        std::cout << "Please enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                rice->ListRice();
                break;
            case 2:
                rice->addRice();
                break;

            case 3:
                rice->RemoveRice();
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
};
#endif //RICEIFY_NAVIGATION_H
