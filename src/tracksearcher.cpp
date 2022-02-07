#include "tracksearcher.h"

trackSearcher::trackSearcher(double d, double e)
{
    found = false;
    if(d == 0.0){
        distance = 0.0;
        distSearch = false;
    }
    else{
        distance = d;
        distSearch = true;
    }

    if(e == 0.0){
        elevation = 0.0;
        elevationSearch = false;
    }
    else{
        elevation = e;
        elevationSearch = true;
    }
}

Track trackSearcher::searchTrack(Graph& graph, list<Vertex*> visits)
{
    if(!distSearch && !elevationSearch){
        return reconstructComplete(TrackGreedy(graph, visits));
    }
    else{
        return TrackBacktracking(graph, visits);
    }
}

// Dijkstra ======================================================================

priority_queue<Vertex*, vector<Vertex*>, comparator> trackSearcher::DijkstraQueueMod(Graph& graph, Vertex* pActual, int n)
{
    priority_queue<Vertex*, vector<Vertex*>, comparator> VOrdenats;
    priority_queue<Vertex*, vector<Vertex*>, comparator> queue;
    int b = 0;
    pActual->m_DijkstraDistance = 0;

    for (Edge* e : pActual->m_Edges) {
        e->m_pDestination->m_DijkstraDistance = pActual->m_DijkstraDistance + e->dist + e->desnivel;
        e->m_pDestination->antecesor = e;
        queue.push(e->m_pDestination);
    }

    while (!queue.empty()) {

        pActual = queue.top();
        if (pActual->salesVisit) {
            VOrdenats.push(pActual);
            pActual->salesVisit = false;
            b++;
            if (b == n)
                break;
        }
        queue.pop();

        for (Edge* e : pActual->m_Edges) {
            if (e->m_pDestination->m_DijkstraDistance > pActual->m_DijkstraDistance + e->dist + e->desnivel) {
                e->m_pDestination->m_DijkstraDistance = pActual->m_DijkstraDistance + e->dist + e->desnivel;
                e->m_pDestination->antecesor = e;
                queue.push(e->m_pDestination);
            }
        }
    }
    for (list<Vertex*> l : graph.m_Vertices){
        for (Vertex* v : l) {
            v->m_DijkstraDistance = 1.79769e+308;
        }
    }
    return VOrdenats;
}


// Greedy ======================================================================

void trackSearcher::marcar(list<Vertex*> &visits) {
    for (Vertex* v : visits)
        v->salesVisit = true;
}

Track trackSearcher::TrackGreedy(Graph& graph, list<Vertex*> visits)
{
    Track cami;

    priority_queue<Vertex*, vector<Vertex*>, comparator> candidatsOrdenats;

    Vertex* actual = visits.front();
    Vertex* last = visits.back();
    visits.pop_back();
    visits.pop_front();

    while (!visits.empty()) {

        marcar(visits);
        candidatsOrdenats = DijkstraQueueMod(graph, actual, visits.size());
        Vertex* aux = candidatsOrdenats.top();
        visits.remove(aux);
        list<Edge*> t;

        while (actual->id != aux->id) {
            t.push_front(aux->antecesor);
            aux = aux->antecesor->m_pOrigin;
        }
        actual = candidatsOrdenats.top();
        cami.m_Edges.splice(cami.m_Edges.end(), t);
    }

    last->salesVisit = true;
    candidatsOrdenats = DijkstraQueueMod(graph, actual, 1);
    Vertex* aux = candidatsOrdenats.top();
    list<Edge*> t;

    while (actual->id != aux->id) {
        t.push_front(aux->antecesor);
        aux = aux->antecesor->m_pOrigin;
    }
    cami.m_Edges.splice(cami.m_Edges.end(), t);

    return cami;
}


// BackTracking ======================================================================

void trackSearcher::BackTracking(stack<Edge*> &trackAct, Vertex* pActual, Vertex* pAnt, int sectionAct, double distAct, double elevationAct) {

    if(found){
        return;
    }

    if((float)(clock() - timer) / CLOCKS_PER_SEC > 30){
        found = true;
        return;
    }

    if(distSearch && elevationSearch){
        int d = sqrt(pow(pActual->x - last->x, 2) + pow(pActual->y - last->y, 2));
        int elev = abs(pActual->elevacion - last->elevacion);
        if(((distAct + d) - distance) + ((elevationAct + elev) - elevation) > abs(distance - bestDistAct) + abs(elevation - bestElevationAct))
            return;
    }
    else{
        if(distSearch){
            int d = sqrt(pow(pActual->x - last->x, 2) + pow(pActual->y - last->y, 2));
            if ((distAct + d) - distance > abs(distance - bestDistAct))
                return;
        }
        else{
            if(elevationSearch){
                int elev = abs(pActual->elevacion - last->elevacion);
                if ((elevationAct + elev) - elevation > abs(elevation - bestElevationAct))
                    return;
            }
        }
    }

    if (pActual->id == last->id && sectionAct + 1 == numSections){
        if(distSearch && elevationSearch){
            if(abs(distance - distAct) + abs(elevation - elevationAct) < abs(distance - bestDistAct) + abs(elevation - bestElevationAct)){
                bestDistAct = distAct;
                bestElevationAct = elevationAct;
                finalTrack = trackAct;
                if(abs(distance - distAct) + abs(elevation - elevationAct) <= 1){
                    found = true;
                }
            }
        }
        else{
            if(distSearch){
                if(abs(distance - distAct) < abs(distance - bestDistAct)){
                    bestDistAct = distAct;
                    finalTrack = trackAct;
                    if(abs(distance - distAct) <= 1){
                        found = true;
                    }
                }
            }
            else{
                if(abs(elevation - elevationAct) < abs(elevation - bestElevationAct)){
                    bestElevationAct = elevationAct;
                    finalTrack = trackAct;
                    if(abs(elevation - elevationAct) <= 1){
                        found = true;
                    }
                }
            }
        }

        return;
    }

    int idAnt = -1;

    if(pAnt != nullptr){
        idAnt = pAnt->id;
    }

    for (Edge* e : pActual->m_Edges) {

        if (!e->stop && e->m_pDestination->id != idAnt) {
            trackAct.push(e);

            bool sVisitsAux = e->m_pDestination->salesVisit;
            int sectionAnt = sectionAct;
            int distanceAux = distAct;
            int elevationAux = elevationAct;
            distAct += e->dist;
            elevationAct += e->desnivel;

            if (e->m_pDestination->salesVisit && e->m_pDestination->id != last->id) {
                sectionAct++;
                e->m_pDestination->salesVisit = false;
            }

            e->stop = true;

            Vertex* ant = nullptr;
            if(e->m_pDestination->m_Edges.size() != 1){
                ant = pActual;
            }

            BackTracking(trackAct, e->m_pDestination, ant, sectionAct, distAct, elevationAct);

            if(found)
                return;

            trackAct.pop();
            e->m_pDestination->salesVisit = sVisitsAux;
            sectionAct = sectionAnt;
            e->stop = false;
            distAct = distanceAux;
            elevationAct = elevationAux;
        }
    }
}

void trackSearcher::ordena(Graph* graph, Track t) {
    int c = 1;
    double f = 0;
    if(distSearch && elevationSearch){
        c = 3;
        f = (distance * t.m_Edges.size()) / t.dist + (elevation * t.m_Edges.size()) / t.desnivel;
    }
    else{
        if(distSearch){
            c = 1;
            f = (distance * t.m_Edges.size()) / t.dist;
        }
        else{
            c = 2;
            f = (elevation * t.m_Edges.size()) / t.desnivel;
        }
    }
    for (list<Vertex*> l : graph->m_Vertices) {
        for(Vertex* v : l){
            v->Ordena(c, f);
        }
    }
}

Track trackSearcher::camiConstruct(stack<Edge*>& camins) {
    Track simplifiedTrack;
    while (!camins.empty()) {
        simplifiedTrack.AddFirst(camins.top());
        camins.pop();
    }

    return reconstructComplete(simplifiedTrack);
}

Track trackSearcher::reconstructComplete(Track simplified)
{
    Track finalTrack;
    for(Edge* e : simplified.m_Edges){
        finalTrack.AddLast(e->completeGraphEdge);
        Edge* act = e->completeGraphEdge;

        while(act->m_pDestination->id != e->m_pDestination->id){
            for(Edge* next : act->m_pDestination->m_Edges){
                if(next->m_pDestination->id != act->m_pOrigin->id){
                    act = next;
                    break;
                }
            }
            finalTrack.AddLast(act);
        }
    }
    return finalTrack;
}

Track trackSearcher::TrackBacktracking(Graph &graph, list<Vertex*> visitar)
{
    qDebug()<<"a";
    Track t = TrackGreedy(graph, visitar);
    t.calcular();
    bestDistAct = t.dist;
    bestElevationAct = t.desnivel;
    marcar(visitar);
    ordena(&graph, t);
    Vertex* first = visitar.front();
    first->salesVisit = false;
    last = visitar.back();
    numSections = visitar.size();
    int sectionAct = 1;
    stack<Edge*> trackAct;
    timer = clock();
    BackTracking(trackAct, first, nullptr, sectionAct, 0.0, 0.0);

    if(t.dist == bestDistAct && t.desnivel == bestElevationAct)
        return reconstructComplete(t);

    return camiConstruct(finalTrack);
}
