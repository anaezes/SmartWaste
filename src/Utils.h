//
// Created by ana on 27-03-2017.
//
#ifndef SMARTWASTE_UTILS_H
#define SMARTWASTE_UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <climits>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <string>

#ifdef linux
#include <unistd.h>
#else
#include <winsock2.h>
#include <Windows.h>
#endif


namespace Utils {

    /**
    * @brief Do sleep according to the operating system
    * @param s
    * */
    void doSleep(int s);

    /**
    * @brief calculate  distance
    * @param x1
    * @param x2
    * @param y1
    * @param y2
    * @return distance
    * */
    int distance(int y1, int x1, int y2, int x2);

    /**
    * @brief split one line in vector of strings
    * @param str
    * @param c
    * @return vector of strings
    * */
    std::vector<std::string> splitLine(std::string str, char c);
}


#endif //SMARTWASTE_UTILS_H
