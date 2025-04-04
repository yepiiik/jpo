#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <QObject>
#include <QNetworkAccessManager>

class WeatherApi : public QObject
{
    Q_OBJECT
public:
    explicit WeatherApi(QObject *parent = nullptr);
    void fetchWeather(double lat, double lon);
    void searchLocation(const QString &location);

signals:
    void weatherReceived(double temperature, double windspeed);
    void locationFound(double lat, double lon);
    void errorOccurred(const QString &message);

private:
    QNetworkAccessManager *networkManager;
    void handleWeatherResponse(QNetworkReply *reply);
    void handleLocationResponse(QNetworkReply *reply);
};

#endif // WEATHERAPI_H