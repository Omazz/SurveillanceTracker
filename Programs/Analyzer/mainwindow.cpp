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
                -RADAR_RANGE_KM,
                -RADAR_RANGE_KM,
                 RADAR_RANGE_KM * 2,
                 RADAR_RANGE_KM * 2
    );

    ui->GW_locator->setScene(m_graphicsScene);
    m_graphicsScene->setBackgroundBrush(QBrush(GRAY));
    ui->GW_locator->scale(0.8, 0.8);
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

    for(qreal curDist = ringDist; curDist <= m_graphicsScene->width() / 2; curDist += ringDist) {
        m_graphicsScene->addEllipse(
            -curDist, -curDist,
            curDist * 2, curDist * 2,
            gridPen
        );
    }

    for(qreal curAngle = 0; curAngle < 2 * M_PI; curAngle += qDegreesToRadians(sectorAngle)) {
        qreal x = m_graphicsScene->width() / 2 * qCos(curAngle);
        qreal y = m_graphicsScene->width() / 2 * qSin(curAngle);
        m_graphicsScene->addLine(0, 0, x, y, gridPen);
    }
}

MainWindow::~MainWindow() {
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
    QVector<qreal> steps;

    for(int i = 0; i < m_originalTrack.size(); ++i) {
        steps.append(i * ui->DSB_updateTime->value());
    }

    auto window = new GraphicsBuilderWidget();
    window->setAttribute(Qt::WA_DeleteOnClose);
    QVector<QVector<qreal>> plots;
    QVector<QString> labels;

    if(ui->CB_withNoise->isChecked()) {
        plots.append(calculateStandardDeviation(m_noiseTrack));
        labels.append("Шум");
    }

    if(ui->CB_ABfilter->isChecked()) {
        plots.append(calculateStandardDeviation(m_alphaBetaTrack));
        labels.append("α-β");
    }

    if(ui->CB_ABfilterWLSM->isChecked()) {
        plots.append(calculateStandardDeviation(m_alphaBetaWlsmTrack));
        labels.append("α-β ВМНК");
    }

    if(ui->CB_KalmanFilterCV->isChecked()) {
        plots.append(calculateStandardDeviation(m_kalmanConstVelocityTrack));
        labels.append("Калман CV");
    }

    if(ui->CB_KalmanFilterCA->isChecked()) {
        plots.append(calculateStandardDeviation(m_kalmanConstAccelerationTrack));
        labels.append("Калман CA");
    }

    if(ui->CB_AdaptiveKalmanFilterCV->isChecked()) {
        plots.append(calculateStandardDeviation(m_adaptiveKalmanConstVelocityTrack));
        labels.append("Адаптивный Калман");
    }

    window->setData("Время, с",
                    "СКО, метры",
                    steps,
                    plots,
                    labels);
    window->setWindowModality(Qt::WindowModality::ApplicationModal);

    window->show();
}

QVector<qreal> MainWindow::calculateStandardDeviation(QVector<QVector<QPointF>> track) {
    QVector<qreal> standartDeviation;

    for(int i = 0; i < m_originalTrack.size(); ++i) {
        QPointF currDeviation(0, 0);

        for(int indexModulation = 0; indexModulation < NUMBER_MODULATIONS; ++indexModulation) {
            QPointF diffNoise = (track[indexModulation][i] - m_originalTrack[i]);
            diffNoise.setX(diffNoise.x() * diffNoise.x());
            diffNoise.setY(diffNoise.y() * diffNoise.y());
            currDeviation += diffNoise;
        }

        standartDeviation.append(qSqrt((currDeviation.x() / NUMBER_MODULATIONS) +
                                       (currDeviation.y() / NUMBER_MODULATIONS)));
    }

    return standartDeviation;
}


void MainWindow::drawLineTrajectory() {
    QPointF currentCoord(ui->SB_firstPointX->value() * 1000.0, ui->SB_firstPointY->value() * 1000.0);
    QPointF endCoord(ui->SB_secondPointX->value() * 1000.0, ui->SB_secondPointY->value() * 1000.0);
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal numberSteps = qSqrt(qPow(currentCoord.x()-endCoord.x(), 2)+qPow(currentCoord.y()-endCoord.y(), 2)) /
                            (velocity * updateTime);
    QPointF step = (endCoord - currentCoord) / numberSteps;

    /* Создание исходной траектории */
    m_originalTrack.append(currentCoord);
    int currentStep = 0;
    while(currentStep < numberSteps) {
        currentCoord += step;
        m_originalTrack.append(currentCoord);
        currentStep++;
    }

    calculateTracks();

    drawTracks();
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
        m_originalTrack.append(QPointF((radius * qCos(currAngle)) - radius, (radius * qSin(currAngle)) - radius));
    } while(currAngle <  M_PI_2);

    calculateTracks();

    drawTracks();
}

void MainWindow::drawCircleTrajectory() {
    qreal radius = ui->SB_radius->value() * 1000.0;

    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal step = velocity * updateTime;
    qreal stepAngle = step / radius;
    qreal currAngle = 0;

    m_originalTrack.append(QPointF(radius, 0));
    do {
        currAngle += stepAngle;
        m_originalTrack.append(QPointF(radius * qCos(currAngle), radius * qSin(currAngle)));
    } while(currAngle < 1.75 * M_PI);

    calculateTracks();

    drawTracks();
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

        flag = !flag;
        measurementsWithNoise.append(QPointF(rhoWithNoise * qSin(thetaWithNoise),
                                             rhoWithNoise * qCos(thetaWithNoise)));
    }

    return measurementsWithNoise;
}

void MainWindow::calculateTracks() {
    qreal updateTime = ui->DSB_updateTime->value();

    for(int i = 0; i < NUMBER_MODULATIONS; ++i) {
        /* Создание зашумленной траектории */
        m_noiseTrack.append(addNoiseToMeasurements(m_originalTrack));

        QVector<Target> targets;
        for(int j = 0; j < m_originalTrack.size(); ++j) {
            targets.append(Target(m_noiseTrack.last()[j], j * updateTime));
        }

        /* Альфа-бета фильтр */
        if(ui->CB_ABfilter->isChecked()) {
            AlphaBetaFilter filter(ui->SB_AB_Kmax->value());
            m_alphaBetaTrack.append(QVector<QPointF>());
            for(int i = 0; i < 3; ++i) {
                m_alphaBetaTrack.last().append(targets[i].coordinate);
            }
            filter.initialization({targets[0], targets[1], targets[2]});
            for(int i = 3; i < targets.size(); ++i) {
                m_alphaBetaTrack.last().append(filter.filterMeasuredValue(targets[i]).coordinate);
            }
        }

        /* Альфа-бета фильтр с взвешенным методом наименьших квадратов */
        if(ui->CB_ABfilterWLSM->isChecked()) {
            AlphaBetaWLeastSquaresFilter filter;
            m_alphaBetaWlsmTrack.append(QVector<QPointF>());
            for(int i = 0; i < 3; ++i) {
                m_alphaBetaWlsmTrack.last().append(targets[i].coordinate);
            }
            filter.initialization({targets[0], targets[1], targets[2]});
            for(int i = 3; i < targets.size(); ++i) {
                m_alphaBetaWlsmTrack.last().append(filter.filterMeasuredValue(targets[i]).coordinate);
            }
        }

        /* Линейный фильтр Калмана */
        if(ui->CB_KalmanFilterCV->isChecked()) {
            KalmanConstVelocityFilter filter(ui->SB_KalmanFilterCV_Kmax->value(),
                                             ui->DSB_KalmanFilterCV_sigmaNoiseRho->value(),
                                             qDegreesToRadians(ui->DSB_KalmanFilterCV_sigmaNoiseTheta->value() / 60.0),
                                             ui->DSB_KalmanFilterCV_sigmaNoiseVelocity->value()
                                             );
            m_kalmanConstVelocityTrack.append(QVector<QPointF>());
            for(int i = 0; i < 3; ++i) {
                m_kalmanConstVelocityTrack.last().append(targets[i].coordinate);
            }
            filter.initialization({targets[0], targets[1], targets[2]});
            for(int i = 3; i < targets.size(); ++i) {
                m_kalmanConstVelocityTrack.last().append(filter.filterMeasuredValue(targets[i]).coordinate);
            }
        }

        /* Квадартичный фильтр Калмана */
        if(ui->CB_KalmanFilterCA->isChecked()) {
            KalmanConstAccelerationFilter filter(ui->SB_KalmanFilterCA_Kmax->value(),
                                                 ui->DSB_KalmanFilterCA_sigmaNoiseRho->value(),
                                                 qDegreesToRadians(ui->DSB_KalmanFilterCA_sigmaNoiseTheta->value() / 60.0),
                                                 ui->DSB_KalmanFilterCA_sigmaNoiseVelocity->value(),
                                                 ui->DSB_KalmanFilterCA_sigmaNoiseAcceleration->value()
                                                 );
            m_kalmanConstAccelerationTrack.append(QVector<QPointF>());
            for(int i = 0; i < 3; ++i) {
                m_kalmanConstAccelerationTrack.last().append(targets[i].coordinate);
            }
            filter.initialization({targets[0], targets[1], targets[2]});
            for(int i = 3; i < targets.size(); ++i) {
                m_kalmanConstAccelerationTrack.last().append(filter.filterMeasuredValue(targets[i]).coordinate);
            }
        }

        /* Адаптивный фильтр Калмана */
        if(ui->CB_AdaptiveKalmanFilterCV->isChecked()) {
            AdaptiveKalmanConstVelocityFilter filter(ui->SB_AdaptiveKalmanFilterCV_Kmax->value(),
                                                     ui->SB_AdaptiveKalmanFilterCV_numberMeasToRecalcR->value(),
                                                     ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseRho->value(),
                                                     qDegreesToRadians(ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseTheta->value() / 60.0),
                                                     ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseVelocity->value()
                                                     );
            m_adaptiveKalmanConstVelocityTrack.append(QVector<QPointF>());
            for(int i = 0; i < 3; ++i) {
                m_adaptiveKalmanConstVelocityTrack.last().append(targets[i].coordinate);
            }
            filter.initialization({targets[0], targets[1], targets[2]});
            for(int i = 3; i < targets.size(); ++i) {
                m_adaptiveKalmanConstVelocityTrack.last().append(filter.filterMeasuredValue(targets[i]).coordinate);
            }
        }
    }

}


void MainWindow::drawTracks() {
    for(int i = 0; i < m_originalTrack.size(); ++i) {
        if(ui->CB_original->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(m_originalTrack[i].x() / 1000.0,
                                                -m_originalTrack[i].y() / 1000.0),
                                         Qt::green)
                                     );
        }

        if(ui->CB_withNoise->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(m_noiseTrack[0][i].x() / 1000.0,
                                                -m_noiseTrack[0][i].y() / 1000.0),
                                         Qt::red)
                                    );
        }

        if(ui->CB_ABfilter->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(m_alphaBetaTrack[0][i].x() / 1000.0,
                                                -m_alphaBetaTrack[0][i].y() / 1000.0),
                                         Qt::blue)
                                    );
        }

        if(ui->CB_ABfilterWLSM->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(m_alphaBetaWlsmTrack[0][i].x() / 1000.0,
                                                -m_alphaBetaWlsmTrack[0][i].y() / 1000.0),
                                         Qt::darkYellow)
                                    );
        }

        if(ui->CB_KalmanFilterCV->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(m_kalmanConstVelocityTrack[0][i].x() / 1000.0,
                                                -m_kalmanConstVelocityTrack[0][i].y() / 1000.0),
                                         Qt::darkRed)
                                    );
        }

        if(ui->CB_KalmanFilterCA->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(m_kalmanConstAccelerationTrack[0][i].x() / 1000.0,
                                                -m_kalmanConstAccelerationTrack[0][i].y() / 1000.0),
                                     Qt::magenta)
                                    );
        }

        if(ui->CB_AdaptiveKalmanFilterCV->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(QPointF(m_adaptiveKalmanConstVelocityTrack[0][i].x() / 1000.0,
                                                           -m_adaptiveKalmanConstVelocityTrack[0][i].y() / 1000.0),
                                     Qt::cyan));
        }
    }
}

void MainWindow::clearModulation() {
    m_originalTrack.clear();
    m_noiseTrack.clear();
    m_alphaBetaTrack.clear();
    m_alphaBetaWlsmTrack.clear();
    m_kalmanConstVelocityTrack.clear();
    m_kalmanConstAccelerationTrack.clear();
    m_adaptiveKalmanConstVelocityTrack.clear();

    m_graphicsScene->clear();

    createGrid();
}

void MainWindow::on_RB_onLine_toggled(bool checked) {
    ui->SB_firstPointX->setEnabled(checked);
    ui->SB_firstPointY->setEnabled(checked);
    ui->SB_secondPointX->setEnabled(checked);
    ui->SB_secondPointY->setEnabled(checked);
    ui->SB_radius->setEnabled(!checked);
}

void MainWindow::on_ComB_filters_currentIndexChanged(int index) {
    ui->SW_filters->setCurrentIndex(index);
}

QPointF MainWindow::polarToCart(qreal rho, qreal theta) {
    return {rho * qCos(theta - M_PI_2), rho * qSin(theta - M_PI_2)};
}
