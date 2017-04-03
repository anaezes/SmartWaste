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

    void doSleep(double s);

    int distance_km(int y1, int x1, int y2, int x2);

    int getScreenXCoord(const long double &lon, const long double &longitudeMin, const long double &longitudeMax, const int &resolucao);

    int getScreenYCoord(const long double &lat, const long double &latitudeMin, const long double &latitudeMax, const int &resolucao);

    int normalizeInt(long id,long minId);

    std::vector<std::string> splitLine(std::string str, char c);
}


#endif //SMARTWASTE_UTILS_H
