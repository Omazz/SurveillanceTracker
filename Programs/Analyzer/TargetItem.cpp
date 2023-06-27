#include "TargetItem.h"

#include <QPen>
#include <QtMath>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

TargetItem::TargetItem(
    QPointF coords,
    QColor color,
    qreal itemSize,
    QGraphicsItem* parent
): QGraphicsItem(parent),
    m_color(color),
    m_itemSize(itemSize)
{
    QPainterPath path;

    path.moveTo(-itemSize / 2, -itemSize / 2);
    path.lineTo(itemSize / 2, itemSize / 2);
    path.moveTo(itemSize / 2, -itemSize / 2);
    path.lineTo(-itemSize / 2, itemSize / 2);

    m_indicator = new QGraphicsPathItem(path, this);

    m_indicator->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    setPos(coords.x(), -coords.y());

    auto pathItem = qgraphicsitem_cast<QGraphicsPathItem*>(m_indicator);

    pathItem->setPen(QPen(m_color, 2));
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
