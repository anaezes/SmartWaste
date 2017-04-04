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

#define DIJKSTRA 0
#define FLOYDWARSHALL 1
#define NUM_TESTS 100

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
bool readNodesFile(string nodesFile, map<int, std::pair< int, int>> &nodeCoordinates, Graph<int> &graph) {
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
bool readRoadsFile(string roadsFile, map<int, bool> &roadsInfoMap) {
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
bool readInfoFile(string infoFile, Graph<int>& graph, map<int, bool> &roadsInfoMap,
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
            int weight = Utils::distance_km(sourceCoords.first, sourceCoords.second, destCoords.first, destCoords.second);

            graph.addEdge(edgeId, nodeSourceId, nodeDestId, weight, roadsInfoMap.find(edgeId)->second);
        }
    }
    return true;
}


/**
* Init GraphViewer
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
* charge txt files in graph
**/
bool initGraph(Graph<int> &graph, map<int, std::pair<int, int>> &nodeCoordinates, map<int, bool> &roadsInfoMap) {
    string nodesFile = "./data/A2.txt";
    string roadsFile = "./data/B2.txt";
    string infoFile = "./data/C2.txt";

    if(!readNodesFile(nodesFile, nodeCoordinates, graph) || !readRoadsFile(roadsFile, roadsInfoMap)
       || !readInfoFile(infoFile, graph, roadsInfoMap, nodeCoordinates))
    {
        cout << "Error to read a file!";
        return false;
    }

    return true;
}

/**
* show menu to user
**/
int showMenu() {
    cout << endl << "Please choose an option: " << endl << endl;
    cout << "Generate a test case:" << endl;
    cout << "  1. Simple case" << endl;
    cout << "  2. Recycling case" << endl << endl;

    cout << "Compute solution:" << endl;
    cout << "  3. Dijkstra" << endl;
    cout << "  4. Floyd Warshall" << endl<< endl;

    cout << "  5. Time comparison of Dijkstra and Floyd Warshall" << endl << endl;

    cout << "  6. Graph connectivity" << endl;
    cout << "  7. Reset" << endl;
    cout << "  8. Quit " << endl << endl;

    cout << ">";
    int option;
    cin >> option;
    return option;
}

/**
* Generate random test cases
**/
void generateRandomCases(Graph<int> &graph, vector<int> &fullNodes, string color, const vector<int> &garages, const vector<int> &centrals)
{
    int num = rand() % 6 + 2;
    int i = 0;
    while(i < num) {
        int id = rand() % 32 + 1;
        if ((std::find(fullNodes.begin(), fullNodes.end(), id) == fullNodes.end()) && (std::find(garages.begin(), garages.end(), id) == garages.end())
            && (std::find(centrals.begin(), centrals.end(), id) == centrals.end())) {
            graph.getGV()->setVertexColor(id, color);
            fullNodes.push_back(id);
            i++;
        }

        if(fullNodes.size() == graph.getNumVertex()-2)
        {
            cout << "All containers are full!" << endl;
            return;
        }
    }
}

void setNodesState(Graph<int> &graph, const vector<int> &fullNodes, string color) {
    Vertex<int>* v;
    for(size_t i = 0; i < fullNodes.size(); i++) {
        v = graph.getVertex(fullNodes[i]);
        if(color == BLUE)
            v->setPaperFull(true);
        else if(color == GREEN)
            v->setGlassFull(true);
        else
            v->setPlasticFull(true);
    }
}


void generateRandomCasesRecycling(Graph<int> &graph, vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic,
                                  const vector<int> &garages, const vector<int> &centrals) {
    generateRandomCases(graph, fullNodesPlastic, YELLOW, garages, centrals);
    setNodesState(graph, fullNodesPlastic, YELLOW);
    generateRandomCases(graph, fullNodesGlass, GREEN, garages, centrals);
    setNodesState(graph, fullNodesGlass, GREEN);
    generateRandomCases(graph, fullNodesPaper, BLUE, garages, centrals);
    setNodesState(graph, fullNodesPaper, BLUE);
}

/**
* Return the next garbage colector to visit (closest the current souce)
**/
int computeNextVertex(Graph<int> &graph, vector<int> &fullNodes) {
    int distMin = INT_MAX;
    int vertexDistMinIndex = 0;
    int nodeDist;
    for(size_t j = 0; j < fullNodes.size(); j++) {
        nodeDist = graph.getVertex(fullNodes[j])->getDist();
        if (nodeDist < distMin) {
            distMin = nodeDist;
            vertexDistMinIndex = j;
        }
    }
    return vertexDistMinIndex;
}


/**
* Add computed path so the solution
**/
void addPath(vector<int> &pathSolution, const vector<int> &newPath) {
    for(size_t i = 0; i < newPath.size(); i++)
        pathSolution.push_back(newPath[i]);
}

void resetNode(Graph<int> &graph, int currentNode, string color, int i, int auxId) {
    Vertex<int>* v = graph.getVertex(currentNode);

    if(i+1 == auxId)
        return;

    if(color == BLUE) {
        v->setPaperFull(false);
        if(v->isGlassFull())
            graph.getGV()->setVertexColor(v->getInfo(), GREEN);
        else if(v->isPlasticFull())
            graph.getGV()->setVertexColor(v->getInfo(), YELLOW);
        else
            graph.getGV()->setVertexColor(v->getInfo(), GRAY);
    }
    else if( color == GREEN) {
        v->setGlassFull(false);
        if(v->isPlasticFull())
            graph.getGV()->setVertexColor(v->getInfo(), YELLOW);
        else
            graph.getGV()->setVertexColor(v->getInfo(), GRAY);
    }
    else {
        v->setPlasticFull(false);
        graph.getGV()->setVertexColor(v->getInfo(), GRAY);
    }
}


/**
* Display solution
**/
void  displaySolution(Graph<int> &graph, vector<int> pathSolution, int auxId, int truckContains, string colorEdge) {

    cout << ">>>> An empty truck";
    if(colorEdge == BLUE)
        cout << " of paper";
    else if(colorEdge == GREEN)
        cout << " of glass";
    else if(colorEdge == YELLOW)
        cout << " of plastic";
    cout << " is on the way..." << endl;

    for(size_t i = 0; i < pathSolution.size()-1; i++)
    {
        int edgeId = graph.getEdge(graph.getVertex(pathSolution[i])->getInfo(), graph.getVertex(pathSolution[i+1])->getInfo());
        graph.getGV()->setEdgeColor(edgeId, colorEdge);
        graph.getGV()->setEdgeThickness(edgeId, 5);
        resetNode(graph, pathSolution[i], colorEdge, i, auxId);
        graph.getGV()->rearrange();
        Utils::doSleep(1000);

        if(i+1 == auxId && truckContains >= TRUCK_CAPACITY) {
            cout << ">>>> Truck";
            if(colorEdge == BLUE)
                cout << " of paper";
            else if(colorEdge == GREEN)
                cout << " of glass";
            else if(colorEdge == YELLOW)
                cout << " of plastic";
            cout << " is full!" << endl;
        }
    }
}

int switchCentral(Graph<int> &graph, const vector<int> &centrals){
    int distMin = INT_MAX;
    int vertexDistMinIndex = 0;
    int nodeCentralDist;
    for(size_t j = 0; j < centrals.size(); j++) {
        nodeCentralDist = graph.getVertex(centrals[j])->getDist();
        if (nodeCentralDist < distMin) {
            distMin = nodeCentralDist;
            vertexDistMinIndex = j;
        }
    }
    return centrals[vertexDistMinIndex];

}

int switchGarage(const Graph<int> &graph, const vector<int> &fullNodes, const vector<int> &garages) {
    int distMin = INT_MAX;
    int vertexDistMinIndex = 0;
    int nodeDist;
    for(size_t i = 0; i < garages.size(); i++) {
        graph.dijkstraShortestPath(garages[i]);
        for (size_t j = 0;  j < fullNodes.size(); j++) {
            nodeDist = graph.getVertex(fullNodes[j])->getDist();
            if (nodeDist < distMin) {
                distMin = nodeDist;
                vertexDistMinIndex = i;
            }
        }
    }
    return garages[vertexDistMinIndex];
}

/**
* main of compute solution
**/
void computeSolution(Graph<int> &graph, vector<int> &fullNodes, string colorEdge, int type, const vector<int> &garages, const vector<int> &centrals) {
    int lastSourceId = 0;
    vector<int> pathSolution;
    int truckContains = 0;
    int auxId;

    int sourceId = switchGarage(graph, fullNodes, garages);

    if(type == FLOYDWARSHALL)
        graph.floydWarshallShortestPath();

    while(!fullNodes.empty() && (truckContains+CONTAINER_CAPACITY <= TRUCK_CAPACITY)) {

        if(type == DIJKSTRA)
            graph.dijkstraShortestPath(sourceId);

        lastSourceId = sourceId;
        int sourceIndex = computeNextVertex(graph, fullNodes);
        sourceId = fullNodes[sourceIndex];
        fullNodes.erase(fullNodes.begin()+sourceIndex);

        if(type == DIJKSTRA)
            addPath(pathSolution, graph.getPath(lastSourceId, sourceId));
        else
            addPath(pathSolution, graph.getfloydWarshallPath(lastSourceId, sourceId));

        truckContains += CONTAINER_CAPACITY;
        if(truckContains >= TRUCK_CAPACITY)
            auxId = pathSolution.size();
    }

    // go back to the central
    graph.dijkstraShortestPath(sourceId);
    int centralId = switchCentral(graph, centrals);
    addPath(pathSolution, graph.getPath(sourceId, centralId));

    displaySolution(graph, pathSolution, auxId, truckContains, colorEdge);

    if(!fullNodes.empty())
        computeSolution(graph, fullNodes, colorEdge, type, garages, centrals);
}

void computeSolutionRecycling(Graph<int> &graph, vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic,
                              int type, const vector<int> &garages, const vector<int> &centrals) {
    computeSolution(graph, fullNodesPaper, BLUE, type, garages, centrals);
    computeSolution(graph, fullNodesGlass, GREEN, type, garages, centrals);
    computeSolution(graph, fullNodesPlastic, YELLOW, type, garages, centrals);
}

/**
* clear the display
**/
void resetDisplay(const Graph<int> &graph) {
    vector<Edge<int>> edges = graph.getEdges();
    for(size_t i = 0; i < edges.size(); i++) {
        int edgeInfo = edges[i].getInfo();
        graph.getGV()->setEdgeColor(edgeInfo, DARK_GRAY);
        graph.getGV()->setEdgeThickness(edgeInfo, 1);
    }

    int nNodes = graph.getNumVertex();
    for(size_t i = 0; i < nNodes; i++)
        graph.getGV()->setVertexColor(i+1, GRAY);
    graph.getGV()->rearrange();
}


/**
* clear the display and reset variables for new tests
**/
void resetGraph(const Graph<int> &graph, vector<int> &fullNodes, vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic) {
    resetDisplay(graph);

    //reset vectors full and reset state of node
    if(!fullNodesPaper.empty())
        for(size_t i = 0; i < fullNodesPaper.size(); i++)
            graph.getVertex(fullNodesPaper[i])->setPaperFull(false);

    if(!fullNodesGlass.empty())
        for(size_t i = 0; i < fullNodesGlass.size(); i++)
            graph.getVertex(fullNodesGlass[i])->setGlassFull(false);

    if(!fullNodesPlastic.empty())
        for(size_t i = 0; i < fullNodesPlastic.size(); i++)
            graph.getVertex(fullNodesPlastic[i])->setPlasticFull(false);

    fullNodes.clear();
    fullNodesPaper.clear();
    fullNodesGlass.clear();
    fullNodesPlastic.clear();
}

/**
* Auxiliary method of analyze the connectivity of the graph
**/
void paintNodes(vector<int> nodes, const Graph<int> &graph, int source){
    for(size_t i = 0; i < nodes.size(); i++) {
        if(nodes[i] != source)
            graph.getGV()->setVertexColor(nodes[i], RED);
        Utils::doSleep(50);
        graph.getGV()->rearrange();
    }
}

/**
* Analyze the connectivity of the graph
**/
void verifyConnectivity(const Graph<int> &graph){
    cout << endl << "Analyzing..." << endl ;
    double average = 0;
    for(size_t i = 0; i < graph.getNumVertex(); i++) {
        int source = graph.getVertex(i+1)->getInfo();
        graph.getGV()->setVertexColor(source, BLUE);
        vector<int> bfs = graph.bfs(graph.getVertex(i+1));
        paintNodes(bfs, graph, source);
        double connectivity = (double) 1 * bfs.size() / graph.getNumVertex();
        average += connectivity;
        Utils::doSleep(1000);
        resetDisplay(graph);
    }
    average /= graph.getNumVertex();
    average *= 100;
    cout << endl << "Average of connectivity: " << average << " %" << endl;
    Utils::doSleep(3000);
}


void auxTimeComparison(Graph<int> graph, vector<int> fullNodes, const int &type) {
    int sourceId = nodeTrucks;
    int lastSourceId = 0;
    vector<int> pathSolution;
    int truckContains = 0;

    if(type == FLOYDWARSHALL)
        graph.floydWarshallShortestPath();


    while(!fullNodes.empty() && (truckContains+CONTAINER_CAPACITY <= TRUCK_CAPACITY)) {

        if(type == DIJKSTRA)
            graph.dijkstraShortestPath(sourceId);

        lastSourceId = sourceId;
        int sourceIndex = computeNextVertex(graph, fullNodes);
        sourceId = fullNodes[sourceIndex];
        fullNodes.erase(fullNodes.begin()+sourceIndex);

        if(type == DIJKSTRA)
            addPath(pathSolution, graph.getPath(lastSourceId, sourceId));
        else
            addPath(pathSolution, graph.getfloydWarshallPath(lastSourceId, sourceId));

        truckContains += CONTAINER_CAPACITY;
    }

    // go back to the central
    graph.dijkstraShortestPath(sourceId);
    addPath(pathSolution, graph.getPath(sourceId, NODE_CENTRAL));
}

void timeComparison(Graph<int> &graph) {
    auto elapsedDijkstra = 0;
    auto elapsedFloyd = 0;
    vector<int> garages;
    vector<int> centrals;
    garages.push_back(13);
    centrals.push_back(20);

    for(int i = 0; i < NUM_TESTS ; i++) {
        vector<int> fullNodesPaper;
        vector<int> fullNodesGlass;
        vector<int> fullNodesPlastic;

        generateRandomCasesRecycling(graph, fullNodesPaper, fullNodesGlass, fullNodesPlastic, garages, centrals);

        auto startDijkstra = std::chrono::system_clock::now();
        auxTimeComparison(graph, fullNodesPaper, DIJKSTRA);
        auxTimeComparison(graph, fullNodesGlass, DIJKSTRA);
        auxTimeComparison(graph, fullNodesPlastic, DIJKSTRA);
        auto endDijkstra = std::chrono::system_clock::now();
        elapsedDijkstra = elapsedDijkstra + std::chrono::duration_cast<std::chrono::milliseconds>(endDijkstra - startDijkstra).count();

        auto startFloyd = std::chrono::system_clock::now();
        auxTimeComparison(graph, fullNodesPaper, FLOYDWARSHALL);
        auxTimeComparison(graph, fullNodesGlass, FLOYDWARSHALL);
        auxTimeComparison(graph, fullNodesPlastic, FLOYDWARSHALL);
        auto endFloyd = std::chrono::system_clock::now();
        elapsedFloyd = elapsedFloyd + std::chrono::duration_cast<std::chrono::milliseconds>(endFloyd - startFloyd).count();

        resetDisplay(graph);
    }

    double averageDijkstra = elapsedDijkstra/NUM_TESTS;
    cout << "Average time Dijkstra: " << averageDijkstra << endl;

    double averageFloyd = elapsedFloyd/NUM_TESTS;
    cout << "Average time Floyd Warshall: " << averageFloyd << endl;

    Utils::doSleep(2000);
}

void initGaragesAndCentrals(Graph<int> &graph, vector<int> &garages, vector<int> &centrals) {
    int numGarages = rand() % 2 + 1;
    int i = 0;
    while(i < numGarages) {
        int id = rand() % 32 + 1;
        if (std::find(garages.begin(), garages.end(), id) == garages.end()) {
            graph.getGV()->setVertexIcon(id, "./images/truck.png");
            garages.push_back(id);
            i++;
        }
    }
    int numCentrals = rand() % 2 + 1;
    i = 0;
    while(i < numCentrals) {
        int id = rand() % 32 + 1;
        if (std::find(garages.begin(), garages.end(), id) == garages.end() && std::find(centrals.begin(), centrals.end(), id) == centrals.end() ) {
            graph.getGV()->setVertexIcon(id, "./images/reciclagem.png");
            centrals.push_back(id);
            i++;
        }
    }
}

int main() {
    srand(time(NULL));
    GraphViewer *gv = initViewer();
    Graph<int> graph(gv);
    std::map<int, std::pair< int, int>> nodeCoordinates;
    std::map<int, bool> roadsInfoMap;

    if(!initGraph(graph, nodeCoordinates, roadsInfoMap))
        return 1;

    vector<int> garages;
    vector<int> centrals;
    initGaragesAndCentrals(graph, garages, centrals);

    vector<int> fullNodes;
    vector<int> fullNodesPaper;
    vector<int> fullNodesGlass;
    vector<int> fullNodesPlastic;
    bool recyclingCase = false;

    while(true) {
        int option = showMenu();
        switch (option) {
            case 1:
                generateRandomCases(graph, fullNodes, RED, garages, centrals);
                recyclingCase = false;
                graph.getGV()->rearrange();
                break;
            case 2:
                generateRandomCasesRecycling(graph, fullNodesPaper, fullNodesGlass, fullNodesPlastic, garages, centrals);
                recyclingCase = true;
                graph.getGV()->rearrange();
                break;
            case 3:
                if(!recyclingCase)
                    computeSolution(graph, fullNodes, RED, DIJKSTRA, garages, centrals);
                else
                    computeSolutionRecycling(graph, fullNodesPaper, fullNodesGlass, fullNodesPlastic, DIJKSTRA, garages, centrals);
                break;
            case 4:
                if(!recyclingCase)
                    computeSolution(graph, fullNodes, RED, FLOYDWARSHALL, garages, centrals);
                else
                    computeSolutionRecycling(graph, fullNodesPaper, fullNodesGlass, fullNodesPlastic, FLOYDWARSHALL, garages, centrals);
                break;
            case 5:
                resetDisplay(graph);
                timeComparison(graph);
                break;
            case 6:
                resetDisplay(graph);
                verifyConnectivity(graph);
                break;
            case 7:
                resetGraph(graph, fullNodes, fullNodesPaper, fullNodesGlass, fullNodesPlastic);
                recyclingCase = false;
                break;
            case 8:
                graph.getGV()->closeWindow();
                return 0;
            default:
                cout << "Option not Valid... try again." << endl;
                Utils::doSleep(1000);
        }
    }
}

