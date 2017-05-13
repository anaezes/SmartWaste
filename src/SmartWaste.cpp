//
// Created by ana on 07-04-2017.
//

#include "SmartWaste.h"
#include "Search.h"

SmartWaste::SmartWaste(GraphViewer *gv) : graph(gv), centrals(), garages() {}

void SmartWaste::initGaragesAndCentrals() {
    int numGarages;
    int numCentrals;

    if(graph.getNumVertex() == BIGGRAPHSIZE) {
        numGarages = rand() % 20 + 10;
        numCentrals = rand() % 20 + 10;
    }
    else if (graph.getNumVertex() == MIDDLEGRAPHSIZE) {
        numGarages = rand() % 10 + 5;
        numCentrals = rand() % 10 + 5;
    }
    else {
        numGarages = rand() % 2 + 1;
        numCentrals = rand() % 2 + 1;
    }

    int i = 0;
    while(i < numGarages) {
        int id = rand() % graph.getNumVertex() + 1;
        if (std::find(garages.begin(), garages.end(), id) == garages.end()) {
            graph.getGV()->setVertexIcon(id, "./images/truck.png");
            garages.push_back(id);
            i++;
        }
    }

    i = 0;
    while(i < numCentrals) {
        int id = rand() % graph.getNumVertex() + 1;
        if (std::find(garages.begin(), garages.end(), id) == garages.end() && std::find(centrals.begin(), centrals.end(), id) == centrals.end() ) {
            graph.getGV()->setVertexIcon(id, "./images/reciclagem.png");
            centrals.push_back(id);
            i++;
        }
    }
    graph.getGV()->rearrange();
}

Graph<int>* SmartWaste::getGraph() {
    return &graph;
}


void SmartWaste::paintNodes(const vector<int> &fullNodes, const string &color){
    for(size_t i = 0; i < fullNodes.size(); i++) {
        graph.getGV()->setVertexColor(fullNodes[i], color);
    }
}



void SmartWaste::generateRandomCases(vector<int> &fullNodes)
{
    int num;
    if(graph.getNumVertex() == SMALLGRAPHSIZE)
        num = rand() % 6 + 2;
    else if(graph.getNumVertex() == MIDDLEGRAPHSIZE)
        num = rand() % 20 + 5;
    else
        num = rand() % 40 + 10;

    int i = 0;
    while(i < num) {
        int id = rand() % graph.getNumVertex() + 1;
        if ((std::find(fullNodes.begin(), fullNodes.end(), id) == fullNodes.end()) && (std::find(garages.begin(), garages.end(), id) == garages.end())
            && (std::find(centrals.begin(), centrals.end(), id) == centrals.end())) {
            fullNodes.push_back(id);
            i++;
        }

        if(fullNodes.size() == (uint)graph.getNumVertex()-2)
        {
            cout << "All containers are full!" << endl;
            return;
        }
    }
}

void SmartWaste::setNodesState(const vector<int> &fullNodes, string color) {
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


void SmartWaste::generateRandomCasesRecycling(vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic) {
    generateRandomCases(fullNodesPlastic);
    setNodesState(fullNodesPlastic, YELLOW);
    paintNodes(fullNodesPlastic, YELLOW);
    generateRandomCases(fullNodesGlass);
    setNodesState(fullNodesGlass, GREEN);
    paintNodes(fullNodesGlass, GREEN);
    generateRandomCases(fullNodesPaper);
    setNodesState(fullNodesPaper, BLUE);
    paintNodes(fullNodesPaper, BLUE);
}



int SmartWaste::computeNextVertex(vector<int> &fullNodes) {
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



void SmartWaste::addPath(vector<int> &pathSolution, const vector<int> &newPath) {
    for(size_t i = 0; i < newPath.size(); i++)
        pathSolution.push_back(newPath[i]);
}

void SmartWaste::resetNode(int currentNode, string color) {
    Vertex<int>* v = graph.getVertex(currentNode);

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



void SmartWaste::displaySolution(vector<int> pathSolution, unsigned int lastNodeId, int truckContains, string colorEdge) {

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
        if(i+1 != lastNodeId)
            resetNode(pathSolution[i], colorEdge);
        graph.getGV()->rearrange();
        if(graph.getNumVertex() == SMALLGRAPHSIZE)
            Utils::doSleep(500);
        else if(graph.getNumVertex() == MIDDLEGRAPHSIZE)
            Utils::doSleep(300);
        else
            Utils::doSleep(200);

        if(i+1 == lastNodeId && truckContains >= TRUCK_CAPACITY) {
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

int SmartWaste::switchCentralDikstra() const{
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

int SmartWaste::switchGarageDikstra(const vector<int> &fullNodes)  {
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


void SmartWaste::computeSolutionDikstra(vector<int> &fullNodes, string colorEdge) {
    int lastSourceId = 0;
    vector<int> pathSolution;
    int truckContains = 0;
    int lastNodeId;

    int sourceId = switchGarageDikstra(fullNodes);

    while(!fullNodes.empty() && (truckContains+CONTAINER_CAPACITY <= TRUCK_CAPACITY)) {
        graph.dijkstraShortestPath(sourceId);
        lastSourceId = sourceId;
        int sourceIndex = computeNextVertex(fullNodes);
        sourceId = fullNodes[sourceIndex];
        fullNodes.erase(fullNodes.begin()+sourceIndex);

        addPath(pathSolution, graph.getPath(lastSourceId, sourceId));

        truckContains += CONTAINER_CAPACITY;
        if(truckContains >= TRUCK_CAPACITY)
            lastNodeId = pathSolution.size();
    }

    // go back to the central
    graph.dijkstraShortestPath(sourceId);
    int centralId = switchCentralDikstra();
    addPath(pathSolution, graph.getPath(sourceId, centralId));

    displaySolution(pathSolution, lastNodeId, truckContains, colorEdge);

    if(!fullNodes.empty())
        computeSolutionDikstra(fullNodes, colorEdge);
}



void SmartWaste::computeSolutionRecycling(vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic) {
        computeSolutionDikstra(fullNodesPaper, BLUE);
        computeSolutionDikstra(fullNodesGlass, GREEN);
        computeSolutionDikstra(fullNodesPlastic, YELLOW);
}



void SmartWaste::resetDisplay() {
    vector<Edge<int>> edges = graph.getEdges();
    for(size_t i = 0; i < edges.size(); i++) {
        int edgeInfo = edges[i].getInfo();
        graph.getGV()->setEdgeColor(edgeInfo, DARK_GRAY);
        graph.getGV()->setEdgeThickness(edgeInfo, 1);
    }

    int nNodes = graph.getNumVertex();
    for(int i = 0; i < nNodes; i++)
        graph.getGV()->setVertexColor(i+1, GRAY);
    graph.getGV()->rearrange();
}



void SmartWaste::resetGraph( vector<int> &fullNodes, vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic) {
    resetDisplay();

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



void SmartWaste::paintNodes(vector<int> nodes, int source){
    for(size_t i = 0; i < nodes.size(); i++) {
        if(nodes[i] != source)
            graph.getGV()->setVertexColor(nodes[i], RED);
        Utils::doSleep(15);
        graph.getGV()->rearrange();
    }
}



void SmartWaste::verifyConnectivity(){
    cout << endl << "Analyzing..." << endl ;
    double average = 0;
    for(int i = 0; i < graph.getNumVertex(); i++) {
        int source = graph.getVertex(i+1)->getInfo();

        if(graph.getNumVertex() == SMALLGRAPHSIZE)
            graph.getGV()->setVertexColor(source, BLUE);

        vector<int> bfs = graph.bfs(graph.getVertex(i+1));
        if(graph.getNumVertex() == SMALLGRAPHSIZE)
            paintNodes(bfs, source);

        double connectivity = (double) 1 * bfs.size() / graph.getNumVertex();
        average += connectivity;

        if(graph.getNumVertex() == SMALLGRAPHSIZE)
        {
            Utils::doSleep(100);
            resetDisplay();
        }
    }
    average /= graph.getNumVertex();
    average *= 100;
    cout << endl << "Average of connectivity: " << average << " %" << endl;
    Utils::doSleep(3000);
}

void SmartWaste::auxTimeComparisonDijkstra(vector<int> fullNodes) {
    int lastSourceId = 0;
    vector<int> pathSolution;

    int sourceId = switchGarageDikstra(fullNodes);

    while(!fullNodes.empty()) {
        graph.dijkstraShortestPath(sourceId);
        lastSourceId = sourceId;
        int sourceIndex = computeNextVertex(fullNodes);
        sourceId = fullNodes[sourceIndex];
        fullNodes.erase(fullNodes.begin()+sourceIndex);
        addPath(pathSolution, graph.getPath(lastSourceId, sourceId));
    }

    graph.dijkstraShortestPath(sourceId);
    int centralId = switchCentralDikstra();
    addPath(pathSolution, graph.getPath(sourceId, centralId));
}

int SmartWaste::switchCentralFloydWarshall(int sourceId) {
    int distMin = INT_MAX;
    int vertexDistMinIndex = 0;
    int nodeCentralDist;
    for(size_t j = 0; j < centrals.size(); j++) {
        nodeCentralDist = graph.getWeightFloydWarshall(centrals[j],sourceId);
        if (nodeCentralDist < distMin) {
            distMin = nodeCentralDist;
            vertexDistMinIndex = j;
        }
    }
    return centrals[vertexDistMinIndex];
}

int SmartWaste::switchGarageFloydWarshall(const vector<int> &fullNodes)  {
    int distMin = INT_MAX;
    int vertexDistMinIndex = 0;
    int nodeDist;
    vector<int> path;

    for(size_t i = 0; i < garages.size(); i++) {
        for (size_t j = 0;  j < fullNodes.size(); j++) {
            nodeDist = graph.getWeightFloydWarshall(garages[i],fullNodes[j]);
            if (nodeDist < distMin) {
                distMin = nodeDist;
                vertexDistMinIndex = i;
            }
        }
    }
    return garages[vertexDistMinIndex];
}

void SmartWaste::auxTimeComparisonFloydWarshal(vector<int> fullNodes) {
    int lastSourceId = 0;
    vector<int> pathSolution;

    graph.floydWarshallShortestPath();
    int sourceId = switchGarageFloydWarshall(fullNodes);

    while(!fullNodes.empty()) {
        lastSourceId = sourceId;
        int sourceIndex = computeNextVertex(fullNodes);
        sourceId = fullNodes[sourceIndex];
        fullNodes.erase(fullNodes.begin()+sourceIndex);
        addPath(pathSolution, graph.getfloydWarshallPath(lastSourceId, sourceId));
    }

    int centralId = switchCentralFloydWarshall(sourceId);
    addPath(pathSolution, graph.getfloydWarshallPath(sourceId, centralId));
}

void SmartWaste::timeComparison() {
    auto elapsedDijkstra = 0;
    auto elapsedFloyd = 0;

    int num_tests = 10;
    cout << "Number of cases to generate: " << endl << ">";
    cin >> num_tests;

    for(int i = 0; i < num_tests; i++) {
        vector<int> fullNodes;
        generateRandomCases(fullNodes);

        auto startDijkstra = std::chrono::system_clock::now();
        auxTimeComparisonDijkstra(fullNodes);
        auto endDijkstra = std::chrono::system_clock::now();
        elapsedDijkstra = elapsedDijkstra + std::chrono::duration_cast<std::chrono::milliseconds>(endDijkstra - startDijkstra).count();

        auto startFloyd = std::chrono::system_clock::now();
        auxTimeComparisonFloydWarshal(fullNodes);
        auto endFloyd = std::chrono::system_clock::now();
        elapsedFloyd = elapsedFloyd + std::chrono::duration_cast<std::chrono::milliseconds>(endFloyd - startFloyd).count();
    }

    double averageDijkstra = elapsedDijkstra/num_tests;
    cout << "Average time Dijkstra: " << averageDijkstra << endl;

    double averageFloyd = elapsedFloyd/num_tests;
    cout << "Average time Floyd Warshall: " << averageFloyd << endl;

    Utils::doSleep(2000);
}

vector<int> SmartWaste::exactSearch(map<string, int> roadsIdMap, string expression) {
    vector<int> viableOptions;

    auto it = roadsIdMap.begin();
    auto ite = roadsIdMap.end();

    while(it != ite) {
        if (Search::kmpStringMatch( (*it).first, expression) != 0) {
            viableOptions.push_back((*it).second);
        }
        it++;
    }

    return viableOptions;
}

vector<int> SmartWaste::approximateSearch(string expression){
    vector<int> results;
    string fileName = "file.txt";

    //iterar arestas ???
    //float dist1 = Search::numApproximateStringMatching(fileName, expression);

    //pintar os resultados no graphviewer guardar em vector os nos e retornar vetor com os nos que ligam essas arestas??
    //fazer display dos nos encontrados e respectivas ruas na consola



    return results;
}

int SmartWaste::chooseNodeToFull(vector<int> results) {
    int choseNode;
    bool found = false;

    do {
        cout << endl << "Node: ";
        cin >> choseNode;

        for(int i = 0; i < results.size(); i++)
            if (results[i] == choseNode)
                found = true;
        if(!found)
            cout << endl << "Node not valid! Try again. ";

    } while(!found);
    
        //pintar os nos possiveis e perguntar qual deles e que quer encher

    return choseNode;
}


void SmartWaste::streetSearch(map<string, int> roadsIdMap) {
    string expression;
    cout << endl << "Expression for search: ";
    cin.ignore();
    getline(cin, expression);
    std::transform(expression.begin(), expression.end(), expression.begin(), ::tolower);

    vector<int> searchResults  = exactSearch(roadsIdMap, expression);
    if(searchResults.size() == 0) {
        cout << "Exact search did not match any results." << endl;
        cout << "Trying the approximate search..." << endl;

        searchResults = approximateSearch(expression);

        if(searchResults.size() == 0)
        {
            cout << "Approximate search did not match any results as well..." << endl;
            return;
        }
    }

    for(int i =0; i < searchResults.size() ; i++)
    {
        cout << searchResults[i] << endl;
        this->graph.getGV()->setEdgeColor(searchResults[i], RED);
        this->graph.getGV()->setEdgeThickness(searchResults[i], 3);
        this->graph.getGV()->rearrange();
        Utils::doSleep(2000);
    }


}