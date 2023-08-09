#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtCore>
#include "../../PriFiles/Filters/AlphaBetaFilter.h"
#include "../../PriFiles/Filters/AlphaBetaLeastSquaresFilter.h"
#include "../../PriFiles/Filters/KalmanConstVelocityFilter.h"
#include "../../PriFiles/Filters/KalmanConstAccelerationFilter.h"
#include "../../PriFiles/Filters/AdaptiveKalmanConstVelocityFilter.h"
#include "../../PriFiles/Filters/AlphaBetaWLeastSquaresFilter.h"
#include "../../PriFiles/GraphUI/GraphicsBuilderWidget.h"
#include "RadarView.h"
#include "TargetItem.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const QColor BLACK = QColor(0x222831);
const QColor BLUE = QColor(0x2B2E4A);
const QColor RED = QColor(0xE84545);
const QColor RED_BROWN = QColor(0x903749);
const QColor RED_BROWN_RADAR = QColor(0xF0291A);
const QColor PURPLE = QColor(0x53354A);
const QColor WHITE = QColor(0xF6F1F1);
const QColor GRAY = QColor(0x464646);

const qint16 RADAR_RANGE_KM = 500;
const quint8 SECTOR_COUNT = 32;


const qreal PEN_TRAJECTORY_WIDTH = 1;

const quint16 NUMBER_MODULATIONS = 1000;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PB_modeling_clicked();

    void on_PB_buildingGraphics_clicked();

    void on_RB_onLine_toggled(bool checked);

    void on_ComB_filters_currentIndexChanged(int index);

private:;
    void createGrid();

    void drawLineTrajectory();

    void drawTurnTrajectory();

    void drawCircleTrajectory();

    QVector<QPointF> addNoiseToMeasurements(QVector<QPointF> measurements);

    qreal calculateNoise(const qreal sigma, bool flag);

    void calculateVariablesTrajectories(qreal updateTime);

    void drawVariablesTrajectories();

    void clearModulation();

    QVector<QPointF> calcAlphaBetaFilter(QVector<Target> targets, uint16_t k_max);

    QVector<QPointF> calcAlphaBetaLeastSquaresFilter(QVector<Target> targets, uint16_t k_max,
                                                     uint16_t numberToExtrapolation);


    QVector<QPointF> calcAlphaBetaWLeastSquaresFilter(QVector<Target> targets);

    QVector<QPointF> calcKalmanConstVelocityFilter(QVector<Target> targets, uint16_t k_max,
                                                   qreal sigmaNoiseRho, qreal sigmaNoiseTheta,
                                                   qreal sigmaNoiseVelocity);

    QVector<QPointF> calcKalmanConstAccelerationFilter(QVector<Target> targets, uint16_t k_max,
                                                       qreal sigmaNoiseRho, qreal sigmaNoiseTheta,
                                                       qreal sigmaNoiseVelocity, qreal sigmaAcceleration);

    QVector<QPointF> calcAdaptiveKalmanConstVelocityFilter(QVector<Target> targets,
                                                           quint16 numberRecalcsP, quint16 numberRecalcsR,
                                                           qreal sigmaNoiseRho, qreal sigmaNoiseTheta,
                                                           qreal sigmaNoiseVelocity);

    QPointF polarToCart(qreal rho, qreal theta);

    QVector<QPointF> mTrajectoryOriginal;
    QVector<QVector<QPointF>> mTrackWithNoise;
    QVector<QVector<QPointF>> mTrackAlphaBetaFilter;
    QVector<QVector<QPointF>> mTrackAlphaBetaFilterMNK;
    QVector<QVector<QPointF>> mTrackAlphaBetaFilterWMNK;
    QVector<QVector<QPointF>> mTrackKalmanFilterCV;
    QVector<QVector<QPointF>> mTrackKalmanFilterCA;
    QVector<QVector<QPointF>> mTrackAdaptiveKalmanFilterCV;


    Ui::MainWindow *ui;
    QGraphicsScene* m_graphicsScene;
};
#endif // MAINWINDOW_H
