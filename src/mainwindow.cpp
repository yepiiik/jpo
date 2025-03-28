#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QProcess>
#include <QRegularExpression>
#include <QComboBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_isDnsActive(false)
{
    // Create central widget and layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Create DNS selection combo box
    QComboBox *dnsCombo = new QComboBox(this);
    dnsCombo->addItem("Custom DNS");
    dnsCombo->addItem("OpenDNS Family Shield (Blocks YouTube)");
    dnsCombo->addItem("CleanBrowsing Family Filter");
    dnsCombo->addItem("AdGuard Family Protection");
    layout->addWidget(dnsCombo);

    // Create DNS input field
    m_dnsInput = new QLineEdit(this);
    m_dnsInput->setPlaceholderText("Enter DNS address (e.g., 8.8.8.8)");
    layout->addWidget(m_dnsInput);

    // Create button
    m_button = new QPushButton("Start", this);
    connect(m_button, &QPushButton::clicked, this, &MainWindow::toggleDnsConfiguration);
    layout->addWidget(m_button);

    // Connect combo box to update DNS input
    connect(dnsCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, dnsCombo](int index) {
        switch(index) {
            case 1: // OpenDNS Family Shield
                m_dnsInput->setText("208.67.222.123");
                break;
            case 2: // CleanBrowsing Family Filter
                m_dnsInput->setText("185.228.168.168");
                break;
            case 3: // AdGuard Family Protection
                m_dnsInput->setText("94.140.14.14");
                break;
            default:
                m_dnsInput->clear();
                break;
        }
    });

    // Set window properties
    setWindowTitle("DNS Configuration");
    resize(300, 200);
}

MainWindow::~MainWindow()
{
    if (m_isDnsActive) {
        removeCustomDns();
    }
}

void MainWindow::toggleDnsConfiguration()
{
    if (!m_isDnsActive) {
        QString dnsAddress = m_dnsInput->text().trimmed();
        
        // Validate DNS address format
        QRegularExpression ipRegex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");
        if (!ipRegex.match(dnsAddress).hasMatch()) {
            QMessageBox::warning(this, "Invalid DNS", "Please enter a valid IP address");
            return;
        }

        if (setCustomDns(dnsAddress)) {
            m_isDnsActive = true;
            updateButtonState();
            QMessageBox::information(this, "Success", "Custom DNS has been set");
        } else {
            QMessageBox::warning(this, "Error", "Failed to set custom DNS");
        }
    } else {
        if (removeCustomDns()) {
            m_isDnsActive = false;
            updateButtonState();
            QMessageBox::information(this, "Success", "Custom DNS has been removed");
        } else {
            QMessageBox::warning(this, "Error", "Failed to remove custom DNS");
        }
    }
}

bool MainWindow::setCustomDns(const QString& dnsAddress)
{
    // Get active network interface
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QString activeInterface;
    
    for (const QNetworkInterface& interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            activeInterface = interface.name();
            break;
        }
    }

    if (activeInterface.isEmpty()) {
        return false;
    }

    // Store original DNS
    QProcess process;
    process.start("netsh", QStringList() << "interface" << "ip" << "show" << "dns" << activeInterface);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    
    QRegularExpression dnsRegex("DNS servers:\\s*([^\\n]+)");
    auto match = dnsRegex.match(output);
    if (match.hasMatch()) {
        m_originalDns = match.captured(1).trimmed();
    }

    // Set new DNS
    process.start("netsh", QStringList() << "interface" << "ip" << "set" << "dns" 
                << "name=" << activeInterface << "static" << dnsAddress);
    return process.waitForFinished();
}

bool MainWindow::removeCustomDns()
{
    // Get active network interface
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QString activeInterface;
    
    for (const QNetworkInterface& interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            activeInterface = interface.name();
            break;
        }
    }

    if (activeInterface.isEmpty()) {
        return false;
    }

    // Reset to DHCP
    QProcess process;
    process.start("netsh", QStringList() << "interface" << "ip" << "set" << "dns" 
                << "name=" << activeInterface << "source=dhcp");
    return process.waitForFinished();
}

void MainWindow::updateButtonState()
{
    m_button->setText(m_isDnsActive ? "Stop" : "Start");
    m_dnsInput->setEnabled(!m_isDnsActive);
}