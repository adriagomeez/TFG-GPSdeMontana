#include "graph.h"
#include "utm.h"

Graph::Graph()
{
    m_Vertices.assign(pow(divCuadrante,2), list<Vertex*>());
    numNodos = 0;
}

// ~CGraph =====================================================================

Graph::~Graph()
{
    m_Vertices.clear();
    m_Edges.clear();
}

// Clear =======================================================================

void Graph::Clear()
{
    for(int i = 0; i<pow(divCuadrante,2); i++){
        m_Vertices[i].clear();
    }
    m_Edges.clear();
    numNodos = 0;
}

void Graph::DeleteSection(list<pair<Vertex *, int>> section)
{
    for(pair<Vertex*,int> s: section){
        m_Vertices[s.second].remove(s.first);
        m_Edges.pop_back();
        m_Edges.pop_back();
        numNodos--;
    }
}

// ClearDistances ==============================================================

//void Graph::ClearDistances()
//{
//    for (CVertex& v : m_Vertices) v.m_DijkstraDistance = -1.0;
//}


// NewVertex ==================================================================

Vertex* Graph::NewVertex(const int name, double x, double y, double lat, double lon, double ele, int cuad)
{
    Vertex *v = new Vertex(name, x, y, lat, lon, ele);
    list<Vertex*>::iterator it;
        bool t = false;
        for(it = m_Vertices[cuad].begin(); it != m_Vertices[cuad].end(); it++){
            if((*it)->x > x){
                m_Vertices[cuad].insert(it, v);
                t = true;
                break;
            }
        }
        if(!t)
            m_Vertices[cuad].push_back(v);
    return v;

}

Vertex* Graph::SearchById(const int id)
{
    for(list<Vertex*> l : m_Vertices){
        for(Vertex* v : l){
            if(v->id == id){
                return v;
            }
        }
    }
    return nullptr;
}

// NewEdge =====================================================================

void Graph::NewEdge(Vertex *pOrigin, Vertex* pDestination)
{
    m_Edges.push_back(Edge(pOrigin, pDestination));
    Edge* pEdge = &m_Edges.back();
    m_Edges.push_back(Edge(pDestination, pOrigin));
    Edge* pEdgeReverse = &m_Edges.back();

    pEdge->reverseEdge = pEdgeReverse;
    pEdgeReverse->reverseEdge = pEdge;

    pOrigin->m_Edges.push_back(pEdge);
    pDestination->m_Edges.push_back(pEdgeReverse);
}

void Graph::NewEdge(Vertex *pOrigin, Vertex *pDestination, double dist, double elevation, Edge* e, Edge* eReverse)
{
    m_Edges.push_back(Edge(pOrigin, pDestination, dist, elevation, e));
    Edge* pEdge = &m_Edges.back();
    m_Edges.push_back(Edge(pDestination, pOrigin, dist, elevation, eReverse));
    Edge* pEdgeReverse = &m_Edges.back();

    pEdge->reverseEdge = pEdgeReverse;
    pEdgeReverse->reverseEdge = pEdge;

    pOrigin->m_Edges.push_back(pEdge);
    pDestination->m_Edges.push_back(pEdgeReverse);
}

void Graph::anotherPath(Vertex *actual, Vertex* anterior, Vertex *destino, int step, int &paths, list<Vertex*> &track, stack<list<Vertex*>> &tracks)
{
    if(step == -1)
        return ;
    else{
        step--;
        for(Edge* e: actual->m_Edges){
            if(e->m_pDestination->id!=anterior->id){
                if(e->m_pDestination->id == destino->id){
                    paths++;
                    track.push_back(e->m_pDestination);
                    tracks.push(track);
                    track.pop_back();
                    break;
                }
                else{
                    track.push_back(e->m_pDestination);
                    anotherPath(e->m_pDestination, actual, destino, step, paths, track, tracks);
                    track.pop_back();
                }
            }
        }
    }
}

bool Graph::duplicatedPath(list<pair<Vertex *, int> > section, stack<list<Vertex *> > &tracks)
{
    list<Vertex *> track = tracks.top();

    int sectionDist = sqrt(pow(track.front()->x - section.front().first->x, 2) + pow(track.front()->y - section.front().first->y, 2));
    list<pair<Vertex*, int>>::iterator it = section.begin();
    Vertex* aux = section.front().first;

    for(it++; it != section.end(); it++){
        sectionDist += sqrt(pow(aux->x - it->first->x, 2) + pow(aux->y - it->first->y, 2));
        aux = it->first;
    }

    sectionDist += sqrt(pow(aux->x - track.back()->x, 2) + pow(aux->y - track.back()->y, 2));

    int trackDist;
    while(!tracks.empty()){
        trackDist = 0;
        track = tracks.top();
        tracks.pop();
        list<Vertex*>::iterator it = track.begin();
        Vertex* aux = *it;
        for(it++; it != track.end(); it++){
            trackDist += sqrt(pow(aux->x - (*it)->x, 2) + pow(aux->y - (*it)->y, 2));;
            aux = *it;
        }

        if(abs(trackDist - sectionDist) < 20)
            return true;
    }
    return false;
}

void Graph::read(const QDomNode &node){
    QDomNode nodes = node.firstChild();
    while(!nodes.isNull()){
        if(nodes.isElement()){
            QDomElement elm = nodes.toElement();
            if(elm.tagName()=="trkpt"){
                int duplCount = 0;
                list<pair<Vertex*, int>> section;
                double lat = elm.attribute("lat").toDouble();
                double lon = elm.attribute("lon").toDouble();
                double ele = nodes.firstChildElement().text().toDouble();
                double x, y;
                LLtoUTM(lat, lon, y, x);
                double latAnt = lat;
                double lonAnt = lon;
                bool c = false;
                int cuad = cuadrante(lat, lon);
                Vertex* dupl = duplicado(x, y, c, cuad);
                Vertex* ant = nullptr;
                Vertex* iniSection = nullptr;
                if (!c){
                    ant = NewVertex(numNodos, x, y, lat, lon, ele, cuad);
                    numNodos++;
                    duplCount++;
                }
                else{
                    ant = dupl;
                }
                iniSection = ant;
                elm = nodes.toElement();
                while (!nodes.isNull()) {
                    lat = elm.attribute("lat").toDouble();
                    lon = elm.attribute("lon").toDouble();

                    double d = dist(lat, lon, latAnt, lonAnt);

                    if (d >= tram_Dif){
                        c = false;
                        cuad = cuadrante(lat, lon);
                        LLtoUTM(lat, lon, y, x);
                        dupl = duplicado(x, y, c, cuad);
                        //c=false;
                        if (!c){
                            latAnt = lat;
                            lonAnt = lon;
                            ele = nodes.firstChildElement().text().toDouble();
                            Vertex* act = NewVertex(numNodos, x, y, lat, lon, ele, cuad);
                            section.push_back(pair<Vertex*,int>(act, cuad));

                            NewEdge(ant, act);

                            numNodos++;
                            ant=act;
                            duplCount++;
                        }
                        else{

                            if(duplCount <= pathLength){
                                int paths = 0;
                                list<Vertex*> track;
                                stack<list<Vertex*>> tracks;
                                track.push_back(iniSection);
                                if(duplCount==0){
                                    anotherPath(iniSection, iniSection, dupl, pathLength, paths, track, tracks);
                                    if(paths==0)
                                        NewEdge(ant, dupl);
                                }
                                else{
                                    anotherPath(iniSection, section.front().first, dupl, pathLength, paths, track, tracks);

                                    if(paths >= 1){
                                        if(duplicatedPath(section, tracks)){
                                            DeleteSection(section);
                                            iniSection->m_Edges.pop_back();
                                        }
                                        else{
                                            duplicatedEdge(ant, dupl);
                                        }
                                    }
                                    else{
                                        duplicatedEdge(ant, dupl);
                                    }
                                }
                            }
                            else{
                                if(ant->id != dupl->id)
                                    duplicatedEdge(ant, dupl);
                            }

                            section.clear();
                            duplCount = 0;
                            ant = dupl;
                            iniSection = ant;
                            latAnt = dupl->lat;
                            lonAnt = dupl->lon;
                        }
                    }
                    nodes = nodes.nextSibling();
                    elm = nodes.toElement();
                }
            }
        }
        read(nodes);
        nodes = nodes.nextSibling();
    }
}

void Graph::load(list<QString> paths){

    for(QString path : paths){
        QDomDocument doc;
        QFile file(path);

        if(file.open(QFile::ReadOnly)){
            if(doc.setContent(&file)){
                QDomElement elem = doc.documentElement();
                read(elem);
            }
        }else{
            qDebug() << "Cannot read file" << file.errorString();
            exit(0);
        }
    }
}

void Graph::duplicatedEdge(Vertex *ant, Vertex *dupl)
{
    bool t = false;
    for(Edge* e : ant->m_Edges){
        if(e->m_pDestination->id == dupl->id){
            t = true;
            break;
        }
    }
    if(!t)
        NewEdge(ant, dupl);
}

double Graph::dist(double lat1, double lon1, double lat2, double lon2)
{
    double incLats = (lat1 - lat2) * pi / 180.0;
    double incLongs = (lon1 - lon2) * pi / 180.0;

    lat1 = lat1 * pi / 180.0;
    lat2 = lat2 * pi / 180.0;

    double a = pow(sin(incLats/2),2) + cos(lat1) * cos(lat2) * pow(sin(incLongs/2),2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c;
}

Vertex* Graph::buscar(double x, double y, bool& c, int cuad, double& distance){
    Vertex* duplicated = nullptr;
    double d = 0.0;
    list<Vertex*>::iterator it = m_Vertices[cuad].begin();
    for(; it != m_Vertices[cuad].end(); it++){
        d = (*it)->x - x;
         if(abs(d)<point_dif){
             d = sqrt(pow(x - (*it)->x, 2) + pow(y - (*it)->y, 2));
             d = ceil(d*100)/100;
             if(d < distance){
                 distance = d;
                 c = true;
                 duplicated = *it;
             }
         }
         else
             if(d>=point_dif)
                 break;
    }
    return duplicated;
}

Vertex* Graph::duplicado(const double x, const double y, bool& c, int cuad)
{
    Vertex* duplicated = nullptr;
    double distance = point_dif;
    duplicated = buscar(x, y, c, cuad, distance);
    int caso = cuadrantesCercanos(x, y, cuad);
    int fila = cuad / divCuadrante;
    int col = cuad % divCuadrante;
    Vertex* dup = nullptr;
    bool cAux = false;
    switch(caso){
        //izquierda
        case 1:
            cuad = fila * divCuadrante + (col-1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        //derecha
        case 2:
            cuad = fila * divCuadrante + (col+1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        //arriba
        case 3:
            cuad = (fila-1) * divCuadrante + col;
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        //esquina superior izquierda
        case 4:
            cuad = fila * divCuadrante + (col-1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila-1) * divCuadrante + (col-1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila-1) * divCuadrante + col;
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        //esquina superior derecha
        case 5:
            cuad = fila * divCuadrante + (col+1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila-1) * divCuadrante + (col+1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila-1) * divCuadrante + col;
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        //abajo
        case 6:
            cuad = (fila+1) * divCuadrante + col;
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        //esquina inferior izquierda
        case 7:
            cuad = fila * divCuadrante + (col-1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila+1) * divCuadrante + (col-1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila+1) * divCuadrante + col;
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        //esquina inferior derecha
        case 8:
            cuad = fila * divCuadrante + (col+1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila+1) * divCuadrante + (col+1);
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            cAux = false;
            cuad = (fila+1) * divCuadrante + col;
            dup = buscar(x, y, cAux, cuad, distance);
            if(cAux){
                duplicated = dup;
                c = true;
            }
            break;
        default:
            break;
    }

    return duplicated;
}


int Graph::cuadrantesCercanos(double x, double y, int cuad){
    Map* mapa = Map::getInstance();
    int fila = cuad / divCuadrante;
    int col = cuad % divCuadrante;
    double frontLeft = mapa->x_coordinate + mapa->difX * col;
    double frontRight = mapa->x_coordinate + mapa->difX * (col + 1);
    double frontUp = mapa->y_coordinate - mapa->difY * fila;
    double frontDown = mapa->y_coordinate - mapa->difY * (fila + 1);
    int t = 0;

    //izquierda
    if(abs(x - frontLeft) < point_dif){
        if(col != 0)
            t+=1;
    }
    else{//derecha
        if(abs(x - frontRight) < point_dif){
            if(col != 3)
                t+=2;
        }
    }
    //arriba
    if(abs(y - frontUp) < point_dif){
        if(fila != 0)
            t+=3;
    }
    else{//abajo
        if(abs(y - frontDown) < point_dif){
            if(fila != 3)
                t+=6;
        }
    }

    return t;

}

int Graph::cuadrante(const double lat, const double lon)
{
    Map* mapa = Map::getInstance();
    return mapa->cuadranteLL(lat, lon);
}

Vertex* Graph::isInPath(const int &x, const int &y, bool& isIn){
    double x_coor, y_coor;
    Map* mapa = Map::getInstance();
    mapa->toCoordinate(x, y, x_coor, y_coor);
    int cuad = mapa->cuadranteUTM(x_coor, y_coor);
    int fila = cuad / divCuadrante;
    int col = cuad % divCuadrante;
    int restFila = fila;
    int restCol = col;
    double dist = 999999999999;
    Vertex* proximo = nullptr;
    for(int i = 0; i < 9 ; i++){
        switch (i) {
        case 1:
            fila = restFila - 1;
            col = restCol - 1;
            break;
        case 2:
            fila = restFila - 1;
            col = restCol;
            break;
        case 3:
            fila = restFila - 1;
            col = restCol + 1;
            break;
        case 4:
            fila = restFila;
            col = restCol - 1;
            break;
        case 5:
            fila = restFila;
            col = restCol + 1;
            break;
        case 6:
            fila = restFila + 1;
            col = restCol - 1;
            break;
        case 7:
            fila = restFila + 1;
            col = restCol;
            break;
        case 8:
            fila = restFila + 1;
            col = restCol + 1;
            break;
        }
        if(fila >= 0 && fila < divCuadrante && col >= 0 && col < divCuadrante){
            cuad = fila * divCuadrante + col;
            list<Vertex*>::iterator it = m_Vertices[cuad].begin();
            double d;
            for(; it != m_Vertices[cuad].end(); it++){
                d = (*it)->x - x_coor;
                 if(abs(d)<dist){
                     d = sqrt(pow(x_coor - (*it)->x, 2) + pow(y_coor - (*it)->y, 2));
                     if(d < dist){
                         proximo = *it;
                         dist = d;
                     }
                 }
                 else
                     if(d>=dist)
                         break;
            }
        }
    }
    if(proximo == nullptr){
        return nullptr;
    }
    int xAux, yAux;
    mapa->toPixel(proximo->x, proximo->y, xAux, yAux);
    float pixelDist = sqrt(pow(x - xAux, 2) + pow(y - yAux, 2));
    if(pixelDist<=10){
        isIn = true;
        return proximo;
    }

    return nullptr;
}

bool exist(list<Vertex*> l, Vertex* n){
    for(Vertex* v : l){
        if(v->id == n->id)
            return true;
    }
    return false;
}
