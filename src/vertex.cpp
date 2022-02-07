#include "vertex.h"

bool compareDistance(Edge* E1, Edge* E2) {
    if (abs(E1->dist - E1->m_pOrigin->sortFactor) < abs(E2->dist - E2->m_pOrigin->sortFactor))
        return true;
    else
        return false;
}

bool compareElevation(Edge* E1, Edge* E2) {
    if (abs(E1->desnivel - E1->m_pOrigin->sortFactor) < abs(E2->desnivel - E2->m_pOrigin->sortFactor))
        return true;
    else
        return false;
}

bool compareAll(Edge* E1, Edge* E2) {
    if (abs(E1->dist + E1->desnivel - E1->m_pOrigin->sortFactor) < abs(E2->dist + E1->desnivel - E2->m_pOrigin->sortFactor))
        return true;
    else
        return false;
}

void Vertex::Ordena(int c, double f)
{
    sortFactor = f;
    switch(c){
        case 1:
            m_Edges.sort(compareDistance);
            break;
        case 2:
            m_Edges.sort(compareElevation);
            break;
        case 3:
            m_Edges.sort(compareAll);
            break;
    }


}


