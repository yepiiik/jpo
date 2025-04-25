#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include "DataFetcher.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Register with same URI as your QML module
    qmlRegisterType<DataFetcher>("jpo", 1, 0, "DataFetcher");
    qmlRegisterType<DatabaseManager>("jpo", 1, 0, "DatabaseManager");

    // Pixel rendering control (rounded borders)
    QSurfaceFormat format;
    format.setSamples(10);
    QSurfaceFormat::setDefaultFormat(format);

    QQmlApplicationEngine engine;
    engine.loadFromModule("jpo", "Main");

    return app.exec();
}
