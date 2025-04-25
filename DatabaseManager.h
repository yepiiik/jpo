#pragma once

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qmutex.h>

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);

    // Station operations
    void storeStation(const QVariantMap &station);
    QVariantList getAllStations();
    QVariantMap getStationById(int id);

    // Sensor operations
    void storeSensor(int stationId, const QVariantMap &sensor);
    QVariantList getSensorsForStation(int stationId);

    // Reading operations
    void storeReading(int sensorId, const QVariantMap &reading);
    QVariantList getReadingsForSensor(int sensorId);

private:
    QString m_jsonFilePath;
    QJsonObject m_jsonData;
    QMutex m_fileMutex;
    QMutex m_dataMutex;

    void loadJsonData();
    void saveJsonData();
};
