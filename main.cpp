#include <iostream>
#include "Headers/Navigation.h"
#include "Headers/Rice.h"

#define SUCCESS 0

bool IsLinux(){
    #ifdef _WIN32
    std::cout << KRED << "Windows detected, operation halted." << RST << std::endl;
    return false;
    #else
    return true;
    #endif
}

int main() {
    // Initialize logger
    LOG_INFO(LogCategory::SYSTEM, "Riceify application starting");
    
    if(IsLinux()){
        try {
            LOG_INFO(LogCategory::SYSTEM, "Linux system detected, proceeding with initialization");
            auto nav = std::make_unique<Navigation>();
            nav->GetHomeDir();
            nav->DisplayMenu();
            LOG_INFO(LogCategory::SYSTEM, "Application completed successfully");
            return SUCCESS;
        } catch (const std::exception& e) {
            LOG_CRITICAL(LogCategory::ERROR_HANDLING, "Fatal error in main", e.what());
            std::cerr << "Fatal error: " << e.what() << std::endl;
            return 1;
        }
    }
    else{
        LOG_ERROR(LogCategory::SYSTEM, "Non-Linux system detected, application cannot run");
        return 1;
    }
}

