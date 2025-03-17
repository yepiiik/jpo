#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Hello, Qt + CMake!");
    resize(400, 300);
}

MainWindow::~MainWindow() {}