//
// Created by ana on 27-03-2017.
//
#ifndef SMARTWASTE_UTILS_H
#define SMARTWASTE_UTILS_H

#include <string>
#include <vector>
#include <sstream>

namespace Utils {
    long double distance_km(long double, long double, long double, long double);

    int getScreenXCoord(const long double &lon, const long double &longitudeMin, const long double &longitudeMax, const int &resolucao);

    int getScreenYCoord(const long double &lat, const long double &latitudeMin, const long double &latitudeMax, const int &resolucao);

    std::vector<std::string> splitLine(std::string str, char c);
}


#endif //SMARTWASTE_UTILS_H
