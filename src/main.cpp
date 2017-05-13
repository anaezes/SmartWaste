#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <limits>
#include <ctime>
#include <algorithm>
#include <chrono>

#include "graphviewer.h"
#include "Graph.h"
#include "SmartWaste.h"

using namespace std;

/**
* @brief Read a file to a vector of lines
* @param fileName
* @param fileLines
* @return true if was success
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
* @brief Read a txt file of nodes
* @param nodesFile
* @param nodeCoordinates
* @param smartWaste
* @return true if was success
**/
bool readNodesFile(string nodesFile, map<int, std::pair< int, int>> &nodeCoordinates, SmartWaste &smartWaste) {
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
        smartWaste.getGraph()->addVertex(nodeId, coordinates);
        nodeCoordinates.insert(pair<long,pair< int,  int>>(nodeId, coordinates));
    }

    return true;
}


/**
* @brief Reads a text file of information of edges: saves edge id and if it's undirected or not.
* @param roadsFile
* @param roadsInfoMap
* @return true if was success
**/
bool readRoadsFile(string roadsFile, map<int, bool> &roadsInfoMap, map<int, string> &idRoadsMap, map<string, int> &roadsIdMap) {
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
        string roadName;
        string line = roadsLines[i];

        int edgeId;
        bool isUndirected;

        pos = line.find(';');
        while (pos != string::npos) {
            substring = line.substr(0, pos);
            stringstream ss(substring);

            if(j==0)
                ss >> edgeId;
            else if(j==1)
                roadName = substring;

            line = line.substr(pos + 1, line.length());
            j++;
            pos  = line.find(';');
        }

        substring = line.substr(0, pos);
        isUndirected = (substring == "True");
        roadsInfoMap.insert(pair<int,bool>(edgeId,isUndirected));
        idRoadsMap.insert(pair<int,string>(edgeId,roadName));
        roadsIdMap.insert(pair<string, int>(roadName, edgeId));
    }
    return true;
}

/**
* @brief Read a txt file - add edge to a graph
* @param infoFile
* @param smartWaste
* @param roadsInfoMap
* @return true if was success
**/
bool readInfoFile(string infoFile, SmartWaste &smartWaste, map<int, bool> &roadsInfoMap, map<int, string> &roadsNameMap,
                  const map<int, std::pair< int, int>> &nodeCoordinates) {
    vector<string> infoLines;
    if (!readFile(infoFile, infoLines))
        return false;

    int nLines;
    stringstream ss(infoLines[0]);
    ss >> nLines;

    for (int i = 1; i <= nLines; i++) {
        string substring;
        string line = infoLines[i];
        int edgeId, nodeSourceId, nodeDestId;

        for (int i = 1; i <= nLines; i++) {
            string substring;
            string line = infoLines[i];

            vector<string> lineParts = Utils::splitLine(infoLines[i], ';');
            ss.clear();
            ss.str(lineParts[0]);
            ss >> edgeId;

            ss.clear();
            ss.str(lineParts[1]);
            ss >> nodeSourceId;

            ss.clear();
            ss.str(lineParts[2]);
            ss >> nodeDestId;

            std::pair<int, int> destCoords = nodeCoordinates.find(nodeSourceId)->second;
            std::pair<int, int> sourceCoords = nodeCoordinates.find(nodeDestId)->second;
            string roadName = roadsNameMap.find(edgeId)->second;
            bool value = roadsInfoMap.find(edgeId)->second;
            int weight = Utils::distance(sourceCoords.first, sourceCoords.second, destCoords.first, destCoords.second);

            smartWaste.getGraph()->addEdge(edgeId, roadName, nodeSourceId, nodeDestId, weight, value);
        }
    }
    return true;
}


/**
* @brief Init GraphViewer
* @return graph viewer
**/
GraphViewer* initViewer() {
    GraphViewer *gv = new GraphViewer(1000, 500, false);
    gv->setBackground("./images/background.jpg");
    gv->createWindow(1000, 500);
    gv->defineVertexColor(GRAY);
    gv->defineEdgeColor(DARK_GRAY);

    return gv;
}

/**
* @brief charge txt files in graph
* @param smartWaste
* @param nodeCoordinates
* @param roadsInfoMap
* @param option : 1 if the smallest, 2 if the medium and 3 if the biggest
* @return true if was success
**/
bool initGraph(SmartWaste &smartWaste, map<int, std::pair<int, int>> &nodeCoordinates, map<int, bool> &roadsInfoMap,
               map<int, string> &idRoadsMap, int option, map<string, int> &roadsNameMap) {

    string nodesFile;
    string roadsFile;
    string infoFile;

    if(option == 1) {
        nodesFile = "./data/A_smallGraph.txt";
        roadsFile = "./data/B_smallGraph.txt";
        infoFile  = "./data/C_smallGraph.txt";

        /* Testar graph nao 100 % conexo
        nodesFile = "./data/A1.txt";
        roadsFile = "./data/B1.txt";
        infoFile  = "./data/C1.txt"; */
    }
    else if(option == 2) {
        nodesFile = "./data/A_mediumGraph.txt";
        roadsFile = "./data/B_mediumGraph.txt";
        infoFile  = "./data/C_mediumGraph.txt";
    }
    else {
        nodesFile = "./data/A_bigGraph.txt";
        roadsFile = "./data/B_bigGraph.txt";
        infoFile  = "./data/C_bigGraph.txt";
    }

    if(!readNodesFile(nodesFile, nodeCoordinates, smartWaste) || !readRoadsFile(roadsFile, roadsInfoMap, idRoadsMap, roadsNameMap)
       || !readInfoFile(infoFile, smartWaste, roadsInfoMap, idRoadsMap, nodeCoordinates))
    {
        cout << "Error to read a file!";
        return false;
    }

    return true;
}


/**
* @brief show menu to user
* @return option
**/
int showMenu() {
    cout << endl << "Please choose an option: " << endl << endl;
    cout << "Generate a test case:" << endl;
    cout << "  1. Simple case" << endl;
    cout << "  2. Recycling case" << endl;
    cout << "  3. Compute solution: Dijkstra" << endl;
    cout << "  4. Time comparison of Dijkstra and Floyd Warshall" << endl;
    cout << "  5. Graph connectivity" << endl << endl;

    cout << "  6. Street Search" << endl << endl;

    cout << "  7. Reset" << endl;
    cout << "  8. Back " << endl << endl;

    cout << ">";
    int option;
    cin >> option;
    return option;
}

/**
* @brief main menu
* @return option
**/
int mainSmartWaste(int optionGraph) {
    srand(time(NULL));
    GraphViewer *gv = initViewer();
    SmartWaste smartWaste(gv);
    std::map<int, std::pair< int, int>> nodeCoordinates;
    std::map<int, bool> roadsInfoMap;
    std::map<int, string> idRoadsMap;
    std::map<string, int> roadsIdMap;

    if(!initGraph(smartWaste, nodeCoordinates, roadsInfoMap, idRoadsMap, optionGraph, roadsIdMap))
        return 1;

    vector<int> fullNodes;
    vector<int> fullNodesPaper;
    vector<int> fullNodesGlass;
    vector<int> fullNodesPlastic;
    bool recyclingCase = false;

    smartWaste.initGaragesAndCentrals();

    while(true) {
        int option = showMenu();
        switch (option) {
            case 1:
                smartWaste.generateRandomCases(fullNodes);
                smartWaste.paintNodes(fullNodes, RED);
                recyclingCase = false;
                smartWaste.getGraph()->getGV()->rearrange();
                break;
            case 2:
                smartWaste.generateRandomCasesRecycling(fullNodesPaper, fullNodesGlass, fullNodesPlastic);
                recyclingCase = true;
                smartWaste.getGraph()->getGV()->rearrange();
                break;
            case 3:
                if(!recyclingCase && !fullNodes.empty())
                    smartWaste.computeSolutionDikstra(fullNodes, RED);
                else if(!fullNodesPaper.empty() || !fullNodesGlass.empty() || !fullNodesPlastic.empty())
                    smartWaste.computeSolutionRecycling(fullNodesPaper, fullNodesGlass, fullNodesPlastic);
                break;
            case 4:
                    smartWaste.timeComparison();
                break;
            case 5:
                if(optionGraph == 1)
                    smartWaste.resetDisplay();
                smartWaste.verifyConnectivity();
                break;
            case 6:
                if(smartWaste.getGraph()->getNumVertex() == SMALLGRAPHSIZE)
                    smartWaste.streetSearch(roadsIdMap, fullNodes);
                else
                    cout<< "Sorry... Option not available. " << endl;
                break;
            case 7:
                smartWaste.resetGraph(fullNodes, fullNodesPaper, fullNodesGlass, fullNodesPlastic);
                recyclingCase = false;
                break;
            case 8:
                smartWaste.getGraph()->getGV()->closeWindow();
                return 0;
            default:
                cout << "Option not Valid... try again." << endl;
                Utils::doSleep(1000);
        }
    }

}

/**
* @brief first menu to choose graph: 1 if small, 2 if medium and 3 if the big graph
* @return option
**/
int chooseGraphMenu() {
    cout << endl << "Please choose an option: " << endl << endl;
    cout << "  1. Small graph " << endl;
    cout << "  2. Medium graph " << endl;
    cout << "  3. Big graph" << endl << endl;
    cout << "  4. Quit " << endl << endl;

    cout << ">";
    int option;
    cin >> option;
    return option;
}

/**
* @brief main menu
* @return 0 to exit
**/
int main() {
    while(true){
        int option = chooseGraphMenu();
        switch (option) {
            case 1:
            case 2:
            case 3:
                mainSmartWaste(option);
                break;
            case 4:
                return 0;
        }
    }
}

