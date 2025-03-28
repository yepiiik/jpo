#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QNetworkInterface>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleDnsConfiguration();
    bool setCustomDns(const QString& dnsAddress);
    bool removeCustomDns();

private:
    QPushButton *m_button;
    QLineEdit *m_dnsInput;
    QString m_originalDns;
    bool m_isDnsActive;
    void updateButtonState();
};

#endif // MAINWINDOW_H 