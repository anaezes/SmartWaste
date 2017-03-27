//
// Created by ana on 27-03-2017.
//
#include <math.h>

namespace Utils {

/**
 * calculate haversine distance for linear distance // coordinates in radians
 * */
    double distance_km(double lat1, double long1, double lat2, double long2) {
        double dlong = (long2 - long1);
        double dlat = (lat2 - lat1);
        double a = pow(sin(dlat / 2.0), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2.0), 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        double d = 6367 * c;

        return d;
    }
}