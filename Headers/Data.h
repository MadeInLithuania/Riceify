//
// Created by tadas on 10/3/22.
//

#ifndef RICEIFY_DATA_H
#define RICEIFY_DATA_H

#include <iostream>
#include <fstream>
#include <filesystem>
class Data{
private:
    //Rice *rice = new Rice(0,"",{},nullptr);
    ~Data() = default;
protected:
    //YAML::Node file = YAML::LoadFile(rice->GetDatabaseFile());
public:

    void RemoveYML(const std::string& _name, time_t _date, unsigned long _mem){

    }
};
#endif //RICEIFY_DATA_H
