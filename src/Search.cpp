#include "Search.h"

int Search::naiveStringMatch(string input, string toSearch) {
    unsigned int occ = 0;

    int aux = input.size() - toSearch.size() + 1;
    if(aux < 0)
        return occ;

    for (size_t i = 0; i < aux; i++) {
        bool failed = false;
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
    unsigned int m = toSearch.size();
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

unsigned int Search::kmpStringMatch(string input, string toSearch, vector<unsigned int> pi) {
    unsigned int         occ = 0;
    unsigned int         n   = input.size();
    unsigned int         m   = toSearch.size();
    unsigned int         q   = 0;

    int aux = n - m + 1;
    if(aux < 0)
        return occ;

    for (size_t i = 1; i <= n; i++) {
        while (q > 0 && toSearch.at(q) != input.at(i - 1))
            q = pi.at(q);

        if (toSearch.at(q) == input.at(i - 1))
            q++;
        if (q == m) {
            occ++;
            q = pi.at(q);
        }
    }
    return occ;
}


int Search::editDistance(string pattern, string text)
{
    int n=text.length();
    vector<int> d(n+1);
    int old,neww;
    for (int j=0; j<=n; j++)
        d[j]=j;
    int m=pattern.length();
    for (int i=1; i<=m; i++) {
        old = d[0];
        d[0]=i;
        for (int j=1; j<=n; j++) {
            if (pattern[i-1]==text[j-1]) neww = old;
            else {
                neww = min(old,d[j]);
                neww = min(neww,d[j-1]);
                neww = neww +1;
            }
            old = d[j];
            d[j] = neww;
        }
    }
    return d[n];
}


float Search::numApproximateStringMatching(string input,string toSearch)
{
    string word;
    int num = 0;
    int nwords = 0;

    stringstream s1(input);
    while (!s1.eof()) {
        s1 >> word;
        num += editDistance(toSearch, word);
        nwords++;
    }
    float res=(float)num/nwords;
    return res;
}

int Search::numStringMatchingKmp(string filename, string toSearch)
{
    ifstream fich(filename.c_str());
    if (!fich)
    {
        cout << "Error to open txt file!\n";
        return 0;
    }

    string line1;
    int num=0;
    vector<unsigned int> pi  = computePrefix(toSearch);

    while (!fich.eof()) {
        getline(fich,line1);
        num+=kmpStringMatch(line1,toSearch, pi);
    }
    fich.close();
    return num;
}

int Search::numStringMatchingNaive(string filename, string toSearch)
{
    ifstream fich(filename.c_str());
    if (!fich)
    {
        cout << "Error to open txt file!\n";
        return 0;
    }

    string line1;
    int num=0;

    while (!fich.eof()) {
        getline(fich,line1);
        num+=naiveStringMatch(line1,toSearch);
    }
    fich.close();
    return num;
}
