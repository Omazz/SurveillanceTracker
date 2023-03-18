#ifndef LOCATORWINDOW_H
#define LOCATORWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QFileDialog>
#include <QTime>
#include <pcap.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LocatorWindow; }
QT_END_NAMESPACE

class LocatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    LocatorWindow(QWidget *parent = nullptr);
    ~LocatorWindow();

private slots:
    void on_PB_start_clicked();

    void closeEvent(QCloseEvent* event);

private:
    Ui::LocatorWindow *ui;
    QUdpSocket* _udpSocket;
    bool _isWorking = true;
};
#endif // LOCATORWINDOW_H
