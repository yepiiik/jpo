#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include "wallpaperservice.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectImage();
    void setWallpaper();

private:
    void setupUI();
    void createConnections();

    QPushButton *selectButton;
    QPushButton *setButton;
    QLabel *imageLabel;
    QString currentImagePath;
    WallpaperService wallpaperService;
};

#endif // MAINWINDOW_H 