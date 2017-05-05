#include "Utils.h"

namespace Utils {

    void doSleep(int s) {
#ifdef linux
        usleep(s*1000);
#else
        Sleep(s);
#endif
    }

    int distance(int y1, int x1, int y2, int x2) {

        int dx = (x2 - x1);
        int dy = (y2 - y1);
        return sqrt(dx*dx+dy*dy);
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
        if(result[result.size()-1].find('\r'))
            result[result.size()-1] = result[result.size()-1].substr(0, result[result.size()-1].length() - 1);

        return result;
    }
}

