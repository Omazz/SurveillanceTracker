#ifndef TARGETITEM_H
#define TARGETITEM_H

#include <QtCore>
#include <QGraphicsItem>

enum TypeAirplaneObject {
    PLOT,
    PLOT_BY_TRACK,
    TRACK,
    PREDICTED_TRACK
};

class TargetItem : public QGraphicsItem
{
public:
    TargetItem();
private:

    qreal _itemSize;
    TypeAirplaneObject _type;
    QGraphicsPolygonItem* _strobe = nullptr;
    QGraphicsItem* _indicator = nullptr;
    QGraphicsLineItem* _headingLine = nullptr;
};

#endif // TARGETITEM_H
