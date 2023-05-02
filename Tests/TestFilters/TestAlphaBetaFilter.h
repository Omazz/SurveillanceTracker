#ifndef TESTALPHABETAFILTER_H
#define TESTALPHABETAFILTER_H


#include <QObject>
#include <QTest>
#include "../../PriFiles/Filters/AlphaBetaFilter.h"

class TestAlphaBetaFilter : public QObject
{
    Q_OBJECT
public:
    explicit TestAlphaBetaFilter(QObject *parent = nullptr);

signals:

private slots:
    void filterTrack_linearWithoutNoise();

    void filterTrack_linearWithNoise();
};

#endif // TESTALPHABETAFILTER_H
