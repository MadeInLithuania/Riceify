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
#include <sys/statvfs.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <dirent.h>
#include <unordered_set>
#include <memory>
#include <future>
#include <thread>
#include "Banner.h"
#include "Colors.h"
#include "Logger.h"

class Rice{
private:
    unsigned long memSize;
    std::string riceName;
    std::list<std::string> riceNames;
    const time_t *creationDate;
    std::vector<Rice> rices;
    std::vector<std::filesystem::path> files;
    std::vector<std::string> dirs;
    int choice;
    
    // Performance optimizations
    mutable std::vector<std::string> cachedDirs;
    mutable bool dirsCacheValid = false;
    mutable std::chrono::steady_clock::time_point lastCacheUpdate;
    static constexpr std::chrono::seconds CACHE_DURATION{5}; // Cache for 5 seconds
    
protected:
    std::string homedir = getenv("HOME");
    std::string dbDir = homedir + "/Riceify/db.rcf";
    std::string riceDir = homedir + "/Riceify/rices/";
    
    // Helper methods for performance
    void invalidateCache() const {
        dirsCacheValid = false;
    }
    
    bool isCacheValid() const {
        if (!dirsCacheValid) return false;
        auto now = std::chrono::steady_clock::now();
        return (now - lastCacheUpdate) < CACHE_DURATION;
    }
    
    void updateCache() const {
        cachedDirs.clear();
        if(std::filesystem::exists(riceDir)) {
            for (const auto &p : std::filesystem::directory_iterator(riceDir)) {
                if(p.is_directory()){
                    cachedDirs.push_back(p.path().string());
                }
            }
        }
        dirsCacheValid = true;
        lastCacheUpdate = std::chrono::steady_clock::now();
    }
    
    // Optimized file operations
    std::string buildCommand(const std::string& base, const std::string& riceName) const {
        return base + riceName;
    }
    
    // Async file operations
    std::future<int> asyncSystemCall(const std::string& cmd) const {
        return std::async(std::launch::async, [cmd]() {
            return system(cmd.c_str());
        });
    }

public:
    Rice(unsigned long _memSize, const std::string& _riceName, std::vector<Rice> _files, const time_t *_creationDate) {
        memSize = _memSize;
        riceName = _riceName;
        _files.push_back(*this);
        creationDate = _creationDate;
    }
    
    // Optimized rice list operations
    void GetRiceList(){
        START_OPERATION("GetRiceList");
        LOG_INFO(LogCategory::USER_ACTIONS, "Listing rice configurations");
        
        if(!isCacheValid()) {
            LOG_DEBUG(LogCategory::CACHE, "Cache invalid, updating");
            updateCache();
        } else {
            LOG_DEBUG(LogCategory::CACHE, "Using cached rice list");
        }
        
        std::cout << "Rices : [" << cachedDirs.size() << "]" << std::endl;
        if(cachedDirs.empty()){
            std::cout << "The list is empty." << std::endl;
            LOG_INFO(LogCategory::USER_ACTIONS, "No rice configurations found");
        }
        else{
            for (size_t i = 0; i < cachedDirs.size(); i++) {
                std::cout << "[" << i+1 << "] - " << cachedDirs[i] << std::endl;
            }
            LOG_INFO(LogCategory::USER_ACTIONS, "Rice list displayed", "Count: " + std::to_string(cachedDirs.size()));
        }
        
        END_OPERATION("GetRiceList");
    }
    
    int GetRiceListWithLen(){
        if(!isCacheValid()) {
            updateCache();
        }
        
        std::cout << "Rices : [" << cachedDirs.size() << "]" << std::endl;
        if(cachedDirs.empty()){
            std::cout << "The list is empty." << std::endl;
        }
        else{
            for (size_t i = 0; i < cachedDirs.size(); i++) {
                std::cout << "[" << i+1 << "] - " << cachedDirs[i] << std::endl;
            }
        }
        return static_cast<int>(cachedDirs.size());
    }
    
    //NUMBER 1
    void ListRice() {
        GetRiceList();
        std::cout << KRED << "You will be redirected soon." << RST << std::endl;
        sleep(3);
        invalidateCache();
        DisplayMenu();
    }

    //NUMBER 2 - Optimized
    void addRice(){
        START_OPERATION("AddRice");
        LOG_INFO(LogCategory::USER_ACTIONS, "Starting rice creation process");
        
        try {
        struct statvfs stat{};
        if (statvfs("/", &stat) != 0) {
            LOG_ERROR(LogCategory::SYSTEM, "Failed to read filesystem statistics");
            std::cerr << "Error (can't read the dir !)";
            exit(-1);
        }

        auto end = std::chrono::system_clock::now();
        std::time_t _time = std::chrono::system_clock::to_time_t(end);
        creationDate = &_time;
        char* ct = ctime(creationDate);

        std::cout << "Please input the rice name :" << std::endl;
        std::cin >> riceName;
        std::cout << "Rice name : " << riceName << std::endl;
        
        LOG_INFO(LogCategory::USER_ACTIONS, "Rice name entered", "Name: " + riceName);
        
        // Show preview of what will be backed up
        std::cout << std::endl;
        PreviewRiceBackup(riceName);
        
        std::cout << "Do you want to proceed with the backup? (y/n): ";
        char proceed;
        std::cin >> proceed;
        
        if(proceed != 'y' && proceed != 'Y') {
            std::cout << "Backup cancelled." << std::endl;
            LOG_INFO(LogCategory::USER_ACTIONS, "Rice backup cancelled by user");
            DisplayMenu();
            return;
        }
        
        LOG_INFO(LogCategory::BACKUP, "Starting rice backup", "Rice name: " + riceName);
        
        std::cout << "Success ! \n" <<
                  "\nRice name : " << KGRN << riceName << RST <<
                  "\nCreation date : " << KGRN << ct << RST << std::endl;
        std::cout << KRED << "Starting backup..." << RST << std::endl;
        
        // Create directory if it doesn't exist
        std::filesystem::path ricePath = homedir + "/Riceify/rices/";
        if(!std::filesystem::exists(ricePath)){
            LOG_DEBUG(LogCategory::FILE_OPERATIONS, "Creating rice directory", "Path: " + ricePath.string());
            std::filesystem::create_directories(ricePath);
        }
        
        CreateFolder(riceName, ricePath.string());
        auto rice = std::make_unique<Rice>(memSize, riceName, rices, creationDate);
        rices.push_back(*rice);
        CopyFiles(riceName);
        invalidateCache();
        
        LOG_INFO(LogCategory::BACKUP, "Rice backup completed successfully", "Rice name: " + riceName);
        }
        catch(std::exception &exception){
            LOG_ERROR(LogCategory::ERROR_HANDLING, "Exception during rice creation", exception.what());
            throw std::exception(exception);
        }
        
        END_OPERATION("AddRice");
    }
    
    //NUMBER 3 - Optimized
    void RemoveRice(){
        int toRemove;
        GetRiceList();
        std::cout << "Please input a rice to delete :" << std::endl;
        std::cin >> toRemove;
        
        if(std::cin.fail() || toRemove < 1 || toRemove > static_cast<int>(cachedDirs.size())){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Not a valid choice !" << std::endl;
            RemoveRice();
            return;
        }
        
        // Remove from filesystem
        std::string ricePath = cachedDirs[toRemove - 1];
        if(std::filesystem::exists(ricePath)) {
            std::filesystem::remove_all(ricePath);
            std::cout << "[" << KGRN << "+" << RST << "] Removed rice: " << ricePath << std::endl;
        }
        
        invalidateCache();
        DisplayMenu();
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
                addRice();
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
    
    // Optimized file discovery
    void GetHomeFilesAndSubfolders() {
        files.clear();
        std::string excludedFolderPath = homedir + "/Riceify/rices/";
        std::filesystem::path home = homedir;
        std::cout << "[" << KCYN << "*" << RST << "] Getting files and subfolders from " << KMAG << home << std::endl;
        
        try{
            if(!std::filesystem::exists(excludedFolderPath)){
                excludedFolderPath = "";
            }
            
            // Use recursive iterator with error handling
            for (const auto &p : std::filesystem::recursive_directory_iterator(home, 
                std::filesystem::directory_options::skip_permission_denied)){
                if(p.path().string() != excludedFolderPath) {
                    files.push_back(p);
                }
            }
        }catch(const std::exception &ex){
            std::cout << KRED << "Error during file discovery: " << ex.what() << RST << std::endl;
        }
        std::cout << "Found " << KMAG << files.size() << RST << " files." << std::endl;
    }

    static void CreateFolder(const std::string& folderName, const std::string& path) {
        std::filesystem::path fullPath = path + "/" + folderName;
        if(!std::filesystem::exists(fullPath)) {
            try{
                std::filesystem::create_directories(fullPath);
                std::cout << "[" << KRED << "!" << RST << "] Created folder at " << fullPath << std::endl;
            }
            catch(const std::exception& e){
                std::cerr << "Error creating folder: " << e.what() << std::endl;
            }
        }
        else std::cout << "[" << KGRN << "*" << RST << "] Folder exists ;) "<< std::endl;
    }
    
    // Optimized file copying with smart file selection
    void CopyFiles(const std::string& riceName){
        START_OPERATION("CopyFiles");
        LOG_INFO(LogCategory::BACKUP, "Starting file copy operations", "Rice name: " + riceName);
        
        std::string ricePath = "~/Riceify/rices/" + riceName;
        
        std::cout << "[" << KCYN << "*" << RST << "] Creating rice backup: " << KMAG << riceName << RST << std::endl;
        
        // Create rice directory structure
        LOG_DEBUG(LogCategory::FILE_OPERATIONS, "Creating directory structure", "Path: " + ricePath);
        auto configFuture = asyncSystemCall("mkdir -p " + ricePath + "/.config");
        auto homeFuture = asyncSystemCall("mkdir -p " + ricePath + "/home");
        
        configFuture.wait();
        homeFuture.wait();
        
        // Copy essential configuration files with exclusions
        std::cout << "[" << KCYN << "*" << RST << "] Copying configuration files..." << std::endl;
        LOG_INFO(LogCategory::BACKUP, "Copying .config directory");
        
        // Copy .config directory (most important for rice)
        auto copyConfigFuture = asyncSystemCall("rsync -a --exclude='cache' --exclude='logs' --exclude='temp' --exclude='tmp' ~/.config/ " + ricePath + "/.config/");
        
        // Copy essential home dotfiles (excluding large/unnecessary files)
        std::string excludePatterns = "--exclude='.cache' --exclude='.local/share' --exclude='.npm' --exclude='.node_modules' --exclude='.git' --exclude='.steam' --exclude='Downloads' --exclude='Pictures' --exclude='Videos' --exclude='Music' --exclude='Documents' --exclude='Desktop' --exclude='.wine' --exclude='.config/Riceify'";
        
        LOG_INFO(LogCategory::BACKUP, "Copying home dotfiles with exclusions");
        auto copyHomeFuture = asyncSystemCall("rsync -a " + excludePatterns + " ~/.??* " + ricePath + "/home/");
        
        // Copy specific important dotfiles that might be in home
        LOG_DEBUG(LogCategory::BACKUP, "Copying specific dotfiles");
        auto copySpecificFiles = asyncSystemCall("rsync -a ~/.bashrc ~/.zshrc ~/.profile ~/.bash_profile ~/.xinitrc ~/.Xresources ~/.gtkrc-2.0 ~/.config/gtk-3.0/settings.ini " + ricePath + "/home/ 2>/dev/null || true");
        
        // Wait for all operations to complete
        copyConfigFuture.wait();
        copyHomeFuture.wait();
        copySpecificFiles.wait();
        
        // Create rice metadata file
        CreateRiceMetadata(riceName, ricePath);
        
        std::cout << "[" << KGRN << "+" << RST << "] Rice backup completed successfully!" << std::endl;
        std::cout << "[" << KGRN << "+" << RST << "] Location: " << KMAG << ricePath << RST << std::endl;
        
        LOG_INFO(LogCategory::BACKUP, "File copy operations completed", "Rice path: " + ricePath);
        
        DisplayMenu();
        END_OPERATION("CopyFiles");
    }
    
    // Create metadata file for the rice
    void CreateRiceMetadata(const std::string& riceName, const std::string& ricePath) {
        try {
            std::ofstream metadata(ricePath + "/rice_metadata.txt");
            if(metadata.is_open()) {
                metadata << "Rice Name: " << riceName << std::endl;
                metadata << "Created: " << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
                metadata << "System: " << std::filesystem::current_path().string() << std::endl;
                metadata << "User: " << getenv("USER") << std::endl;
                metadata << std::endl;
                metadata << "Included directories:" << std::endl;
                metadata << "- ~/.config/ (excluding cache, logs, temp)" << std::endl;
                metadata << "- Essential dotfiles from home" << std::endl;
                metadata << std::endl;
                metadata << "Excluded directories:" << std::endl;
                metadata << "- ~/.cache/" << std::endl;
                metadata << "- ~/.local/share/" << std::endl;
                metadata << "- Downloads, Pictures, Videos, Music, Documents" << std::endl;
                metadata << "- .git repositories" << std::endl;
                metadata << "- Steam, Wine, npm cache" << std::endl;
                metadata.close();
                std::cout << "[" << KGRN << "*" << RST << "] Created rice metadata file." << std::endl;
            }
        } catch(const std::exception& e) {
            std::cout << "[" << KYEL << "!" << RST << "] Could not create metadata file: " << e.what() << std::endl;
        }
    }
    
    // Get list of files that will be copied (for preview)
    void PreviewRiceBackup(const std::string& riceName) {
        std::cout << "[" << KCYN << "*" << RST << "] Preview of files to be backed up:" << std::endl;
        std::cout << std::endl;
        
        // Show .config contents
        std::cout << KMAG << "~/.config/ contents:" << RST << std::endl;
        system("find ~/.config -maxdepth 2 -type d | head -20 | sed 's|^|  |'");
        std::cout << std::endl;
        
        // Show important dotfiles
        std::cout << KMAG << "Important dotfiles:" << RST << std::endl;
        system("ls -la ~/.??* | grep -E '\\.(bashrc|zshrc|profile|xinitrc|Xresources|gtkrc)' | sed 's|^|  |'");
        std::cout << std::endl;
        
        // Show excluded directories
        std::cout << KYEL << "Excluded directories (not copied):" << RST << std::endl;
        std::cout << "  ~/.cache/" << std::endl;
        std::cout << "  ~/.local/share/" << std::endl;
        std::cout << "  Downloads, Pictures, Videos, Music, Documents" << std::endl;
        std::cout << "  .git repositories, Steam, Wine, npm cache" << std::endl;
        std::cout << std::endl;
    }
    
    void CopyFolder(const std::string& cmd, const std::string& onCopyStr = "", const std::string & onSuccess = ""){
        try{
            std::cout << onCopyStr << std::endl;
            int result = system(cmd.c_str());
            if(result == 0) {
                std::cout << "[" << KGRN << "+" << RST << "]" << onSuccess << std::endl;
            } else {
                std::cout << "[" << KRED << "!" << RST << "] Command failed with exit code: " << result << std::endl;
            }
        }
        catch(const std::exception &e){
            std::cout << "Error : " << KRED << e.what() << RST << std::endl;
        }
    }
    
    // Optimized rice switching
    void SwitchRice(){
        int index;
        int len = GetRiceListWithLen();
        if(len <= 1){
            std::cout << "Can't switch with one or no rice." << std::endl;
            sleep(2);
            DisplayMenu();
            return;
        }
        
        std::cout << "Please select the rice to switch : " << std::endl;
        std::cin >> index;
        
        if(std::cin.fail() || index < 1 || index > len){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid selection." << std::endl;
            SwitchRice();
            return;
        }
        
        std::string chosenRice = cachedDirs[index - 1];
        std::cout << "Chosen the rice : " << chosenRice << std::endl;
        
        if(!std::filesystem::exists(chosenRice)){
            std::cout << "Directory doesn't exist. Operation halted." << std::endl;
            sleep(2);
            DisplayMenu();
            return;
        }
        
        std::cout << KRED << "This step requires sudo authentification." << RST << std::endl; 
        std::string finalCMD = "sudo cp -rT " + chosenRice + "/ ~";
        std::cout << finalCMD << std::endl;
        
        int result = system(finalCMD.c_str());
        if(result != 0)
            std::cerr << "[" << KRED << "!" << RST << "]" << "Error occurred." << std::endl;
        else
            std::cout << "Please reboot your device to finalize the changes." << std::endl;
    }
    
    // Optimized rice editing
    void EditRice(){
        int choice;
        GetRiceList();
        std::cout << "Please choose the rice to edit :" << std::endl;
        std::cin >> choice;
        
        if(std::cin.fail() || choice < 1 || choice > static_cast<int>(cachedDirs.size())){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Index is out of range." << std::endl;
            EditRice();
            return;
        }
        
        std::string ricePath = cachedDirs[choice-1];
        std::cout << "Starting copying from " << KMAG << homedir << RST << " to " << KMAG << ricePath << RST << std::endl;
        std::string cmd = "sudo cp -rT ~ " + ricePath;
        std::cout << KRED << "This step requires sudo authentification." << RST << std::endl; 
        
        int result = system(cmd.c_str());
        if(result != 0)
            std::cerr << "Error while copying." << std::endl;
        else{
            std::cout << "[" << KGRN << "*" << RST << "] Task completed successfully." << std::endl;
        }
        
        std::cout << KRED << "\nYou will be redirected soon." << RST << std::endl;
        sleep(3);
        invalidateCache();
        DisplayMenu();
    }
};
#endif //RICEIFY_RICE_H
