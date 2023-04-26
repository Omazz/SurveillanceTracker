#ifndef RadarWindow_H
#define RadarWindow_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QFileDialog>
#include <QTime>
#include <pcap.h>

#include "../../PriFiles/DataSettings/SettingsTracker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RadarWindow; }
QT_END_NAMESPACE

class RadarWindow : public QMainWindow
{
    Q_OBJECT

public:
    RadarWindow(QWidget *parent = nullptr);
    ~RadarWindow();

private slots:
    void on_PB_start_clicked();

    void closeEvent(QCloseEvent* event);

private:
    Ui::RadarWindow *ui;
    QUdpSocket* _udpSocket;
    bool _isWorking = true;
};
#endif // RadarWindow_H
