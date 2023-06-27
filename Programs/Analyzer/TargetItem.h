#ifndef TARGETITEM_H
#define TARGETITEM_H

#include <QtCore>
#include <QGraphicsItem>
#include <QGraphicsView>


class TargetItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };

    TargetItem(QPointF coords,
               QColor color = Qt::red,
               qreal itemSize = 10.0,
               QGraphicsItem* parent = nullptr);


    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    int type() const override;


private:
    QGraphicsItem* m_indicator;
    QGraphicsLineItem* m_headingLine = nullptr;
    QColor m_color;
    qreal m_itemSize;
};

#endif // TARGETITEM_H
