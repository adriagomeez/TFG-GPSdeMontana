#ifndef TRACK_H
#define TRACK_H

#include "graph.h"

class Track {
public:
    double dist, desnivel;
    list<Edge*> m_Edges;


    Track() { dist = 0.0; desnivel = 0.0; }

    void AddFirst(Edge* pEdge) {
        m_Edges.push_front(pEdge);
    }
    void AddLast(Edge* pEdge) {
        m_Edges.push_back(pEdge);
    }
    void Clear() {
        m_Edges.clear();
        dist = 0.0;
        desnivel = 0.0;
    }
    void calcular();
};

#endif // TRACK_H
