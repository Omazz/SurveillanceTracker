#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QMainWindow>
#include <QGraphicsScene>
#include "GraphicsBuilderWidget.h"
#include "RadarView.h"
#include "TargetItem.h"
#include "TrackHandler.h"


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

    void drawTracks();

    void initializationScene();

    FiltrationParams getFiltrationParams();

    TrackHandler m_trackHandler;

    Ui::MainWindow *ui;
    QGraphicsScene* m_graphicsScene;
};
#endif // MAINWINDOW_H
