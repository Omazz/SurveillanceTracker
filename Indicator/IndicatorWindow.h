#ifndef INDICATORWINDOW_H
#define INDICATORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class IndicatorWindow; }
QT_END_NAMESPACE

class IndicatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    IndicatorWindow(QWidget *parent = nullptr);
    ~IndicatorWindow();

private:
    Ui::IndicatorWindow *ui;
};
#endif // INDICATORWINDOW_H
