#include "GraphicsViewZoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
  : QObject(view), _view(view)
{
  _view->viewport()->installEventFilter(this);
  _view->setMouseTracking(true);
  _modifiers = Qt::ControlModifier;
  _zoomFactorBase = 1.0015;

}

void GraphicsViewZoom::gentleZoom(double factor) {
  _view->scale(factor, factor);
  _view->centerOn(targetScenePos);
  QPointF delta_viewport_pos = targetViewportPos - QPointF(_view->viewport()->width() / 2.0,
                                                             _view->viewport()->height() / 2.0);
  QPointF viewport_center = _view->mapFromScene(targetScenePos) - delta_viewport_pos;
  _view->centerOn(_view->mapToScene(viewport_center.toPoint()));
  emit zoomed();
}

void GraphicsViewZoom::setModifiers(Qt::KeyboardModifiers modifiers) {
  _modifiers = modifiers;

}

void GraphicsViewZoom::setZoomFactorBase(double value) {
  _zoomFactorBase = value;
}

bool GraphicsViewZoom::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::MouseMove) {
    QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
    QPointF delta = targetViewportPos - mouse_event->pos();
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
      targetViewportPos = mouse_event->pos();
      targetScenePos = _view->mapToScene(mouse_event->pos());
    }
  } else if (event->type() == QEvent::Wheel) {
    QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
    if (QApplication::keyboardModifiers() == _modifiers) {
      if (wheel_event->orientation() == Qt::Vertical) {
        double angle = wheel_event->angleDelta().y();
        double factor = qPow(_zoomFactorBase, angle);
        gentleZoom(factor);
        return true;
      }
    }
  }
  Q_UNUSED(object)
  return false;
}
