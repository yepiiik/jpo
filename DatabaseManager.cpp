#include "DatabaseManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QtConcurrent>
#include <QFutureWatcher>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(dataPath);
    }
    m_jsonFilePath = dir.filePath("airquality_data.json");
    qDebug() << "Database file path:" << m_jsonFilePath;
    loadJsonData();
}

void DatabaseManager::loadJsonData() {
    QMutexLocker locker(&m_fileMutex);

    QFile file(m_jsonFilePath);
    if (!file.exists()) {
        m_jsonData = QJsonObject{
            {"stations", QJsonArray()},
            {"sensors", QJsonArray()},
            {"readings", QJsonArray()}
        };
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open JSON file:" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON file";
        return;
    }

    m_jsonData = doc.object();
}

void DatabaseManager::saveJsonData() {
    // Make a deep copy of the data to avoid race conditions
    QJsonObject dataCopy = m_jsonData;

    QtConcurrent::run([this, dataCopy]() {
        QMutexLocker locker(&m_fileMutex);

        QSaveFile file(m_jsonFilePath);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Failed to open JSON file for writing:" << file.errorString();
            return;
        }

        QJsonDocument doc(dataCopy);
        if (file.write(doc.toJson()) == -1) {
            qWarning() << "Failed to write JSON data:" << file.errorString();
            file.cancelWriting();
            return;
        }

        if (!file.commit()) {
            qWarning() << "Failed to commit file changes:" << file.errorString();
        }
    });
}

// Station operations
void DatabaseManager::storeStation(const QVariantMap &station) {
    QMutexLocker locker(&m_dataMutex);

    QJsonArray stations = m_jsonData["stations"].toArray();

    // Remove existing station if present
    for (int i = 0; i < stations.size(); i++) {
        if (stations[i].toObject()["id"].toInt() == station["id"].toInt()) {
            stations.removeAt(i);
            break;
        }
    }

    stations.append(QJsonObject::fromVariantMap(station));
    m_jsonData["stations"] = stations;
    saveJsonData();
}

QVariantList DatabaseManager::getAllStations() {
    QMutexLocker locker(&m_dataMutex);
    return m_jsonData["stations"].toArray().toVariantList();
}

// Sensor operations
void DatabaseManager::storeSensor(int stationId, const QVariantMap &sensor) {
    QMutexLocker locker(&m_dataMutex);

    QJsonArray sensors = m_jsonData["sensors"].toArray();

    // Remove existing sensor if present
    for (int i = 0; i < sensors.size(); i++) {
        if (sensors[i].toObject()["id"].toInt() == sensor["id"].toInt()) {
            sensors.removeAt(i);
            break;
        }
    }

    QJsonObject sensorObj = QJsonObject::fromVariantMap(sensor);
    sensorObj["station_id"] = stationId;
    sensors.append(sensorObj);
    m_jsonData["sensors"] = sensors;
    saveJsonData();
}

QVariantList DatabaseManager::getSensorsForStation(int stationId) {
    QMutexLocker locker(&m_dataMutex);

    QVariantList result;
    QJsonArray sensors = m_jsonData["sensors"].toArray();

    for (const QJsonValue &sensor : sensors) {
        if (sensor.toObject()["station_id"].toInt() == stationId) {
            QVariantMap sensorMap = sensor.toObject().toVariantMap();
            sensorMap.remove("station_id");
            result.append(sensorMap);
        }
    }

    return result;
}

// Reading operations
void DatabaseManager::storeReading(int sensorId, const QVariantMap &reading) {
    QMutexLocker locker(&m_dataMutex);

    QJsonArray readings = m_jsonData["readings"].toArray();

    QJsonObject readingObj = QJsonObject::fromVariantMap(reading);
    readingObj["sensor_id"] = sensorId;
    readings.append(readingObj);
    m_jsonData["readings"] = readings;
    saveJsonData();
}

QVariantList DatabaseManager::getReadingsForSensor(int sensorId) {
    QMutexLocker locker(&m_dataMutex);

    QVariantList result;
    QJsonArray readings = m_jsonData["readings"].toArray();

    for (const QJsonValue &reading : readings) {
        if (reading.toObject()["sensor_id"].toInt() == sensorId) {
            QVariantMap readingMap = reading.toObject().toVariantMap();
            readingMap.remove("sensor_id");
            result.append(readingMap);
        }
    }

    return result;
}
