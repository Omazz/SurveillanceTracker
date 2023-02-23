#include "GraphicsBuilderWidget.h"
#include "ui_GraphicsBuilderWidget.h"

GraphicsBuilderWidget::GraphicsBuilderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicsBuilderWidget)
{
    ui->setupUi(this);
    connect(ui->W_graphics, &QCustomPlot::selectionChangedByUser, this, &GraphicsBuilderWidget::selectionChanged);
}

GraphicsBuilderWidget::~GraphicsBuilderWidget()
{
    delete ui;
}

void GraphicsBuilderWidget::setData(QString titleX, QString titleY, QVector<qreal> arrayX,
                                    QVector<QVector<qreal>> arraysY, QVector<QString> namesFunction) {

    ui->W_graphics->xAxis->setLabel(titleX);
    ui->W_graphics->yAxis->setLabel(titleY);
    ui->W_graphics->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->W_graphics->legend->setFont(legendFont);
    ui->W_graphics->legend->setSelectedFont(legendFont);
    ui->W_graphics->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
    ui->W_graphics->xAxis->setRange(0, arrayX.last());
    for(int i = 0; i < arraysY.size(); ++i) {

        ui->W_graphics->addGraph();
        Qt::GlobalColor currentColor;
        if(i == 0) {
            currentColor = Qt::red;
        } else if(i == 1) {
            currentColor = Qt::magenta;
        } else if (i == 2) {
            currentColor = Qt::green;
        } else {
            currentColor = Qt::black;
        }
        ui->W_graphics->graph()->setName(namesFunction[i]);
        ui->W_graphics->graph()->setPen(QPen(currentColor));
        ui->W_graphics->graph()->setData(arrayX, arraysY[i]);
    }

    ui->W_graphics->axisRect()->setupFullAxesBox(true);
    ui->W_graphics->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->W_graphics->rescaleAxes();
    ui->W_graphics->update();
}


void GraphicsBuilderWidget::selectionChanged() {
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->W_graphics->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->W_graphics->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->W_graphics->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->W_graphics->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->W_graphics->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->W_graphics->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->W_graphics->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->W_graphics->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->W_graphics->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->W_graphics->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->W_graphics->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->W_graphics->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->W_graphics->graphCount(); ++i)
  {
    QCPGraph *graph = ui->W_graphics->graph(i);
    QCPPlottableLegendItem *item = ui->W_graphics->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);

      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}
