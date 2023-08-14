#ifndef RadarWindow_H
#define RadarWindow_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QFileDialog>
#include <QTime>
#include <pcap.h>

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
    QScopedPointer<QUdpSocket> m_udpSocket;
    bool m_isWorking = true;
};
#endif // RadarWindow_H
