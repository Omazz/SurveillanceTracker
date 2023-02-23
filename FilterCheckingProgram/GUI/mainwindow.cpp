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
    connect(ui->LW_filters, &QListWidget::currentRowChanged, this, [this](int row) {
        ui->SW_filters->setCurrentIndex(row);
    });

    zoom = new GraphicsViewZoom(ui->GW_locator);
    zoom->setModifiers(Qt::NoModifier);

    for (int i = 1; i <= NUMBER_CIRCLES; ++i) {
        _graphicsScene->addEllipse(-i * MIN_CIRCLE_DIAMETER / 2, -i * MIN_CIRCLE_DIAMETER / 2,
                                   i * MIN_CIRCLE_DIAMETER, i * MIN_CIRCLE_DIAMETER,
                                   QPen(RED, PEN_INDICATOR_WIDTH, Qt::SolidLine));
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



void MainWindow::on_PB_modeling_clicked()
{
    clearModulation();
    if(ui->RB_onLine->isChecked() == true) {
        drawLineTrajectory();

    } else if(ui->RB_turn->isChecked() == true) {
        drawTurnTrajectory();

    } else if(ui->RB_onCircle->isChecked() == true) {
        drawCircleTrajectory();
    }
}


void MainWindow::drawLineTrajectory() {
    QPointF currentCoord(-NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * 0.5, 0);
    QPointF endCoord(0, NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * 0.5);
    qreal velocity = ui->DSB_velocity->value();
    qreal updateTime = ui->DSB_updateTime->value();
    qreal stepX = velocity * updateTime * qCos(M_PI / 4.0);
    qreal stepY = velocity * updateTime * qSin(M_PI / 4.0);
    _trajectoryOriginal.append(currentCoord);

    while(currentCoord.x() < endCoord.x()) {
        currentCoord += QPointF(stepX, stepY);
        _trajectoryOriginal.append(currentCoord);
    }

    _trajectoryWithNoise = addNoiseToMeasurements(_trajectoryOriginal);
    _trajectoryAlphaBetaFilter = AlphaBetaFilter::calculateFilteredTrajectory(_trajectoryWithNoise, updateTime,
                                                                              ui->SB_AB_Kmax->value());
    _trajectoryAlphaBetaFilterMNK = AlphaBetaFilter::calculateFilteredTrajectory_MNK(_trajectoryWithNoise, updateTime,
                                                                              ui->SB_AB_MNK_Kmax->value(),
                                                                              ui->SB_AB_MNK_MNKmax->value());

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

    _trajectoryWithNoise = addNoiseToMeasurements(_trajectoryOriginal);
    _trajectoryAlphaBetaFilter = AlphaBetaFilter::calculateFilteredTrajectory(_trajectoryWithNoise, updateTime,
                                                                              ui->SB_AB_Kmax->value());
    _trajectoryAlphaBetaFilterMNK = AlphaBetaFilter::calculateFilteredTrajectory_MNK(_trajectoryWithNoise, updateTime,
                                                                              ui->SB_AB_MNK_Kmax->value(),
                                                                              ui->SB_AB_MNK_MNKmax->value());

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


    _trajectoryWithNoise = addNoiseToMeasurements(_trajectoryOriginal);
    _trajectoryAlphaBetaFilter = AlphaBetaFilter::calculateFilteredTrajectory(_trajectoryWithNoise, updateTime,
                                                                              ui->SB_AB_Kmax->value());
    _trajectoryAlphaBetaFilterMNK = AlphaBetaFilter::calculateFilteredTrajectory_MNK(_trajectoryWithNoise, updateTime,
                                                                              ui->SB_AB_MNK_Kmax->value(),
                                                                              ui->SB_AB_MNK_MNKmax->value());

    drawVariablesTrajectories();
}

QVector<QPointF> MainWindow::addNoiseToMeasurements(QVector<QPointF> measurements) {
    std::random_device randomDevice{};
    std::mt19937 generator{randomDevice()};
    std::normal_distribution<> normalDistributionRho{0, ui->DSB_sigmaRho->value()};
    std::normal_distribution<> normalDistributionTheta{0, ui->DSB_sigmaTheta->value()};

    QVector<QPointF> measurementsWithNoise;

    for(int i = 0; i < measurements.size(); ++i) {
        qreal rho = sqrt(qPow(measurements[i].x(), 2) + qPow(measurements[i].y(), 2));
        qreal theta = qAtan2(measurements[i].y(), measurements[i].x());
        qreal rhoWithNoise = rho + normalDistributionRho(generator);
        qreal thetaWithNoise = theta + qDegreesToRadians(normalDistributionTheta(generator));

        measurementsWithNoise.append(QPointF(rhoWithNoise * qCos(thetaWithNoise),
                                             rhoWithNoise * qSin(thetaWithNoise)));
    }

    return measurementsWithNoise;
}

void MainWindow::drawVariablesTrajectories() {
    for(int i = 0; i < _trajectoryOriginal.size() - 1; ++i) {

        _graphicsScene->addLine(QLine(_trajectoryOriginal[i].x(), _trajectoryOriginal[i].y(),
                                      _trajectoryOriginal[i + 1].x(), _trajectoryOriginal[i + 1].y()),
                                QPen(Qt::white, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        if(ui->CB_withNoise->isChecked()) {
            _graphicsScene->addLine(QLine(_trajectoryWithNoise[i].x(), _trajectoryWithNoise[i].y(),
                                          _trajectoryWithNoise[i + 1].x(), _trajectoryWithNoise[i + 1].y()),
                                    QPen(Qt::red, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        }

        if(ui->CB_ABfilter->isChecked()) {
            _graphicsScene->addLine(QLine(_trajectoryAlphaBetaFilter[i].x(), _trajectoryAlphaBetaFilter[i].y(),
                                          _trajectoryAlphaBetaFilter[i + 1].x(), _trajectoryAlphaBetaFilter[i + 1].y()),
                                    QPen(Qt::green, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
        }

        if(ui->CB_ABfilterMNK->isChecked()) {
            _graphicsScene->addLine(QLine(_trajectoryAlphaBetaFilterMNK[i].x(), _trajectoryAlphaBetaFilterMNK[i].y(),
                                          _trajectoryAlphaBetaFilterMNK[i + 1].x(), _trajectoryAlphaBetaFilterMNK[i + 1].y()),
                                    QPen(Qt::blue, PEN_TRAJECTORY_WIDTH, Qt::PenStyle::SolidLine));
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
    }

    for(qreal i = 0; 2.0 * M_PI - i > 0.1e-6; i += M_PI_4) {
        qreal x_cord = 0.5 * NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * qSin(i);
        qreal y_cord = 0.5 * NUMBER_CIRCLES * MIN_CIRCLE_DIAMETER * qCos(i);
        _graphicsScene->addLine(QLine(0, 0, x_cord, y_cord),
                                QPen(RED, PEN_INDICATOR_WIDTH, Qt::SolidLine));
    }
}
