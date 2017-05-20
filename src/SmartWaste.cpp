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
    cout << "Average time Dijkstra: " << averageDijkstra << " ms" << endl;

    double averageFloyd = elapsedFloyd/num_tests;
    cout << "Average time Floyd Warshall: " << averageFloyd << " ms" <<endl;

    Utils::doSleep(2000);
}

vector<int> SmartWaste::exactSearchKmp(map<string, int> roadsIdMap, string expression) {
    vector<int> viableOptions;

    auto it = roadsIdMap.begin();
    auto ite = roadsIdMap.end();

    while(it != ite) {
        if (Search::kmpStringMatch( (*it).first, expression) != 0)
            viableOptions.push_back((*it).second);
        it++;
    }

    return viableOptions;
}

vector<int> SmartWaste::exactSearchNaive(map<string, int> roadsIdMap, string expression) {
    vector<int> viableOptions;

    auto it = roadsIdMap.begin();
    auto ite = roadsIdMap.end();

    while(it != ite) {
        if (Search::naiveStringMatch( (*it).first, expression) != 0)
            viableOptions.push_back((*it).second);
        it++;
    }

    return viableOptions;
}


vector<int> SmartWaste::approximateSearch(map<string, int> roadsIdMap, string expression){
    multimap<float, int> tmp;
    vector<int> viableOptions;

    auto it = roadsIdMap.begin();
    auto ite = roadsIdMap.end();

    while(it != ite) {
        float searchNew = Search::numApproximateStringMatching((*it).first, expression);
            tmp.insert(pair<float,int>(searchNew, (*it).second));
        it++;
    }
    std::map<float,int>::iterator it2 = tmp.begin();
    int i = 0;
    while (it2 != tmp.end() && i < 4)
    {
        viableOptions.push_back((*it2).second);
        it2++;
        i++;
    }

    return viableOptions;
}

int SmartWaste::chooseNodeToFull(int idEdge) {
    int nNodes = 0;
    int source = graph.getEdge(idEdge)->getSource()->getInfo();
    int dest = this->graph.getEdge(idEdge)->getDest()->getInfo();

    cout << "Available containers:" << endl;
    if(std::find(garages.begin(), garages.end(), source) == garages.end() &&
       (std::find(centrals.begin(), centrals.end(), source) == centrals.end())) {
        cout << " - " << source << endl;
        nNodes++;
    }
    if(std::find(garages.begin(), garages.end(), dest) == garages.end() &&
       (std::find(centrals.begin(), centrals.end(), dest) == centrals.end())) {
        cout << " - " << dest << endl;
        nNodes++;
    }

    if(nNodes == 0) {
        cout << "These containers can not be filled,  try new search..." << endl;
        resetEdgeStreet(idEdge);
        this->graph.getGV()->rearrange();
        return -1;
    }

    cout << "Choose node: " << endl << "> ";
    int option;
    cin >> option;

    while((nNodes == 2 && option != source && option != dest)
          || (nNodes == 1 && (option != source && option != dest))){
        cout << option << endl;
        cout << "Node not valid, try again..." << endl << "> ";
        cin >> option;
    }

    cout << "Success!" << endl;

    resetEdgeStreet(idEdge);
    this->graph.getGV()->setVertexColor(option, RED);
    this->graph.getGV()->rearrange();

    Utils::doSleep(1000);

    return option;
}


void SmartWaste::streetSearch(map<string, int> roadsIdMap, vector<int> &fullNodes) {
    string expression;
    cout << endl << "Expression for search: ";
    cin.ignore();
    getline(cin, expression);
    std::transform(expression.begin(), expression.end(), expression.begin(), ::tolower);

    vector<int> searchResults  = exactSearchKmp(roadsIdMap, expression);
    if(searchResults.size() == 0) {
        cout << "Exact search did not match any results." << endl;
        cout << "Trying the approximate search..." << endl;

        searchResults = approximateSearch(roadsIdMap, expression);

        if(searchResults.size() == 0)
        {
            cout << "Approximate search did not match any results as well..." << endl;
            return;
        }
    }

    int street = chooseStreet(searchResults);

    cout << "Do you want to choose any container to fill? (y/n)  " << endl << "> ";
    char option;
    cin >> option;

    while(!isalpha(option)){
        cout << "Option not valid, try again..." << endl << "> ";
        cin >> option;
    }

    if(option == 'y' || option == 'Y') {
        int nodeId = chooseNodeToFull(street);
        if(nodeId != -1)
            fullNodes.push_back(nodeId);
    }
}

int SmartWaste::chooseStreet(vector<int> searchResults) {
    cout << "Results: " << endl;
    for(size_t i =0; i < searchResults.size() ; i++)
        cout << i+1 << " - " << this->graph.getEdge(searchResults[i])->getRoadName() << endl;

    if(searchResults.size() == 1) {
        colorStreet(searchResults[0]);
        return searchResults[0];
    }

    unsigned int option;
    cout << endl << "Which street you want to choose ? " << endl;
    cout << "> ";
    cin >> option;
    cout << option << endl;
    while(option <= 0 || option > searchResults.size())
    {
        cout << "Option not valid, try again..." << endl << "> ";
        cin >> option;
    }

    colorStreet(searchResults[option-1]);

    return searchResults[option-1];
}

void SmartWaste::colorStreet(int edgeId) {
    this->graph.getGV()->setEdgeColor(edgeId, RED);
    this->graph.getGV()->setEdgeThickness(edgeId, 5);
    this->graph.getGV()->rearrange();
}

void SmartWaste::resetEdgeStreet(int idEdge) {
    this->graph.getGV()->clearEdgeColor(idEdge);
    this->graph.getGV()->setEdgeThickness(idEdge, 1);
    this->graph.getGV()->rearrange();
}

void SmartWaste::timeComparisonExactSearch(map<string, int> roadsIdMap) {

    cout << "Average time (100 tests / 3 sizes of strings): " << endl << endl;
    cout << " -> Knuth–Morris–Pratt" << endl;
    timeComparisonKMPStringsSizes(roadsIdMap);
    cout << " -> Naive" << endl;
    timeComparisonNaiveStringsSizes(roadsIdMap);

    Utils::doSleep(500);


    cout << "Average time (100 tests / 3 sizes of files): " << endl << endl;
    cout << " -> Knuth–Morris–Pratt" << endl;
    timeComparisonKMPFilesSizes();
    cout << " -> Naive" << endl;
    timeComparisonNaiveFilesSizes();

    Utils::doSleep(2000);
}

void SmartWaste::timeComparisonKMPStringsSizes(map<string, int> roadsIdMap) {
    int nTests = 100;
    string smallExpression = "de";
    string mediumExpression = "cedofeita";
    string bigExpression = "rua de costa cabral";

    // small
    auto elapsedKmp = 0;
    auto startKmp = std::chrono::system_clock::now();
    for(int i = 0; i < nTests; i++)
        exactSearchKmp(roadsIdMap, smallExpression);
    auto endKmp = std::chrono::system_clock::now();
    elapsedKmp = std::chrono::duration_cast<std::chrono::microseconds>(endKmp - startKmp).count();
    cout << "       Small expression : " << elapsedKmp/nTests << " us"<< endl;

    //medium
    startKmp = std::chrono::system_clock::now();
    for(int i = 0; i < nTests; i++)
        exactSearchKmp(roadsIdMap, mediumExpression);
    endKmp = std::chrono::system_clock::now();
    elapsedKmp = std::chrono::duration_cast<std::chrono::microseconds>(endKmp - startKmp).count();
    cout << "       Medium expression: " << elapsedKmp/nTests << " us"<< endl;

    // big
    startKmp = std::chrono::system_clock::now();
    for(int i = 0; i < nTests; i++)
        exactSearchKmp(roadsIdMap, bigExpression);
    endKmp = std::chrono::system_clock::now();
    elapsedKmp = std::chrono::duration_cast<std::chrono::microseconds>(endKmp - startKmp).count();
    cout << "       Big expression: " << elapsedKmp/nTests << " us"<< endl << endl;
}



void SmartWaste::timeComparisonNaiveStringsSizes(map<string, int> roadsIdMap) {
    int nTests = 100;
    string smallExpression = "de";
    string mediumExpression = "cedofeita";
    string bigExpression = "rua de costa cabral";

    //small
    auto     elapsedNaive = 0;
    auto     startNaive   = std::chrono::system_clock::now();
    for (int i = 0; i < 100; i++)
        exactSearchNaive(roadsIdMap, smallExpression);
    auto     endNaive     = std::chrono::system_clock::now();
    elapsedNaive = std::chrono::duration_cast<std::chrono::microseconds>(endNaive - startNaive).count();
    cout << "       Small expression: " << elapsedNaive / nTests << " us" << endl;

    //medium
    startNaive   = std::chrono::system_clock::now();
    for (int i = 0; i < 100; i++)
        exactSearchNaive(roadsIdMap, mediumExpression);
    endNaive     = std::chrono::system_clock::now();
    elapsedNaive = std::chrono::duration_cast<std::chrono::microseconds>(endNaive - startNaive).count();
    cout << "       Medium expression: " << elapsedNaive / nTests << " us" << endl;

    //big
    startNaive   = std::chrono::system_clock::now();
    for (int i = 0; i < 100; i++)
        exactSearchNaive(roadsIdMap, bigExpression);
    endNaive     = std::chrono::system_clock::now();
    elapsedNaive = std::chrono::duration_cast<std::chrono::microseconds>(endNaive - startNaive).count();
    cout << "       Big expression: " << elapsedNaive / nTests << " us" << endl << endl;
}

void SmartWaste::timeComparisonKMPFilesSizes(){
    int nTests = 100;
    string smallFile = "./data/smallFile.txt";
    string mediumFile = "./data/mediumFile.txt";
    string bigFile = "./data/bigFile.txt";
    string expression = "elementum";

    // small
    auto elapsedKmp = 0;
    auto startKmp = std::chrono::system_clock::now();
    for(int i = 0; i < nTests; i++)
        Search::numStringMatchingKmp(smallFile, expression);
    auto endKmp = std::chrono::system_clock::now();
    elapsedKmp = std::chrono::duration_cast<std::chrono::microseconds>(endKmp - startKmp).count();
    cout << "       Small file : " << elapsedKmp/nTests << " us"<< endl;

    //medium
    elapsedKmp = 0;
    startKmp = std::chrono::system_clock::now();
    for(int i = 0; i < nTests; i++)
        Search::numStringMatchingKmp(mediumFile, expression);
    endKmp = std::chrono::system_clock::now();
    elapsedKmp = std::chrono::duration_cast<std::chrono::microseconds>(endKmp - startKmp).count();
    cout << "       Medium file: " << elapsedKmp/nTests << " us"<< endl;

    // big
    elapsedKmp = 0;
    startKmp = std::chrono::system_clock::now();
    for(int i = 0; i < nTests; i++)
        Search::numStringMatchingKmp(bigFile, expression);
    endKmp = std::chrono::system_clock::now();
    elapsedKmp = std::chrono::duration_cast<std::chrono::microseconds>(endKmp - startKmp).count();
    cout << "       Big file: " << elapsedKmp/nTests << " us"<< endl << endl;
}
void SmartWaste::timeComparisonNaiveFilesSizes(){
    int nTests = 100;
    string smallFile = "./data/smallFile.txt";
    string mediumFile = "./data/mediumFile.txt";
    string bigFile = "./data/bigFile.txt";
    string expression = "elementum";

    //small
    auto     elapsedNaive = 0;
    auto     startNaive   = std::chrono::system_clock::now();
    for (int i = 0; i < 100; i++)
        Search::numStringMatchingNaive(smallFile, expression);
    auto     endNaive     = std::chrono::system_clock::now();
    elapsedNaive = std::chrono::duration_cast<std::chrono::microseconds>(endNaive - startNaive).count();
    cout << "       Small file: " << elapsedNaive / nTests << " us" << endl;

    //medium
    startNaive   = std::chrono::system_clock::now();
    for (int i = 0; i < 100; i++)
        Search::numStringMatchingNaive(mediumFile, expression);
    endNaive     = std::chrono::system_clock::now();
    elapsedNaive = std::chrono::duration_cast<std::chrono::microseconds>(endNaive - startNaive).count();
    cout << "       Medium file: " << elapsedNaive / nTests << " us" << endl;

    //big
    startNaive   = std::chrono::system_clock::now();
    for (int i = 0; i < 100; i++)
        Search::numStringMatchingNaive(bigFile, expression);
    endNaive     = std::chrono::system_clock::now();
    elapsedNaive = std::chrono::duration_cast<std::chrono::microseconds>(endNaive - startNaive).count();
    cout << "       Big file: " << elapsedNaive / nTests << " us" << endl;
}