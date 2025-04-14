#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QGeoCoordinate>

class DataFetcher : public QObject
{
    Q_OBJECT
public:
    explicit DataFetcher(QObject *parent = nullptr);

    Q_INVOKABLE void fetchStations();

signals:
    void stationsReady(const QVariantList &stations);

private:
    QNetworkAccessManager *m_manager;
};