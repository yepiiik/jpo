#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Create UI elements
    selectButton = new QPushButton("Select Image", this);
    setButton = new QPushButton("Set as Wallpaper", this);
    imageLabel = new QLabel(this);
    imageLabel->setMinimumSize(400, 300);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("No image selected");
    imageLabel->setStyleSheet("border: 1px solid #ccc;");

    // Add widgets to layout
    layout->addWidget(imageLabel);
    layout->addWidget(selectButton);
    layout->addWidget(setButton);

    // Set window properties
    setWindowTitle("Wallpaper Changer");
    setMinimumSize(500, 400);
}

void MainWindow::createConnections()
{
    connect(selectButton, &QPushButton::clicked, this, &MainWindow::selectImage);
    connect(setButton, &QPushButton::clicked, this, &MainWindow::setWallpaper);
}

void MainWindow::selectImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Image", "", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    
    if (!fileName.isEmpty()) {
        currentImagePath = fileName;
        QPixmap pixmap(fileName);
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), 
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::setWallpaper()
{
    if (currentImagePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select an image first!");
        return;
    }

    if (wallpaperService.setWallpaper(currentImagePath)) {
        QMessageBox::information(this, "Success", "Wallpaper has been changed successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to change wallpaper!");
    }
} 