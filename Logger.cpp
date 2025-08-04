//
// Created by Riceify Team
// Logger.cpp - Implementation file for Logger class
//

#include "Logger.h"

// Static member definitions
std::unique_ptr<Logger> Logger::instance = nullptr;
std::mutex Logger::logMutex; 
