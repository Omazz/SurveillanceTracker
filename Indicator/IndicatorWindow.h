#ifndef INDICATORWINDOW_H
#define INDICATORWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QtCore>

#include "MessageHandler.h"
#include "RadarView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class IndicatorWindow; }
QT_END_NAMESPACE

const QColor WHITE = QColor(0xF6F1F1);
const QColor GRAY = QColor(0x464646);
const QColor BLUE = QColor(0x19A7CE);
const QColor RED = QColor(0xE21818);

const qint16 RADAR_RANGE_KM = 1000;
const quint8 SECTOR_COUNT = 32;
const int CIRCLE_RADIUS = 15;

const quint8 PEN_INDICATOR_WIDTH = 5;

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

private:
    Ui::IndicatorWindow *ui;
    QGraphicsScene* _graphicsScene;

    QGraphicsLineItem* _line;
    QVector<QGraphicsEllipseItem*> _sectors;

    MessageHandler* _messageHandler;
};
#endif // INDICATORWINDOW_H
