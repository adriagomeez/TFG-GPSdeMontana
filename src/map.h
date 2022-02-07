#ifndef MAP_H
#define MAP_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <math.h>
#include <QScreen>
#include <QGuiApplication>
#include "utm.h"

class Map
{
protected:
    static Map* mapa;
    cv::Mat mapImage, printedImage, scaledMapImage;

public:
    double x_size, y_size, x_coordinate, y_coordinate, scaled_ratio, xFin_coordinate, yFin_coordinate;
    double latI, lonI, latF, lonF;
    double difLat, difLon, difX, difY;
    int screenWidth, screenHeight, croppedX, croppedY;
    const int divCuadrante = 10;

private:
    Map();

public:    
    static Map* getInstance(){
        if(mapa == nullptr){
            mapa = new Map();

        }
        return mapa;
    }

    void setMap(QString id);
    void setScaleRatio(const int &width);

    cv::Mat* getMap(){
        return &mapImage;
    }

    cv::Mat* getScaledMap(){
        return &scaledMapImage;
    }

    cv::Mat* getPrintedImage(){
        return &printedImage;
    }

    void toPixel(float x, float y, int &x_p, int &y_p);
    void toCoordinate(int x_p, int y_p, double &x, double &y);

    int cuadranteLL(const double lat, const double lon);
    int cuadranteUTM(const double x, const double y);
};

#endif // MAP_H
