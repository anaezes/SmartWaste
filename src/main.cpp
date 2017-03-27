#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "graphviewer.h"
#include "Graph.h"
#include "Utils.h"

using namespace std;

/**
* Read a file to a vector of lines
**/
bool readFile(const string &fileName, vector<string> &fileLines) {
    ifstream cFile;
    string line;

    /* try to open the file */
    cFile.open(fileName.c_str());

    if(!cFile.is_open())
        return false;
    else
    {   /* Read file content */
        while(getline(cFile, line))
            fileLines.push_back(line); /*store line in the vector*/
        cFile.close();
        return true;
    }
}

/**
* Read a txt file of nodes
**/
bool readNodesFile(string nodesFile, Graph<int> graph, map<long, std::pair<double, double>> nodeCoordinates) {
    vector<string> nodeLines;

    if (!readFile(nodesFile, nodeLines))
        return false;

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

            line = line.substr(pos + 1, line.length());
            pos = line.find(';');
            j++;
        }

        pair<double, double > coordinates = make_pair(latitude_in_radians, longitude_in_radians);
        nodeCoordinates.insert(pair<long,pair<double, double>>(nodeId, coordinates));
    }
    return true;
}

/**
* Reads a text file of information of edges: saves edge id and if it's undirected or not.
**/
bool readRoadsFile(string roadsFile, map<long, bool> &roadsInfoMap) {
    vector<string> roadsLines;
    if (!readFile(roadsFile, roadsLines))
        return false;

    int nLines;
    stringstream ss(roadsLines[0]);
    ss >> nLines;

    for (int i = 1; i <= nLines; i++) {
        size_t pos;
        size_t j = 0;
        string substring;
        string line = roadsLines[i];

        long edgeId;
        bool isUndirected;

        pos = line.find(';');
        while (pos != string::npos) {
            substring = line.substr(0, pos);
            stringstream ss(substring);

            switch (j) {
                case 0:
                    ss >> edgeId;
                    break;
                case 2:
                    isUndirected = (substring == "True");
                    break;
            }

            line = line.substr(pos + 1, line.length());
            j++;
            if(j!=2)
                pos  = line.find(';');
            else
                pos  = line.find('\r');


        }
        roadsInfoMap.insert(pair<long,bool>(edgeId,isUndirected));
    }
    return true;
}

/**
* Read a txt file - add edge to a graph
**/
bool readInfoFile(string infoFile, Graph<int> graph, map<long, bool> &roadsInfoMap, const map<long, std::pair<double, double>> &nodeCoordinates) {
    vector<string> infoLines;
    if (!readFile(infoFile, infoLines))
        return false;

    int nLines;
    stringstream ss(infoLines[0]);
    ss >> nLines;
    double dist = 0;
    long currEdgeId = -1;
    long firstNodeId = -1;
    long lastNodeId = -1;
    for (int i = 1; i <= nLines; i++) {
        size_t pos;
        size_t j = 0;
        string substring;
        string line = infoLines[i];

        long edgeId, nodeSourceId, nodeDestId;

        pos = line.find(';');
        while (pos != string::npos) {
            substring = line.substr(0, pos);
            stringstream ss(substring);

            switch (j) {
                case 0:
                    ss >> edgeId;
                    break;
                case 1:
                    ss >> nodeSourceId;
                    break;
                case 2:
                    ss >> nodeDestId;
                    break;
            }

            line = line.substr(pos + 1, line.length());
            pos  = line.find(';');
            j++;
        }

        std::pair<double, double> destCoords = nodeCoordinates.find(nodeDestId)->second;
        std::pair<double, double> sourceCoords = nodeCoordinates.find(nodeSourceId)->second;
        double currDistance = Utils::distance_km(sourceCoords.first, sourceCoords.second, destCoords.first, destCoords.second);

        cout << "currEdgeId: " << currEdgeId << endl;
        if(currEdgeId == edgeId) {
            dist += currDistance;
            lastNodeId = nodeDestId;
        }
        else {
            if(firstNodeId != -1) {
                graph.addVertex(firstNodeId);
                graph.addVertex(lastNodeId);
                graph.addEdge(currEdgeId, firstNodeId, lastNodeId, dist, roadsInfoMap.find(edgeId)->second);
            }
            dist = currDistance;
            currEdgeId = edgeId;
            firstNodeId = nodeSourceId;
        }


    }
    return true;
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

    string nodesFile = "./data/A1.txt";
    GraphViewer *gv = initViewer();
    Graph<int> graph(gv);
    std::map<long, std::pair<double, double>> nodeCoordinates;
    if(!readNodesFile(nodesFile, graph, nodeCoordinates))
    {
        cout << "Error to read A1.txt!";
        return 1;
    }

    gv->rearrange();

    std::map<long, bool> roadsInfoMap;
    string roadsFile = "./data/B1.txt";
    if(!readRoadsFile(roadsFile, roadsInfoMap))
    {
        cout << "Error to read B1.txt!";
        return 1;
    }


    string infoFile = "./data/C1.txt";
    if(!readInfoFile(infoFile, graph, roadsInfoMap, nodeCoordinates))
    {
        cout << "Error to read C1.txt!";
        return 1;
    }

    gv->rearrange();

    return 0;
}