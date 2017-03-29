#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <limits>

#include "graphviewer.h"
#include "Graph.h"
#include "Utils.h"

using namespace std;

/**
 * Para ler os ficheiros:
 * A1, B1, C1 - Nova Iorque
 * A2, B2, C2 - Macedo
 * A3, B3, C3 - Porto
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
bool readNodesFile(string nodesFile, map<long, std::pair< long double, long double>> &nodeCoordinates, Graph<long> &graph) {
    vector<string> nodeLines;

    if (!readFile(nodesFile, nodeLines))
        return false;

    int nLines;
    stringstream ss(nodeLines[0]);
    ss >> nLines;


    long double latitudeMin =  std::numeric_limits<double>::max();
    long double longitudeMin =  std::numeric_limits<double>::max();

    long double latitudeMax =  std::numeric_limits<double>::lowest();
    long double longitudeMax =  std::numeric_limits<double>::lowest();

    long nodeId;
    long double longitude_in_radians;
    long double latitude_in_radians;

    for (int i = 1; i < nLines; i++) {
        string substring;
        string line = nodeLines[i];

        vector<string> lineParts = Utils::splitLine(nodeLines[i], ';');
        ss.clear();
        ss.str(lineParts[0]);
        ss >> nodeId;

        ss.clear();
        ss.str(lineParts[3]);
        ss >> longitude_in_radians;

        ss.clear();
        ss.str(lineParts[4]);
        ss >> latitude_in_radians;

        if(latitude_in_radians < latitudeMin)
            latitudeMin = latitude_in_radians;

        if(longitude_in_radians < longitudeMin)
            longitudeMin = longitude_in_radians;

        if(latitude_in_radians > latitudeMax)
            latitudeMax = latitude_in_radians;

        if(longitude_in_radians > longitudeMax)
            longitudeMax = longitude_in_radians;

        pair< long double, long double > coordinates = make_pair(latitude_in_radians, longitude_in_radians);
        nodeCoordinates.insert(pair<long,pair< long double,  long double>>(nodeId, coordinates));
    }

    graph.setMinCoords(latitudeMin, longitudeMin);
    graph.setMaxCoords(latitudeMax, longitudeMax);

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
bool readInfoFile(string infoFile, Graph<long>& graph, map<long, bool> &roadsInfoMap, const map<long, std::pair< long double, long double>> &nodeCoordinates) {
    vector<string> infoLines;
    if (!readFile(infoFile, infoLines))
        return false;

    int nLines;
    stringstream ss(infoLines[0]);
    ss >> nLines;
    long double dist = 0;
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

        std::pair<long double, long double> destCoords = nodeCoordinates.find(nodeDestId)->second;
        std::pair<long double, long double> sourceCoords = nodeCoordinates.find(nodeSourceId)->second;

        double currDistance = Utils::distance_km(sourceCoords.first, sourceCoords.second, destCoords.first, destCoords.second);

        if(currEdgeId == edgeId) {
            dist += currDistance;
            lastNodeId = nodeDestId;
        }
        else {
            if(firstNodeId != -1) {
                graph.addVertex(firstNodeId, sourceCoords);
                graph.addVertex(lastNodeId, destCoords);

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
    GraphViewer *gv = new GraphViewer(600, 600, false);

    gv->setBackground("background.jpg");
    gv->createWindow(600, 600);
    //gv->defineEdgeDashed(true);
    gv->defineVertexColor("blue");
    gv->defineEdgeColor("black");

    return gv;
}

int main() {

    string nodesFile = "./data/A1.txt";
    GraphViewer *gv = initViewer();
    Graph<long> graph(gv);
    std::map<long, std::pair< long double, long double>> nodeCoordinates;
    if(!readNodesFile(nodesFile, nodeCoordinates, graph))
    {
        cout << "Error to read A1.txt!";
        return 1;
    }
    cout << "pim" << endl;
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

    gv->addNode(545,0,0);
    gv->rearrange();

    return 0;
}