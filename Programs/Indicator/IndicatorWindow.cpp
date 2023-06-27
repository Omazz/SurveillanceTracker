#include "IndicatorWindow.h"
#include "ui_IndicatorWindow.h"

IndicatorWindow::IndicatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IndicatorWindow)
    , m_graphicsScene(new QGraphicsScene)
    , m_messageHandler(new MessageHandler)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    m_graphicsScene->setSceneRect(
        -RADAR_RANGE_KM, -RADAR_RANGE_KM,
        RADAR_RANGE_KM * 2, RADAR_RANGE_KM * 2
    );

    ui->GW_locator->setScene(m_graphicsScene);
    m_graphicsScene->setBackgroundBrush(QBrush(GRAY));
    ui->GW_locator->scale(0.8, 0.8);


    qreal rho = m_graphicsScene->height() / 2 + 50;

    for (int i = 0; i < SECTOR_COUNT; i++) {
        qreal theta = static_cast<qreal>(i) / SECTOR_COUNT * M_PI * 2.0;
        QPointF pos = polarToCart(rho, theta);
        m_sectors += m_graphicsScene->addEllipse(
                        pos.x() - CIRCLE_RADIUS, pos.y() - CIRCLE_RADIUS,
                        CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2,
                        QPen(WHITE, 4)
                    );
    }

    createGrid();

    m_sectors[0]->setBrush(QBrush(RED));

    connect(m_messageHandler.data(), &MessageHandler::changeSector, this, &IndicatorWindow::changeSector);
    connect(ui->GW_locator,  &RadarView::visiblePlotsChanged, this, &IndicatorWindow::onVisiblePlotsChanged);
    connect(m_messageHandler.data(), &MessageHandler::newPlot, this, &IndicatorWindow::onNewPlot);
    connect(m_messageHandler.data(), &MessageHandler::newTrack, this, &IndicatorWindow::onNewTrack);
    connect(m_messageHandler.data(), &MessageHandler::newExtrapolatedTrack, this, &IndicatorWindow::onNewExtrapolatedTrack);
}

void IndicatorWindow::changeSector(qreal angle_deg) {
    m_line->setLine(0, 0,
                   m_graphicsScene->width() / 2 * qCos(M_PI - qDegreesToRadians(angle_deg)),
                   -m_graphicsScene->width() / 2 * qSin(M_PI - qDegreesToRadians(angle_deg)));
    for(int i = 0; i < m_sectors.size(); ++i) {
        m_sectors[i]->setBrush(QBrush(GRAY));
    }
    int secNumber = (angle_deg - 90.0) / 360.0 * SECTOR_COUNT;
    secNumber = secNumber >= 0 ? secNumber % SECTOR_COUNT : secNumber + SECTOR_COUNT;

    m_sectors[secNumber]->setBrush(QBrush(RED));

    m_graphicsScene->update();
}

void IndicatorWindow::onNewPlot(qreal rho_km, qreal angle_rad) {
//    qDebug() << "onNewPlot: " << QThread::currentThread();

    TargetItem* targetItem = new TargetItem(QPointF(rho_km, angle_rad));
    QTimer* timer =  new QTimer();

    timer->start(TIME_TO_REMOVE_PLOT);
    targetItem->setVisible(m_plotsIsVisible);

    TargetWithTimer targetWithTimer(targetItem, timer);

    connect(timer, &QTimer::timeout, this , &IndicatorWindow::onTimeoutPlot);

    m_plots += targetWithTimer;
    m_graphicsScene->addItem(targetItem);
    m_graphicsScene->update();
}


void IndicatorWindow::onNewTrack(qreal x_km, qreal y_km, qreal directionAngle_rad) {
    TargetItem* targetItem = new TargetItem(QPointF(x_km, y_km), TRACK, directionAngle_rad);
    QTimer* timer =  new QTimer();

    timer->start(TIME_TO_REMOVE_TRACK);

    TargetWithTimer targetWithTimer(targetItem, timer);

    connect(timer, &QTimer::timeout, this , &IndicatorWindow::onTimeoutTrack);

    m_tracks += targetWithTimer;
    m_graphicsScene->addItem(targetItem);
    m_graphicsScene->update();
}

void IndicatorWindow::onNewExtrapolatedTrack(qreal x_km, qreal y_km, qreal directionAngle_rad) {
    TargetItem* targetItem = new TargetItem(QPointF(x_km, y_km), EXTRAPOLATED_TRACK, directionAngle_rad);
    QTimer* timer =  new QTimer();

    timer->start(TIME_TO_REMOVE_TRACK);

    TargetWithTimer targetWithTimer(targetItem, timer);

    connect(timer, &QTimer::timeout, this , &IndicatorWindow::onTimeoutTrack);

    m_tracks.append(targetWithTimer);
    m_graphicsScene->addItem(targetItem);
    m_graphicsScene->update();
}

void IndicatorWindow::onVisiblePlotsChanged() {
    m_plotsIsVisible = !m_plotsIsVisible;

    for(int i = 0; i < m_plots.size(); ++i) {
        m_plots[i].target.data()->setVisible(m_plotsIsVisible);
    }
}

void IndicatorWindow::onTimeoutPlot() {
//    qDebug() << "onTimeoutPlot: " << QThread::currentThread();
    for(auto iterator = m_plots.begin(); iterator != m_plots.end(); iterator++) {
        if(iterator->timer.data() == sender()) {
            m_graphicsScene->removeItem(iterator->target.data());
            m_plots.erase(iterator);
            return;
        }
    }
    return;
}

void IndicatorWindow::onTimeoutTrack() {
//    qDebug() << "onTimeoutTrack: " << QThread::currentThread();
    for(auto iterator = m_tracks.begin(); iterator != m_tracks.end(); iterator++) {
        if(iterator->timer.data() == sender()) {
            m_graphicsScene->removeItem(iterator->target.data());
            m_tracks.erase(iterator);
            return;
        }
    }
    return;
}

void IndicatorWindow::createGrid() {
    qreal ringDist = 50;
    qreal sectorAngle = 30;

    QPen gridPen(WHITE);
    gridPen.setCosmetic(true);

    for (qreal curDist = ringDist; curDist <= m_graphicsScene->width() / 2; curDist += ringDist) {
        m_graphicsScene->addEllipse(
            -curDist, -curDist,
            curDist * 2, curDist * 2,
            gridPen
        );
    }

    for (qreal curAngle = 0; curAngle < 2 * M_PI; curAngle += qDegreesToRadians(sectorAngle)) {
        qreal x = m_graphicsScene->width() / 2 * qCos(curAngle);
        qreal y = m_graphicsScene->width() / 2 * qSin(curAngle);
        m_graphicsScene->addLine(0, 0, x, y, gridPen);
    }

    m_line = m_graphicsScene->addLine(0, 0, 0, -m_graphicsScene->width() / 2.0, QPen(BLUE, 2));
}

QPointF IndicatorWindow::polarToCart(qreal rho, qreal theta) {
    return {rho * qCos(theta - M_PI_2), rho * qSin(theta - M_PI_2)};
}

IndicatorWindow::~IndicatorWindow()
{
    delete ui;
}

