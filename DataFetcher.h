#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QCache>
#include <QColor>
#include <qelapsedtimer.h>
#include "DatabaseManager.h"

class DatabaseManager;

class DataFetcher : public QObject {
    Q_OBJECT
public:
    explicit DataFetcher(QObject *parent = nullptr);

    Q_INVOKABLE void fetchStations(bool forceRefresh = false);
    Q_INVOKABLE void fetchSensors(int stationId);
    Q_INVOKABLE void fetchAllParameters();
    Q_INVOKABLE QVariantList getStationsForParameter(const QString& paramCode);
    Q_INVOKABLE void fetchSensorData(int sensorId);
    Q_INVOKABLE QVariantList getCachedStations();

signals:
    void stationsReady(const QVariantList &stations);
    void sensorsReady(const QVariantList &sensors);
    void parametersReady(const QVariantMap meters);
    void errorOccurred(const QString &message);
    void sensorDataReady(int sensorId, QVariantList readings);

private:
    QElapsedTimer m_requestTimer;
    QMap<QNetworkReply*, qint64> m_requestStartTimes;
    QNetworkAccessManager *m_manager;
    QVariantList m_cachedStations;
    QDateTime m_lastFetchTime;
    QVariantMap m_parameterStationMap;
    DatabaseManager m_dbManager;

    void updateParameterMap();
    bool isCacheValid() const;
};
