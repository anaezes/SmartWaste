//
// Created by ana on 07-04-2017.
//

#ifndef SMARTWASTE_SMARTWASTE_H
#define SMARTWASTE_SMARTWASTE_H

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

#define SMALLGRAPHSIZE  32
#define MIDDLEGRAPHSIZE 156
#define BIGGRAPHSIZE    300



class SmartWaste {
private:
    Graph<int>  graph;
    vector<int> centrals;
    vector<int> garages;

public:

    /**
     * @brief constructor of class
     * @param gv : graph viewer for the display
     * */
    SmartWaste(GraphViewer *gv);


    /**
    * @brief Generate random places for the garages and the centrals
    **/
    void initGaragesAndCentrals();


    /**
    * @brief Get the graph
    * @return the graph
    **/
    Graph<int>* getGraph();


    /**
    * @brief Paint all nodes of the vector
    * @param fullNodes
    * @param color
    **/
    void paintNodes(const vector<int> &fullNodes, const string &color);


    /**
    * @brief Generate random test cases
    * @param fullNodes
    **/
    void generateRandomCases(vector<int> &fullNodes);


    /**
    * @brief Generate random test cases
    * @param fullNodes
    * @param color
    **/
    void setNodesState(const vector<int> &fullNodes, string color);


    /**
    * @brief Generate random test cases
    * @param fullNodesGlass
    * @param fullNodesPaper
    * @param fullNodesPlastic
    **/
    void generateRandomCasesRecycling(vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic);


    /**
    * @brief Return the next garbage colector to visit (closest the current souce)
    * @param fullNodes
    * @return index of the next node
    **/
    int computeNextVertex(vector<int> &fullNodes);

    /**
    * @brief Add computed path so the solution
    * @param pathSolution : path solution already computed
    * @param newPath : new part of the path to add
    **/
    void addPath(vector<int> &pathSolution, const vector<int> &newPath);


    /**
    * @brief Method to reset a node or change its color if it is full of another residue
    * @param currentNode : current node id
    * @param color: current color of this node
    **/
    void resetNode(int currentNode, string color);

    /**
    * @brief Display solution
    * @param pathSolution : vector of path solution
    * @param lastNodeId : Auxiliary id for verify if need reset this node
    * @param truckContains : Truck contents
    * @param colorEdge : Color to paint the edge
    **/
    void displaySolution(vector<int> pathSolution, unsigned int lastNodeId, int truckContains, string colorEdge);

    /**
     * @brief Choose the nearest recycling central with Dikstra algorithm.
     * @return central node id
     * */
    int switchCentralDikstra() const;

    /**
    * @brief Choose the closest garage of trucks depending on the full containers with Dikstra algorithm.
    * @param fullNodes
    * @return garage node id
    * */
    int switchGarageDikstra(const vector<int> &fullNodes);

    /**
    * @brief main of compute solution with Dikstra algorithm
    * @param fullNodes
    * @param color of the edge
    **/
    void computeSolutionDikstra(vector<int> &fullNodes, string colorEdge);

    /**
     * @brief Chooses which algorithm will be used, sending the vectors of the nodes according to the type of waste
     * @param full nodes of paper
     * @param full nodes of glass
     * @param full nodes of plastic
     * */
    void computeSolutionRecycling(vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic);

    /**
    * @brief clear the display
    **/
    void resetDisplay();

    /**
    * @brief clear the display and reset variables for new tests
    * @param fullNodes
    * @param fullNodesPaper
    * @param fullNodesGlass
    * @param fullNodesPlastic
    **/
    void resetGraph(vector<int> &fullNodes, vector<int> &fullNodesPaper, vector<int> &fullNodesGlass, vector<int> &fullNodesPlastic);

    /**
    * @brief Auxiliary method of analyze the connectivity of the graph
    * @param nodes
    * @param source: node id for init the analyze, therefore it is not to paint
    **/
    void paintNodes(vector<int> nodes, int source);

    /**
    * @brief Analyze and displays the connectivity of the graph
    **/
    void verifyConnectivity();

    /**
    * @brief Auxiliary method of analyze time of Dikstra algorithm
    * @param fullNodes
    * */
    void auxTimeComparisonDijkstra(vector<int> fullNodes);

    /**
    * @brief Choose the nearest recycling central with Floyd Warshall algorithm.
    * @param sourceId
    * @return central node id
    * */
    int switchCentralFloydWarshall(int sourceId);

    /**
    * @brief Choose the closest garage of trucks depending on the full containers with Floyd Warshall algorithm.
    * @param fullNodes
    * @return garage node id
    * */
    int switchGarageFloydWarshall(const vector<int> &fullNodes);

    /**
    * @brief Auxiliary method of analyze time of Floyd Warshall  algorithm
    * @param fullNodes
    * */
    void auxTimeComparisonFloydWarshal(vector<int> fullNodes);

    /**
     * @brief Method to compare execution times of the Floyd Warshall and the Dikstra algorithm
     * */
    void timeComparison();

    vector<int> exactSearch(map<string, int> roadsIdMap, string expression);

    vector<int> approximateSearch(string expression);

    int chooseNodeToFull(int idEdge);

    void streetSearch(map<string, int> roadsIdMap, vector<int> &fullNodes);

    int chooseStreet(vector<int> searchResults);

};


#endif //SMARTWASTE_SMARTWASTE_H
