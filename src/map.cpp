#include "map.h"

Map::Map()
{
    mapa=nullptr;
    x_size = 0.0;
    y_size = 0.0;
    x_coordinate = 0.0;
    y_coordinate = 0.0;
    scaled_ratio = 0.0;
    xFin_coordinate = 0.0;
    yFin_coordinate = 0.0;
    latI = 0.0;
    lonI = 0.0;
    latF = 0.0;
    lonF = 0.0;
    croppedY = 0;
    croppedX = 0;
}

void Map::setMap(QString id)
{
    id.remove(0 , 8);
    mapImage = cv::imread(id.toUtf8().constData(), cv::IMREAD_LOAD_GDAL);
    cvtColor(mapImage, mapImage,cv::COLOR_BGR2RGB);
    id.replace(".tif",".tfw");
    QFile file(id);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        int c=0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            switch (c) {
                case 0:
                    x_size = line.toDouble();
                    break;
                case 3:
                    y_size = line.toDouble();
                    break;
                case 4:
                    x_coordinate = line.toDouble();
                    break;
                case 5:
                    y_coordinate = line.toDouble();
                    break;
                default:
                    break;
            }
            c++;
        }

     file.close();
     xFin_coordinate = x_size * mapImage.size[1] + x_coordinate;
     yFin_coordinate = y_size * mapImage.size[0] + y_coordinate;


     UTMtoLL(y_coordinate, x_coordinate, 31, latI,  lonI);
     UTMtoLL(yFin_coordinate, xFin_coordinate, 31, latF,  lonF);
     difLat = abs(latF - latI) / divCuadrante;
     difLon = abs(lonF - lonI) / divCuadrante;
     difX = abs(x_coordinate - xFin_coordinate) / divCuadrante;
     difY = abs(y_coordinate - yFin_coordinate) / divCuadrante;
    }
}

void Map::setScaleRatio(const int &width){
    scaled_ratio = mapImage.size[1]/float(width);
}

void Map::toPixel(float x, float y, int &x_p, int &y_p)
{
    x_p = ((x - x_coordinate) / (x_size * scaled_ratio)) - croppedX;
    y_p = ((y - y_coordinate) / (y_size * scaled_ratio)) - croppedY;
}

void Map::toCoordinate(int x_p, int y_p, double &x, double &y)
{
    x = ((x_p + croppedX) * scaled_ratio * x_size) + x_coordinate;
    y = ((y_p + croppedY) * scaled_ratio * y_size) + y_coordinate;
}

int Map::cuadranteLL(const double lat, const double lon)
{
    int fila = abs(lat - latI) / difLat;
    int columna = abs(lon - lonI) / difLon;
    return fila * divCuadrante + columna;
}

int Map::cuadranteUTM(const double x, const double y)
{
    int columna = abs(x - x_coordinate) / (abs(xFin_coordinate - x_coordinate) / divCuadrante);
    int fila = abs(y - y_coordinate) / (abs(yFin_coordinate - y_coordinate) / divCuadrante);
    return fila * divCuadrante + columna;
}
