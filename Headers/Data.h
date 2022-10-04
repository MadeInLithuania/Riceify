//
// Created by tadas on 10/3/22.
//

#ifndef RICEIFY_DATA_H
#define RICEIFY_DATA_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "Rice.h"
#include "../yaml-cpp/include/yaml-cpp/yaml.h"
class Data : public Rice{
private:
    Rice *rice = new Rice(0,"",{},nullptr);
    ~Data() = default;
protected:
    YAML::Node file = YAML::LoadFile(rice->GetDatabaseFile());
public:
    YAML::Node GetFile(){
    return file;
    }
    void EmitYML(){
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Name";
    out << YAML::Value << rice->GetRiceName();
    out << YAML::Key << "Creation date";
    out << YAML::Value << rice->GetCreationDate();
    out << YAML::Key << "Memory";
    out << YAML::Value << rice->GetMemSize();
    out << YAML::EndMap;
    }
    void RemoveYML(const std::string& _name, time_t _date, unsigned long _mem){

    }
};//e
#endif //RICEIFY_DATA_H
