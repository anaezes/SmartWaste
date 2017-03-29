//
// Created by ana on 27-03-2017.
//
#include <math.h>
#include "Utils.h"

namespace Utils {
/**
 * calculate haversine distance for linear distance // coordinates in radians
 * */
    long double distance_km(long double lat1, long double long1, long double lat2, long double long2) {

        long double dlong = (long2 - long1);
        long double dlat = (lat2 - lat1);
        long double a = pow(sin(dlat / 2.0), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2.0), 2);
        long double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        long double d = 6367 * c;

        return d;
    }

    long double radToDegrees(const long double &coor) {
        return coor*M_PI/180;
    }


    int getScreenXCoord(const long double &lon, const long double &longitudeMin, const long double &longitudeMax, const int &resolucao) {
        int dx = resolucao /(radToDegrees(longitudeMax) - radToDegrees(longitudeMin));

        return ((radToDegrees(lon) - radToDegrees(longitudeMin)) * dx);
    }

    int getScreenYCoord(const long double &lat, const long double &latitudeMin, const long double &latitudeMax, const int &resolucao) {
        int dy = resolucao /(radToDegrees(latitudeMax) - radToDegrees(latitudeMin));

        return (resolucao - ((radToDegrees(lat )- radToDegrees(latitudeMin)) * dy));
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

    int normalizeInt(long id, long minId) {
        return (id-minId)%INT_MAX;
    }
}

