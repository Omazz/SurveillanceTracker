#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    setFixedSize(this->width(), this->height());
    ui->setupUi(this);
    _graphicsScene = new QGraphicsScene();
    ui->GW_locator->setScene(_graphicsScene);
    _graphicsScene->setBackgroundBrush(QBrush(BLUE));
    ui->GW_locator->scale(0.08, 0.08);
    ui->PB_buildingGraphics->setEnabled(false);
    connect(ui->LW_filters, &QListWidget::currentRowChanged, this, [this](int row) {
        ui->SW_filters->setCurrentIndex(row);
    });

    zoom = new GraphicsViewZoom(ui->GW_locator);
    zoom->setModifiers(Qt::NoModifier);

    for (int i = 1; i <= NUMBER_CIRCLES; ++i) {
        _graphicsScene->addEllipse(-i * MIN_CIRCLE_DIAMETER / 2, -i * MIN_CIRCLE_DIAMETER / 2,
                                   i * MIN_CIRCLE_DIAMETER, i * MIN_CIRCLE_DIAMETER,
                                   QPen(RED, PEN_INDICATOR_WIDTH, Qt::SolidLine));

        QFont serifFont("Times", 14, QFont::Bold);
        auto text = _graphicsScene->addSimpleText(QString::number(0.5 * i * MIN_CIRCLE_DIAMETER).append(" м"), serifFont);
        text->setPen(QPen(Qt::white));
        text->setBrush(QBrush(Qt::white));

        text->setPos(10, 0.5 * i * MIN_CIRCLE_DIAMETER - 30);
    }

    for(qreal i = 0; 2.0 * M_PI - i > 0.1e-6; i += M_PI_4) {
        qreal x_cord = 0.5 * NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * qSin(i);
        qreal y_cord = 0.5 * NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * qCos(i);
        _graphicsScene->addLine(QLine(0, 0, x_cord, y_cord),
                                QPen(RED, PEN_INDICATOR_WIDTH, Qt::SolidLine));
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

    for(int i = 0; i < _trajectoryOriginal.size(); ++i) {
        QPointF currSigmaNoise(0, 0);
        QPointF currSigmaAlphaBetaFilter(0, 0);
        QPointF currSigmaAlphaBetaFilter_MNK(0, 0);
        QPointF currSigmaKalmanFilterCV(0, 0);

        step.append(i);

        for(int indexModulation = 0; indexModulation < NUMBER_MODULATIONS; ++indexModulation) {
            QPointF diffNoise = (_trajectoryWithNoise[indexModulation][i] - _trajectoryOriginal[i]);
            diffNoise.setX(diffNoise.x() * diffNoise.x());
            diffNoise.setY(diffNoise.y() * diffNoise.y());
            currSigmaNoise += diffNoise;

            QPointF diffAB = (_trajectoryAlphaBetaFilter[indexModulation][i] - _trajectoryOriginal[i]);
            diffAB.setX(diffAB.x() * diffAB.x());
            diffAB.setY(diffAB.y() * diffAB.y());
            currSigmaAlphaBetaFilter += diffAB;

            QPointF diffAB_MNK = (_trajectoryAlphaBetaFilterMNK[indexModulation][i] - _trajectoryOriginal[i]);
            diffAB_MNK.setX(diffAB_MNK.x() * diffAB_MNK.x());
            diffAB_MNK.setY(diffAB_MNK.y() * diffAB_MNK.y());
            currSigmaAlphaBetaFilter_MNK += diffAB_MNK;

            QPointF diffKalman_CV = (_trajectoryKalmanFilterCV[indexModulation][i] - _trajectoryOriginal[i]);
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
    QPointF currentCoord(-NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * 0.5 * qSin(M_PI_4), -NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * 0.5 * qSin(M_PI_4));
    QPointF endCoord(NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * 0.5 * qSin(M_PI_4), NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * 0.5* qSin(M_PI_4));
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal stepX = velocity * updateTime * qCos(M_PI / 4.0);
    qreal stepY = velocity * updateTime * qSin(M_PI / 4.0);
    _trajectoryOriginal.append(currentCoord);

    while(currentCoord.x() < endCoord.x()) {
        currentCoord += QPointF(stepX, stepY);
        _trajectoryOriginal.append(currentCoord);
    }

    calculateVariablesTrajectories(updateTime);

    drawVariablesTrajectories();
}

void MainWindow::drawTurnTrajectory() {
    qreal radius = 0.5 * NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER;


    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal step = velocity * updateTime;
    qreal stepAngle = step / radius;
    qreal currAngle = 0;

    do {
        currAngle += stepAngle;
        _trajectoryOriginal.append(QPointF((radius * qCos(currAngle)) - radius, (radius * qSin(currAngle)) - radius));
    } while(currAngle <  M_PI_2);

    calculateVariablesTrajectories(updateTime);

    drawVariablesTrajectories();
}

void MainWindow::drawCircleTrajectory() {
    qreal radius = (NUMBER_CIRCLES + 0.5) / 2.0 * MIN_CIRCLE_DIAMETER / 2.0;

    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal step = velocity * updateTime;
    qreal stepAngle = step / radius;
    qreal currAngle = 0;

    _trajectoryOriginal.append(QPointF(radius, 0));
    do {
        currAngle += stepAngle;
        _trajectoryOriginal.append(QPointF(radius * qCos(currAngle), radius * qSin(currAngle)));
    } while(currAngle < 1.75 * M_PI);

    calculateVariablesTrajectories(updateTime);

    drawVariablesTrajectories();
}

QVector<QPointF> MainWindow::addNoiseToMeasurements(QVector<QPointF> measurements) {
    std::random_device randomDevice{};
    std::mt19937 generator{randomDevice()};
    std::normal_distribution<> normalDistributionRho{0, ui->DSB_sigmaRho->value()};
    std::normal_distribution<> normalDistributionTheta{0, ui->DSB_sigmaTheta->value()};

    QVector<QPointF> measurementsWithNoise;
    bool flag = true;
    for(int i = 0; i < measurements.size(); ++i) {
        qreal rho = sqrt(qPow(measurements[i].x(), 2) + qPow(measurements[i].y(), 2));
        qreal theta = qAtan2(measurements[i].y(), measurements[i].x());
        //qreal rhoWithNoise = rho + normalDistributionRho(generator);
        //qreal thetaWithNoise = theta + qDegreesToRadians(normalDistributionTheta(generator));
        qreal rhoWithNoise = rho + calculateNoise(ui->DSB_sigmaRho->value(), flag);
        qreal thetaWithNoise = theta + qDegreesToRadians(calculateNoise(ui->DSB_sigmaTheta->value(), flag));

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
        _trajectoryWithNoise.append(addNoiseToMeasurements(_trajectoryOriginal));

        _trajectoryAlphaBetaFilter.append(AlphaBetaFilter::calculateFilteredTrajectory(_trajectoryWithNoise.last(),
                                                                                       updateTime,
                                                                                       ui->SB_AB_Kmax->value()));

        _trajectoryAlphaBetaFilterMNK.append(AlphaBetaFilter::calculateFilteredTrajectory_MNK(_trajectoryWithNoise.last(),
                                                                                              updateTime,
                                                                                              ui->SB_AB_MNK_Kmax->value(),
                                                                                              ui->SB_AB_MNK_MNKmax->value()));

        _trajectoryKalmanFilterCV.append(KalmanFilter::calculateKalmanFilter_CV(_trajectoryWithNoise.last(),
                                                                                updateTime,
                                                                                ui->DSB_KalmanFilterCV_sigmaNoiseCoord->value(),
                                                                                ui->DSB_KalmanFilterCV_sigmaNoiseVelocity->value(),
                                                                                ui->DSB_KalmanFilterCV_sigmaNoiseAcceleration->value()));
    }
}


void MainWindow::drawVariablesTrajectories() {
    for(int i = 0; i < _trajectoryOriginal.size() - 1; ++i) {

        _graphicsScene->addLine(QLine(_trajectoryOriginal[i].x(), _trajectoryOriginal[i].y(),
                                      _trajectoryOriginal[i + 1].x(), _trajectoryOriginal[i + 1].y()),
                                QPen(Qt::white, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        if(ui->CB_withNoise->isChecked()) {
            _graphicsScene->addLine(QLine(_trajectoryWithNoise[0][i].x(), _trajectoryWithNoise[0][i].y(),
                                          _trajectoryWithNoise[0][i + 1].x(), _trajectoryWithNoise[0][i + 1].y()),
                                    QPen(Qt::red, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        }

        if(ui->CB_ABfilter->isChecked()) {
            _graphicsScene->addLine(QLine(_trajectoryAlphaBetaFilter[0][i].x(), _trajectoryAlphaBetaFilter[0][i].y(),
                                          _trajectoryAlphaBetaFilter[0][i + 1].x(), _trajectoryAlphaBetaFilter[0][i + 1].y()),
                                    QPen(Qt::green, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        }

        if(ui->CB_ABfilterMNK->isChecked()) {
            _graphicsScene->addLine(QLine(_trajectoryAlphaBetaFilterMNK[0][i].x(), _trajectoryAlphaBetaFilterMNK[0][i].y(),
                                          _trajectoryAlphaBetaFilterMNK[0][i + 1].x(), _trajectoryAlphaBetaFilterMNK[0][i + 1].y()),
                                    QPen(Qt::blue, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        }

        if(ui->CB_KalmanFilterCV->isChecked()) {
            _graphicsScene->addLine(QLine(_trajectoryKalmanFilterCV[0][i].x(), _trajectoryKalmanFilterCV[0][i].y(),
                                          _trajectoryKalmanFilterCV[0][i + 1].x(), _trajectoryKalmanFilterCV[0][i + 1].y()),
                                    QPen(Qt::yellow, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        }
    }
}

void MainWindow::clearModulation() {
    _trajectoryOriginal.clear();
    _trajectoryWithNoise.clear();
    _trajectoryAlphaBetaFilter.clear();
    _trajectoryAlphaBetaFilterMNK.clear();
    _trajectoryKalmanFilterCV.clear();

    _graphicsScene->clear();

    for (int i = 1; i <= NUMBER_CIRCLES; ++i) {
        _graphicsScene->addEllipse(-i * MIN_CIRCLE_DIAMETER / 2, -i * MIN_CIRCLE_DIAMETER / 2,
                                   i * MIN_CIRCLE_DIAMETER, i * MIN_CIRCLE_DIAMETER,
                                   QPen(RED, PEN_INDICATOR_WIDTH, Qt::SolidLine));
        QFont serifFont("Times", 14, QFont::Bold);
        auto text = _graphicsScene->addSimpleText(QString::number(0.5 * i * MIN_CIRCLE_DIAMETER).append(" м"), serifFont);
        text->setPen(QPen(Qt::white));
        text->setBrush(QBrush(Qt::white));

        text->setPos(10, 0.5 * i * MIN_CIRCLE_DIAMETER - 30);
    }

    for(qreal i = 0; 2.0 * M_PI - i > 0.1e-6; i += M_PI_4) {
        qreal x_cord = 0.5 * NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * qSin(i);
        qreal y_cord = 0.5 * NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * qCos(i);
        _graphicsScene->addLine(QLine(0, 0, x_cord, y_cord),
                                QPen(RED, PEN_INDICATOR_WIDTH, Qt::SolidLine));
    }
}



