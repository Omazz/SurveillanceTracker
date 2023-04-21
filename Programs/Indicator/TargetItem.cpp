#include "TargetItem.h"

#include <QPen>
#include <QtMath>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QHostAddress>

TargetItem::TargetItem(
    QPointF coords,
    TypeAirplaneObject type,
    qreal itemSize,
    QGraphicsItem* parent
): QGraphicsItem(parent),
    _type(type),
    _itemSize(itemSize)
{
    QPainterPath path;

    path.moveTo(-itemSize / 2, -itemSize / 2);
    path.lineTo(itemSize / 2, itemSize / 2);
    path.moveTo(itemSize / 2, -itemSize / 2);
    path.lineTo(-itemSize / 2, itemSize / 2);

    _indicator = new QGraphicsPathItem(path, this);

    _indicator->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    if (_type == TRACK || _type == PREDICTED_TRACK) {
        setPos(coords.x(), -coords.y());
    } else if (_type == PLOT || _type == PLOT_BY_TRACK) {
        qreal x = coords.x() * sin(coords.y());
        qreal y = coords.x() * cos(coords.y());

        setPos(x, -y);
    }

    auto pathItem = qgraphicsitem_cast<QGraphicsPathItem*>(_indicator);
    switch(_type) {

    case PLOT:
        pathItem->setPen(QPen(Qt::red, 2));
        break;

    case PLOT_BY_TRACK:
        pathItem->setPen(QPen(Qt::green, 2));
        break;

    case TRACK:
        pathItem->setPen(QPen(Qt::blue, 2));
        break;

    case PREDICTED_TRACK:
        pathItem->setPen(QPen(Qt::yellow, 2));
        break;
    }
}

QRectF TargetItem::boundingRect() const {
    return {0, 0, _itemSize, _itemSize};
}

void TargetItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

}

int TargetItem::type() const
{
    return Type;
}

TypeAirplaneObject TargetItem::getType() {
    return _type;
}
