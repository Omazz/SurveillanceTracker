#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtCore>
#include "AlphaBetaFilter.h"
#include "AlphaBetaLeastSquaresFilter.h"
#include "KalmanConstVelocityFilter.h"
#include "KalmanConstAccelerationFilter.h"
#include "AdaptiveKalmanConstVelocityFilter.h"
#include "AlphaBetaWLeastSquaresFilter.h"
#include "GraphicsBuilderWidget.h"
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

    QVector<qreal> calculateStandardDeviation(QVector<QVector<QPointF>> array);

    void drawLineTrajectory();

    void drawTurnTrajectory();

    void drawCircleTrajectory();

    QVector<QPointF> addNoiseToMeasurements(QVector<QPointF> measurements);

    void calculateTracks();

    QVector<QPointF> filterMeasurements(AbstractFilter* const filter, QVector<Target> measurements);

    void drawTracks();

    void clearModulation();

    QPointF polarToCart(qreal rho, qreal theta);

    QVector<QPointF> m_originalTrack;
    QVector<QVector<QPointF>> m_noiseTrack;
    QVector<QVector<QPointF>> m_alphaBetaTrack;
    QVector<QVector<QPointF>> m_alphaBetaWlsmTrack;
    QVector<QVector<QPointF>> m_kalmanConstVelocityTrack;
    QVector<QVector<QPointF>> m_kalmanConstAccelerationTrack;
    QVector<QVector<QPointF>> m_adaptiveKalmanConstVelocityTrack;

    Ui::MainWindow *ui;
    QGraphicsScene* m_graphicsScene;
};
#endif // MAINWINDOW_H
