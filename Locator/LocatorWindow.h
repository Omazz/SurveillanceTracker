#ifndef LOCATORWINDOW_H
#define LOCATORWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

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

private:
    Ui::LocatorWindow *ui;
    QUdpSocket* _udpSocket;
};
#endif // LOCATORWINDOW_H
