/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>

#include "Utils.h"
#include "graphviewer.h"

template <class T> class Edge;
template <class T> class Graph;

using namespace std;

static const int X_RES = 2000;
static const int Y_RES = 1000;

template <class T>
class Vertex {
    T info;
    vector<Edge<T>  > adj;
    bool visited;
    void addEdge(Vertex<T> *dest, double w);
    bool removeEdgeTo(Vertex<T> *d);
public:
    Vertex(T in);
    friend class Graph<T>;
};

template <class T>
class Edge {
    Vertex<T> * dest;
    double weight;
public:
    Edge(Vertex<T> *d, double w);
    friend class Graph<T>;
    friend class Vertex<T>;
};


template <class T>
class Graph {
    vector<Vertex<T> *> vertexSet;
    void dfs(Vertex<T> *v, vector<T> &res) const;
    GraphViewer *gv;
    bool addEdge(const T &sourc, const T &dest, double w);
    long double latitudeMin;
    long double longitudeMin;
    long double latitudeMax;
    long double longitudeMax;
public:
    Graph(GraphViewer*);
    bool addVertex(const T &in, std::pair< double, double> coords);
    bool addEdge(const T &edgeId, const T &sourc, const T &dest, double w, bool isUndirected);
    bool removeVertex(const T &in);
    bool removeEdge(const T &sourc, const T &dest);
    vector<T> dfs() const;
    vector<T> bfs(Vertex<T> *v) const;
    int maxNewChildren(Vertex<T> *v, T &inf) const;
    vector<Vertex<T> * > getVertexSet() const;
    int getNumVertex() const;
    void setMinCoords(const long double &lat, const long double &lon);
    void setMaxCoords(const long double &lat, const long double &lon);
};

template <class T>
void Graph<T>::setMinCoords(const long double &lat, const long double &lon) {
    this->latitudeMin = lat;
    this->longitudeMin = lon;
}

template <class T>
void Graph<T>::setMaxCoords(const long double &lat, const long double &lon){
    this->latitudeMax = lat;
    this->longitudeMax = lon;
}

template <class T>
Graph<T>::Graph(GraphViewer* gv) :
        gv(gv)
{}

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
    return vertexSet;
}

template <class T>
bool Graph<T>::addVertex(const T &in, std::pair<double, double> coords) {
    typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
    typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
    for (; it!=ite; it++)
        if ((*it)->info == in) return false;
    Vertex<T> *v1 = new Vertex<T>(in);
    vertexSet.push_back(v1);

    int x = Utils::getScreenXCoord(coords.second, this->longitudeMin, this->longitudeMax,X_RES);
    int y = Utils::getScreenYCoord(coords.first, this->latitudeMin, this->latitudeMax, Y_RES);
    gv->addNode(in, x, y);
    return true;
}


template <class T>
bool Graph<T>::removeVertex(const T &in) {
    typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
    typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
    for (; it!=ite; it++) {
        if ((*it)->info == in) {
            Vertex<T> * v= *it;
            vertexSet.erase(it);
            typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
            typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
            for (; it1!=it1e; it1++) {
                (*it1)->removeEdgeTo(v);
            }
            delete v;
            return true;
        }
    }
    return false;
}

template <class T>
bool Graph<T>::addEdge(const T &edgeId, const T &sourc, const T &dest, double w, bool isUndirected)
{
    addEdge(sourc, dest,  w);
    if(isUndirected) {
        addEdge(dest, sourc, w);
        gv->addEdge(edgeId, sourc, dest,EdgeType::UNDIRECTED);

        //para diferenciar
        gv->setVertexColor(sourc, RED);
        gv->setVertexColor(dest, RED);
    }
    else {
        gv->addEdge(edgeId, sourc, dest, EdgeType::DIRECTED);
        gv->setVertexColor(sourc, "green");
    }
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
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
    vS->addEdge(vD,w);

    return true;
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
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
    return vS->removeEdgeTo(vD);
}

template <class T>
vector<T> Graph<T>::dfs() const {
    typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
    typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
    for (; it !=ite; it++)
        (*it)->visited=false;
    vector<T> res;
    it=vertexSet.begin();
    for (; it !=ite; it++)
        if ( (*it)->visited==false )
            dfs(*it,res);
    return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v,vector<T> &res) const {
    v->visited = true;
    res.push_back(v->info);
    typename vector<Edge<T> >::iterator it= (v->adj).begin();
    typename vector<Edge<T> >::iterator ite= (v->adj).end();
    for (; it !=ite; it++)
        if ( it->dest->visited == false )
            dfs(it->dest, res);
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
    return res;
}

template <class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const {
    vector<T> res;
    queue<Vertex<T> *> q;
    queue<int> level;
    int maxChildren=0;
    inf =v->info;
    q.push(v);
    level.push(0);
    v->visited = true;
    while (!q.empty()) {
        Vertex<T> *v1 = q.front();
        q.pop();
        res.push_back(v1->info);
        int l=level.front();
        level.pop(); l++;
        int nChildren=0;
        typename vector<Edge<T> >::iterator it=v1->adj.begin();
        typename vector<Edge<T> >::iterator ite=v1->adj.end();
        for (; it!=ite; it++) {
            Vertex<T> *d = it->dest;
            if (d->visited==false) {
                d->visited=true;
                q.push(d);
                level.push(l);
                nChildren++;
            }
        }
        if (nChildren>maxChildren) {
            maxChildren=nChildren;
            inf = v1->info;
        }
    }
    return maxChildren;
}


template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
    typename vector<Edge<T> >::iterator it= adj.begin();
    typename vector<Edge<T> >::iterator ite= adj.end();
    while (it!=ite) {
        if (it->dest == d) {
            adj.erase(it);
            return true;
        }
        else it++;
    }
    return false;
}

template <class T>
Vertex<T>::Vertex(T in):info(in), visited(false) {}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
    Edge<T> edgeD(dest,w);
    adj.push_back(edgeD);
}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w){}

#endif /* GRAPH_H_ */
