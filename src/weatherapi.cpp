#include "weatherapi.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

WeatherApi::WeatherApi(QObject *parent) : QObject(parent), 
    networkManager(new QNetworkAccessManager(this)) {}

void WeatherApi::fetchWeather(double lat, double lon)
{
    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", QString::number(lat));
    query.addQueryItem("longitude", QString::number(lon));
    query.addQueryItem("current_weather", "true");
    url.setQuery(query);

    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        handleWeatherResponse(reply);
    });
}

void WeatherApi::searchLocation(const QString &location)
{
    QUrl url("https://geocoding-api.open-meteo.com/v1/search");
    QUrlQuery query;
    query.addQueryItem("name", location);
    url.setQuery(query);

    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        handleLocationResponse(reply);
    });
}

void WeatherApi::handleWeatherResponse(QNetworkReply *reply)
{
    if (reply->error()) {
        emit errorOccurred(reply->errorString());
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject currentWeather = doc.object()["current_weather"].toObject();

    emit weatherReceived(
        currentWeather["temperature"].toDouble(),
        currentWeather["windspeed"].toDouble()
    );
    reply->deleteLater();
}

void WeatherApi::handleLocationResponse(QNetworkReply *reply)
{
    if (reply->error()) {
        emit errorOccurred(reply->errorString());
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = doc.object()["results"].toArray();
    if (!results.isEmpty()) {
        QJsonObject firstResult = results[0].toObject();
        emit locationFound(
            firstResult["latitude"].toDouble(),
            firstResult["longitude"].toDouble()
        );
    }
    reply->deleteLater();
}