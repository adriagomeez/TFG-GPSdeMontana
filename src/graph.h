#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include "edge.h"
#include <list>
#include "vertex.h"
#include <QtXml>
#include <QDebug>
#include <QFile>
#include <vector>
#include <stack>
#include "map.h"

bool exist(list<Vertex*> l, Vertex* n);

class Graph{

public:
    vector<list<Vertex*>> m_Vertices;
    list<Edge> m_Edges;
    int numNodos;
    const float R = 6378.0F, tram_Dif = 0.011, point_dif = 11;
    const int divCuadrante = 10, pathLength = 8;

public:
    Graph();
    ~Graph();
    void Clear();
//  void ClearDistances();

    // Vertices
    Vertex* NewVertex(const int name, double x, double y, double lat, double lon, double ele, int cuad);
    Vertex* SearchById(const int id);
    void DeleteSection(list<pair<Vertex*,int>> section);

    // Edges
    void NewEdge(Vertex *pOrigin, Vertex* pDestination);
    void NewEdge(Vertex *pOrigin, Vertex* pDestination, double dist, double elevation, Edge* e, Edge* eReverse);

    // Functions
    void anotherPath(Vertex* actual,  Vertex* anterior, Vertex* destino, int step, int &paths, list<Vertex*> &track, stack<list<Vertex*>> &tracks);
    bool duplicatedPath(list<pair<Vertex*,int>> section, stack<list<Vertex*>> &tracks);
    void read(const QDomNode &node);
    void load(list<QString> paths);
    void duplicatedEdge(Vertex* ant, Vertex* dupl);
    double dist(double lat1, double lon1, double lat2, double lon2);
    Vertex* duplicado(const double x, const double y, bool& c, int cuad);
    int cuadrantesCercanos(double x, double y, int cuad);
    int cuadrante(const double lat, const double lon);
    Vertex* buscar(double x, double y, bool&c, int cuad, double& distance);
    Vertex* isInPath(const int &x,const  int &y, bool& isIn);
};

#endif // GRAPH_H
