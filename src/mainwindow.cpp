#include "mainwindow.h"
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QLabel *mLabel = new QLabel("Hello", this);
}

MainWindow::~MainWindow()
{}