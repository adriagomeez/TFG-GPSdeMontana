#include "graph_object.h"

Graph_Object::Graph_Object(QObject *parent) : QThread(parent)
{
}
void Graph_Object::show()
{
    Map* mapa = Map::getInstance();
    int x1, y1, x2, y2;

    for(list<Edge>::iterator it = graph.m_Edges.begin();it != graph.m_Edges.end(); it++, it++){
        mapa->toPixel(it->m_pOrigin->x, it->m_pOrigin->y, x1, y1);
        mapa->toPixel(it->m_pDestination->x, it->m_pDestination->y, x2, y2);
        emit doCreateEdge(x1, y1, x2, y2);
    }

    for(Edge* e : t.m_Edges){
        mapa->toPixel(e->m_pOrigin->x, e->m_pOrigin->y, x1, y1);
        mapa->toPixel(e->m_pDestination->x, e->m_pDestination->y, x2, y2);
        emit doCreateTrack(x1, y1, x2, y2);
    }

//    for(list<Edge>::iterator it = simplifiedGraph.m_Edges.begin();it != simplifiedGraph.m_Edges.end(); it++, it++){
//            mapa->toPixel(it->m_pOrigin->x, it->m_pOrigin->y, x1, y1);
//            mapa->toPixel(it->m_pDestination->x, it->m_pDestination->y, x2, y2);
//            emit doCreateEdge(x1, y1, x2, y2);
//            qDebug()<<it->m_pOrigin->id;
//            qDebug()<<it->m_pDestination->id;
//            qDebug()<<it->dist;
//            qDebug()<<"";
////            mapa->toPixel(it->completeGraphEdge->m_pOrigin->x, it->completeGraphEdge->m_pOrigin->y, x1, y1);
////            mapa->toPixel(it->completeGraphEdge->m_pDestination->x, it->completeGraphEdge->m_pDestination->y, x2, y2);
////            emit doCreateEdge(x1, y1, x2, y2);
//        }

    emit doCreateMap();
}

void Graph_Object::searchTrack(double distance, double elevation)
{
    searchDist = distance * 1000;
    searchElev = elevation;
    act = 1;
    start();
}

void Graph_Object::clear()
{
    graph.Clear();
    t.Clear();
    visitar.clear();
    paths.clear();
}

void Graph_Object::clearParadas()
{
    list<Vertex*>::iterator it = visitar.begin();
    it++;
    int f = visitar.size() - 2;
    for(int x = 0; x < f; x++){
        it = visitar.erase(it);
    }
    t.Clear();
}

void Graph_Object::resetTrack()
{
    t.Clear();
    visitar.clear();
}

void Graph_Object::saveTrack(QString name)
{
    name.remove(0 , 8);
    QDomDocument document;

//    QDomElement header = document.createElement("?xml");
//    header.setAttribute("version", "1.0");
//    header.setAttribute("encoding", "UTF-8");
//    document.appendChild(header);

    QDomElement root = document.createElement("gpx");
    root.setAttribute("creator", "GPS de Montaña");
    root.setAttribute("link", "github");
    document.appendChild(root);

    QDomElement trk = document.createElement("trk");
    root.appendChild(trk);

    QDomElement n = document.createElement("name");
    n.appendChild(document.createTextNode(name.split("/")[name.split("/").size()-1]));
    trk.appendChild(n);

    QDomElement trkseg = document.createElement("trkseg");
    trk.appendChild(trkseg);

    Edge* edge = t.m_Edges.front();
    QDomElement trkpt = document.createElement("trkpt");
    trkpt.setAttribute("lat", edge->m_pOrigin->lat);
    trkpt.setAttribute("lon", edge->m_pOrigin->lon);
    trkseg.appendChild(trkpt);
    QDomElement ele = document.createElement("ele");
    ele.appendChild(document.createTextNode(QString::number(edge->m_pOrigin->elevacion)));
    trkpt.appendChild(ele);

    for(Edge* e : t.m_Edges){
        trkpt = document.createElement("trkpt");
        trkpt.setAttribute("lat", e->m_pDestination->lat);
        trkpt.setAttribute("lon", e->m_pDestination->lon);
        trkseg.appendChild(trkpt);
        ele = document.createElement("ele");
        ele.appendChild(document.createTextNode(QString::number(e->m_pDestination->elevacion)));
        trkpt.appendChild(ele);
    }

    QFile file(name);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open the file for writing failed";
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
    }
}

void Graph_Object::saveGraph(QString name)
{
    name.remove(0 , 8);
    QDomDocument document;

    QDomElement root = document.createElement("gpx");
    root.setAttribute("creator", "GPS de Montaña");
    root.setAttribute("link", "github");
    document.appendChild(root);

    QDomElement trk = document.createElement("trk");
    root.appendChild(trk);

    QDomElement n = document.createElement("name");
    n.appendChild(document.createTextNode(name.split("/")[name.split("/").size()-1]));
    trk.appendChild(n);

    QDomElement trkseg = document.createElement("trkseg");
    trk.appendChild(trkseg);
    saveGraphRecursive(document, trk, trkseg, graph.m_Edges.front().m_pOrigin, graph.m_Edges.front().m_pOrigin);

    trkseg = document.createElement("trkseg");
    trk.appendChild(trkseg);
    saveGraphRecursive(document, trk, trkseg, graph.m_Edges.front().m_pOrigin, graph.m_Edges.front().m_pOrigin);

    QFile file(name);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open the file for writing failed";
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
    }

    for(list<Vertex*> l : graph.m_Vertices){
        for(Vertex* v : l){
            v->saveGraphVisit = false;
        }
    }

}

void Graph_Object::saveGraphRecursive(QDomDocument &doc, QDomElement &trk, QDomElement &trkseg, Vertex *actual, Vertex* anterior)
{
    bool intersection = false;

    while(!intersection){

        QDomElement trkpt = doc.createElement("trkpt");
        trkpt.setAttribute("lat", actual->lat);
        trkpt.setAttribute("lon", actual->lon);
        trkseg.appendChild(trkpt);
        QDomElement ele = doc.createElement("ele");
        ele.appendChild(doc.createTextNode(QString::number(actual->elevacion)));
        trkpt.appendChild(ele);
        actual->saveGraphVisit = true;
        if(actual->m_Edges.size() > 2){
            intersection = true;
            for(Edge* e : actual->m_Edges){
                if(e->m_pDestination->saveGraphVisit == false){
                    QDomElement trksegNew = doc.createElement("trkseg");
                    trk.appendChild(trksegNew);
                    trkpt = doc.createElement("trkpt");
                    trkpt.setAttribute("lat", actual->lat);
                    trkpt.setAttribute("lon", actual->lon);
                    trksegNew.appendChild(trkpt);
                    ele = doc.createElement("ele");
                    ele.appendChild(doc.createTextNode(QString::number(actual->elevacion)));
                    trkpt.appendChild(ele);
                    saveGraphRecursive(doc, trk, trksegNew, e->m_pDestination, actual);
                }
            }
        }
        else{
            int antId = actual->id;
            for(Edge* e : actual->m_Edges){
                if(e->m_pDestination->saveGraphVisit == false){
                    anterior = actual;
                    actual = e->m_pDestination;
                }
            }
            if(actual->id == antId){
                intersection = true;
                for(Edge* e : actual->m_Edges){
                    if(e->m_pDestination->id != anterior->id){
                        QDomElement trkptNew = doc.createElement("trkpt");
                        trkptNew.setAttribute("lat", e->m_pDestination->lat);
                        trkptNew.setAttribute("lon", e->m_pDestination->lon);
                        trkseg.appendChild(trkptNew);
                        QDomElement eleNew = doc.createElement("ele");
                        eleNew.appendChild(doc.createTextNode(QString::number(e->m_pDestination->elevacion)));
                        trkptNew.appendChild(eleNew);
                    }
                }
            }
        }
    }
}

void Graph_Object::createSimplifiedGraph()
{
    for (Vertex* v : visitar)
        v->salesVisit = true;

    Vertex* actComplete = visitar.front();
    Vertex* actSimplified = simplifiedGraph.NewVertex(actComplete->id, actComplete->x, actComplete->y, actComplete->lat, actComplete->lon, actComplete->elevacion, 0);
    actComplete->saveGraphVisit = true;
    for(Edge* e : actComplete->m_Edges){
        if(!e->m_pDestination->saveGraphVisit){
            createSimplifiedGraphRecursive(e->m_pDestination, actSimplified, actComplete);
        }
    }

    for(list<Vertex*> l : graph.m_Vertices){
        for(Vertex* v : l){
            v->saveGraphVisit = false;
            v->salesVisit = false;
        }
    }

}

void Graph_Object::createSimplifiedGraphRecursive(Vertex *actComplete, Vertex* actSimplified, Vertex* antComplete)
{
    bool intersection = false;
    double dist = 0.0;
    double elevation = 0.0;
    Edge* completeEdge = nullptr;
    for(Edge* e : antComplete->m_Edges){
        if(e->m_pDestination->id == actComplete->id){
            completeEdge = e;
            dist += e->dist;
            elevation += e->desnivel;
            break;
        }
    }

    while(!intersection){
        if(actComplete->m_Edges.size() != 2 || actComplete->salesVisit){

            Vertex* nextSimplified = nullptr;
            if(!actComplete->saveGraphVisit){
                nextSimplified = simplifiedGraph.NewVertex(actComplete->id, actComplete->x, actComplete->y, actComplete->lat, actComplete->lon, actComplete->elevacion, 0);
                actComplete->saveGraphVisit = true;
            }
            else{
                nextSimplified = simplifiedGraph.SearchById(actComplete->id);
            }
            intersection = true;
            Edge* completeEdgeReverse = nullptr;
            for(Edge* e : actComplete->m_Edges){
                if(e->m_pDestination->id == antComplete->id){
                    completeEdgeReverse = e;
                    break;
                }
            }
            simplifiedGraph.NewEdge(actSimplified, nextSimplified, dist, elevation, completeEdge, completeEdgeReverse);

            for(Edge* e : actComplete->m_Edges){
                if(!e->m_pDestination->saveGraphVisit){
                    createSimplifiedGraphRecursive(e->m_pDestination, nextSimplified, actComplete);
                }
                else{
                    if(e->m_pDestination->m_Edges.size() != 2){
                        bool exist = false;
                        for(Edge* nE : nextSimplified->m_Edges){
                            if(nE->m_pDestination->id == e->m_pDestination->id){
                                exist = true;
                                break;
                            }
                        }
                        if(!exist){
                            Vertex* nextSimplifiedVisited = simplifiedGraph.SearchById(e->m_pDestination->id);
                            simplifiedGraph.NewEdge(nextSimplified, nextSimplifiedVisited, e->dist, e->desnivel, e, e->reverseEdge);
                        }
                    }
                }
            }
        }
        else{
            actComplete->saveGraphVisit = true;
            int antId = actComplete->id;
            for(Edge* e : actComplete->m_Edges){
                if(!e->m_pDestination->saveGraphVisit){
                    dist += e->dist;
                    elevation += e->desnivel;
                    antComplete = actComplete;
                    actComplete = e->m_pDestination;
                }
            }
            if(actComplete->id == antId){
                antId = antComplete->id;
                for(Edge* e : actComplete->m_Edges){
                    if((e->m_pDestination->m_Edges.size() != 2 || e->m_pDestination->salesVisit) && e->m_pDestination->id != antId){
                        dist += e->dist;
                        elevation += e->desnivel;
                        antComplete = actComplete;
                        actComplete = e->m_pDestination;
                    }
                }
            }
        }
    }
}

void Graph_Object::createSimplifiedVisits()
{
    for(Vertex* v : visitar){
        simplifiedVisits.push_back(simplifiedGraph.SearchById(v->id));
    }
}

void Graph_Object::calculateGraphParams()
{
    distanciaMax = 0.0;
    desnivelMax = 0.0;

    for(list<Edge>::iterator it = graph.m_Edges.begin();it != graph.m_Edges.end(); it++){
        distanciaMax += it->dist;
        desnivelMax += it->desnivel;
    }
}

void Graph_Object::run()
{
    if(act == 0){
        graph.load(paths);
        emit doCalculate();
        show();
        paths.clear();
    }
    else{
        if(act == 1){
            trackSearcher searcher(searchDist , searchElev);
            simplifiedGraph.Clear();
            simplifiedVisits.clear();
            createSimplifiedGraph();
            createSimplifiedVisits();
            t = searcher.searchTrack(simplifiedGraph, simplifiedVisits);
            t.calcular();
            simplifiedVisits.clear();
//            simplifiedGraph.Clear();
            emit doTrackSearched();
        }
    }
}

void Graph_Object::load(QString p)
{

    for(QString s : p.split(",")){
        s.remove(0 , 8);
        paths.push_back(s);
    }

    act = 0;
    start();
}

bool Graph_Object::isInPath(int x, int y, int caso){
    bool c=false;
    if (caso == 0){
        Vertex* v = graph.isInPath(x, y, c);
        if(c){
            visitar.push_back(v);
            return true;
        }else{
            return false;
        }
    }
    else{
        if(caso == 1){
            Vertex* v = graph.isInPath(x, y, c);
            if(c && !exist(visitar, v)){
                visitar.push_back(v);
                return true;
            }
            else
                return false;
        }
        else{
            Vertex* v = graph.isInPath(x, y, c);
            if(c && !exist(visitar, v)){
                list<Vertex*>::iterator it = visitar.end();
                it--;
                visitar.insert(it,v);
                return true;
            }
            else
                return false;
        }
    }
}

int Graph_Object::getX(int caso){
    Map* mapa = nullptr;
    mapa = Map::getInstance();
    int x, y;
    if(caso == 0){
        mapa->toPixel(visitar.front()->x, visitar.front()->y, x, y);
        return x;
    }
    else{
        if(caso == 1){
            mapa->toPixel(visitar.back()->x, visitar.back()->y, x, y);
            return x;
        }
        else{
            list<Vertex*>::iterator it = visitar.end();
            it--;
            it--;
            mapa->toPixel((*it)->x, (*it)->y, x, y);
            return x;
        }
    }
}

int Graph_Object::getY(int caso){
    Map* mapa = nullptr;
    mapa = Map::getInstance();
    int x, y;
    if(caso==0){
        mapa->toPixel(visitar.front()->x, visitar.front()->y, x, y);
        return y;
    }
    else{
        if(caso == 1){
            mapa->toPixel(visitar.back()->x, visitar.back()->y, x, y);
            return y;
        }
        else{
            list<Vertex*>::iterator it = visitar.end();
            it--;
            it--;
            mapa->toPixel((*it)->x, (*it)->y, x, y);
            return y;
        }
    }
}

int Graph_Object::getXParada(int v)
{
    Map* mapa = nullptr;
    mapa = Map::getInstance();
    int x, y;
    list<Vertex*>::iterator it = visitar.begin();
    it++;
    for(int i = 0; i < v; i++){
        it++;
    }
    mapa->toPixel((*it)->x, (*it)->y, x, y);
    return x;
}

int Graph_Object::getYParada(int v)
{
    Map* mapa = nullptr;
    mapa = Map::getInstance();
    int x, y;
    list<Vertex*>::iterator it = visitar.begin();
    it++;
    for(int i = 0; i < v; i++){
        it++;
    }
    mapa->toPixel((*it)->x, (*it)->y, x, y);
    return y;
}

double Graph_Object::getLatParada()
{
    list<Vertex*>::iterator it = visitar.end();
    it--;
    it--;
    return (*it)->lat;
}

double Graph_Object::getLonParada()
{
    list<Vertex*>::iterator it = visitar.end();
    it--;
    it--;
    return (*it)->lon;
}

double Graph_Object::getTrackDistance()
{
    int a = t.dist * 10;
    int b = a % 10;
    a = a / 10;
    if(b >= 5)
        a++;
    return double(a) / 1000;
}

double Graph_Object::getTrackElevation()
{
    int a = t.desnivel * 100;
    int b = a % 10;
    a = a / 10;
    if(b >= 5)
        a++;
    return double(a) / 10;
}

