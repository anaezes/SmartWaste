#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <limits>
#include <ctime>

#include "graphviewer.h"
#include "Graph.h"

using namespace std;

/**
 * Para ler os ficheiros:
 * A1, B1, C1 - Nova Iorque
 * A2, B2, C2 - Macedo
 * A3, B3, C3 - PortoP
 * */

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
bool readNodesFile(string nodesFile, map<long, std::pair< int, int>> &nodeCoordinates, Graph<long> &graph) {
    vector<string> nodeLines;

    if (!readFile(nodesFile, nodeLines))
        return false;

    int nLines;
    stringstream ss(nodeLines[0]);
    ss >> nLines;

    long nodeId;
    int x, y;

    for (int i = 1; i <= nLines; i++) {
        string substring;
        string line = nodeLines[i];

        vector<string> lineParts = Utils::splitLine(nodeLines[i], ';');
        ss.clear();
        ss.str(lineParts[0]);
        ss >> nodeId;

        ss.clear();
        ss.str(lineParts[3]);
        ss >> x;

        ss.clear();
        ss.str(lineParts[4]);
        ss >> y;

        pair< int, int> coordinates = make_pair(y, x);
        graph.addVertex(nodeId, coordinates);
        nodeCoordinates.insert(pair<long,pair< int,  int>>(nodeId, coordinates));
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

            if(j==0)
                ss >> edgeId;

            line = line.substr(pos + 1, line.length());
            j++;
            pos  = line.find(';');
        }
        substring = line.substr(0, pos);
        isUndirected = (substring == "True");
        roadsInfoMap.insert(pair<long,bool>(edgeId,isUndirected));
    }
    return true;
}

/**
* Read a txt file - add edge to a graph
**/
bool readInfoFile(string infoFile, Graph<long>& graph, map<long, bool> &roadsInfoMap,
                  const map<long, std::pair< int, int>> &nodeCoordinates) {
    vector<string> infoLines;
    if (!readFile(infoFile, infoLines))
        return false;

    int nLines;
    stringstream ss(infoLines[0]);
    ss >> nLines;

    for (int i = 1; i <= nLines; i++) {
        size_t pos;
        size_t j = 0;
        string substring;
        string line = infoLines[i];

        long edgeId, nodeSourceId, nodeDestId;

        srand (time(NULL));

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

        std::pair<int, int> destCoords = nodeCoordinates.find(nodeSourceId)->second;
        std::pair<int, int> sourceCoords = nodeCoordinates.find(nodeDestId)->second;
        int weight = Utils::distance_km(sourceCoords.first, sourceCoords.second, destCoords.first, destCoords.second);

        graph.addEdge(edgeId, nodeSourceId, nodeDestId, weight, roadsInfoMap.find(edgeId)->second);
    }
    return true;
}

GraphViewer* initViewer() {
    GraphViewer *gv = new GraphViewer(1000, 1000, false);
    gv->setBackground("background.jpg");
    gv->createWindow(2000, 1000);
    gv->defineVertexColor("blue");
    gv->defineEdgeColor("black");

    return gv;
}

int main() {

    string nodesFile = "./data/A2.txt";
    GraphViewer *gv = initViewer();
    Graph<long> graph(gv);
    std::map<long, std::pair< int, int>> nodeCoordinates;
    if(!readNodesFile(nodesFile, nodeCoordinates, graph))
    {
        cout << "Error to read A2.txt!";
        return 1;
    }

       std::map<long, bool> roadsInfoMap;
       string roadsFile = "./data/B2.txt";
       if(!readRoadsFile(roadsFile, roadsInfoMap))
       {
           cout << "Error to read B2.txt!";
           return 1;
       }

       string infoFile = "./data/C2.txt";
       if(!readInfoFile(infoFile, graph, roadsInfoMap, nodeCoordinates))
       {
           cout << "Error to read C2.txt!";
           return 1;
       }

    return 0;
}