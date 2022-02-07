#include "track.h"

void Track::calcular()
{
    dist = 0.0;
    desnivel = 0.0;
    for(Edge* e : m_Edges){
        dist += e->dist;
        desnivel += e->desnivel;
    }
}
