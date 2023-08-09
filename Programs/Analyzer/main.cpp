#include "mainwindow.h"
#include "../../PriFiles/LoadingScreen/AppLoadScreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("../../ApplicationIcons/AnalyzerIcon.ico"));
    AppLoadScreen loadScreen(QPixmap("../../ApplicationIcons/loadscreen_Analyzer.png"));
    loadScreen.showLoadingScreen(2500, &w);
    w.show();
    w.activateWindow();

    return a.exec();
}
