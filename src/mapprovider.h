#ifndef MAPPROVIDER_H
#define MAPPROVIDER_H

#include <QQuickAsyncImageProvider>
#include "map.h"
#include <time.h>
#include <opencv2/core.hpp>
#include <QString>

class MapProvider : public QQuickImageProvider
{
public:
    MapProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
private:
    int width, height, zoom, screenWidth, screenHeight, croppedX, croppedY, widthInc;
    int* scaledCroppedX;
    int* scaledCroppedY;
    double scale;

    Map* mapa;
    cv::Mat* mapImage;
    cv::Mat* printedImage;
    cv::Mat* scaledMapImage;


    QImage initialization();
    QImage drag(int x, int y);
    QImage zoomIn();
    QImage zoomOut();
};

#endif // MAPPROVIDER_H
