/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <climits>
#include <cmath>

#include "Utils.h"
#include "graphviewer.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;

const int INT_INFINITY = INT_MAX;

#define TRUCK_CAPACITY 1000
#define CONTAINER_CAPACITY 200


/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
    T info;
    vector<Edge<T>  > adj;
    bool visited;
    bool processing;
    int indegree;
    double dist;
    bool paperState;
    bool glassState;
    bool plasticState;
public:
    Vertex(T in);
    friend class Graph<T>;
    Edge<T> addEdge(T infoEdge, const string &name, Vertex<T> *dest, double w);
    T getInfo() const;
    int getDist() const;
    bool operator<(const Vertex<T> vertex);
    Vertex* path;
    vector<Edge<T>  > getEdges();
    bool isPaperFull() const;
    bool isGlassFull() const;
    bool isPlasticFull() const;
    void setPaperFull(bool b);
    void setGlassFull(bool b);
    void setPlasticFull(bool b);
};


template <class T>
struct vertex_greater_than {
    bool operator()(Vertex<T> * a, Vertex<T> * b) const {
        return a->getDist() > b->getDist();
    }
};


template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false), processing(false), indegree(0), dist(0) {
    path = NULL;
    paperState = false;
    glassState = false;
    plasticState = false;
}


template <class T>
Edge<T> Vertex<T>::addEdge(T info, const string &roadName, Vertex<T> *dest, double w) {
    Edge<T> edgeD(info, roadName, dest, w);
    adj.push_back(edgeD);
    return edgeD;
}


template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
int Vertex<T>::getDist() const {
    return this->dist;
}


template <class T>
bool Vertex<T>::isPaperFull() const {
    return paperState;
}

template <class T>
bool Vertex<T>::isGlassFull() const {
    return glassState;
}

template <class T>
bool Vertex<T>::isPlasticFull() const{
    return plasticState;
}

template <class T>
void Vertex<T>::setPaperFull(bool b) {
    paperState = b;
}

template <class T>
void Vertex<T>::setGlassFull(bool b) {
    glassState = b;
}

template <class T>
void Vertex<T>::setPlasticFull(bool b) {
    plasticState = b;
}



/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
    Vertex<T> * dest;
    double weight;
    T info;
    string roadName;
public:
    Edge(T i, const string &roadName, Vertex<T> *d, double w);
    T getInfo();
    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(T i, const string &roadName, Vertex<T> *d, double w) {

    this->dest= dest;
    this->weight = weight;
    this->roadName = roadName;
    this->info= info;

}

template <class T>
T Edge<T>::getInfo(){
    return info;
}


/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
    GraphViewer *gv;
    vector<Vertex<T> *> vertexSet;
    vector<Edge<T>> edges;
    int ** W;
    int ** P;
    bool addEdge(const T &edgeId, const string &roadName, const T &sourc, const T &dest, double w);
public:
    Graph(GraphViewer*);
    bool addVertex(const T &in, std::pair< double, double> coords);
    void addEdge(const T &edgeId, const string &roadName, const T &sourc, const T &dest, int w, bool isUndirected);
    vector<T> bfs(Vertex<T> *v) const;
    int getNumVertex() const;
    Vertex<T>* getVertex(const T &v) const;
    vector<T> getPath(const T &origin, const T &dest);
    void dijkstraShortestPath(const T &s) const;
    void floydWarshallShortestPath();
    int edgeCost(int vOrigIndex, int vDestIndex);
    vector<T> getfloydWarshallPath(const T &origin, const T &dest);
    void getfloydWarshallPathAux(int index1, int index2, vector<T> & res);
    int getWeightFloydWarshall(int index1, int index2);
    GraphViewer* getGV() const;
    int getEdge(const T &source, const T &dest);
    vector<Edge<T>> getEdges() const;
    void resetVisited() const;
};

template <class T>
int Graph<T>::getWeightFloydWarshall(int index1, int index2) {
    return W[index1][index2];
}

template <class T>
vector<Edge<T>> Graph<T>::getEdges() const {
    return edges;
}

template <class T>
int Graph<T>::getEdge(const T &source, const T &dest) {
    Vertex<T>* tmp = getVertex(source);

    typename vector<Edge<T> >::iterator it=  tmp->adj.begin();
    typename vector<Edge<T> >::iterator ite= tmp->adj.end();

    while(it!=ite) {
        if((*it).dest->getInfo() == dest)
            return (*it).getInfo();
        it++;
    }
return -1;
}

template <class T>
GraphViewer* Graph<T>::getGV() const {
    return gv;
}

template <class T>
Graph<T>::Graph(GraphViewer* gv) :
        gv(gv), edges()
{}

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}


template <class T>
bool Graph<T>::addVertex(const T &in, std::pair<double, double> coords) {
    typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
    typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
    for (; it!=ite; it++)
        if ((*it)->info == in) return false;
    Vertex<T> *v1 = new Vertex<T>(in);
    vertexSet.push_back(v1);

    gv->addNode(in, coords.second, coords.first);
    //gv->setVertexLabel(in, ".");
    return true;
}


template <class T>
void Graph<T>::addEdge(const T &edgeId, const string &roadName, const T &sourc, const T &dest, int w, bool isUndirected) {

    addEdge(edgeId, roadName, sourc, dest,  w);
    gv->setEdgeLabel(edgeId, roadName);

    if(isUndirected) {
        addEdge(edgeId, roadName, dest, sourc, w);
        gv->addEdge(edgeId, sourc, dest, EdgeType::UNDIRECTED);
        //gv->setEdgeLabel(edgeId, to_string(w));
    }
    else {
        gv->addEdge(edgeId, sourc, dest, EdgeType::DIRECTED);
        //gv->setEdgeLabel(edgeId, to_string(w));
    }
}

template <class T>
bool Graph<T>::addEdge(const T &edgeId, const string &roadName, const T &sourc, const T &dest, double w) {
    typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
    typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
    int found=0;
    Vertex<T> *vS, *vD;
    while (found!=2 && it!=ite ) {
        if ( (*it)->info == sourc )
        { vS=*it; found++;}
        if ( (*it)->info == dest )
        { vD=*it; found++;}
        it ++;
    }
    if (found!=2) return false;
    vD->indegree++;
    Edge<T> aux = vS->addEdge(edgeId, roadName, vD, w);
    this->edges.push_back(aux);

    return true;
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
    vector<T> res;
    queue<Vertex<T> *> q;
    q.push(v);
    v->visited = true;
    while (!q.empty()) {
        Vertex<T> *v1 = q.front();
        q.pop();
        res.push_back(v1->info);
        typename vector<Edge<T> >::iterator it=v1->adj.begin();
        typename vector<Edge<T> >::iterator ite=v1->adj.end();
        for (; it!=ite; it++) {
            Vertex<T> *d = it->dest;
            if (d->visited==false) {
                d->visited=true;
                q.push(d);
            }
        }
    }
    resetVisited();
    return res;
}


template <class T>
Vertex<T>* Graph<T>::getVertex(const T &v) const {
    for(unsigned int i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->info == v) return vertexSet[i];
    return NULL;
}


template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest){
    list<T> buffer;
    Vertex<T>* v = getVertex(dest);

    buffer.push_front(v->info);
    while ( v->path != NULL &&  v->path->info != origin) {
        v = v->path;
        buffer.push_front(v->info);
    }
    if( v->path != NULL )
        buffer.push_front(v->path->info);


    vector<T> res;
    while( !buffer.empty() ) {
        res.push_back( buffer.front() );
        buffer.pop_front();
    }
    return res;
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &origin, const T &dest){
    int originIndex = -1, destinationIndex = -1;
    for(unsigned int i = 0; i < vertexSet.size(); i++)
    {
        if(vertexSet[i]->info == origin)
            originIndex = i;
        if(vertexSet[i]->info == dest)
            destinationIndex = i;
        if(originIndex != -1 && destinationIndex != -1)
            break;
    }

    vector<T> res;
    if(W[originIndex][destinationIndex] == INT_INFINITY)
        return res;
    res.push_back(vertexSet[originIndex]->info);
    if(P[originIndex][destinationIndex] != -1)
    {
        int intermedIndex = P[originIndex][destinationIndex];
        getfloydWarshallPathAux(originIndex, intermedIndex, res);
        res.push_back(vertexSet[intermedIndex]->info);
        getfloydWarshallPathAux(intermedIndex,destinationIndex, res);
    }

    res.push_back(vertexSet[destinationIndex]->info);

    return res;
}



template<class T>
void Graph<T>::getfloydWarshallPathAux(int index1, int index2, vector<T> & res) {
    if(P[index1][index2] != -1) {
        getfloydWarshallPathAux(index1, P[index1][index2], res);
        res.push_back(vertexSet[P[index1][index2]]->info);
        getfloydWarshallPathAux(P[index1][index2],index2, res);
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &s) const {
    for(unsigned int i = 0; i < vertexSet.size(); i++) {
        vertexSet[i]->path = NULL;
        vertexSet[i]->dist = INT_INFINITY;
        vertexSet[i]->processing = false;
    }

    Vertex<T>* v = getVertex(s);
    v->dist = 0;
    vector< Vertex<T>* > pq;
    pq.push_back(v);
    make_heap(pq.begin(), pq.end());


    while( !pq.empty() ) {
        v = pq.front();
        pop_heap(pq.begin(), pq.end());
        pq.pop_back();

        for(unsigned int i = 0; i < v->adj.size(); i++) {
            Vertex<T>* w = v->adj[i].dest;
            if(v->dist + v->adj[i].weight < w->dist ) {
                w->dist = v->dist + v->adj[i].weight;
                w->path = v;

                if(!w->processing) {
                    w->processing = true;
                    pq.push_back(w);
                }

                make_heap (pq.begin(),pq.end(),vertex_greater_than<T>());
            }
        }
    }
}

template<class T>
int Graph<T>::edgeCost(int vOrigIndex, int vDestIndex) {
    if(vertexSet[vOrigIndex] == vertexSet[vDestIndex])
        return 0;

    for(unsigned int i = 0; i < vertexSet[vOrigIndex]->adj.size(); i++)
    {
        if(vertexSet[vOrigIndex]->adj[i].dest == vertexSet[vDestIndex])
            return vertexSet[vOrigIndex]->adj[i].weight;
    }

    return INT_INFINITY;
}


template<class T>
void Graph<T>::floydWarshallShortestPath() {
    W = new int * [vertexSet.size()];
    P = new int * [vertexSet.size()];
    for(unsigned int i = 0; i < vertexSet.size(); i++)
    {
        W[i] = new int[vertexSet.size()];
        P[i] = new int[vertexSet.size()];
        for(unsigned int j = 0; j < vertexSet.size(); j++)
        {
            W[i][j] = edgeCost(i,j);
            P[i][j] = -1;
        }
    }

    for(unsigned int k = 0; k < vertexSet.size(); k++)
        for(unsigned int i = 0; i < vertexSet.size(); i++)
            for(unsigned int j = 0; j < vertexSet.size(); j++)
            {
                if(W[i][k] == INT_INFINITY || W[k][j] == INT_INFINITY)
                    continue;

                int val = min ( W[i][j], W[i][k]+W[k][j] );
                if(val != W[i][j])
                {
                    W[i][j] = val;
                    P[i][j] = k;
                }
            }

}
template<class T>
void Graph<T>::resetVisited() const{
    for(unsigned int i = 0; i < vertexSet.size(); i++) {
        vertexSet[i]->visited = false;
    }
}

#endif /* GRAPH_H_ */
