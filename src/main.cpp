#include <QGuiApplication>
#include <QQuickView>
#include "graph_object.h"
#include "mapprovider.h"
#include "loader_object.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<Graph_Object>("graph_object", 1, 0, "Graph");
    qmlRegisterType<loader_object>("loader_object", 1, 0, "Loader");

    QQuickView view;
    view.engine()->addImageProvider("MapProvider", new MapProvider());
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    if (!view.errors().isEmpty())
        return -1;
    view.showMaximized();

    return app.exec();
}
