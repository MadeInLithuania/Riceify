//
// Created by tadas on 7/19/22.
//

#ifndef RICEIFY_COLORS_H
#define RICEIFY_COLORS_H

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#include <iostream>
#include <random>

class Colors{
public:
    Colors() = default;
    ~Colors() = default;
    static std::string RandomColor(){
        std::string colors[8] = {KRED, KGRN, KYEL, KBLU, KMAG, KCYN, KWHT};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 6);
        return colors[dis(gen)];
    }
    static std::string ResetColor(){
        return RST;
    }
};

#endif //RICEIFY_COLORS_H
