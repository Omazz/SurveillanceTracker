#include "IndicatorWindow.h"
#include "ui_IndicatorWindow.h"

IndicatorWindow::IndicatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IndicatorWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    mGraphicsScene = new QGraphicsScene();

    mGraphicsScene->setSceneRect(
        -RADAR_RANGE_KM, -RADAR_RANGE_KM,
        RADAR_RANGE_KM * 2, RADAR_RANGE_KM * 2
    );

    mMessageHandler = new MessageHandler(this);
    ui->GW_locator->setScene(mGraphicsScene);
    mGraphicsScene->setBackgroundBrush(QBrush(GRAY));
    ui->GW_locator->scale(0.8, 0.8);


    qreal rho = mGraphicsScene->height() / 2 + 50;

    for (int i = 0; i < SECTOR_COUNT; i++) {
        qreal theta = static_cast<qreal>(i) / SECTOR_COUNT * M_PI * 2.0;
        QPointF pos = polarToCart(rho, theta);
        mSectors += mGraphicsScene->addEllipse(
                        pos.x() - CIRCLE_RADIUS, pos.y() - CIRCLE_RADIUS,
                        CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2,
                        QPen(WHITE, 4)
                    );
    }


    createGrid();

    mSectors[0]->setBrush(QBrush(RED));

    connect(mMessageHandler, &MessageHandler::changeSector, this, &IndicatorWindow::changeSector);
    connect(ui->GW_locator,  &RadarView::visiblePlotsChanged, this, &IndicatorWindow::onVisiblePlotsChanged);
    connect(mMessageHandler, &MessageHandler::newPlot, this, &IndicatorWindow::onNewPlot);
    connect(mMessageHandler, &MessageHandler::newTrack, this, &IndicatorWindow::onNewTrack);
    connect(mMessageHandler, &MessageHandler::newExtrapolatedTrack, this, &IndicatorWindow::onNewExtrapolatedTrack);
}

void IndicatorWindow::changeSector(qreal angle_deg) {
    mLine->setLine(0, 0,
                   mGraphicsScene->width() / 2 * qCos(M_PI - qDegreesToRadians(angle_deg)),
                   -mGraphicsScene->width() / 2 * qSin(M_PI - qDegreesToRadians(angle_deg)));
    for(int i = 0; i < mSectors.size(); ++i) {
        mSectors[i]->setBrush(QBrush(GRAY));
    }
    int secNumber = (angle_deg - 90.0) / 360.0 * SECTOR_COUNT;
    secNumber = secNumber >= 0 ? secNumber % SECTOR_COUNT : secNumber + SECTOR_COUNT;

    mSectors[secNumber]->setBrush(QBrush(RED));

    mGraphicsScene->update();
}

void IndicatorWindow::onNewPlot(qreal rho_km, qreal angle_rad) {
    TargetItem* targetItem = new TargetItem(QPointF(rho_km, angle_rad));
    QTimer* timer =  new QTimer(this);

    timer->start(TIME_TO_REMOVE_PLOT);
    targetItem->setVisible(mPlotsIsVisible);
    std::pair<TargetItem*, QTimer*> targetWithTimer(targetItem, timer);

    connect(timer, &QTimer::timeout, this, [this, targetWithTimer]() {
        if (!mPlots.contains(targetWithTimer)) {
            return;
        }

        mGraphicsScene->removeItem(targetWithTimer.first);
        delete targetWithTimer.first;
        mPlots.removeOne(targetWithTimer);
    });

    mPlots += targetWithTimer;
    mGraphicsScene->addItem(targetItem);
    mGraphicsScene->update();
}


void IndicatorWindow::onNewTrack(qreal x_km, qreal y_km, qreal directionAngle_rad) {
    TargetItem* targetItem = new TargetItem(QPointF(x_km, y_km), TRACK, directionAngle_rad);
    QTimer* timer =  new QTimer(this);

    timer->start(TIME_TO_REMOVE_TRACK);

    std::pair<TargetItem*, QTimer*> targetWithTimer(targetItem, timer);

    connect(timer, &QTimer::timeout, this, [this, targetWithTimer]() {
        if (!mTracks.contains(targetWithTimer)) {
            return;
        }

        mGraphicsScene->removeItem(targetWithTimer.first);
        delete targetWithTimer.first;
        mTracks.removeOne(targetWithTimer);
    });

    mTracks += targetWithTimer;
    mGraphicsScene->addItem(targetItem);
    mGraphicsScene->update();
}

void IndicatorWindow::onNewExtrapolatedTrack(qreal x_km, qreal y_km, qreal directionAngle_rad) {
    TargetItem* targetItem = new TargetItem(QPointF(x_km, y_km), EXTRAPOLATED_TRACK, directionAngle_rad);
    QTimer* timer =  new QTimer(this);

    timer->start(TIME_TO_REMOVE_TRACK);

    std::pair<TargetItem*, QTimer*> targetWithTimer(targetItem, timer);

    connect(timer, &QTimer::timeout, this, [this, targetWithTimer]() {
        if (!mTracks.contains(targetWithTimer)) {
            return;
        }

        mGraphicsScene->removeItem(targetWithTimer.first);
        delete targetWithTimer.first;
        mTracks.removeOne(targetWithTimer);
    });

    mTracks += targetWithTimer;
    mGraphicsScene->addItem(targetItem);
    mGraphicsScene->update();
}

void IndicatorWindow::onVisiblePlotsChanged() {
    mPlotsIsVisible = !mPlotsIsVisible;

    for(auto plot : mPlots) {
        plot.first->setVisible(mPlotsIsVisible);
    }
}

void IndicatorWindow::createGrid() {

    qreal ringDist = 50;
    qreal sectorAngle = 30;

    QPen gridPen(WHITE);
    gridPen.setCosmetic(true);

    for (qreal curDist = ringDist; curDist <= mGraphicsScene->width() / 2; curDist += ringDist) {
        mGraphicsScene->addEllipse(
            -curDist, -curDist,
            curDist * 2, curDist * 2,
            gridPen
        );
    }

    for (qreal curAngle = 0; curAngle < 2 * M_PI; curAngle += qDegreesToRadians(sectorAngle)) {
        qreal x = mGraphicsScene->width() / 2 * qCos(curAngle);
        qreal y = mGraphicsScene->width() / 2 * qSin(curAngle);
        mGraphicsScene->addLine(0, 0, x, y, gridPen);
    }

    mLine = mGraphicsScene->addLine(0, 0, 0, -mGraphicsScene->width() / 2.0, QPen(BLUE, 2));
}

QPointF IndicatorWindow::polarToCart(qreal rho, qreal theta) {
    return {rho * qCos(theta - M_PI_2), rho * qSin(theta - M_PI_2)};
}

IndicatorWindow::~IndicatorWindow()
{
    delete ui;
}

