#ifndef LOADER_OBJECT_H
#define LOADER_OBJECT_H

#include <QObject>
#include "map.h"

class loader_object : public QThread
{
    Q_OBJECT
public:
    explicit loader_object(QObject *parent = nullptr);

signals:
    void doMapLoaded();

private:
    Map *mapa;
    int caso, zoom, widthInc;
    cv::Mat* mapImage;
    cv::Mat* scaledMapImage;
    QString path;

    void zoomInOut();
    void initialization();
    void load();

protected:
    void run();

public slots:
    void mapAction(int c);
    void loadMap(int c, QString p);

};

#endif // LOADER_OBJECT_H
