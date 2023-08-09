#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    m_graphicsScene = new QGraphicsScene();

    m_graphicsScene->setSceneRect(
        -RADAR_RANGE_KM, -RADAR_RANGE_KM,
        RADAR_RANGE_KM * 2, RADAR_RANGE_KM * 2
    );

    ui->GW_locator->setScene(m_graphicsScene);
    m_graphicsScene->setBackgroundBrush(QBrush(GRAY));
    ui->GW_locator->scale(0.8, 0.8);


    qreal rho = m_graphicsScene->height() / 2 + 50;


    ui->GW_locator->setScene(m_graphicsScene);
    m_graphicsScene->setBackgroundBrush(QBrush(BLUE));
    ui->GW_locator->scale(0.8, 0.8);

    createGrid();

    ui->PB_buildingGraphics->setEnabled(false);


}

void MainWindow::createGrid() {
    qreal ringDist = 50;
    qreal sectorAngle = 30;

    QPen gridPen(WHITE);
    gridPen.setCosmetic(true);

    for (qreal curDist = ringDist; curDist <= m_graphicsScene->width() / 2; curDist += ringDist) {
        m_graphicsScene->addEllipse(
            -curDist, -curDist,
            curDist * 2, curDist * 2,
            gridPen
        );
    }

    for (qreal curAngle = 0; curAngle < 2 * M_PI; curAngle += qDegreesToRadians(sectorAngle)) {
        qreal x = m_graphicsScene->width() / 2 * qCos(curAngle);
        qreal y = m_graphicsScene->width() / 2 * qSin(curAngle);
        m_graphicsScene->addLine(0, 0, x, y, gridPen);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_PB_modeling_clicked() {
    clearModulation();
    ui->PB_buildingGraphics->setEnabled(true);
    if(ui->RB_onLine->isChecked() == true) {
        drawLineTrajectory();

    } else if(ui->RB_turn->isChecked() == true) {
        drawTurnTrajectory();

    } else if(ui->RB_onCircle->isChecked() == true) {
        drawCircleTrajectory();
    }
}

void MainWindow::on_PB_buildingGraphics_clicked() {
    QVector<qreal> step;
    QVector<QPointF> sigmaNoise;
    QVector<QPointF> sigmaAlphaBetaFilter;
    QVector<QPointF> sigmaAlphaBetaFilter_MNK;
    QVector<QPointF> sigmaAlphaBetaFilter_WMNK;
    QVector<QPointF> sigmaKalmanFilterCV;
    QVector<QPointF> sigmaKalmanFilterCA;
    QVector<QPointF> sigmaAdaptiveKalmanFilterCV;

    for(int i = 0; i < mTrajectoryOriginal.size(); ++i) {
        QPointF currSigmaNoise(0, 0);
        QPointF currSigmaAlphaBetaFilter(0, 0);
        QPointF currSigmaAlphaBetaFilter_MNK(0, 0);
        QPointF currSigmaAlphaBetaFilter_WMNK(0, 0);
        QPointF currSigmaKalmanFilterCV(0, 0);
        QPointF currSigmaKalmanFilterCA(0, 0);
        QPointF currSigmaAdaptiveKalmanFilterCV(0, 0);

        step.append(i * ui->DSB_updateTime->value());

        for(int indexModulation = 0; indexModulation < NUMBER_MODULATIONS; ++indexModulation) {
            QPointF diffNoise = (mTrackWithNoise[indexModulation][i] - mTrajectoryOriginal[i]);
            diffNoise.setX(diffNoise.x() * diffNoise.x());
            diffNoise.setY(diffNoise.y() * diffNoise.y());
            currSigmaNoise += diffNoise;

            QPointF diffAB = (mTrackAlphaBetaFilter[indexModulation][i] - mTrajectoryOriginal[i]);
            diffAB.setX(diffAB.x() * diffAB.x());
            diffAB.setY(diffAB.y() * diffAB.y());
            currSigmaAlphaBetaFilter += diffAB;

            QPointF diffAB_MNK = (mTrackAlphaBetaFilterMNK[indexModulation][i] - mTrajectoryOriginal[i]);
            diffAB_MNK.setX(diffAB_MNK.x() * diffAB_MNK.x());
            diffAB_MNK.setY(diffAB_MNK.y() * diffAB_MNK.y());
            currSigmaAlphaBetaFilter_MNK += diffAB_MNK;

            QPointF diffAB_WMNK = (mTrackAlphaBetaFilterWMNK[indexModulation][i] - mTrajectoryOriginal[i]);
            diffAB_WMNK.setX(diffAB_WMNK.x() * diffAB_WMNK.x());
            diffAB_WMNK.setY(diffAB_WMNK.y() * diffAB_WMNK.y());
            currSigmaAlphaBetaFilter_WMNK += diffAB_WMNK;

            QPointF diffKalman_CV = (mTrackKalmanFilterCV[indexModulation][i] - mTrajectoryOriginal[i]);
            diffKalman_CV.setX(diffKalman_CV.x() * diffKalman_CV.x());
            diffKalman_CV.setY(diffKalman_CV.y() * diffKalman_CV.y());
            currSigmaKalmanFilterCV += diffKalman_CV;

            QPointF diffKalman_CA = (mTrackKalmanFilterCA[indexModulation][i] - mTrajectoryOriginal[i]);
            diffKalman_CA.setX(diffKalman_CA.x() * diffKalman_CA.x());
            diffKalman_CA.setY(diffKalman_CA.y() * diffKalman_CA.y());
            currSigmaKalmanFilterCA += diffKalman_CA;

            QPointF diffAdaptiveKalman_CV = (mTrackAdaptiveKalmanFilterCV[indexModulation][i] - mTrajectoryOriginal[i]);
            diffAdaptiveKalman_CV.setX(diffAdaptiveKalman_CV.x() * diffAdaptiveKalman_CV.x());
            diffAdaptiveKalman_CV.setY(diffAdaptiveKalman_CV.y() * diffAdaptiveKalman_CV.y());
            currSigmaAdaptiveKalmanFilterCV += diffAdaptiveKalman_CV;

        }

        sigmaNoise.append(QPointF(qSqrt(currSigmaNoise.x() / NUMBER_MODULATIONS),
                                  qSqrt(currSigmaNoise.y() / NUMBER_MODULATIONS)));

        sigmaAlphaBetaFilter.append(QPointF(qSqrt(currSigmaAlphaBetaFilter.x() / NUMBER_MODULATIONS),
                                            qSqrt(currSigmaAlphaBetaFilter.y() / NUMBER_MODULATIONS)));

        sigmaAlphaBetaFilter_MNK.append(QPointF(qSqrt(currSigmaAlphaBetaFilter_MNK.x() / NUMBER_MODULATIONS),
                                                qSqrt(currSigmaAlphaBetaFilter_MNK.y() / NUMBER_MODULATIONS)));

        sigmaAlphaBetaFilter_WMNK.append(QPointF(qSqrt(currSigmaAlphaBetaFilter_WMNK.x() / NUMBER_MODULATIONS),
                                                qSqrt(currSigmaAlphaBetaFilter_WMNK.y() / NUMBER_MODULATIONS)));

        sigmaKalmanFilterCV.append(QPointF(qSqrt(currSigmaKalmanFilterCV.x() / NUMBER_MODULATIONS),
                                           qSqrt(currSigmaKalmanFilterCV.y() / NUMBER_MODULATIONS)));

        sigmaKalmanFilterCA.append(QPointF(qSqrt(currSigmaKalmanFilterCA.x() / NUMBER_MODULATIONS),
                                           qSqrt(currSigmaKalmanFilterCA.y() / NUMBER_MODULATIONS)));

        sigmaAdaptiveKalmanFilterCV.append(QPointF(qSqrt(currSigmaAdaptiveKalmanFilterCV.x() / NUMBER_MODULATIONS),
                                                   qSqrt(currSigmaAdaptiveKalmanFilterCV.y() / NUMBER_MODULATIONS)));
    }



    QVector<qreal> resSigmaNoise;
    QVector<qreal> resSigmaNoiseX;
    QVector<qreal> resSigmaNoiseY;
    QVector<qreal> resSigmaAlphaBetaFilter;
    QVector<qreal> resSigmaAlphaBetaFilter_MNK;
    QVector<qreal> resSigmaAlphaBetaFilter_WMNK;
    QVector<qreal> resSigmaKalmanFilterCV;
    QVector<qreal> resSigmaKalmanFilterCA;
    QVector<qreal> resSigmaAdaptiveKalmanFilterCV;

    for(int i = 0; i < step.size(); ++i) {
        resSigmaNoise.append(qSqrt(qPow(sigmaNoise[i].x(), 2)
                                   + qPow(sigmaNoise[i].y(), 2)));

        resSigmaNoiseX.append(sigmaNoise[i].x());

        resSigmaNoiseY.append(sigmaNoise[i].y());

        resSigmaAlphaBetaFilter.append(qSqrt(qPow(sigmaAlphaBetaFilter[i].x(), 2)
                                             + qPow(sigmaAlphaBetaFilter[i].y(), 2)));
        resSigmaAlphaBetaFilter_MNK.append(qSqrt(qPow(sigmaAlphaBetaFilter_MNK[i].x(), 2)
                                                 + qPow(sigmaAlphaBetaFilter_MNK[i].y(), 2)));

        resSigmaAlphaBetaFilter_WMNK.append(qSqrt(qPow(sigmaAlphaBetaFilter_WMNK[i].x(), 2)
                                                 + qPow(sigmaAlphaBetaFilter_WMNK[i].y(), 2)));

        resSigmaKalmanFilterCV.append(qSqrt(qPow(sigmaKalmanFilterCV[i].x(), 2)
                                            + qPow(sigmaKalmanFilterCV[i].y(), 2)));

        resSigmaKalmanFilterCA.append(qSqrt(qPow(sigmaKalmanFilterCA[i].x(), 2)
                                            + qPow(sigmaKalmanFilterCA[i].y(), 2)));

        resSigmaAdaptiveKalmanFilterCV.append(qSqrt(qPow(sigmaAdaptiveKalmanFilterCV[i].x(), 2)
                                                    + qPow(sigmaAdaptiveKalmanFilterCV[i].y(), 2)));

    }


    auto window = new GraphicsBuilderWidget();
    window->setAttribute(Qt::WA_DeleteOnClose);
    QVector<QVector<qreal>> arrays = {resSigmaNoise};
    QVector<QString> strings = {"Зашумленная траектория"};

    if(ui->CB_ABfilter->isChecked()) {
        arrays.append(resSigmaAlphaBetaFilter);
        strings.append("Альфа-бета фильтр");
    }

    if(ui->CB_ABfilterMNK->isChecked()) {
        arrays.append(resSigmaAlphaBetaFilter_MNK);
        strings.append("Альфа-бета фильтр МНК");
    }

    if(ui->CB_ABfilterWMNK->isChecked()) {
        arrays.append(resSigmaAlphaBetaFilter_WMNK);
        strings.append("Альфа-бета фильтр ВМНК");
    }


    if(ui->CB_KalmanFilterCV->isChecked()) {
        arrays.append(resSigmaKalmanFilterCV);
        strings.append("Фильтр Калмана CV");
    }

    if(ui->CB_KalmanFilterCA->isChecked()) {
        arrays.append(resSigmaKalmanFilterCA);
        strings.append("Фильтр Калмана CA");
    }

    if(ui->CB_AdaptiveKalmanFilterCV->isChecked()) {
        arrays.append(resSigmaAdaptiveKalmanFilterCV);
        strings.append("Адаптивный фильтр Калмана CV");
    }

    window->setData("Время, с", "СКО, метры", step,
                    arrays,
                    strings);
    window->setWindowModality(Qt::WindowModality::ApplicationModal);

    window->show();
}


void MainWindow::drawLineTrajectory() {
    QPointF currentCoord(ui->SB_firstPointX->value() * 1000.0, ui->SB_firstPointY->value() * 1000.0);
    QPointF endCoord(ui->SB_secondPointX->value() * 1000.0, ui->SB_secondPointY->value() * 1000.0);
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal numberSteps = qSqrt(qPow(currentCoord.x()-endCoord.x(), 2)+qPow(currentCoord.y()-endCoord.y(), 2)) /
                            (velocity * updateTime);
    QPointF step = (endCoord - currentCoord) / numberSteps;
    mTrajectoryOriginal.append(currentCoord);

    int currentStep = 0;
    while(currentStep < numberSteps) {
        currentCoord += step;
        mTrajectoryOriginal.append(currentCoord);
        currentStep++;
    }

    calculateVariablesTrajectories(updateTime);

    drawVariablesTrajectories();
}

void MainWindow::drawTurnTrajectory() {
    qreal radius = ui->SB_radius->value() * 1000.0;
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal step = velocity * updateTime;
    qreal stepAngle = step / radius;
    qreal currAngle = 0;

    do {
        currAngle += stepAngle;
        mTrajectoryOriginal.append(QPointF((radius * qCos(currAngle)) - radius, (radius * qSin(currAngle)) - radius));
    } while(currAngle <  M_PI_2);

    calculateVariablesTrajectories(updateTime);

    drawVariablesTrajectories();
}

void MainWindow::drawCircleTrajectory() {
    qreal radius = ui->SB_radius->value() * 1000.0;

    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal step = velocity * updateTime;
    qreal stepAngle = step / radius;
    qreal currAngle = 0;

    mTrajectoryOriginal.append(QPointF(radius, 0));
    do {
        currAngle += stepAngle;
        mTrajectoryOriginal.append(QPointF(radius * qCos(currAngle), radius * qSin(currAngle)));
    } while(currAngle < 1.75 * M_PI);

    calculateVariablesTrajectories(updateTime);

    drawVariablesTrajectories();
}

QVector<QPointF> MainWindow::addNoiseToMeasurements(QVector<QPointF> measurements) {
    std::random_device randomDevice{};
    std::mt19937 generator{randomDevice()};
    std::normal_distribution<> normalDistributionRho{0, ui->DSB_sigmaRho->value()};
    std::normal_distribution<> normalDistributionTheta{0, ui->DSB_sigmaTheta->value() / 60.0};

    QVector<QPointF> measurementsWithNoise;
    bool flag = true;
    for(int i = 0; i < measurements.size(); ++i) {
        qreal rho = sqrt(qPow(measurements[i].x(), 2) + qPow(measurements[i].y(), 2));
        qreal theta = qAtan2(measurements[i].y(), measurements[i].x());

        if(qIsNull(measurements[i].y())) {
            theta = 0;
        }

        theta = M_PI_2 - theta;
        if(theta < 0) {
            theta += (2.0 * M_PI);
        }

        qreal rhoWithNoise = rho + normalDistributionRho(generator);
        qreal thetaWithNoise = theta + qDegreesToRadians(normalDistributionTheta(generator));
        //qreal rhoWithNoise = rho + calculateNoise(ui->DSB_sigmaRho->value(), flag);
        //qreal thetaWithNoise = theta + qDegreesToRadians(calculateNoise(ui->DSB_sigmaTheta->value() / 60.0, flag));

        flag = !flag;
        measurementsWithNoise.append(QPointF(rhoWithNoise * qSin(thetaWithNoise),
                                             rhoWithNoise * qCos(thetaWithNoise)));
    }

    return measurementsWithNoise;
}

qreal MainWindow::calculateNoise(const qreal sigma, bool flag) {
    qreal phi = QRandomGenerator::global()->generateDouble();
    qreal r =  QRandomGenerator::global()->generateDouble();

    if(flag) {
        return sigma * cos(2.0 * 3.14 * phi) * sqrt(-2.0 * log(r));
    } else {
        return sigma * sin(2.0 * 3.14 * phi) * sqrt(-2.0 * log(r));
    }
}

void MainWindow::calculateVariablesTrajectories(qreal updateTime) {

    for(int i = 0; i < NUMBER_MODULATIONS; ++i) {
        mTrackWithNoise.append(addNoiseToMeasurements(mTrajectoryOriginal));

        QVector<Target> targets;
        for(int j = 0; j < mTrackWithNoise.last().size(); ++j) {
            targets.append(Target(mTrackWithNoise.last()[j], j * updateTime));
        }

        mTrackAlphaBetaFilter.append(
                    calcAlphaBetaFilter(targets,
                                        ui->SB_AB_Kmax->value()));

        mTrackAlphaBetaFilterMNK.append(
                    calcAlphaBetaLeastSquaresFilter(targets,
                                                    ui->SB_AB_MNK_Kmax->value(),
                                                    ui->SB_AB_MNK_MNKmax->value()));

        mTrackAlphaBetaFilterWMNK.append(
                    calcAlphaBetaWLeastSquaresFilter(targets));

        mTrackKalmanFilterCV.append(
                    calcKalmanConstVelocityFilter(targets,
                                                  ui->SB_KalmanFilterCV_Kmax->value(),
                                                  ui->DSB_KalmanFilterCV_sigmaNoiseRho->value(),
                                                  qDegreesToRadians(ui->DSB_KalmanFilterCV_sigmaNoiseTheta->value() / 60.0),
                                                  ui->DSB_KalmanFilterCV_sigmaNoiseVelocity->value()));

        mTrackKalmanFilterCA.append(
                    calcKalmanConstAccelerationFilter(targets,
                                                      ui->SB_KalmanFilterCA_Kmax->value(),
                                                      ui->DSB_KalmanFilterCA_sigmaNoiseRho->value(),
                                                      qDegreesToRadians(ui->DSB_KalmanFilterCA_sigmaNoiseTheta->value() / 60.0),
                                                      ui->DSB_KalmanFilterCA_sigmaNoiseVelocity->value(),
                                                      ui->DSB_KalmanFilterCA_sigmaNoiseAcceleration->value()));

        mTrackAdaptiveKalmanFilterCV.append(
                    calcAdaptiveKalmanConstVelocityFilter(targets,
                                                          ui->SB_AdaptiveKalmanFilterCV_Kmax->value(),
                                                          ui->SB_AdaptiveKalmanFilterCV_numberMeasToRecalcR->value(),
                                                          ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseRho->value(),
                                                          qDegreesToRadians(ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseTheta->value() / 60.0),
                                                          ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseVelocity->value()));

    }
}

QVector<QPointF> MainWindow::calcAlphaBetaFilter(QVector<Target> targets, uint16_t k_max) {
    QVector<QPointF> result;
    AlphaBetaFilter abFilter(k_max);
    for(int i = 0; i < 3; ++i) {
        result.append(targets[i].coordinate);
    }
    abFilter.initialization({targets[0], targets[1], targets[2]});
    for(int i = 3; i < targets.size(); ++i) {
        result.append(abFilter.filterMeasuredValue(targets[i]).coordinate);
    }
    return result;
}


QVector<QPointF> MainWindow::calcAlphaBetaLeastSquaresFilter(QVector<Target> targets,
                                                             uint16_t k_max, uint16_t numberToExtrapolation) {
    QVector<QPointF> result;
    AlphaBetaLeastSquaresFilter abLeastSquaresFilter(k_max, numberToExtrapolation);
    for(int i = 0; i < 3; ++i) {
        result.append(targets[i].coordinate);
    }
    abLeastSquaresFilter.initialization({targets[0], targets[1], targets[2]});
    for(int i = 3; i < targets.size(); ++i) {
        result.append(abLeastSquaresFilter.filterMeasuredValue(targets[i]).coordinate);
    }
    return result;
}

QVector<QPointF> MainWindow::calcAlphaBetaWLeastSquaresFilter(QVector<Target> targets) {
    QVector<QPointF> result;
    AlphaBetaWLeastSquaresFilter abWLeastSquaresFilter;
    for(int i = 0; i < 3; ++i) {
        result.append(targets[i].coordinate);
    }
    abWLeastSquaresFilter.initialization({targets[0], targets[1], targets[2]});
    for(int i = 3; i < targets.size(); ++i) {
        result.append(abWLeastSquaresFilter.filterMeasuredValue(targets[i]).coordinate);
    }
    return result;
}

QVector<QPointF> MainWindow::calcKalmanConstVelocityFilter(QVector<Target> targets, quint16 k_max,
                                                           qreal sigmaNoiseRho, qreal sigmaNoiseTheta,
                                                           qreal sigmaNoiseVelocity) {
    QVector<QPointF> result;
    KalmanConstVelocityFilter kalmanConstVelocityFilter(k_max,
                                                        sigmaNoiseRho,
                                                        sigmaNoiseTheta,
                                                        sigmaNoiseVelocity);
    for(int i = 0; i < 3; ++i) {
        result.append(targets[i].coordinate);
    }
    kalmanConstVelocityFilter.initialization({targets[0], targets[1], targets[2]});
    for(int i = 3; i < targets.size(); ++i) {
        result.append(kalmanConstVelocityFilter.filterMeasuredValue(targets[i]).coordinate);
    }
    return result;
}

QVector<QPointF> MainWindow::calcKalmanConstAccelerationFilter(QVector<Target> targets, quint16 k_max,
                                                               qreal sigmaNoiseRho, qreal sigmaNoiseTheta,
                                                               qreal sigmaNoiseVelocity,
                                                               qreal sigmaAcceleration) {
    QVector<QPointF> result;
    KalmanConstAccelerationFilter kalmanConstAccelerationFilter(k_max,
                                                                sigmaNoiseRho,
                                                                sigmaNoiseTheta,
                                                                sigmaNoiseVelocity,
                                                                sigmaAcceleration);
    for(int i = 0; i < 3; ++i) {
        result.append(targets[i].coordinate);
    }
    kalmanConstAccelerationFilter.initialization({targets[0], targets[1], targets[2]});
    for(int i = 3; i < targets.size(); ++i) {
        result.append(kalmanConstAccelerationFilter.filterMeasuredValue(targets[i]).coordinate);
    }
    return result;
}

QVector<QPointF> MainWindow::calcAdaptiveKalmanConstVelocityFilter(QVector<Target> targets,
                                                                   quint16 numberRecalcsP, quint16 numberRecalcsR,
                                                                   qreal sigmaNoiseRho, qreal sigmaNoiseTheta,
                                                                   qreal sigmaNoiseVelocity) {
    QVector<QPointF> result;
    AdaptiveKalmanConstVelocityFilter adaptiveKalmanConstVelocityFilter(numberRecalcsP,
                                                                        numberRecalcsR,
                                                                        sigmaNoiseRho,
                                                                        sigmaNoiseTheta,
                                                                        sigmaNoiseVelocity);
    for(int i = 0; i < 3; ++i) {
        result.append(targets[i].coordinate);
    }
    adaptiveKalmanConstVelocityFilter.initialization({targets[0], targets[1], targets[2]});
    for(int i = 3; i < targets.size(); ++i) {
        result.append(adaptiveKalmanConstVelocityFilter.filterMeasuredValue(targets[i]).coordinate);
    }
    return result;
}

void MainWindow::drawVariablesTrajectories() {
    for(int i = 0; i < mTrajectoryOriginal.size(); ++i) {
        if(ui->CB_original->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(QPointF(mTrajectoryOriginal[i].x() / 1000.0,
                                                        -mTrajectoryOriginal[i].y() / 1000.0),
                                     Qt::green));
        }

        if(ui->CB_withNoise->isChecked()) {
//            m_graphicsScene->addEllipse((mTrackWithNoise[0][i].x()) - 0.1,
//                                       (-mTrackWithNoise[0][i].y()) - 0.1,
//                                       0.2, 0.2,
//                                       QPen(Qt::red, 0.1), QBrush(Qt::red));
            m_graphicsScene->addItem(new TargetItem(QPointF(mTrackWithNoise[0][i].x() / 1000.0,
                                                           -mTrackWithNoise[0][i].y() / 1000.0),
                                     Qt::red));
        }

        if(ui->CB_ABfilter->isChecked()) {
//            m_graphicsScene->addEllipse((mTrackAlphaBetaFilter[0][i].x()) - 0.1,
//                                       (-mTrackAlphaBetaFilter[0][i].y()) - 0.1,
//                                       0.2, 0.2,
//                                       QPen(Qt::green, 0.1), QBrush(Qt::green));

            m_graphicsScene->addItem(new TargetItem(QPointF(mTrackAlphaBetaFilter[0][i].x() / 1000.0,
                                                           -mTrackAlphaBetaFilter[0][i].y() / 1000.0),
                                     Qt::blue));
        }

        if(ui->CB_ABfilterMNK->isChecked()) {
//            m_graphicsScene->addEllipse((mTrackAlphaBetaFilterMNK[0][i].x() / 1000.0) - 0.1,
//                                       (-mTrackAlphaBetaFilterMNK[0][i].y() / 1000.0) - 0.1,
//                                       0.2, 0.2,
//                                       QPen(Qt::blue, 0.1), QBrush(Qt::blue));
            m_graphicsScene->addItem(new TargetItem(QPointF(mTrackAlphaBetaFilterMNK[0][i].x() / 1000.0,
                                                           -mTrackAlphaBetaFilterMNK[0][i].y() / 1000.0),
                                     Qt::blue));
        }

        if(ui->CB_ABfilterWMNK->isChecked()) {
//            m_graphicsScene->addEllipse((mTrackAlphaBetaFilterMNK[0][i].x() / 1000.0) - 0.1,
//                                       (-mTrackAlphaBetaFilterMNK[0][i].y() / 1000.0) - 0.1,
//                                       0.2, 0.2,
//                                       QPen(Qt::blue, 0.1), QBrush(Qt::blue));
            m_graphicsScene->addItem(new TargetItem(QPointF(mTrackAlphaBetaFilterWMNK[0][i].x() / 1000.0,
                                                           -mTrackAlphaBetaFilterWMNK[0][i].y() / 1000.0),
                                     Qt::blue));
        }

        if(ui->CB_KalmanFilterCV->isChecked()) {
//            m_graphicsScene->addEllipse((mTrackKalmanFilterCV[0][i].x() / 1000.0) - 0.1,
//                                       (-mTrackKalmanFilterCV[0][i].y() / 1000.0) - 0.1,
//                                       0.2, 0.2,
//                                       QPen(Qt::yellow, 0.1), QBrush(Qt::yellow));
            m_graphicsScene->addItem(new TargetItem(QPointF(mTrackKalmanFilterCV[0][i].x() / 1000.0,
                                                           -mTrackKalmanFilterCV[0][i].y() / 1000.0),
                                     Qt::blue));
        }

        if(ui->CB_KalmanFilterCA->isChecked()) {
//            m_graphicsScene->addEllipse((mTrackKalmanFilterCA[0][i].x() / 1000.0) - 0.1,
//                                       (-mTrackKalmanFilterCA[0][i].y() / 1000.0) - 0.1,
//                                       0.2, 0.2,
//                                       QPen(Qt::magenta, 0.1), QBrush(Qt::magenta));
            m_graphicsScene->addItem(new TargetItem(QPointF(mTrackKalmanFilterCA[0][i].x() / 1000.0,
                                                           -mTrackKalmanFilterCA[0][i].y() / 1000.0),
                                     Qt::magenta));
        }

        if(ui->CB_AdaptiveKalmanFilterCV->isChecked()) {
//            m_graphicsScene->addEllipse((mTrackAdaptiveKalmanFilterCV[0][i].x() / 1000.0) - 0.1,
//                                       (-mTrackAdaptiveKalmanFilterCV[0][i].y() / 1000.0) - 0.1,
//                                       0.2, 0.2,
//                                       QPen(Qt::cyan, 0.1), QBrush(Qt::cyan));
            m_graphicsScene->addItem(new TargetItem(QPointF(mTrackAdaptiveKalmanFilterCV[0][i].x() / 1000.0,
                                                           -mTrackAdaptiveKalmanFilterCV[0][i].y() / 1000.0),
                                     Qt::cyan));
        }
    }
}

void MainWindow::clearModulation() {
    mTrajectoryOriginal.clear();
    mTrackWithNoise.clear();
    mTrackAlphaBetaFilter.clear();
    mTrackAlphaBetaFilterMNK.clear();
    mTrackAlphaBetaFilterWMNK.clear();
    mTrackKalmanFilterCV.clear();
    mTrackKalmanFilterCA.clear();
    mTrackAdaptiveKalmanFilterCV.clear();

    m_graphicsScene->clear();

    createGrid();
}




void MainWindow::on_RB_onLine_toggled(bool checked)
{
    ui->SB_firstPointX->setEnabled(checked);
    ui->SB_firstPointY->setEnabled(checked);
    ui->SB_secondPointX->setEnabled(checked);
    ui->SB_secondPointY->setEnabled(checked);
    ui->SB_radius->setEnabled(!checked);
}


void MainWindow::on_ComB_filters_currentIndexChanged(int index)
{
    ui->SW_filters->setCurrentIndex(index);
}

QPointF MainWindow::polarToCart(qreal rho, qreal theta) {
    return {rho * qCos(theta - M_PI_2), rho * qSin(theta - M_PI_2)};
}
