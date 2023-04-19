#include "IndicatorWindow.h"
#include "ui_IndicatorWindow.h"

IndicatorWindow::IndicatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IndicatorWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    _graphicsScene = new QGraphicsScene();


    _graphicsScene->setSceneRect(
        -RADAR_RANGE_KM, -RADAR_RANGE_KM,
        RADAR_RANGE_KM * 2, RADAR_RANGE_KM * 2
    );

    _messageHandler = new MessageHandler(this);

    ui->GW_locator->setScene(_graphicsScene);
    _graphicsScene->setBackgroundBrush(QBrush(GRAY));
    ui->GW_locator->scale(0.4096, 0.4096);


    qreal rho = _graphicsScene->height() / 2 + 50;

    for (int i = 0; i < SECTOR_COUNT; i++) {
        qreal theta = static_cast<qreal>(i) / SECTOR_COUNT * M_PI * 2.0;
        QPointF pos = polarToCart(rho, theta);
        _sectors += _graphicsScene->addEllipse(
                        pos.x() - CIRCLE_RADIUS, pos.y() - CIRCLE_RADIUS,
                        CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2,
                        QPen(WHITE, 4)
                    );
    }


    createGrid();

    _sectors[0]->setBrush(QBrush(RED));

    connect(_messageHandler, &MessageHandler::changeSector, this, &IndicatorWindow::changeSector);
    connect(_messageHandler, &MessageHandler::newPlot, this, &IndicatorWindow::onNewPlot);
}

void IndicatorWindow::changeSector(qreal angle_deg) {
    _line->setLine(0, 0,
                   _graphicsScene->width() / 2 * qCos(M_PI - qDegreesToRadians(angle_deg)),
                   -_graphicsScene->width() / 2 * qSin(M_PI - qDegreesToRadians(angle_deg)));
    for(int i = 0; i < _sectors.size(); ++i) {
        _sectors[i]->setBrush(QBrush(GRAY));
    }
    int secNumber = (angle_deg - 90.0) / 360.0 * SECTOR_COUNT;
    secNumber = secNumber >= 0 ? secNumber % SECTOR_COUNT : secNumber + SECTOR_COUNT;

    _sectors[secNumber]->setBrush(QBrush(RED));

    _graphicsScene->update();
}

void IndicatorWindow::onNewPlot(qreal rho_km, qreal angle_rad) {
    _graphicsScene->addItem(new TargetItem(QPointF(rho_km, angle_rad)));
    _graphicsScene->update();
}

void IndicatorWindow::createGrid() {

    qreal ringDist = 100;
    qreal sectorAngle = 45;

    QPen gridPen(WHITE);
    gridPen.setCosmetic(true);

    for (qreal curDist = ringDist; curDist <= _graphicsScene->width() / 2; curDist += ringDist) {
        _graphicsScene->addEllipse(
            -curDist, -curDist,
            curDist * 2, curDist * 2,
            gridPen
        );
    }

    for (qreal curAngle = 0; curAngle < 2 * M_PI; curAngle += qDegreesToRadians(sectorAngle)) {
        qreal x = _graphicsScene->width() / 2 * qCos(curAngle);
        qreal y = _graphicsScene->width() / 2 * qSin(curAngle);
        _graphicsScene->addLine(0, 0, x, y, gridPen);
    }

    _line = _graphicsScene->addLine(0, 0, 0, -_graphicsScene->width() / 2.0, QPen(BLUE, 2));
}

QPointF IndicatorWindow::polarToCart(qreal rho, qreal theta) {
    return {rho * qCos(theta - M_PI_2), rho * qSin(theta - M_PI_2)};
}

IndicatorWindow::~IndicatorWindow()
{
    delete ui;
}

