#include "mainwindow.h"
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create UI elements
    selectButton = new QPushButton("Select Image", this);

    // Set window properties
    setWindowTitle("Wallpaper Changer");
    setMinimumSize(500, 400);
}

MainWindow::~MainWindow()
{
}