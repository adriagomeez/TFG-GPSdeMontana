#include "loader_object.h"

loader_object::loader_object(QObject *parent) : QThread(parent)
{
    mapa = Map::getInstance();
    mapImage = mapa->getMap();
    scaledMapImage = mapa->getScaledMap();
}

void loader_object::zoomInOut()
{
    double scale = double((mapImage->size[1] - widthInc * zoom)) / double(mapa->screenWidth);
    int pixels = (widthInc * zoom) / scale;
    int w = mapa->screenWidth + pixels;
    int h = w * mapImage->size[0] / mapImage->size[1];
    cv::resize(*mapImage, *scaledMapImage, cv::Size(w,h), 0, 0, cv::INTER_AREA);
}

void loader_object::initialization()
{
    zoom = 0;
    mapa->setMap(path);
    widthInc = (mapImage->size[1] - 500) / 20;
    emit doMapLoaded();
}

void loader_object::run()
{
    load();
}

void loader_object::loadMap(int c, QString p)
{
    path = p;
    caso = c;
    start();
}

void loader_object::mapAction(int c)
{
    terminate();
    caso = c;
    start();
}

void loader_object::load()
{
    int c = caso;

    if(c == 0){
        zoom++;
        zoomInOut();
    }
    else {
        if(c == 1){
            zoom--;
            zoomInOut();
        }
        else{
            if(c == 2){
                initialization();
            }
        }
    }
}
