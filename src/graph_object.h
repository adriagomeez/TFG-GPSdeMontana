#ifndef GRAPH_OBJECT_H
#define GRAPH_OBJECT_H

#include <QObject>
#include <QQuickView>
#include <QThread>
#include "graph.h"
#include "map.h"
#include "tracksearcher.h"
#include <QPixmap>


class Graph_Object : public QThread
{
    Q_OBJECT
public:
    explicit Graph_Object(QObject *parent = nullptr);

signals:
    void doCreateEdge(float x1, float y1, float x2, float y2);
    void doCreateTrack(float x1, float y1, float x2, float y2);
    void doCreateMap();
    void doTrackSearched();
    void doCalculate();

private:
    Graph graph, simplifiedGraph;
    list<Vertex*> visitar, simplifiedVisits;
    list<QString> paths;
    Track t;
    int act;
    double distanciaMax, desnivelMax;
    float searchDist, searchElev;

protected:
    void run();

public slots:
    void load(QString paths);
    bool isInPath(int x, int y, int caso);
    int getX(int caso);
    int getY(int caso);
    int getXParada(int v);
    int getYParada(int v);
    double getLatInicio(){return visitar.front()->lat;}
    double getLonInicio(){return visitar.front()->lon;}
    double getLatFin(){return visitar.back()->lat;}
    double getLonFin(){return visitar.back()->lon;}
    double getLatParada();
    double getLonParada();
    double getTrackDistance();
    double getTrackElevation();
    int getDistMax(){return distanciaMax/1000;}
    int getDesnivelMax(){return desnivelMax;}
    void searchTrack(double distance, double elevation);
    void show();
    void clear();
    void clearParadas();
    void resetTrack();
    void saveTrack(QString name);
    void saveGraph(QString name);
    void saveGraphRecursive(QDomDocument &doc, QDomElement &trk, QDomElement &trkseg, Vertex* actual, Vertex* anterior);
    void createSimplifiedGraph();
    void createSimplifiedGraphRecursive(Vertex *actComplete, Vertex* actSimplified, Vertex* antComplete);
    void createSimplifiedVisits();
    void calculateGraphParams();
};

#endif // GRAPH_OBJECT_H
