#ifndef GRAPHICSBUILDERWIDGET_H
#define GRAPHICSBUILDERWIDGET_H

#include <QWidget>
#include <QtCharts>

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


private:
    Ui::GraphicsBuilderWidget *ui;
    QChart* m_chart;
    QChartView* m_chartView;
};

#endif // GRAPHICSBUILDERWIDGET_H
