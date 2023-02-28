#ifndef TEST_ALPHABETAFILTER_H
#define TEST_ALPHABETAFILTER_H

#include <QObject>
#include <QTest>
#include "../FilterCheckingFiles/Filters/AlphaBetaFilter.h"

class Test_AlphaBetaFilter : public QObject
{
    Q_OBJECT
public:
    explicit Test_AlphaBetaFilter(QObject *parent = nullptr);

signals:

private slots:
    void calculateFilteredTrajectory_LinearWithoutNoise();

    void calculateFilteredTrajectory_LinearWithNoise();

    void calculateFilteredTrajectory_MNK_LinearWithoutNoise();

    void calculateFilteredTrajectory_MNK_LinearWithNoise();
};

#endif // TEST_ALPHABETAFILTER_H
