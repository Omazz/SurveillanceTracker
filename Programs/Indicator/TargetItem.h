#ifndef TARGETITEM_H
#define TARGETITEM_H

#include <QtCore>
#include <QGraphicsItem>
#include <QGraphicsView>

enum TypeAirplaneObject {
    PLOT,
    PLOT_BY_TRACK,
    TRACK,
    EXTRAPOLATED_TRACK
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
               TypeAirplaneObject type = PLOT,
               qreal directionAngle = 0,
               qreal itemSize = 10.0,
               QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    int type() const override;

    TypeAirplaneObject getType();

private:
    QGraphicsItem* mIndicator;
    QGraphicsLineItem* mHeadingLine = nullptr;
    TypeAirplaneObject mType;
    qreal mItemSIze;
};

#endif // TARGETITEM_H
