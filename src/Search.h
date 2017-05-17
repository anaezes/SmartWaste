#ifndef SMARTWASTE_SEARCH_H
#define SMARTWASTE_SEARCH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

namespace Search {
    int naiveStringMatch(string input, string toSearch);

    vector<unsigned int> computePrefix(string toSearch);

    unsigned int kmpStringMatch(string input, string toSearch);

    unsigned int kmpStringMatchOptimized(string input, string toSearch, vector<unsigned int> pi);

    int editDistance(string pattern, string text);

    float numApproximateStringMatching(string filename, string toSearch);

    int numStringMatchingKmp(string filename,string toSearch);

    int numStringMatchingNaive(string filename,string toSearch);
}


#endif //SMARTWASTE_SEARCH_H
