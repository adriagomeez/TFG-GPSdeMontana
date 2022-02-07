#include "edge.h"



Edge::Edge(Vertex *pOrigin, Vertex *pDestination)
{
    m_pOrigin = pOrigin;
    m_pDestination = pDestination;
    dist = sqrt(pow(m_pOrigin->x - m_pDestination->x, 2)+pow(m_pOrigin->y - m_pDestination->y, 2));
    desnivel = abs(m_pOrigin->elevacion - m_pDestination->elevacion);
    stop = false;
    completeGraphEdge = nullptr;
}

Edge::Edge(Vertex *pOrigin, Vertex *pDestination, double distance, double elevation, Edge* e)
{
    m_pOrigin = pOrigin;
    m_pDestination = pDestination;
    dist = distance;
    desnivel = elevation;
    stop = false;
    completeGraphEdge = e;
}
