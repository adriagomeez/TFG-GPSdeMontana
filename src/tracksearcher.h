#ifndef TRACKSEARCHER_H
#define TRACKSEARCHER_H
#include <queue>
#include "graph.h"
#include "track.h"
#include <ctime>


struct comparator {
    bool operator()(Vertex* V1, Vertex* V2) { return V1->m_DijkstraDistance > V2->m_DijkstraDistance; }
};

class trackSearcher
{
public:
    trackSearcher(double d, double e);
    Track searchTrack(Graph& graph, list<Vertex*> visits);

private:
    double distance, elevation, bestDistAct, bestElevationAct;
    bool distSearch, elevationSearch;
    stack<Edge*> finalTrack;
    Vertex* last;
    int numSections;
    bool found;
    clock_t timer;


    void marcar(list<Vertex*> &visits);
    priority_queue<Vertex*, vector<Vertex*>, comparator> DijkstraQueueMod(Graph& graph, Vertex* pActual, int n);
    Track TrackGreedy(Graph& graph, list<Vertex*> visits);

    Track TrackBacktracking(Graph &graph, list<Vertex*> visitar);
    void BackTracking(stack<Edge*> &trackAct, Vertex* pActual, Vertex* pAnt, int sectionAct, double distAct, double elevationAct);
    void ordena(Graph* graph, Track t);
    Track camiConstruct(stack<Edge*>& camins);
    Track reconstructComplete(Track simplified);

};

#endif // TRACKSEARCHER_H
