#include <iostream>
#include "Headers/Navigation.h"
#include "Headers/Rice.h"

#define SUCCESS 0

int main() {
    auto *nav = new Navigation();
    auto *rice = new Rice(0,"",{},0);
    rice->CheckForDatabase();
    nav->GetHomeDir();
    nav->DisplayMenu();
    return SUCCESS;
}

