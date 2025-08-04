//
// Created by tadas on 7/20/22.
//

#ifndef RICEIFY_NAVIGATION_H
#define RICEIFY_NAVIGATION_H

#include <iostream>
#include <memory>
#include "Rice.h"
#include "logs.h"
#include "Banner.h"

class Navigation{
private:
    int choice{};
    std::unique_ptr<Rice> rice;
    std::unique_ptr<Banner> banner;
    std::unique_ptr<Logs> logs;

public:
    Navigation() : rice(std::make_unique<Rice>(0,"",std::vector<Rice>{},nullptr)),
                   banner(std::make_unique<Banner>()),
                   logs(std::make_unique<Logs>()) {}
    
    ~Navigation() = default;
    
    // Prevent copying
    Navigation(const Navigation&) = delete;
    Navigation& operator=(const Navigation&) = delete;
    
    // Allow moving
    Navigation(Navigation&&) = default;
    Navigation& operator=(Navigation&&) = default;
    
    void GetHomeDir(){
        ClearTerminal();
        if(!std::filesystem::exists(logs->GetDirLogFile()))
        {
            system(logs->GetCmdLog().c_str());
        }
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
        std::cout << "6. Exit" << std::endl;
        GetChoice();
    }
    
    void GetChoice(){
        std::cout << "Please enter your choice: ";
        std::cin >> choice;
        
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            return;
        }
        
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
                rice->EditRice();
                break;
            case 5:
                rice->SwitchRice();
                break;
            case 6:
                std::cout << "Goodbye!" << std::endl;
                exit(0);
            default:
                std::cout << "Invalid choice. Please select 1-6." << std::endl;
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
