#include "AirplaneHandler.h"

AirplaneHandler::AirplaneHandler() {
    qDebug() << "\nProgram successfully start with settings!"
             << "\n\nANTETNNA SYSTEM PARAMETERS"
             << "\nScan period, msecs: " << SettingsTracker::SCAN_MSECS
             << "\nWait information, msecs: " << SettingsTracker::WAIT_INFO_MSECS
             << "\n\nLOCK PARAMETERS"
             << "\nNumber of plots to lock: " << SettingsTracker::NUMBER_OF_PLOTS_TO_LOCK
             << "\nNumber of scans to lock: " << SettingsTracker::NUMBER_OF_SCANS_TO_LOCK
             << "\n\nHOLD PARAMETERS"
             << "\nNumber of missing plots to remove track: " << SettingsTracker::NUMBER_OF_MISSING_PLOTS
             << "\nMin velocity, m/secs: " << SettingsTracker::MIN_VELOCITY_M_SECS
             << "\nMax velocity, m/secs: " << SettingsTracker::MAX_VELOCITY_M_SECS
             << "\nMax difference angle, deg: " << SettingsTracker::MAX_ANGLE_DEG
             << "\nCoefficient increase strobe of hold: " << SettingsTracker::COEF_STROBE_HOLD
             << "\n\nMANEUVER PARAMETERS"
             << "\nVelocity, m/s: " << SettingsTracker::MANEUVER_VELOCITY_M_SECS
             << "\nAngle, deg: " << SettingsTracker::MANEUVER_ANGLE_DEG
             << "\n\nEnd settings!\n";

    mMessageHandler = new MessageHandler(this);
    connect(mMessageHandler, &MessageHandler::newPlot, this, &AirplaneHandler::onNewPlot);

    mTimer = new QTimer(this);
    mTimer->setTimerType(Qt::PreciseTimer);
    connect(mTimer, &QTimer::timeout, this, &AirplaneHandler::checkPlotsAndTracks);
    mTimer->setInterval(TIME_TO_CHECK_MSECS);
    mTimer->start();
}

void AirplaneHandler::onNewPlot(Plot plot) {

    mMessageHandler->sendDatagram(plot.asterixPlot().Data);

    if(tryAddToTrack(plot) == false) {
        if(tryCreateTrack(plot) == false) {
            mPlots.push_back(plot);
        } else {
            //mMessageHandler->sendDatagram(plot.asterixPlot().Data);
        }
    } else {
        //mMessageHandler->sendDatagram(plot.asterixPlot().Data);
    }
}

void AirplaneHandler::checkPlotsAndTracks() {
    qreal currentTime = QDateTime::currentMSecsSinceEpoch();

    /* Работа с плотами */
    QMutableListIterator<Plot> plotsIterator(mPlots);
    while(plotsIterator.hasNext()) {
        Plot currentPlot = plotsIterator.next();
        if((currentTime - (1000.0 * currentPlot.time())) > TIME_TO_REMOVE_PLOT) {
            plotsIterator.remove();
        }
    }

    /* Работа с траекторией */
    QMutableListIterator<Airplane*> airplanesIterator(mAirplanes);
    while(airplanesIterator.hasNext()) {
        Airplane* currentAirplane = airplanesIterator.next();

        if(currentTime - (1000.0 * currentAirplane->timeToNextPlot()) > SettingsTracker::WAIT_INFO_MSECS) {
            currentAirplane->updateExtrapolationPlot();

            qreal difTime = currentTime - (1000.0 * currentAirplane->measuredPlot().time());

            mMessageHandler->sendDatagram(currentAirplane->getExtrapolationPlot());

            if(difTime > TIME_TO_REMOVE_TRACK) {
                airplanesIterator.remove();
                Airplane* buffer = currentAirplane;
                currentAirplane = nullptr;
                delete buffer;
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
    QMutableListIterator<Airplane*> airplanesIterator(mAirplanes);
    while(airplanesIterator.hasNext()) {
        Airplane* currentAirplane = airplanesIterator.next();

        Plot airplanePlot = currentAirplane->filteredPlot();
        qreal difX = pow(airplanePlot.x() - plot.x(), 2.0);
        qreal difY = pow(airplanePlot.y() - plot.y(), 2.0);

        qreal difTime = plot.time() - airplanePlot.time();

        qreal coefMinR = 1.0 - (SettingsTracker::COEF_STROBE_HOLD * currentAirplane->counterExtrapolations());
        qreal coefMaxR = 1.0 + (SettingsTracker::COEF_STROBE_HOLD * currentAirplane->counterExtrapolations());
        qreal Rmin = SettingsTracker::MIN_VELOCITY_M_SECS * difTime * coefMinR - ADDITION_TO_STROBE_HOLD_M;
        qreal Rmax = SettingsTracker::MAX_VELOCITY_M_SECS * difTime * coefMaxR + ADDITION_TO_STROBE_HOLD_M;

        qreal angleBetweenTwoPoints = qDegreesToRadians(Plot::fromDecartToPolar(plot.y() - airplanePlot.y(),
                                                                                plot.x() - airplanePlot.x()).second);
        qreal differenceAngles = abs(angleBetweenTwoPoints - currentAirplane->directionAngle());

        qreal maxDeviationAngle = qDegreesToRadians(SettingsTracker::MAX_ANGLE_DEG) *
                (1.0 + (SettingsTracker::COEF_STROBE_HOLD * currentAirplane->counterExtrapolations()));

        if((differenceAngles - M_PI) > 0.1e-6) {
           differenceAngles = (2.0 * M_PI) - differenceAngles;
        }

        qreal differenceDopplerFreq = qAbs(airplanePlot.frequencyDoppler() - plot.frequencyDoppler());

        qreal differenceAmplitude = qAbs(airplanePlot.amplitude() - plot.amplitude());

        if(((difX + difY) >= qPow(Rmin, 2.0)) &&
           ((difX + difY) <= qPow(Rmax, 2.0)) &&
            (differenceAngles < maxDeviationAngle)) {// &&
//            (differenceDopplerFreq < TrackerSettings::MAX_DIF_DOPPLER_FREQ_HOLD_HZ) &&
//            (differenceAmplitude < TrackerSettings::MAX_DIF_AMPLITUDE_HOLD)) {

            currentAirplane->setTrack(plot);

            mMessageHandler->sendDatagram(currentAirplane->getFilteredPlot());

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
    for(int i = mPlots.size() - 1; i >= 0; i--) {
        Plot currentPlot = mPlots[i];
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

            mAirplanes.append(track);

            mMessageHandler->sendDatagram(track->getFilteredPlot());

            incrementTrackNumber();

            mPlots.removeAt(i);

            return true;
        }
    }

    return false;
}

bool AirplaneHandler::tryCreateTrackBy3Plots(Plot plot) {
    Plot secondPlot;
    bool flag = false;
    int i;
    for(i = mPlots.size() - 1; i >= 0; i--) {
        Plot currentPlot = mPlots[i];
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
        Plot currentPlot = mPlots[i];
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

    mAirplanes.append(track);

    mMessageHandler->sendDatagram(track->getFilteredPlot());

    incrementTrackNumber();

    for(int i = 0; i < mPlots.size(); ++i) {
        if(mPlots[i] == firstPlot) {
            mPlots.removeAt(i);
        }
    }

    for(int i = 0; i < mPlots.size(); ++i) {
        if(mPlots[i] == secondPlot) {
            mPlots.removeAt(i);
        }
    }

    return true;
}

bool AirplaneHandler::tryCreateTrackBy4Plots(Plot plot) {
    Plot thirdPlot;
    bool flag = false;
    int i;
    for(i = mPlots.size() - 1; i >= 0; i--) {
        Plot currentPlot = mPlots[i];
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
        Plot currentPlot = mPlots[i];
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
        Plot currentPlot = mPlots[i];
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

    mAirplanes.append(track);

    mMessageHandler->sendDatagram(track->getFilteredPlot());

    incrementTrackNumber();

    for(int i = 0; i < mPlots.size(); ++i) {
        if(mPlots[i] == firstPlot) {
            mPlots.removeAt(i);
        }
    }

    for(int i = 0; i < mPlots.size(); ++i) {
        if(mPlots[i] == secondPlot) {
            mPlots.removeAt(i);
        }
    }

    for(int i = 0; i < mPlots.size(); ++i) {
        if(mPlots[i] == thirdPlot) {
            mPlots.removeAt(i);
        }
    }

    return true;
}
