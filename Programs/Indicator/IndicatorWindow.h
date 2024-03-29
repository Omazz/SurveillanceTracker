#ifndef INDICATORWINDOW_H
#define INDICATORWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QtCore>
#include <QTimer>
#include "MessageHandler.h"
#include "RadarView.h"
#include "TargetItem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class IndicatorWindow; }
QT_END_NAMESPACE

const QColor WHITE = QColor(0xF6F1F1);
const QColor GRAY = QColor(0x464646);
const QColor BLUE = QColor(0x19A7CE);
const QColor RED = QColor(0xE21818);

const qint16 RADAR_RANGE_KM = 500;
const quint8 SECTOR_COUNT = 32;
const int CIRCLE_RADIUS = 8;

const int TIME_TO_REMOVE_PLOT = 5000;
const int TIME_TO_REMOVE_TRACK = 50000;

const quint8 PEN_INDICATOR_WIDTH = 1;


struct TargetWithTimer{
    QSharedPointer<TargetItem> target;
    QSharedPointer<QTimer> timer;

    TargetWithTimer(TargetItem* t, QTimer* tm) :
            target(t),
            timer(tm)
    {}
};

class IndicatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    IndicatorWindow(QWidget *parent = nullptr);

    void createGrid();

    QPointF polarToCart(qreal rho, qreal theta);

    ~IndicatorWindow();

public slots:
    void changeSector(qreal angle_deg);

    void onNewPlot(qreal rho_km, qreal angle_rad);

    void onNewTrack(qreal x_km, qreal y_km, qreal directionAngle_rad);

    void onNewExtrapolatedTrack(qreal x_km, qreal y_km, qreal directionAngle_rad);

    void onVisiblePlotsChanged();

    void onTimeoutPlot();

    void onTimeoutTrack();

private:
    Ui::IndicatorWindow *ui;
    QGraphicsScene* m_graphicsScene;
    QGraphicsLineItem* m_line;
    QVector<QGraphicsEllipseItem*> m_sectors;

    QList<TargetWithTimer> m_plots;
    QList<TargetWithTimer> m_tracks;

    bool m_plotsIsVisible = true;

    QScopedPointer<MessageHandler> m_messageHandler;
};
#endif // INDICATORWINDOW_H
