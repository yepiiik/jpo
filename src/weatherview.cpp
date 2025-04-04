#include "weatherview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

WeatherView::WeatherView(QWidget *parent) : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    QHBoxLayout *searchLayout = new QHBoxLayout();
    locationInput = new QLineEdit();
    searchButton = new QPushButton("Search");
    searchLayout->addWidget(locationInput);
    searchLayout->addWidget(searchButton);

    temperatureLabel = new QLabel("Temperature: -");
    windLabel = new QLabel("Wind: -");
    statusLabel = new QLabel("Ready");

    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(temperatureLabel);
    mainLayout->addWidget(windLabel);
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);
    setWindowTitle("Weather App");

    connect(searchButton, &QPushButton::clicked, [this]() {
        emit locationRequested(locationInput->text());
    });
}

void WeatherView::displayWeather(double temperature, double windspeed)
{
    temperatureLabel->setText(QString("Temperature: %1°C").arg(temperature));
    windLabel->setText(QString("Wind: %1 km/h").arg(windspeed));
    statusLabel->setText("Data updated");
}

void WeatherView::showError(const QString &message)
{
    statusLabel->setText("Error: " + message);
}