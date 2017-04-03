//
// Created by ana on 27-03-2017.
//
#include "Utils.h"





namespace Utils {

    void doSleep(double s) {
#ifdef linux
        usleep(s*1000000);
#else
        Sleep(s*1000);
#endif
    }

/**
 * calculate haversine distance for linear distance // coordinates in radians
 * */
    int distance_km(int y1, int x1, int y2, int x2) {

        int dx = (x2 - x1);
        int dy = (y2 - y1);
        return sqrt(dx*dx+dy*dy);
    }

    long double radToDegrees(const long double &coor) {
        return coor*M_PI/180;
    }

    int getScreenXCoord(const long double &lon, const long double &longitudeMin, const long double &longitudeMax, const int &resolucao) {
        return floor(((lon-longitudeMin)*resolucao/(longitudeMax-longitudeMin)));

    }

    int getScreenYCoord(const long double &lat, const long double &latitudeMin, const long double &latitudeMax, const int &resolucao) {
        return resolucao-floor(((lat-latitudeMin)*resolucao/(latitudeMax-latitudeMin)));
    }

    std::vector<std::string> splitLine(std::string str, char c) {
        std::vector<std::string> result;
        size_t pos = str.find(c);
        std::string substring;
        std::string aux;
        while (pos != std::string::npos) {
            substring = str.substr(0, pos);
            std::stringstream ss(substring);
            ss >> aux;
            result.push_back(aux);
            str = str.substr(pos + 1, str.length());
            pos  = str.find(c);
        }

        result.push_back(str);
        if(result[result.size()-1].find('\n') || result[result.size()-1].find('\r'))
            result[result.size()-1] = result[result.size()-1].substr(0, result[result.size()-1].length() - 1);

        return result;
    }
}

