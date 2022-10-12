//
// Created by tadas on 7/20/22.
//

#ifndef RICEIFY_NAVIGATION_H
#define RICEIFY_NAVIGATION_H

#include <iostream>
#include "Rice.h"
#include "logs.h"
#include "Banner.h"

class Navigation{
private:
    int choice{};
    Rice *rice = new Rice(0,"",{},0);
    Banner *banner = new Banner();
    Logs logs = *new Logs();

public:
    void GetHomeDir(){
        ClearTerminal();
        if(!std::filesystem::exists(logs.GetDirLogFile()))
        {
            try{
                system(rice->GetCmdLog().c_str());
                rice->WriteToLog("[" + Rice::GetCurrentTime() + "]" + "Created log file at " + rice->GetDirLogFile() + "\n");
                std::cout << "[" << KRED << "!" << RST << "] Created log file at " << rice->GetDirLogFile() << std::endl;

            }
            catch(std::exception &e){
                std::cout << "[" << KRED << "!" << RST << "] Failed to create log file at " << rice->GetDirLogFile() << ": " << e.what() << std::endl;
                rice->WriteToLog("[" + Rice::GetCurrentTime() + "]" + "Failed to create log file at " + rice->GetDirLogFile() + ": " + e.what() + "\n");
                throw std::exception();
            }
        }
        std::cout << "[" << KGRN << "*" << RST << "] Log file already exists at " << rice->GetDirLogFile() << std::endl;
        rice->WriteToLog("[" + Rice::GetCurrentTime() + "] Log file already exists at " + rice->GetDirLogFile() + "\n");
        std::string homedir = getenv("HOME");
        std::cout << "Home directory is " << KMAG << homedir << RST << std::endl;
        std::cout << "The PID of the process is " << KMAG << getpid() << RST << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
    }
    void DisplayMenu(){
        banner->GetRandomBanner();
        SetAuthorBanner();
        std::cout << "1. List all the rices" << std::endl;
        std::cout << "2. Add a rice" << std::endl;
        std::cout << "3. Remove a rice" << std::endl;
        std::cout << "4. Edit a rice" << std::endl;
        std::cout << "5. Switch rices" << std::endl;
        std::cout << "6. Clear log file" << std::endl;
        std::cout << "7. Exit" << std::endl;
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
                rice->ClearLog();
                break;
            case 7:
                exit(1);
            default:
                std::cout << "Invalid choice" << std::endl;
                break;
        }
    }
    static void ClearTerminal(){
        system("clear");
    }
    static void SetAuthorBanner(){
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << KYEL << "Riceify" << RST << " made by " << KGRN << "ZukiLTU <3" << RST << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
    }
};
#endif //RICEIFY_NAVIGATION_H
