#include "DataFetcher.h"
#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QColor>

const int CACHE_TIMEOUT_MS = 10 * 60 * 1000; // 10 minutes

DataFetcher::DataFetcher(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)), m_dbManager(parent)
{}

bool DataFetcher::isCacheValid() const {
    return !m_cachedStations.isEmpty() &&
           m_lastFetchTime.msecsTo(QDateTime::currentDateTime()) < CACHE_TIMEOUT_MS;
}

void DataFetcher::updateParameterMap() {
    m_parameterStationMap.clear();

    for (const QVariant &stationVar : m_cachedStations) {
        QVariantMap station = stationVar.toMap();
        QVariantList sensors = station["sensors"].toList();

        for (const QVariant &sensorVar : sensors) {
            QVariantMap sensor = sensorVar.toMap();
            QString paramCode = sensor["paramCode"].toString();

            // Prepare station info to store
            QVariantMap stationInfo {
                {"id", station["id"]},
                {"name", station["name"]},
                {"latitude", station["latitude"]},
                {"longitude", station["longitude"]}
            };

            // Get existing or create new station list for this parameter
            QVariantList stationsForParam;
            if (m_parameterStationMap.contains(paramCode)) {
                stationsForParam = m_parameterStationMap[paramCode].toList();
            }
            stationsForParam.append(stationInfo);
            m_parameterStationMap[paramCode] = stationsForParam;
        }
    }

}

void DataFetcher::fetchStations(bool forceRefresh) {
    if (!forceRefresh) {
        // First try to load from database
        QVariantList cachedStations = m_dbManager.getAllStations();
        if (!cachedStations.isEmpty()) {
            m_cachedStations = cachedStations;
            m_lastFetchTime = QDateTime::currentDateTime();
            emit stationsReady(m_cachedStations);

            // Also try to load sensors from DB
            for (const QVariant &station : m_cachedStations) {
                QVariantMap stationMap = station.toMap();
                QVariantList sensors = m_dbManager.getSensorsForStation(stationMap["id"].toInt());
                if (!sensors.isEmpty()) {
                    stationMap["sensors"] = sensors;
                    // Update the cached station
                    for (auto &s : m_cachedStations) {
                        if (s.toMap()["id"] == stationMap["id"]) {
                            s = stationMap;
                            break;
                        }
                    }
                }
            }

            updateParameterMap();
            return;
        }
    }

    QUrl url("https://api.gios.gov.pl/pjp-api/rest/station/findAll");
    QNetworkRequest request(url);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            reply->deleteLater();
            return;
        }

        QVariantList stations;
        QJsonArray data = QJsonDocument::fromJson(reply->readAll()).array();

        for (const QJsonValue &item : data) {
            QJsonObject obj = item.toObject();
            stations.append(QVariantMap{
                {"id", obj["id"].toInt()},
                {"name", obj["stationName"].toString()},
                {"city", obj["city"].toObject()["name"].toString()},
                {"latitude", obj["gegrLat"].toString().toDouble()},
                {"longitude", obj["gegrLon"].toString().toDouble()},
                {"sensors", QVariantList()} // Will be filled by fetchSensors
            });
        }

        m_cachedStations = stations;
        m_lastFetchTime = QDateTime::currentDateTime();
        emit stationsReady(stations);
        reply->deleteLater();

        // Pre-fetch sensors for all stations to build complete map
        for (const QVariant &station : stations) {
            fetchSensors(station.toMap()["id"].toInt());
            m_dbManager.storeStation(station.toMap());
        }
    });
}

void DataFetcher::fetchSensors(int stationId) {
    // First try to load from database
    QVariantList cachedSensors = m_dbManager.getSensorsForStation(stationId);
    if (!cachedSensors.isEmpty()) {
        // Update cached station with sensors
        for (auto &station : m_cachedStations) {
            QVariantMap stationMap = station.toMap();
            if (stationMap["id"].toInt() == stationId) {
                stationMap["sensors"] = cachedSensors;
                station = stationMap;
                break;
            }
        }
        updateParameterMap();
        emit sensorsReady(cachedSensors);
        return;
    }

    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/station/sensors/%1").arg(stationId));
    QNetworkRequest request(url);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            reply->deleteLater();
            return;
        }

        QVariantList sensors;
        QJsonArray data = QJsonDocument::fromJson(reply->readAll()).array();

        for (const QJsonValue &item : data) {
            QJsonObject obj = item.toObject();
            QJsonObject param = obj["param"].toObject();
            sensors.append(QVariantMap{
                {"id", obj["id"].toInt()},
                {"paramName", param["paramName"].toString()},
                {"paramCode", param["paramCode"].toString()},
                {"paramFormula", param["paramFormula"].toString()}
            });
        }

        // Update cached station with sensors
        for (auto &station : m_cachedStations) {
            QVariantMap stationMap = station.toMap();
            if (stationMap["id"].toInt() == stationId) {
                stationMap["sensors"] = sensors;
                station = stationMap;
                break;
            }
        }

        for (const QVariant &sensor : std::as_const(sensors)) {
            m_dbManager.storeSensor(stationId, sensor.toMap());
        }

        updateParameterMap();
        emit sensorsReady(sensors);
        reply->deleteLater();
    });
}

QVariantList DataFetcher::getStationsForParameter(const QString& paramCode) {
    if (m_parameterStationMap.contains(paramCode)) {
        return m_parameterStationMap[paramCode].toList();
    }
    return QVariantList();
}


void DataFetcher::fetchAllParameters() {
    if (m_cachedStations.isEmpty()) {
        fetchStations();
        return;
    }

    qDebug() << m_parameterStationMap;

    emit parametersReady(m_parameterStationMap);
}

void DataFetcher::fetchSensorData(int sensorId) {
    // First try to load from database
    QVariantList cachedReadings = m_dbManager.getReadingsForSensor(sensorId);
    if (!cachedReadings.isEmpty()) {
        emit sensorDataReady(sensorId, cachedReadings);
        return;
    }

    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/data/getData/%1").arg(sensorId));
    QNetworkRequest request(url);

    // Start timing the request
    m_requestTimer.start();
    QNetworkReply *reply = m_manager->get(request);
    m_requestStartTimes[reply] = m_requestTimer.elapsed();

    qDebug() << "[" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
             << "] Starting request to" << url.toString();

    connect(reply, &QNetworkReply::finished, [=]() {
        // Log latency
        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyDeleter(reply);

        // Calculate latency
        qint64 startTime = m_requestStartTimes.take(reply);
        qint64 endTime = m_requestTimer.elapsed();
        qint64 latencyMs = endTime - startTime;
        qint64 responseSize = reply->bytesAvailable();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "[" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
            << "] Request failed after" << latencyMs << "ms -"
            << reply->errorString();
            emit errorOccurred(reply->errorString());
            return;
        }

        qDebug() << "[" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
                 << "] Response received in" << latencyMs << "ms"
                 << "Size:" << responseSize << "bytes";

        // Log processing time
        qint64 processingStart = m_requestTimer.elapsed();
        // -------------------------------------------------------------


        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            reply->deleteLater();
            return;
        }

        QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonArray values = data["values"].toArray();

        QVariantList readings;
        for (const QJsonValue &item : values) {
            QJsonObject obj = item.toObject();
            QString dateTimeStr = obj["date"].toString();
            QDateTime dateTime = QDateTime::fromString(dateTimeStr, Qt::ISODate);
            if (!dateTime.isValid()) {
                continue;
            }

            QVariant value = obj["value"].toVariant();
            if (!value.isValid() || value.isNull()) {
                continue;
            }

            readings.append(QVariantMap{
                {"dateTime", dateTime},
                {"value", value}
            });
        }

        qint64 processingTime = m_requestTimer.elapsed() - processingStart;

        // Store readings in database
        for (const QVariant &reading : readings) {
            m_dbManager.storeReading(sensorId, reading.toMap());
        }

        emit sensorDataReady(sensorId, readings);
        reply->deleteLater();

        // ------------------------------------------------


        qDebug() << "[" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
                 << "] Processing completed in" << processingTime << "ms";

    });
}

QVariantList DataFetcher::getCachedStations() {
    return m_cachedStations;
}
