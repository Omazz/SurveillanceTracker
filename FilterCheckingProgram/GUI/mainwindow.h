#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtCore>
#include <Filters/AlphaBetaFilter.h>
#include "GraphicsViewZoom.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const QColor BLACK = QColor(0x222831);
const QColor BLUE = QColor(0x2B2E4A);
const QColor RED = QColor(0xE84545);
const QColor RED_BROWN = QColor(0x903749);
const QColor PURPLE = QColor(0x53354A);

const qint16 MIN_CIRCLE_DIAMETER = 1000;
const quint16 NUMBER_CIRCLES = 10;
const quint8 PEN_INDICATOR_WIDTH = 5;


const quint8 PEN_TRAJECTORY_WIDTH = 8;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PB_modeling_clicked();

private:

    void drawLineTrajectory();

    void drawTurnTrajectory();

    void drawCircleTrajectory();

    QVector<QPointF> addNoiseToMeasurements(QVector<QPointF> measurements);

    void drawVariablesTrajectories();

    void clearModulation();

    QVector<QPointF> _trajectoryOriginal;
    QVector<QPointF> _trajectoryWithNoise;
    QVector<QPointF> _trajectoryAlphaBetaFilter;
    QVector<QPointF> _trajectoryAlphaBetaFilterMNK;
    QVector<QPointF> _trajectoryKalmanFilterCV;

    Ui::MainWindow *ui;
    QGraphicsScene* _graphicsScene;
    GraphicsViewZoom* zoom;
};
#endif // MAINWINDOW_H
