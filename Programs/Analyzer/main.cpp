#include "GUI/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowIcon(QIcon("../../ApplicationIcons/AnalyzerIcon.ico"));
    w.show();
    return a.exec();
}
