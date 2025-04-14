#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "DataFetcher.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // Register with same URI as your QML module
    qmlRegisterType<DataFetcher>("jpo", 1, 0, "DataFetcher");

    QQmlApplicationEngine engine;
    engine.loadFromModule("jpo", "Main");
    
    return app.exec();
}
