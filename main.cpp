#include <iostream>
#include "Headers/Navigation.h"
#include "Headers/Rice.h"

int main() {
    auto *nav = new Navigation();
    auto *rice = new Rice(0,"",{},0);
    rice->CheckForDatabase();
    nav->GetHomeDir();
    nav->DisplayMenu();
    return 0;
}

void SetCommandAs(std::string command, std::string newCommand) {
    std::string path = "/usr/local/bin/" + command;
    std::string newPath = "/usr/local/bin/" + newCommand;
    std::string commandLine = "sudo mv " + path + " " + newPath;
    system(commandLine.c_str());
}
