#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    mGraphicsScene = new QGraphicsScene();

    mGraphicsScene->setSceneRect(
        -RADAR_RANGE_KM, -RADAR_RANGE_KM,
        RADAR_RANGE_KM * 2, RADAR_RANGE_KM * 2
    );

    ui->GW_locator->setScene(mGraphicsScene);
    mGraphicsScene->setBackgroundBrush(QBrush(BLUE));
    ui->GW_locator->scale(0.8, 0.8);

    createGrid();

    ui->PB_buildingGraphics->setEnabled(false);

    connect(ui->LW_filters, &QListWidget::currentRowChanged, this, [this](int row) {
            ui->SW_filters->setCurrentIndex(row);
    });
}

void MainWindow::createGrid() {
    qreal ringDist = SECTOR_RANGE_KM;
    qreal sectorAngle = SECTOR_ANGLE_DEG;

    QPen gridPen(RED_BROWN);
    gridPen.setCosmetic(true);

    for (qreal curDist = ringDist; curDist <= mGraphicsScene->width() / 2; curDist += ringDist) {
        mGraphicsScene->addEllipse(
            -curDist, -curDist,
            curDist * 2, curDist * 2,
            gridPen
        );
    }

    for (qreal curAngle = 0; curAngle < 2 * M_PI; curAngle += qDegreesToRadians(sectorAngle)) {
        qreal x = mGraphicsScene->width() / 2 * qCos(curAngle);
        qreal y = mGraphicsScene->width() / 2 * qSin(curAngle);
        mGraphicsScene->addLine(0, 0, x, y, gridPen);
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
    QVector<QPointF> sigmaKalmanFilterCV;

    for(int i = 0; i < mTrajectoryOriginal.size(); ++i) {
        QPointF currSigmaNoise(0, 0);
        QPointF currSigmaAlphaBetaFilter(0, 0);
        QPointF currSigmaAlphaBetaFilter_MNK(0, 0);
        QPointF currSigmaKalmanFilterCV(0, 0);

        step.append(i);

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

            QPointF diffKalman_CV = (mTrackKalmanFilterCV[indexModulation][i] - mTrajectoryOriginal[i]);
            diffKalman_CV.setX(diffKalman_CV.x() * diffKalman_CV.x());
            diffKalman_CV.setY(diffKalman_CV.y() * diffKalman_CV.y());
            currSigmaKalmanFilterCV += diffKalman_CV;
        }

        sigmaNoise.append(QPointF(qSqrt(currSigmaNoise.x() / NUMBER_MODULATIONS),
                                  qSqrt(currSigmaNoise.y() / NUMBER_MODULATIONS)));

        sigmaAlphaBetaFilter.append(QPointF(qSqrt(currSigmaAlphaBetaFilter.x() / NUMBER_MODULATIONS),
                                            qSqrt(currSigmaAlphaBetaFilter.y() / NUMBER_MODULATIONS)));

        sigmaAlphaBetaFilter_MNK.append(QPointF(qSqrt(currSigmaAlphaBetaFilter_MNK.x() / NUMBER_MODULATIONS),
                                                qSqrt(currSigmaAlphaBetaFilter_MNK.y() / NUMBER_MODULATIONS)));

        sigmaKalmanFilterCV.append(QPointF(qSqrt(currSigmaKalmanFilterCV.x() / NUMBER_MODULATIONS),
                                           qSqrt(currSigmaKalmanFilterCV.y() / NUMBER_MODULATIONS)));
    }



    QVector<qreal> resSigmaNoise;
    QVector<qreal> resSigmaAlphaBetaFilter;
    QVector<qreal> resSigmaAlphaBetaFilter_MNK;
    QVector<qreal> resSigmaKalmanFilterCV;

    for(int i = 0; i < step.size(); ++i) {
        resSigmaNoise.append(qSqrt(qPow(sigmaNoise[i].x(), 2)
                                   + qPow(sigmaNoise[i].y(), 2)));

        resSigmaAlphaBetaFilter.append(qSqrt(qPow(sigmaAlphaBetaFilter[i].x(), 2)
                                             + qPow(sigmaAlphaBetaFilter[i].y(), 2)));

        resSigmaAlphaBetaFilter_MNK.append(qSqrt(qPow(sigmaAlphaBetaFilter_MNK[i].x(), 2)
                                                 + qPow(sigmaAlphaBetaFilter_MNK[i].y(), 2)));

        resSigmaKalmanFilterCV.append(qSqrt(qPow(sigmaKalmanFilterCV[i].x(), 2)
                                            + qPow(sigmaKalmanFilterCV[i].y(), 2)));
    }


    auto window = new GraphicsBuilderWidget();
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setData("Состояние модели", "СКО, метры", step,
                    {resSigmaNoise, resSigmaAlphaBetaFilter, resSigmaAlphaBetaFilter_MNK, resSigmaKalmanFilterCV},
                    {"Шум", "Альфа-бета фильтр", "Альфа-бета фильтр МНК", "Фильтр Калмана CV"});
    window->setWindowModality(Qt::WindowModality::ApplicationModal);
    window->show();
}


void MainWindow::drawLineTrajectory() {
    QPointF currentCoord(ui->SB_firstPointX->value(), -ui->SB_firstPointY->value());
    QPointF endCoord(ui->SB_secondPointX->value(), -ui->SB_secondPointY->value());
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal numberSteps = qSqrt(qPow(currentCoord.x()-endCoord.x(), 2)+qPow(currentCoord.y()-endCoord.y(), 2)) /
                            (velocity * updateTime / 1000.0);
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
    qreal radius = ui->SB_radius->value();
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal step = velocity * updateTime / 1000.0;
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
    qreal radius = ui->SB_radius->value();

    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal step = velocity * updateTime / 1000.0;
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
        //qreal rhoWithNoise = rho + normalDistributionRho(generator);
        //qreal thetaWithNoise = theta + qDegreesToRadians(normalDistributionTheta(generator));
        qreal rhoWithNoise = rho + calculateNoise(ui->DSB_sigmaRho->value(), flag);
        qreal thetaWithNoise = theta + qDegreesToRadians(calculateNoise(ui->DSB_sigmaTheta->value() / 60.0, flag));

        flag = !flag;
        measurementsWithNoise.append(QPointF(rhoWithNoise * qCos(thetaWithNoise),
                                             rhoWithNoise * qSin(thetaWithNoise)));
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

        mTrackAlphaBetaFilter.append(AlphaBetaFilter::calculateFilteredTrajectory(mTrackWithNoise.last(),
                                                                                       updateTime,
                                                                                       ui->SB_AB_Kmax->value()));

        mTrackAlphaBetaFilterMNK.append(AlphaBetaFilter::calculateFilteredTrajectory_MNK(mTrackWithNoise.last(),
                                                                                              updateTime,
                                                                                              ui->SB_AB_MNK_Kmax->value(),
                                                                                              ui->SB_AB_MNK_MNKmax->value()));

        mTrackKalmanFilterCV.append(KalmanFilter::calculateKalmanFilter_CV(mTrackWithNoise.last(),
                                                                                updateTime,
                                                                                ui->DSB_KalmanFilterCV_sigmaNoiseCoord->value(),
                                                                                ui->DSB_KalmanFilterCV_sigmaNoiseVelocity->value(),
                                                                                ui->DSB_KalmanFilterCV_sigmaNoiseAcceleration->value()));
    }
}


void MainWindow::drawVariablesTrajectories() {
    for(int i = 0; i < mTrajectoryOriginal.size(); ++i) {
        if(ui->CB_original->isChecked()) {
            mGraphicsScene->addEllipse(mTrajectoryOriginal[i].x() - 0.1,
                                       mTrajectoryOriginal[i].y() - 0.1,
                                       0.2, 0.2,
                                       QPen(Qt::white, 0.1), QBrush(Qt::white));
        }

        if(ui->CB_withNoise->isChecked()) {
            mGraphicsScene->addEllipse(mTrackWithNoise[0][i].x() - 0.1,
                                       mTrackWithNoise[0][i].y() - 0.1,
                                       0.2, 0.2,
                                       QPen(Qt::red, 0.1), QBrush(Qt::red));
        }

        if(ui->CB_ABfilter->isChecked()) {
            mGraphicsScene->addEllipse(mTrackAlphaBetaFilter[0][i].x() - 0.1,
                                       mTrackAlphaBetaFilter[0][i].y() - 0.1,
                                       0.2, 0.2,
                                       QPen(Qt::green, 0.1), QBrush(Qt::green));
        }

        if(ui->CB_ABfilterMNK->isChecked()) {
            mGraphicsScene->addEllipse(mTrackAlphaBetaFilterMNK[0][i].x() - 0.1,
                                       mTrackAlphaBetaFilterMNK[0][i].y() - 0.1,
                                       0.2, 0.2,
                                       QPen(Qt::blue, 0.1), QBrush(Qt::blue));
        }

        if(ui->CB_KalmanFilterCV->isChecked()) {
            mGraphicsScene->addEllipse(mTrackKalmanFilterCV[0][i].x() - 0.1,
                                       mTrackKalmanFilterCV[0][i].y() - 0.1,
                                       0.2, 0.2,
                                       QPen(Qt::yellow, 0.1), QBrush(Qt::yellow));
        }
    }
}

void MainWindow::clearModulation() {
    mTrajectoryOriginal.clear();
    mTrackWithNoise.clear();
    mTrackAlphaBetaFilter.clear();
    mTrackAlphaBetaFilterMNK.clear();
    mTrackKalmanFilterCV.clear();

    mGraphicsScene->clear();

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

