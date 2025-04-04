#include <QApplication>
#include "weatherview.h"
#include "weatherapi.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    WeatherView view;
    WeatherApi api;
    
    // Connect view to api
    QObject::connect(&view, &WeatherView::locationRequested,
                     &api, &WeatherApi::searchLocation);
    
    // Connect api results to view
    QObject::connect(&api, &WeatherApi::weatherReceived,
                     &view, &WeatherView::displayWeather);
    QObject::connect(&api, &WeatherApi::errorOccurred,
                     &view, &WeatherView::showError);
    
    // Connect location found to weather fetch
    QObject::connect(&api, &WeatherApi::locationFound,
                     [&api](double lat, double lon) {
        api.fetchWeather(lat, lon);
    });

    view.show();
    return app.exec();
}