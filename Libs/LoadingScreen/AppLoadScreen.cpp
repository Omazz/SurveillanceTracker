#include "AppLoadScreen.h"


AppLoadScreen::AppLoadScreen(const QPixmap& pixmap) : QSplashScreen(pixmap) {

}

void AppLoadScreen::showLoadingScreen(qreal time_msecs, QWidget* widget) {
    this->show();

    QDateTime currentTime = QDateTime::currentDateTime();
    currentTime = currentTime.addMSecs(time_msecs);
    while(QDateTime::currentDateTime() < currentTime) {
        qApp->processEvents();
    }

    this->finish(widget);
}

void AppLoadScreen::mousePressEvent(QMouseEvent* event) {
    event->ignore();
}

