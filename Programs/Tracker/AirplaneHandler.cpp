#include "AirplaneHandler.h"

AirplaneHandler::AirplaneHandler() {
    m_messageHandler = new MessageHandler(this);
    connect(m_messageHandler, &MessageHandler::newPlot, this, &AirplaneHandler::onNewPlot);

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &AirplaneHandler::checkPlotsAndTracks);
    m_timer->setInterval(TIME_TO_CHECK_MSECS);
    m_timer->start();
}

bool AirplaneHandler::onNewPlot(Plot plot) {
    if(!plot.asterixPlot().Data.isEmpty()) {
        m_messageHandler->sendDatagram(plot.asterixPlot().Data);
    }

    if(tryAddToTrack(plot) == false) {
        if(tryCreateTrack(plot) == false) {
            m_plots.push_back(plot);
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}

void AirplaneHandler::checkPlotsAndTracks() {
    qreal currentTime = QDateTime::currentMSecsSinceEpoch();

    /* Работа с плотами */
    QMutableListIterator<Plot> plotsIterator(m_plots);
    while(plotsIterator.hasNext()) {
        Plot currentPlot = plotsIterator.next();
        if((currentTime - (1000.0 * currentPlot.time())) > TIME_TO_REMOVE_PLOT) {
            plotsIterator.remove();
        }
    }
}

void AirplaneHandler::onSkippingMeasurement() {
    QObject* obj = sender();
    if(obj) {
        Airplane* airplane = qobject_cast<Airplane*>(obj);
        if(airplane) {
            m_messageHandler->sendDatagram(airplane->getExtrapolationPlot());
        }
    }
}

void AirplaneHandler::onRemoveTrack() {
    QObject* obj = sender();
    if(obj) {
        Airplane* airplane = qobject_cast<Airplane*>(obj);
        if(airplane) {
            QMutableListIterator<Airplane*> airplanesIterator(m_airplanes);
            while(airplanesIterator.hasNext()) {
                Airplane* currentAirplane = airplanesIterator.next();
                if(currentAirplane == airplane) {
                    airplanesIterator.remove();
                    Airplane* bufferAirplane = currentAirplane;
                    currentAirplane = nullptr;
                    delete bufferAirplane;
                    return;
                }
            }
        }
    }
}


bool AirplaneHandler::tryCreateTrack(Plot plot) {
    bool result = false;

    switch(SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK) {
    case 2:
        result = tryCreateTrackBy2Plots(plot);
        break;
    case 3:
        result = tryCreateTrackBy3Plots(plot);
        break;
    case 4:
        result = tryCreateTrackBy4Plots(plot);
        break;
    }

    return result;
}

bool AirplaneHandler::tryAddToTrack(Plot plot) {
    qreal currentTime = static_cast<qreal>(QDateTime::currentMSecsSinceEpoch()) / 1000.0;
    qreal x = plot.x();
    qreal y = plot.y();

    QMutableListIterator<Airplane*> airplanesIterator(m_airplanes);
    while(airplanesIterator.hasNext()) {
        Airplane* currentAirplane = airplanesIterator.next();

        Plot extrapolatedPlot = currentAirplane->doExtrapolation(currentTime).first;
        qreal x0 = extrapolatedPlot.x();
        qreal y0 = extrapolatedPlot.y();
        qreal rho = extrapolatedPlot.range();
        qreal meanDeviation = qSqrt(qPow(SettingsTracker::MEAN_DEVIATION_RHO_M, 2) +
                                    qPow(rho * SettingsTracker::MEAN_DEVIATION_ANGLE_RAD, 2));
        qreal numberExtrapolations = currentAirplane->counterExtrapolations();
        qreal increaseCoef = 1.0 + ((SettingsTracker::COEF_STROBE_HOLD - 1.0) * numberExtrapolations);
        qreal R = 3.5 * meanDeviation * increaseCoef;

        qreal differenceDopplerFreq = qAbs(extrapolatedPlot.frequencyDoppler() - plot.frequencyDoppler());
        qreal differenceAmplitude = qAbs(extrapolatedPlot.amplitude() - plot.amplitude());

        if((qPow(x-x0, 2) + qPow(y-y0, 2)) <= qPow(R, 2.0)) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_HOLD_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_HOLD)) {

            currentAirplane->setTrack(plot);
            m_messageHandler->sendDatagram(currentAirplane->getFilteredPlot());
            connect(this, &AirplaneHandler::restartTimer, currentAirplane, &Airplane::onRestartTimer);
            emit restartTimer();
            disconnect(this, &AirplaneHandler::restartTimer, currentAirplane, &Airplane::onRestartTimer);

            return true;
        }
    }

    return false;
}

void AirplaneHandler::incrementTrackNumber() {
    CURRENT_TRACK_NUMBER++;
    if(CURRENT_TRACK_NUMBER == 512) {
        CURRENT_TRACK_NUMBER = 1;
    }
}


bool AirplaneHandler::tryCreateTrackBy2Plots(Plot plot) {
    for(int i = m_plots.size() - 1; i >= 0; i--) {
        Plot currentPlot = m_plots[i];
        qreal difX = pow(currentPlot.x() - plot.x(), 2.0);
        qreal difY = pow(currentPlot.y() - plot.y(), 2.0);

        qreal difTime = plot.time() - currentPlot.time();

        qreal Rmin = SettingsTracker::MIN_VELOCITY_M_SECS * difTime - ADDITION_TO_STROBE_LOCK_M;
        qreal Rmax = SettingsTracker::MAX_VELOCITY_M_SECS * difTime + ADDITION_TO_STROBE_LOCK_M;

        qreal differenceDopplerFreq = qAbs(plot.frequencyDoppler() - currentPlot.frequencyDoppler());
        qreal differenceAmplitude = qAbs(plot.amplitude() - currentPlot.amplitude());

        if(((difX + difY) >= qPow(Rmin, 2.0)) && ((difX + difY) <= pow(Rmax, 2.0))) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_LOCK_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_LOCK)) {

            Airplane* track = new Airplane({currentPlot, plot}, CURRENT_TRACK_NUMBER);
            QThread* thread = new QThread();
            track->moveToThread(thread);
            connect(track, &Airplane::skippingMeasurement, this, &AirplaneHandler::onSkippingMeasurement);
            connect(track, &Airplane::removeTrack, this, &AirplaneHandler::onRemoveTrack, Qt::QueuedConnection);
            connect(track, &Airplane::removeTrack, thread, &QThread::quit, Qt::QueuedConnection);
            connect(thread, &QThread::finished, thread, &QThread::deleteLater, Qt::QueuedConnection);
            thread->start();
            m_airplanes.append(track);

            m_messageHandler->sendDatagram(track->getFilteredPlot());

            incrementTrackNumber();

            m_plots.removeAt(i);

            return true;
        }
    }

    return false;
}

bool AirplaneHandler::tryCreateTrackBy3Plots(Plot plot) {
    Plot secondPlot;
    bool flag = false;
    int i;
    for(i = m_plots.size() - 1; i >= 0; i--) {
        Plot currentPlot = m_plots[i];
        qreal difX = pow(currentPlot.x() - plot.x(), 2.0);
        qreal difY = pow(currentPlot.y() - plot.y(), 2.0);

        qreal difTime = plot.time() - currentPlot.time();

        qreal Rmin = SettingsTracker::MIN_VELOCITY_M_SECS * difTime - ADDITION_TO_STROBE_LOCK_M;
        qreal Rmax = SettingsTracker::MAX_VELOCITY_M_SECS * difTime + ADDITION_TO_STROBE_LOCK_M;

        qreal differenceDopplerFreq = qAbs(plot.frequencyDoppler() - currentPlot.frequencyDoppler());
        qreal differenceAmplitude = qAbs(plot.amplitude() - currentPlot.amplitude());

        if(((difX + difY) >= qPow(Rmin, 2.0)) && ((difX + difY) <= pow(Rmax, 2.0))) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_LOCK_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_LOCK)) {
            secondPlot = currentPlot;
            flag = true;
            break;
        }
    }

    i--;

    if(!flag) {
        return false;
    }

    Plot firstPlot;
    flag = false;

    for( ; i >= 0; i--) {
        Plot currentPlot = m_plots[i];
        qreal difX = pow(currentPlot.x() - secondPlot.x(), 2.0);
        qreal difY = pow(currentPlot.y() - secondPlot.y(), 2.0);

        qreal difTime = secondPlot.time() - currentPlot.time();

        qreal Rmin = SettingsTracker::MIN_VELOCITY_M_SECS * difTime - ADDITION_TO_STROBE_LOCK_M;
        qreal Rmax = SettingsTracker::MAX_VELOCITY_M_SECS * difTime + ADDITION_TO_STROBE_LOCK_M;

        qreal differenceDopplerFreq = qAbs(currentPlot.frequencyDoppler() - secondPlot.frequencyDoppler());
        qreal differenceAmplitude = qAbs(currentPlot.amplitude() - secondPlot.amplitude());

        if(((difX + difY) >= qPow(Rmin, 2.0))  && ((difX + difY) <= pow(Rmax, 2.0))) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_LOCK_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_LOCK)) {
            firstPlot = currentPlot;
            flag = true;
            break;
        }
    }

    if(!flag) {
        return false;
    }

    Airplane* track = new Airplane({firstPlot, secondPlot, plot}, CURRENT_TRACK_NUMBER);
    QThread* thread = new QThread();
    track->moveToThread(thread);
    connect(track, &Airplane::skippingMeasurement, this, &AirplaneHandler::onSkippingMeasurement);
    connect(track, &Airplane::removeTrack, this, &AirplaneHandler::onRemoveTrack, Qt::QueuedConnection);
    connect(track, &Airplane::removeTrack, thread, &QThread::quit, Qt::QueuedConnection);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater, Qt::QueuedConnection);
    thread->start();
    m_airplanes.append(track);

    m_messageHandler->sendDatagram(track->getFilteredPlot());

    incrementTrackNumber();

    for(int i = 0; i < m_plots.size(); ++i) {
        if(m_plots[i] == firstPlot) {
            m_plots.removeAt(i);
        }
    }

    for(int i = 0; i < m_plots.size(); ++i) {
        if(m_plots[i] == secondPlot) {
            m_plots.removeAt(i);
        }
    }

    return true;
}

bool AirplaneHandler::tryCreateTrackBy4Plots(Plot plot) {
    Plot thirdPlot;
    bool flag = false;
    int i;
    for(i = m_plots.size() - 1; i >= 0; i--) {
        Plot currentPlot = m_plots[i];
        qreal difX = pow(currentPlot.x() - plot.x(), 2.0);
        qreal difY = pow(currentPlot.y() - plot.y(), 2.0);

        qreal difTime = plot.time() - currentPlot.time();

        qreal Rmin = SettingsTracker::MIN_VELOCITY_M_SECS * difTime - ADDITION_TO_STROBE_LOCK_M;
        qreal Rmax = SettingsTracker::MAX_VELOCITY_M_SECS * difTime + ADDITION_TO_STROBE_LOCK_M;

        qreal differenceDopplerFreq = qAbs(plot.frequencyDoppler() - currentPlot.frequencyDoppler());

        qreal differenceAmplitude = qAbs(plot.amplitude() - currentPlot.amplitude());

        if(((difX + difY) >= qPow(Rmin, 2.0)) && ((difX + difY) <= pow(Rmax, 2.0))) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_LOCK_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_LOCK)) {
            thirdPlot = currentPlot;
            flag = true;
            break;
        }
    }

    i--;
    if(!flag) {
        return false;
    }

    Plot secondPlot;
    flag = false;

    for( ; i >= 0; i--) {
        Plot currentPlot = m_plots[i];
        qreal difX = pow(currentPlot.x() - thirdPlot.x(), 2.0);
        qreal difY = pow(currentPlot.y() - thirdPlot.y(), 2.0);

        qreal difTime = thirdPlot.time() - currentPlot.time();

        qreal Rmin = SettingsTracker::MIN_VELOCITY_M_SECS * difTime - ADDITION_TO_STROBE_LOCK_M;
        qreal Rmax = SettingsTracker::MAX_VELOCITY_M_SECS * difTime + ADDITION_TO_STROBE_LOCK_M;

        qreal differenceDopplerFreq = qAbs(currentPlot.frequencyDoppler() - thirdPlot.frequencyDoppler());
        qreal differenceAmplitude = qAbs(currentPlot.amplitude() - thirdPlot.amplitude());

        if(((difX + difY) >= qPow(Rmin, 2.0))  && ((difX + difY) <= pow(Rmax, 2.0))) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_LOCK_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_LOCK)) {
            secondPlot = currentPlot;
            flag = true;
            break;
        }
    }

    i--;
    if(!flag) {
        return false;
    }

    Plot firstPlot;
    flag = false;

    for( ; i >= 0; i--) {
        Plot currentPlot = m_plots[i];
        qreal difX = pow(currentPlot.x() - secondPlot.x(), 2.0);
        qreal difY = pow(currentPlot.y() - secondPlot.y(), 2.0);

        qreal difTime = secondPlot.time() - currentPlot.time();

        qreal Rmin = SettingsTracker::MIN_VELOCITY_M_SECS * difTime - ADDITION_TO_STROBE_LOCK_M;
        qreal Rmax = SettingsTracker::MAX_VELOCITY_M_SECS * difTime + ADDITION_TO_STROBE_LOCK_M;

        qreal differenceDopplerFreq = qAbs(currentPlot.frequencyDoppler() - secondPlot.frequencyDoppler());
        qreal differenceAmplitude = qAbs(currentPlot.amplitude() - secondPlot.amplitude());

        if(((difX + difY) >= qPow(Rmin, 2.0))  && ((difX + difY) <= pow(Rmax, 2.0))) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_LOCK_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_LOCK)) {
            firstPlot = currentPlot;
            flag = true;
            break;
        }
    }

    if(!flag) {
        return false;
    }

    Airplane* track = new Airplane({firstPlot, secondPlot, thirdPlot, plot}, CURRENT_TRACK_NUMBER);
    QThread* thread = new QThread();
    track->moveToThread(thread);
    connect(track, &Airplane::skippingMeasurement, this, &AirplaneHandler::onSkippingMeasurement);
    connect(track, &Airplane::removeTrack, this, &AirplaneHandler::onRemoveTrack, Qt::QueuedConnection);
    connect(track, &Airplane::removeTrack, thread, &QThread::quit, Qt::QueuedConnection);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater, Qt::QueuedConnection);
    thread->start();
    m_airplanes.append(track);

    m_messageHandler->sendDatagram(track->getFilteredPlot());

    incrementTrackNumber();

    for(int i = 0; i < m_plots.size(); ++i) {
        if(m_plots[i] == firstPlot) {
            m_plots.removeAt(i);
        }
    }

    for(int i = 0; i < m_plots.size(); ++i) {
        if(m_plots[i] == secondPlot) {
            m_plots.removeAt(i);
        }
    }

    for(int i = 0; i < m_plots.size(); ++i) {
        if(m_plots[i] == thirdPlot) {
            m_plots.removeAt(i);
        }
    }

    return true;
}
