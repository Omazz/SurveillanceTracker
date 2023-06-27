#include "RadarView.h"


#include <QMouseEvent>
#include <QtMath>
#include <QTimer>
#include <QPainterPath>
#include <QShortcut>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QGraphicsItem>
#include <QDebug>


RadarView::RadarView(QWidget* parent) : QGraphicsView(parent) {
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setDragMode(QGraphicsView::ScrollHandDrag);
}


void RadarView::wheelEvent(QWheelEvent* event) {
    qreal zoomFactor = qPow(1.0015, event->angleDelta().y());
    zoom(zoomFactor);

    QPointF mousePos = mapToScene(mapFromGlobal(event->globalPos()));
    QPointF centerPos = mapToScene(viewport()->rect().center());
    QPointF delta = (mousePos - centerPos) / 4;

    translate(-delta.x(), -delta.y());
}

void RadarView::keyPressEvent(QKeyEvent* event) {
    QGraphicsView::keyPressEvent(event);

    const qreal ZOOM_FACTOR = 1.25;
    if (event->key() == Qt::Key_Equal || event->key() == Qt::Key_Plus) {
        zoom(ZOOM_FACTOR);
    } else if (event->key() == Qt::Key_Minus) {
        zoom(1 / ZOOM_FACTOR);
    } else if(event->key() == Qt::Key_A  || event->key() == 1060) { // 1060 -- это Ф (русская раскладка)
        resetTransform();
        zoom(1 / ZOOM_FACTOR);
    } else if(event->key() == Qt::Key_R || event->key() == 1050) { // 1050 -- это К (русская раскладка)
        m_isDraggedMode = !m_isDraggedMode;
        if(m_isDraggedMode) {
            setDragMode(QGraphicsView::ScrollHandDrag);
        } else {
            setDragMode(QGraphicsView::RubberBandDrag);

        }
    } else if(event->key() == Qt::Key_Space) {
        emit visiblePlotsChanged();
    }
}

void RadarView::zoom(qreal factor) {
    QPointF oldPos = mapToScene(viewport()->rect().center());

    if (transform().m22() * factor >= 0.8 && transform().m22() * factor <= 60) {
        scale(factor, factor);
    }

    QPointF newPos = mapToScene(viewport()->rect().center());

    QPointF delta = newPos - oldPos;
    translate(delta.x(), delta.y());
}

void RadarView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);

    if (dragMode() == QGraphicsView::RubberBandDrag) {
        QRectF selected = scene()->selectionArea().boundingRect();
        QRectF selectedView = mapFromScene(selected).boundingRect();
        if (selectedView.width() > 15 && selectedView.height() > 15) {
            fitInView(selected, Qt::KeepAspectRatio);
            if (transform().m22() > 60) {
                qreal fixFactor = 60 / transform().m22();
                scale(fixFactor, fixFactor);
            }
        }
        setDragMode(QGraphicsView::ScrollHandDrag);
        m_isDraggedMode = !m_isDraggedMode;
    }
}
