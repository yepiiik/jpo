#ifndef WEATHERVIEW_H
#define WEATHERVIEW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class WeatherView : public QMainWindow
{
    Q_OBJECT
public:
    explicit WeatherView(QWidget *parent = nullptr);
    void displayWeather(double temperature, double windspeed);
    void showError(const QString &message);

signals:
    void locationRequested(const QString &location);

private:
    QLineEdit *locationInput;
    QPushButton *searchButton;
    QLabel *temperatureLabel;
    QLabel *windLabel;
    QLabel *statusLabel;
};

#endif // WEATHERVIEW_H