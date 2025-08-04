//
// Created by Riceify Team
// Logger.h - Comprehensive logging system for Riceify
//

#ifndef RICEIFY_LOGGER_H
#define RICEIFY_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <memory>
#include <filesystem>
#include <vector>
#include <map>
#include "Colors.h"

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4
};

enum class LogCategory {
    GENERAL,
    PERFORMANCE,
    FILE_OPERATIONS,
    USER_ACTIONS,
    SYSTEM,
    BACKUP,
    RESTORE,
    CACHE,
    ERROR_HANDLING
};

class Logger {
private:
    static std::unique_ptr<Logger> instance;
    static std::mutex logMutex;
    
    std::ofstream logFile;
    std::string logFilePath;
    LogLevel currentLevel;
    bool consoleOutput;
    bool fileOutput;
    bool performanceTracking;
    
    // Performance tracking
    std::map<std::string, std::chrono::high_resolution_clock::time_point> operationStartTimes;
    std::map<std::string, std::vector<double>> operationDurations;
    
    // Statistics
    struct LogStats {
        int totalLogs = 0;
        int debugLogs = 0;
        int infoLogs = 0;
        int warningLogs = 0;
        int errorLogs = 0;
        int criticalLogs = 0;
        std::map<LogCategory, int> categoryCounts;
    } stats;
    
    Logger() : currentLevel(LogLevel::INFO), consoleOutput(true), fileOutput(true), performanceTracking(true) {
        initializeLogFile();
    }
    
    void initializeLogFile() {
        std::string homeDir = getenv("HOME") ? getenv("HOME") : "";
        std::string logDir = homeDir + "/Riceify/logs/";
        
        // Create logs directory if it doesn't exist
        if (!std::filesystem::exists(logDir)) {
            std::filesystem::create_directories(logDir);
        }
        
        // Create log file with timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
        
        logFilePath = logDir + "riceify_" + ss.str() + ".log";
        logFile.open(logFilePath, std::ios::app);
        
        if (logFile.is_open()) {
            log(LogLevel::INFO, LogCategory::SYSTEM, "Logger initialized", "Log file: " + logFilePath);
        }
    }
    
    std::string getLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }
    
    std::string getCategoryString(LogCategory category) {
        switch (category) {
            case LogCategory::GENERAL: return "GENERAL";
            case LogCategory::PERFORMANCE: return "PERFORMANCE";
            case LogCategory::FILE_OPERATIONS: return "FILE_OPS";
            case LogCategory::USER_ACTIONS: return "USER_ACTIONS";
            case LogCategory::SYSTEM: return "SYSTEM";
            case LogCategory::BACKUP: return "BACKUP";
            case LogCategory::RESTORE: return "RESTORE";
            case LogCategory::CACHE: return "CACHE";
            case LogCategory::ERROR_HANDLING: return "ERROR_HANDLING";
            default: return "UNKNOWN";
        }
    }
    
    std::string getColorCode(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return KCYN;
            case LogLevel::INFO: return KGRN;
            case LogLevel::WARNING: return KYEL;
            case LogLevel::ERROR: return KRED;
            case LogLevel::CRITICAL: return KMAG;
            default: return RST;
        }
    }
    
    std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

public:
    static Logger& getInstance() {
        if (!instance) {
            instance = std::unique_ptr<Logger>(new Logger());
        }
        return *instance;
    }
    
    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // Main logging method
    void log(LogLevel level, LogCategory category, const std::string& message, const std::string& details = "") {
        if (level < currentLevel) return;
        
        std::lock_guard<std::mutex> lock(logMutex);
        
        std::string timestamp = getTimestamp();
        std::string levelStr = getLevelString(level);
        std::string categoryStr = getCategoryString(category);
        std::string colorCode = getColorCode(level);
        
        // Update statistics
        stats.totalLogs++;
        stats.categoryCounts[category]++;
        
        switch (level) {
            case LogLevel::DEBUG: stats.debugLogs++; break;
            case LogLevel::INFO: stats.infoLogs++; break;
            case LogLevel::WARNING: stats.warningLogs++; break;
            case LogLevel::ERROR: stats.errorLogs++; break;
            case LogLevel::CRITICAL: stats.criticalLogs++; break;
        }
        
        // Format log message
        std::string logMessage = "[" + timestamp + "] [" + levelStr + "] [" + categoryStr + "] " + message;
        if (!details.empty()) {
            logMessage += " | " + details;
        }
        
        // Console output
        if (consoleOutput) {
            std::cout << colorCode << logMessage << RST << std::endl;
        }
        
        // File output
        if (fileOutput && logFile.is_open()) {
            logFile << logMessage << std::endl;
            logFile.flush();
        }
    }
    
    // Convenience methods
    void debug(LogCategory category, const std::string& message, const std::string& details = "") {
        log(LogLevel::DEBUG, category, message, details);
    }
    
    void info(LogCategory category, const std::string& message, const std::string& details = "") {
        log(LogLevel::INFO, category, message, details);
    }
    
    void warning(LogCategory category, const std::string& message, const std::string& details = "") {
        log(LogLevel::WARNING, category, message, details);
    }
    
    void error(LogCategory category, const std::string& message, const std::string& details = "") {
        log(LogLevel::ERROR, category, message, details);
    }
    
    void critical(LogCategory category, const std::string& message, const std::string& details = "") {
        log(LogLevel::CRITICAL, category, message, details);
    }
    
    // Performance tracking
    void startOperation(const std::string& operationName) {
        if (!performanceTracking) return;
        
        std::lock_guard<std::mutex> lock(logMutex);
        operationStartTimes[operationName] = std::chrono::high_resolution_clock::now();
        debug(LogCategory::PERFORMANCE, "Operation started", "Operation: " + operationName);
    }
    
    void endOperation(const std::string& operationName) {
        if (!performanceTracking) return;
        
        std::lock_guard<std::mutex> lock(logMutex);
        
        auto it = operationStartTimes.find(operationName);
        if (it != operationStartTimes.end()) {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - it->second);
            double durationMs = duration.count() / 1000.0;
            
            operationDurations[operationName].push_back(durationMs);
            
            info(LogCategory::PERFORMANCE, "Operation completed", 
                 "Operation: " + operationName + " | Duration: " + std::to_string(durationMs) + "ms");
            
            operationStartTimes.erase(it);
        }
    }
    
    // Configuration methods
    void setLogLevel(LogLevel level) {
        currentLevel = level;
        info(LogCategory::SYSTEM, "Log level changed", "New level: " + getLevelString(level));
    }
    
    void setConsoleOutput(bool enabled) {
        consoleOutput = enabled;
        info(LogCategory::SYSTEM, "Console output " + std::string(enabled ? "enabled" : "disabled"));
    }
    
    void setFileOutput(bool enabled) {
        fileOutput = enabled;
        info(LogCategory::SYSTEM, "File output " + std::string(enabled ? "enabled" : "disabled"));
    }
    
    void setPerformanceTracking(bool enabled) {
        performanceTracking = enabled;
        info(LogCategory::SYSTEM, "Performance tracking " + std::string(enabled ? "enabled" : "disabled"));
    }
    
    // Statistics and reporting
    void printStats() {
        std::lock_guard<std::mutex> lock(logMutex);
        
        std::cout << "\n" << KMAG << "=== RICEIFY LOGGING STATISTICS ===" << RST << std::endl;
        std::cout << "Total logs: " << KGRN << stats.totalLogs << RST << std::endl;
        std::cout << "Debug: " << KCYN << stats.debugLogs << RST << std::endl;
        std::cout << "Info: " << KGRN << stats.infoLogs << RST << std::endl;
        std::cout << "Warning: " << KYEL << stats.warningLogs << RST << std::endl;
        std::cout << "Error: " << KRED << stats.errorLogs << RST << std::endl;
        std::cout << "Critical: " << KMAG << stats.criticalLogs << RST << std::endl;
        
        std::cout << "\n" << KMAG << "Category breakdown:" << RST << std::endl;
        for (const auto& [category, count] : stats.categoryCounts) {
            std::cout << "  " << getCategoryString(category) << ": " << KGRN << count << RST << std::endl;
        }
        
        if (!operationDurations.empty()) {
            std::cout << "\n" << KMAG << "Performance summary:" << RST << std::endl;
            for (const auto& [operation, durations] : operationDurations) {
                if (!durations.empty()) {
                    double avg = 0;
                    double min = durations[0];
                    double max = durations[0];
                    
                    for (double duration : durations) {
                        avg += duration;
                        min = std::min(min, duration);
                        max = std::max(max, duration);
                    }
                    avg /= durations.size();
                    
                    std::cout << "  " << operation << " (avg/min/max): " 
                              << KGRN << std::fixed << std::setprecision(2)
                              << avg << "ms / " << min << "ms / " << max << "ms" << RST << std::endl;
                }
            }
        }
        
        std::cout << "\n" << KMAG << "Log file: " << RST << logFilePath << std::endl;
    }
    
    // Export performance data
    void exportPerformanceData(const std::string& filename) {
        std::lock_guard<std::mutex> lock(logMutex);
        
        std::ofstream exportFile(filename);
        if (exportFile.is_open()) {
            exportFile << "Operation,Average(ms),Min(ms),Max(ms),Count" << std::endl;
            
            for (const auto& [operation, durations] : operationDurations) {
                if (!durations.empty()) {
                    double avg = 0;
                    double min = durations[0];
                    double max = durations[0];
                    
                    for (double duration : durations) {
                        avg += duration;
                        min = std::min(min, duration);
                        max = std::max(max, duration);
                    }
                    avg /= durations.size();
                    
                    exportFile << operation << "," << avg << "," << min << "," << max << "," << durations.size() << std::endl;
                }
            }
            
            exportFile.close();
            info(LogCategory::PERFORMANCE, "Performance data exported", "File: " + filename);
        }
    }
    
    // Cleanup
    ~Logger() {
        if (logFile.is_open()) {
            info(LogCategory::SYSTEM, "Logger shutting down");
            logFile.close();
        }
    }
};

// Global logger instance
#define LOGGER Logger::getInstance()

// Convenience macros
#define LOG_DEBUG(category, message, details) LOGGER.debug(category, message, details)
#define LOG_INFO(category, message, details) LOGGER.info(category, message, details)
#define LOG_WARNING(category, message, details) LOGGER.warning(category, message, details)
#define LOG_ERROR(category, message, details) LOGGER.error(category, message, details)
#define LOG_CRITICAL(category, message, details) LOGGER.critical(category, message, details)

#define START_OPERATION(name) LOGGER.startOperation(name)
#define END_OPERATION(name) LOGGER.endOperation(name)

#endif // RICEIFY_LOGGER_H
