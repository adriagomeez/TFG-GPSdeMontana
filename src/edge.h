#ifndef EDGE_H
#define EDGE_H
#include "vertex.h"
#include "math.h"

class Vertex;

class Edge {
public:

    Vertex* m_pOrigin;
    Vertex* m_pDestination;
    double dist, desnivel;
    bool stop;
    Edge* completeGraphEdge;
    Edge* reverseEdge;

public:
    Edge(Vertex* pOrigin, Vertex* pDestination);
    Edge(Vertex* pOrigin, Vertex* pDestination, double distance, double elevation, Edge* e);

};

#endif // EDGE_H
