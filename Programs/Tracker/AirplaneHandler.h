#ifndef AIRPLANEHANDLER_H
#define AIRPLANEHANDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QtCore>
#include <QByteArray>

#include "Airplane.h"
#include "MessageHandler.h"
#include "../../PriFiles/Settings/SettingsTracker.h"


static quint16 CURRENT_TRACK_NUMBER = 1; // 1...511

class AirplaneHandler : public QObject
{
    Q_OBJECT

private:
    constexpr static qreal TIME_TO_CHECK_MSECS = 50;
    constexpr static qreal ADDITION_TO_STROBE_HOLD_M = 5;
    constexpr static qreal ADDITION_TO_STROBE_LOCK_M = 7.5;

public:
    AirplaneHandler();

    bool tryCreateTrack(Plot plot);

    bool tryAddToTrack(Plot plot);

    void incrementTrackNumber();

public slots:
    void onNewPlot(Plot plot);

    void checkPlotsAndTracks();

private:
    bool tryCreateTrackBy2Plots(Plot plot);

    bool tryCreateTrackBy3Plots(Plot plot);

    bool tryCreateTrackBy4Plots(Plot plot);

    MessageHandler* m_messageHandler;

    qreal TIME_TO_REMOVE_TRACK =
            (qreal)(SettingsTracker::NUMBER_OF_MISSING_PLOTS * SettingsTracker::SCAN_MSECS) + SettingsTracker::WAIT_INFO_MSECS;

    qreal TIME_TO_REMOVE_PLOT =
            (qreal)((SettingsTracker::NUMBER_OF_SCANS_TO_LOCK - 1) * SettingsTracker::SCAN_MSECS)
                        + SettingsTracker::WAIT_INFO_MSECS;


    QList<Plot> m_plots;
    QList<Airplane*> m_airplanes;

    QTimer* m_timer;
};

#endif // AIRPLANEHANDLER_H
