#include "Search.h"




int Search::naiveStringMatch(string input, string toSearch) {
    unsigned int occ = 0;

    for (size_t i = 0; i < input.size() - toSearch.size() + 1; i++) {
        bool        failed = false;
        for (size_t j      = 0; j < toSearch.size(); j++) {
            if (input.at(i + j) != toSearch.at(j)) {
                failed = true;
                break;
            }
        }
        if (!failed) {
            occ++;
        }
    }

    return occ;
}

vector<unsigned int> Search::computePrefix(string toSearch) {
    unsigned int         m = toSearch.size();
    vector<unsigned int> pi(m + 1, 0);

    unsigned int k = 0;

    for (size_t q = 2; q <= m; q++) {
        while (k > 0 && toSearch.at(k) != toSearch.at(q - 1)) {
            k = pi.at(k);
        }
        if (toSearch.at(k) == toSearch.at(q - 1)) {
            k++;
        }
        pi.at(q) = k;
    }

    return pi;
}

unsigned int Search::kmpStringMatch(string input, string toSearch) {
    unsigned int         occ = 0;
    unsigned int         n   = input.size();
    unsigned int         m   = toSearch.size();
    vector<unsigned int> pi  = computePrefix(toSearch);
    unsigned int         q   = 0;
    for (size_t          i   = 1; i <= n; i++) {
        while (q > 0 && toSearch.at(q) != input.at(i - 1)) {
            q = pi.at(q);
        }
        if (toSearch.at(q) == input.at(i - 1)) {
            q++;
        }
        if (q == m) {
            occ++;
            q = pi.at(q);
        }
    }
    return occ;
}

int Search::numStringMatching(string filename, string toSearch) {
    ifstream file(filename.c_str());

    if (!file.is_open()) {
        return 0;
    }

    unsigned int occ = 0;

    string line;

    while (getline(file, line)) {
        occ += kmpStringMatch(line, toSearch);
    }

    return occ;
}

int Search::editDistance(string pattern, string text) {
    vector<vector<int>> D(pattern.size() + 1, vector<int>(text.size() + 1, 0));

    for (unsigned int i = 0; i <= pattern.size(); i++)
        D[i][0]         = i;
    for (unsigned int j = 0; j <= text.size(); j++)
        D[0][j]         = j;

    for (unsigned int i = 1; i <= pattern.size(); i++) {
        for (unsigned int j = 1; j <= text.size(); j++) {
            if (pattern[i - 1] == text[j - 1]) {
                D[i][j] = D[i - 1][j - 1];
            } else {

                D[i][j] = 1 + min(min(D[i - 1][j - 1], D[i - 1][j]), D[i][j - 1]);
            }
        }
    }

    return D[pattern.size()][text.size()];

}

float Search::numApproximateStringMatching(string filename, string toSearch) {
    unsigned int words         = 0;
    unsigned int totalDistance = 0;

    ifstream input(filename);
    string   line;
    string   word;
    while (getline(input, line)) {
        stringstream lineStream(line);
        while (lineStream >> word) {
            words++;
            totalDistance += editDistance(toSearch, word);
        }

    }
    return (float) totalDistance / words;
}

/*
void test_stringMatching() {

    string toSearch = "estrutura de dados";

    string filename1 = "text1.txt";
    int num1 = numStringMatching(filename1, toSearch);

    ASSERT_EQUAL(3, num1);

    string filename2 = "text2.txt";
    int num2 = numStringMatching(filename2, toSearch);

    ASSERT_EQUAL(2, num2);

}

void test_approximateStringMatching() {

    string toSearch = "estrutur";

    string filename1 = "text1.txt";
    float dist1 = numApproximateStringMatching(filename1, toSearch);

    ASSERT_EQUAL_DELTA(7.76, dist1, 0.01);

    string filename2 = "text2.txt";
    float dist2 = numApproximateStringMatching(filename2, toSearch);

    ASSERT_EQUAL_DELTA(7.49, dist2, 0.01);

}


}*/
