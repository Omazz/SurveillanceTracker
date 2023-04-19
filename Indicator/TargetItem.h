#ifndef TARGETITEM_H
#define TARGETITEM_H

#include <QtCore>
#include <QGraphicsItem>
#include <QGraphicsView>
#include "RadarTarget.h"

enum TypeAirplaneObject {
    PLOT,
    PLOT_BY_TRACK,
    TRACK,
    PREDICTED_TRACK
};

class TargetItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };

    /*
     * Если прямоугольные координаты, то в км
     * Если полярные, то в км и радианах
     */
    TargetItem(QPointF coords,
               qreal itemSize = 10.0,
               TypeAirplaneObject type = PLOT,
               QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    int type() const override;

    TypeAirplaneObject getType();

private:

    qreal _itemSize;
    TypeAirplaneObject _type;
    QGraphicsItem* _indicator;

};

#endif // TARGETITEM_H
