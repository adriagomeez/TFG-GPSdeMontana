#include "mapprovider.h"

Map* Map::mapa;
MapProvider::MapProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
    mapa = Map::getInstance();
    mapImage = mapa->getMap();
    printedImage = mapa->getPrintedImage();
    scaledMapImage = mapa->getScaledMap();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    screenWidth = screenGeometry.width();
    screenHeight = screenGeometry.height();
    mapa->screenHeight = screenHeight;
    mapa->screenWidth = screenWidth;
    scaledCroppedX = &mapa->croppedX;
    scaledCroppedY = &mapa->croppedY;

}

QImage MapProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    int zoomAnt = zoom;
    int i = 0;
    int x = 0, y = 0;
    for(QString s : id.split("/")){
        switch(i){
        case 0:
            zoom = s.toInt();
            break;
        case 1:
            x = s.toInt();
            break;
        case 2:
            y = s.toInt();
            break;
        }
        i++;
    }

    if(zoom == -1){
        QImage imgIn = initialization();
        *size = QSize(imgIn.size());
        return imgIn;
    }
    else {
        if(zoom == -2){
            zoom = zoomAnt;
            QImage imgIn = drag(x, y);
            *size = QSize(imgIn.size());
            return imgIn;
        }
        else{
            if(zoom > zoomAnt){
                QImage imgIn = zoomIn();
                *size = QSize(imgIn.size());
                return imgIn;
            }
            else {
                if(zoom < zoomAnt){
                    QImage imgIn = zoomOut();
                    *size = QSize(imgIn.size());
                    return imgIn;
                }
            }
        }
    }
    return QImage();
}

QImage MapProvider::initialization()
{
    width = mapImage->size[1];
    height = (width * screenHeight) / screenWidth;
    mapa->setScaleRatio(screenWidth);
    scale = double(width) / double(screenWidth);

    widthInc = (mapImage->size[1] - 500) / 20;

    croppedX = 0;
    croppedY = 0;
    *scaledCroppedX = 0;
    *scaledCroppedY = 0;

    *printedImage = (*mapImage)(cv::Rect(0 ,0 , width, height));

    cv::resize(*printedImage, *printedImage, cv::Size(screenWidth, screenHeight), 0, 0, cv::INTER_AREA);
    cv::resize(*mapImage, *scaledMapImage, cv::Size(screenWidth, screenWidth * mapImage->size[0] / mapImage->size[1]), 0, 0, cv::INTER_AREA);;

    QImage imgIn= QImage((uchar*) printedImage->data, printedImage->cols, printedImage->rows, printedImage->step, QImage::Format_RGB888);

    return imgIn;
}

QImage MapProvider::drag(int x, int y)
{
    if(*scaledCroppedX - x >= 0 && *scaledCroppedX - x + screenWidth <= scaledMapImage->size[1]){
        croppedX -= x * scale;
        *scaledCroppedX -= x;
    }

    if(*scaledCroppedY - y >= 0 && *scaledCroppedY - y + screenHeight <= scaledMapImage->size[0]){
        croppedY -= y * scale;
        *scaledCroppedY -= y;
    }

    *printedImage = (*scaledMapImage)(cv::Rect(*scaledCroppedX, *scaledCroppedY, screenWidth ,screenHeight));

    QImage imgIn= QImage((uchar*) printedImage->data, printedImage->cols, printedImage->rows, printedImage->step, QImage::Format_RGB888);

    return imgIn;
}

QImage MapProvider::zoomIn()
{
    int heightAnt = height;
    width -= widthInc;
    height = (width * screenHeight) / screenWidth;

    croppedX += widthInc/2;
    croppedY += abs(height - heightAnt)/2;

    scale = double(width) / double(screenWidth);
    int pixels = widthInc * zoom / scale;
    int w = pixels + screenWidth;
    mapa->setScaleRatio(w);
    *scaledCroppedX = croppedX / scale;
    *scaledCroppedY = croppedY / scale;

    *printedImage = (*mapImage)(cv::Rect(croppedX ,croppedY ,width ,height));

    cv::resize(*printedImage, *printedImage, cv::Size(screenWidth, screenHeight), 0, 0, cv::INTER_AREA);

    //cv::medianBlur(*printedImage, *printedImage,1);

    QImage imgIn= QImage((uchar*) printedImage->data, printedImage->cols, printedImage->rows, printedImage->step, QImage::Format_RGB888);

    return imgIn;
}

QImage MapProvider::zoomOut()
{
    int heightAnt = height;
    width += widthInc;
    height = (width * screenHeight) / screenWidth;

    croppedX -= widthInc/2;
    if(croppedX < 0){
        croppedX = 0;
    }
    else{
        if(croppedX + width > mapImage->size[1]){
               croppedX = mapImage->size[1] - width;
        }
    }

    croppedY -= abs(height - heightAnt)/2;
    if(croppedY < 0){
        croppedY = 0;
    }
    else{
        if(croppedY + height > mapImage->size[0]){
               croppedY = mapImage->size[0] - height;
        }
    }

    scale = double(width) / double(screenWidth);
    int pixels = widthInc * zoom / scale;
    int w = pixels + screenWidth;
    mapa->setScaleRatio(w);
    *scaledCroppedX = croppedX / scale;
    *scaledCroppedY = croppedY / scale;

    *printedImage = (*mapImage)(cv::Rect(croppedX ,croppedY ,width ,height));

    cv::resize(*printedImage, *printedImage, cv::Size(screenWidth, screenHeight), 0, 0, cv::INTER_AREA);

    QImage imgIn= QImage((uchar*) printedImage->data, printedImage->cols, printedImage->rows, printedImage->step, QImage::Format_RGB888);

    return imgIn;
}
