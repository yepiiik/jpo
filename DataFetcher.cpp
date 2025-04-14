#include "DataFetcher.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

DataFetcher::DataFetcher(QObject *parent) : QObject(parent), 
    m_manager(new QNetworkAccessManager(this)) {}

void DataFetcher::fetchStations()
{
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/station/findAll");
    QNetworkRequest request(url);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Error:" << reply->errorString();
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
                {"longitude", obj["gegrLon"].toString().toDouble()}
            });
        }
        
        emit stationsReady(stations);
        reply->deleteLater();
    });
}