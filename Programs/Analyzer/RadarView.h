#pragma once

#include <QGraphicsView>

class TargetItem;

class RadarView: public QGraphicsView {
Q_OBJECT

public:
    explicit RadarView(QWidget* parent = nullptr);

signals:
    void visiblePlotsChanged();

protected:
    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_isDraggedMode = true;
    QPointF is_previousMousePosition;

    void zoom(qreal factor);
};

