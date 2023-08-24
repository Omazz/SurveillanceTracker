#include "MainWindow.h"
#include "ui_MainWindow.h"

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

    m_trackHandler = new TrackHandler();
    m_workerThread = new QThread();
    m_trackHandler->moveToThread(m_workerThread);
    m_workerThread->start();
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
    initializationScene();
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
    QVector<QPointF> originalTrack = m_trackHandler->originalTrack();

    for(int i = 0; i < originalTrack.size(); ++i) {
        steps.append(i * ui->DSB_updateTime->value());
    }

    auto window = new GraphicsBuilderWidget();
    window->setAttribute(Qt::WA_DeleteOnClose);
    QVector<QVector<qreal>> plots;
    QVector<QString> labels;

    if(ui->CB_withNoise->isChecked() && !m_trackHandler->noiseTrack().empty()) {
        plots.append(m_trackHandler->calculateStandardDeviation(m_trackHandler->noiseTrack()));
        labels.append("Шум");
    }

    if(ui->CB_ABfilter->isChecked() && !m_trackHandler->alphaBetaTrack().empty()) {
        plots.append(m_trackHandler->calculateStandardDeviation(m_trackHandler->alphaBetaTrack()));
        labels.append("α-β");
    }

    if(ui->CB_ABfilterWLSM->isChecked() && !m_trackHandler->alphaBetaWlsmTrack().empty()) {
        plots.append(m_trackHandler->calculateStandardDeviation(m_trackHandler->alphaBetaWlsmTrack()));
        labels.append("α-β ВМНК");
    }

    if(ui->CB_KalmanFilterCV->isChecked() && !m_trackHandler->kalmanConstVelocityTrack().empty()) {
        plots.append(m_trackHandler->calculateStandardDeviation(m_trackHandler->kalmanConstVelocityTrack()));
        labels.append("Калман CV");
    }

    if(ui->CB_KalmanFilterCA->isChecked() && !m_trackHandler->kalmanConstAccelerationTrack().empty()) {
        plots.append(m_trackHandler->calculateStandardDeviation(m_trackHandler->kalmanConstAccelerationTrack()));
        labels.append("Калман CA");
    }

    if(ui->CB_AdaptiveKalmanFilterCV->isChecked() && !m_trackHandler->adaptiveKalmanConstVelocityTrack().empty()) {
        plots.append(m_trackHandler->calculateStandardDeviation(m_trackHandler->adaptiveKalmanConstVelocityTrack()));
        labels.append("Адаптивный Калман");
    }

    window->setData(
                    "Время, с",
                    "СКО, метры",
                    steps,
                    plots,
                    labels
                   );

    window->setWindowModality(Qt::WindowModality::ApplicationModal);

    window->show();
}


void MainWindow::drawLineTrajectory() {
    QPointF start(ui->SB_firstPointX->value() * 1000.0, ui->SB_firstPointY->value() * 1000.0);
    QPointF end(ui->SB_secondPointX->value() * 1000.0, ui->SB_secondPointY->value() * 1000.0);
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();

    m_trackHandler->clearTracks();

    m_trackHandler->calculateLineTrack(start, end, velocity, updateTime);

    m_trackHandler->calculateTracks(getFiltrationParams(), ui->DSB_updateTime->value(),
                                   ui->DSB_sigmaRho->value(), ui->DSB_sigmaTheta->value());

    drawTracks();
}

void MainWindow::drawTurnTrajectory() {
    qreal radius = ui->SB_radius->value() * 1000.0;
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();

    m_trackHandler->clearTracks();

    m_trackHandler->calculateTurnTrack(radius, velocity, updateTime);

    m_trackHandler->calculateTracks(getFiltrationParams(), ui->DSB_updateTime->value(),
                                   ui->DSB_sigmaRho->value(), ui->DSB_sigmaTheta->value());

    drawTracks();
}

void MainWindow::drawCircleTrajectory() {
    qreal radius = ui->SB_radius->value() * 1000.0;
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();

    m_trackHandler->clearTracks();

    m_trackHandler->calculateCircleTrack(radius, velocity, updateTime);

    m_trackHandler->calculateTracks(getFiltrationParams(), ui->DSB_updateTime->value(),
                                   ui->DSB_sigmaRho->value(), ui->DSB_sigmaTheta->value());

    drawTracks();
}

void MainWindow::drawTracks() {

    QVector<QPointF> originalTrack = m_trackHandler->originalTrack();
    QVector<QPointF> noiseTrack;
    QVector<QPointF> alphaBetaTrack;
    QVector<QPointF> alphaBetaWlsmTrack;
    QVector<QPointF> kalmanConstVelocityTrack;
    QVector<QPointF> kalmanConstAccelerationTrack;
    QVector<QPointF> adaptiveKalmanConstVelocityTrack;

    if(!m_trackHandler->noiseTrack().empty()) {
        noiseTrack = m_trackHandler->noiseTrack().first();
    }


    if(!m_trackHandler->alphaBetaTrack().empty()) {
        alphaBetaTrack = m_trackHandler->alphaBetaTrack().first();
    }

    if(!m_trackHandler->alphaBetaWlsmTrack().empty()) {
        alphaBetaWlsmTrack = m_trackHandler->alphaBetaWlsmTrack().first();
    }

    if(!m_trackHandler->kalmanConstVelocityTrack().empty()) {
        kalmanConstVelocityTrack = m_trackHandler->kalmanConstVelocityTrack().first();
    }


    if(!m_trackHandler->kalmanConstAccelerationTrack().empty()) {
        kalmanConstAccelerationTrack = m_trackHandler->kalmanConstAccelerationTrack().first();
    }

    if(!m_trackHandler->adaptiveKalmanConstVelocityTrack().empty()) {
        adaptiveKalmanConstVelocityTrack = m_trackHandler->adaptiveKalmanConstVelocityTrack().first();
    }

    for(int i = 0; i < originalTrack.size(); ++i) {
        if(ui->CB_original->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(originalTrack[i].x() / 1000.0,
                                                -originalTrack[i].y() / 1000.0),
                                         Qt::green)
                                     );
        }

        if(ui->CB_withNoise->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(noiseTrack[i].x() / 1000.0,
                                                -noiseTrack[i].y() / 1000.0),
                                         Qt::red)
                                    );
        }

        if(ui->CB_ABfilter->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(alphaBetaTrack[i].x() / 1000.0,
                                                -alphaBetaTrack[i].y() / 1000.0),
                                         Qt::blue)
                                    );
        }

        if(ui->CB_ABfilterWLSM->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(alphaBetaWlsmTrack[i].x() / 1000.0,
                                                -alphaBetaWlsmTrack[i].y() / 1000.0),
                                         Qt::darkYellow)
                                    );
        }

        if(ui->CB_KalmanFilterCV->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(kalmanConstVelocityTrack[i].x() / 1000.0,
                                                -kalmanConstVelocityTrack[i].y() / 1000.0),
                                         Qt::darkRed)
                                    );
        }

        if(ui->CB_KalmanFilterCA->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(kalmanConstAccelerationTrack[i].x() / 1000.0,
                                                -kalmanConstAccelerationTrack[i].y() / 1000.0),
                                         Qt::magenta)
                                    );
        }

        if(ui->CB_AdaptiveKalmanFilterCV->isChecked()) {
            m_graphicsScene->addItem(new TargetItem(
                                         QPointF(adaptiveKalmanConstVelocityTrack[i].x() / 1000.0,
                                                -adaptiveKalmanConstVelocityTrack[i].y() / 1000.0),
                                         Qt::cyan)
                                    );
        }
    }
}

void MainWindow::initializationScene() {
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

FiltrationParams MainWindow::getFiltrationParams() {
    FiltrationParams params;

    if(ui->CB_ABfilter->isChecked()) {
        params.alphaBeta.isExist = true;
        params.alphaBeta.k_max = ui->SB_AB_Kmax->value();
    }

    if(ui->CB_ABfilterWLSM->isChecked()) {
        params.alphaBetaWLS.isExist = true;
        params.alphaBetaWLS.k_max = ui->SB_AB_MNK_Kmax->value();
        params.alphaBetaWLS.WLS_max = ui->SB_AB_MNK_MNKmax->value();
    }

    if(ui->CB_KalmanFilterCV->isChecked()) {
        params.kalmanCV.isExist = true;
        params.kalmanCV.k_max = ui->SB_KalmanFilterCV_Kmax->value();
        params.kalmanCV.rhoMSE = ui->DSB_KalmanFilterCV_sigmaNoiseRho->value();
        params.kalmanCV.thetaMSE = ui->DSB_KalmanFilterCV_sigmaNoiseTheta->value();
        params.kalmanCV.velocityMSE = ui->DSB_KalmanFilterCV_sigmaNoiseVelocity->value();
    }

    if(ui->CB_KalmanFilterCA->isChecked()) {
        params.kalmanCA.isExist = true;
        params.kalmanCA.k_max = ui->SB_KalmanFilterCA_Kmax->value();
        params.kalmanCA.rhoMSE = ui->DSB_KalmanFilterCA_sigmaNoiseRho->value();
        params.kalmanCA.thetaMSE = ui->DSB_KalmanFilterCA_sigmaNoiseTheta->value();
        params.kalmanCA.velocityMSE = ui->DSB_KalmanFilterCA_sigmaNoiseVelocity->value();
        params.kalmanCA.accelerationMSE = ui->DSB_KalmanFilterCA_sigmaNoiseAcceleration->value();
    }

    if(ui->CB_AdaptiveKalmanFilterCV->isChecked()) {
        params.adaptiveKalmanCV.isExist = true;
        params.adaptiveKalmanCV.numberRecalcsP = ui->SB_AdaptiveKalmanFilterCV_Kmax->value();
        params.adaptiveKalmanCV.numberRecalcsR = ui->SB_AdaptiveKalmanFilterCV_numberMeasToRecalcR->value();
        params.adaptiveKalmanCV.rhoMSE = ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseRho->value();
        params.adaptiveKalmanCV.thetaMSE = ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseTheta->value();
        params.adaptiveKalmanCV.velocityMSE = ui->DSB_AdaptiveKalmanFilterCV_sigmaNoiseVelocity->value();
    }

    return params;
}

