#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

#include "graphviewer.h"
#include "Graph.h"

using namespace std;

/**
* Read a file to a vector of lines
**/
bool readFile(const string &fileName, vector<string> &fileLines) {
    ifstream cFile;
    string line;

    /* try to open the file */
    cFile.open(fileName.c_str());

    if(!cFile.is_open()) {
        cout << "nao abriu" << endl ;
        return false;
    }
    else
    {   /* Read file content */
        while(getline(cFile, line))
            fileLines.push_back(line); /*store line in the vector*/
        cFile.close();
        return true;
    }
}

bool readNodesFile(string nodesFile, Graph<int> graph) {
    vector<string> nodeLines;

    if (!readFile(nodesFile, nodeLines))
        return false;
    else {
        int nLines;
        stringstream ss(nodeLines[0]);
        ss >> nLines;

        for (int i = 1; i <= nLines; i++) {
            size_t pos;
            size_t j = 0;
            string substring;
            string line = nodeLines[i];

            long nodeId;
            double longitude_in_radians, latitude_in_radians;

            pos = line.find(';');
            while (pos != string::npos) {
                substring = line.substr(0, pos);
                stringstream ss(substring);

                switch (j) {
                    case 0:
                        ss >> nodeId;
                        break;
                    case 3:
                        ss >> longitude_in_radians;
                        break;
                    case 4:
                        ss >> latitude_in_radians ;
                        break;
                }

                line = line.substr(pos + 2, line.length());
                pos = line.find(';');
                j++;
            }
            graph.addVertex(nodeId, longitude_in_radians, latitude_in_radians);
        }
    }
}

bool readRoadsFile(string roadsFile) {
    vector<string> roadsLines;
    if (!readFile(roadsFile, roadsLines))
        return false;
    else {

    }
}

bool readInfoFile(string infoFile) {
    vector<string> infoLines;
    if (!readFile(infoFile, infoLines))
        return false;
    else {

    }
}

/**
 * calculate haversine distance for linear distance // coordinates in radians
 * */
double haversine_km(double lat1, double long1, double lat2, double long2) {
    double dlong = (long2 - long1);
    double dlat = (lat2 - lat1);
    double a = pow(sin(dlat / 2.0), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2.0), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = 6367 * c;

    return d;
}

GraphViewer* initViewer() {
    GraphViewer *gv = new GraphViewer(600, 600, true);

    gv->setBackground("background.jpg");
    gv->createWindow(1000, 1000);
    gv->defineEdgeDashed(true);

    gv->defineVertexColor("blue");
    gv->defineEdgeColor("black");

    return gv;
}

int main() {
    //read txt files
    string nodesFile = "./data/A2.txt";
    GraphViewer *gv = initViewer();
    Graph<int> graph(gv);

    readNodesFile(nodesFile, graph);
    gv->rearrange();

//    string roadsFile = "../data/B2.txt";
//    readNodesFile(roadsFile, graph);

//    string infoFile = "../data/C2.txt";
//    readNodesFile(infoFile, graph);

    return 0;
}