#include "GraphicsBuilderWidget.h"
#include "ui_GraphicsBuilderWidget.h"

GraphicsBuilderWidget::GraphicsBuilderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicsBuilderWidget)
{
    ui->setupUi(this);
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    setFixedSize(width(), height());
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_chartView);
    setLayout(layout);
}

GraphicsBuilderWidget::~GraphicsBuilderWidget()
{
    delete ui;
}

void GraphicsBuilderWidget::setData(QString titleX, QString titleY, QVector<qreal> arrayX,
                                    QVector<QVector<qreal>> arraysY, QVector<QString> namesPlot) {

    QValueAxis* axisX = new QValueAxis(this);
    QValueAxis* axisY = new QValueAxis(this);
    axisY->setLabelFormat("%.2f");
    axisX->setRange(arrayX.first(), arrayX.last());
    qreal minY = arraysY[0][0];
    qreal maxY = arraysY[0][0];
    for(int i = 0; i < arraysY.size(); ++i) {
        for(int j = 0; j < arraysY[0].size(); ++j) {
            minY = qMin(minY, arraysY[i][j]);
            maxY = qMax(maxY, arraysY[i][j]);
        }
    }
    axisY->setRange(minY, maxY);
    axisX->setTitleText(titleX);
    axisY->setTitleText(titleY);
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    for(int i = 0; i < arraysY.size(); ++i) {
        QLineSeries* currSeries = new QLineSeries(this);
        for(int j = 0; j < arrayX.size(); ++j) {
            currSeries->append(arrayX[j], arraysY[i][j]);
        }

        Qt::GlobalColor currentColor;
        if(i == 0) {
            currentColor = Qt::red;
        } else if(i == 1) {
            currentColor = Qt::green;
        } else if (i == 2) {
            currentColor = Qt::blue;
        } else if (i == 3) {
            currentColor = Qt::magenta;
        } else if (i == 4) {
            currentColor = Qt::yellow;
        } else if (i == 5) {
            currentColor = Qt::cyan;
        } else {
            currentColor = Qt::black;
        }

        currSeries->setColor(currentColor);
        currSeries->setName(namesPlot[i]);
        m_chart->addSeries(currSeries);
    }
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
}
