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
    qreal directionAngle,
    qreal itemSize,
    QGraphicsItem* parent
): QGraphicsItem(parent),
    m_type(type),
    m_itemSize(itemSize)
{
    QPainterPath path;

    path.moveTo(-itemSize / 2, -itemSize / 2);
    path.lineTo(itemSize / 2, itemSize / 2);
    path.moveTo(itemSize / 2, -itemSize / 2);
    path.lineTo(-itemSize / 2, itemSize / 2);

    m_indicator = new QGraphicsPathItem(path, this);

    m_indicator->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    if (m_type == TRACK || m_type == EXTRAPOLATED_TRACK) {
        setPos(coords.x(), -coords.y());
        qDebug() << coords.x() << -coords.y();
    } else if (m_type == PLOT || m_type == PLOT_BY_TRACK) {
        qreal x = coords.x() * sin(coords.y());
        qreal y = coords.x() * cos(coords.y());

        setPos(x, -y);
    }

    auto pathItem = qgraphicsitem_cast<QGraphicsPathItem*>(m_indicator);
    switch(m_type) {

    case PLOT:
        pathItem->setPen(QPen(Qt::red, 2));
        break;

    case PLOT_BY_TRACK:
        pathItem->setPen(QPen(Qt::green, 2));
        break;

    case TRACK:
        pathItem->setPen(QPen(Qt::blue, 2));

        m_headingLine = new QGraphicsLineItem(
            {QPointF(0, 0), QPointF(30 * qCos(directionAngle), -30.0 * qSin(directionAngle))},
            this
        );

        m_headingLine->setPen(QPen(QColor(0, 190, 255)));
        m_headingLine->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        break;

    case EXTRAPOLATED_TRACK:
        pathItem->setPen(QPen(Qt::yellow, 2));
        m_headingLine = new QGraphicsLineItem(
            {QPointF(0, 0), QPointF(30 * qCos(directionAngle), -30.0 * qSin(directionAngle))},
            this
        );

        m_headingLine->setPen(QPen(QColor(0, 190, 255)));
        m_headingLine->setFlag(QGraphicsItem::ItemIgnoresTransformations);

        break;
    }
}


QRectF TargetItem::boundingRect() const {
    return {0, 0, m_itemSize, m_itemSize};
}

void TargetItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

}

int TargetItem::type() const
{
    return Type;
}

TypeAirplaneObject TargetItem::getType() {
    return m_type;
}
