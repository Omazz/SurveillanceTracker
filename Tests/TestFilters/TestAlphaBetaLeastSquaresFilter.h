#ifndef TESTALPHABETALEASTSQUARESFILTER_H
#define TESTALPHABETALEASTSQUARESFILTER_H


#include <QObject>
#include <QTest>
#include "../../PriFiles/Filters/AlphaBetaLeastSquaresFilter.h"

class TestAlphaBetaLeastSquaresFilter : public QObject
{
    Q_OBJECT
public:
    explicit TestAlphaBetaLeastSquaresFilter(QObject *parent = nullptr);

signals:

private slots:
    void filterTrack_linearWithoutNoise();

    void filterTrack_linearWithNoise();

};

#endif // TESTALPHABETALEASTSQUARESFILTER_H
