#include "LocatorWindow.h"
#include "ui_LocatorWindow.h"
#include <QRegExpValidator>

LocatorWindow::LocatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LocatorWindow)
{
    ui->setupUi(this);

    _udpSocket = new QUdpSocket(this);

    setFixedSize(width(), height());
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->LE_ipAddress->setValidator(ipValidator);
}

LocatorWindow::~LocatorWindow()
{
    delete ui;
}


void LocatorWindow::on_PB_start_clicked() {

}

