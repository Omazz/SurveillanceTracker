#ifndef GRAPHICSBUILDERWIDGET_H
#define GRAPHICSBUILDERWIDGET_H

#include <QWidget>
#include "qcustomplot/qcustomplot.h"

namespace Ui {
class GraphicsBuilderWidget;
}

class GraphicsBuilderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphicsBuilderWidget(QWidget *parent = nullptr);
    ~GraphicsBuilderWidget();

    void setData(QString titleX, QString titleY,
                 QVector<qreal> arrayX,
                 QVector<QVector<qreal>> arraysY,
                 QVector<QString> namesFunction);
private slots:
    void selectionChanged();

private:
    Ui::GraphicsBuilderWidget *ui;
};

#endif // GRAPHICSBUILDERWIDGET_H
