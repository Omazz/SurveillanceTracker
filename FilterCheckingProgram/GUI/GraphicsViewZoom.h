#ifndef GRAPHICSVIEWZOOM_H
#define GRAPHICSVIEWZOOM_H

#include <QObject>
#include <QGraphicsView>

class GraphicsViewZoom : public QObject
{
    Q_OBJECT
public:

    GraphicsViewZoom(QGraphicsView* view);

    void gentleZoom(double factor);

    void setModifiers(Qt::KeyboardModifiers modifiers);

    void setZoomFactorBase(double value);

private:
  QGraphicsView* _view;
  Qt::KeyboardModifiers _modifiers;
  double _zoomFactorBase;
  QPointF targetScenePos, targetViewportPos;

  bool eventFilter(QObject* object, QEvent* event);

signals:
  void zoomed();
};

#endif // GRAPHICSVIEWZOOM_H
