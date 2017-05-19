#ifndef SMARTWASTE_SEARCH_H
#define SMARTWASTE_SEARCH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

namespace Search {

    /**
    * @brief Naive algorithm implementation
    * @param input
    * @param toSearch
    * @return 0 if expression don't match
    **/
    int naiveStringMatch(string input, string toSearch);


    /**
    * @brief Computes prefix for kmp algorithm
    * @param toSearch
    * @return vector
    **/
    vector<unsigned int> computePrefix(string toSearch);


    /**
    * @brief Knuth-Morris-Pratt (KMP) algorithm implementation
    * @param input
    * @param toSearch
    * @param pi - prefix
    * @return 0 if expression don't match
    **/
    unsigned int kmpStringMatch(string input, string toSearch, vector<unsigned int> pi);


    /**
    * @brief Compute edit distance
    * Edit distances is the minimum number of changes needed to transform one string into another.
    * @param pattern
    * @param text
    * @return the edit distance between two strings
    **/
    int editDistance(string pattern, string text);


    /**
    * @brief Compute average of edit distances
    * @param input
    * @param toSearch
    * @return average edit distances
    **/
    float numApproximateStringMatching(string input, string toSearch);


    /**
    * @brief Compute number of matches
    * @param filename - text file where to search
    * @param toSearch - expression to search
    * @return number of matches
    **/
    int numStringMatchingKmp(string filename, string toSearch);

    /**
    * @brief Compute number of matches
    * @param filename - text file where to search
    * @param toSearch - expression to search
    * @return number of matches
    **/
    int numStringMatchingNaive(string filename,string toSearch);
}


#endif //SMARTWASTE_SEARCH_H
