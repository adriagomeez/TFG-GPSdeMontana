#ifndef VERTEX_H
#define VERTEX_H
#include <string>
#include "edge.h"
#include <list>

using namespace std;

class Edge;
class Vertex {
public:
    int id;
    double lat, lon, x, y;
    list<Edge*> m_Edges;
    double elevacion;
    double m_DijkstraDistance;
    Edge* antecesor;
    bool salesVisit, saveGraphVisit;
    double sortFactor;

    Vertex(){}
    ~Vertex(){m_Edges.clear();}
    void Ordena(int c, double f);
    Vertex(const int identificador, double X, double Y, double latitude, double longitude, double ele)
        : id(identificador)
        , lat(latitude)
        , lon(longitude)
        , x(X)
        , y(Y)
        , elevacion(ele)
        , m_DijkstraDistance(1.79769e+308)
        , salesVisit(false)
        , saveGraphVisit(false){}
};

#endif // VERTEX_H
