#include "IndicatorWindow.h"
#include "ui_IndicatorWindow.h"

IndicatorWindow::IndicatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IndicatorWindow)
{
    ui->setupUi(this);
}

IndicatorWindow::~IndicatorWindow()
{
    delete ui;
}

