#pragma once

#include <QGraphicsView>

class TargetItem;

class RadarView: public QGraphicsView {
Q_OBJECT

public:
    explicit RadarView(QWidget* parent = nullptr);

signals:
    void zoomChanged(qreal factor);

protected:
    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool _dragged = true;
    QPointF _prevMousePos;

    void zoom(qreal factor);
};

