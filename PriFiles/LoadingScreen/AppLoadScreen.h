#ifndef APPLOADSCREEN_H
#define APPLOADSCREEN_H

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QMouseEvent>
#include <QDateTime>


class AppLoadScreen : public QSplashScreen
{
public:
    AppLoadScreen(const QPixmap& pixmap);

    void showLoadingScreen(qreal time_msec = 3000, QWidget* widget = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

};

#endif // APPLOADSCREEN_H
