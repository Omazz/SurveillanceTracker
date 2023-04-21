#ifndef TEST_ASTERIXREADER_H
#define TEST_ASTERIXREADER_H

#include <QObject>
#include <QTest>
#include "../../PriFiles/TrackerFiles/asterixreader.h"

class Test_AsterixReader : public QObject
{
    Q_OBJECT
public:
    explicit Test_AsterixReader(QObject *parent = nullptr);

signals:

private slots:
    void parseAsterix34_checkField_LEN();

    void parseAsterix34_checkField_FSPEC();

    void parseAsterix34_checkField_DataSourceIdentifier();

    void parseAsterix34_checkField_MessageType();

    void parseAsterix34_checkField_TimeOfDay();



    void parseAsterix48_checkField_LEN();

    void parseAsterix48_checkField_FSPEC();

    void parseAsterix48_checkField_DataSourceIdentifier();

    void parseAsterix48_checkField_TimeOfDay();

    void parseAsterix48_checkField_TargetReportDescriptor();

    void parseAsterix48_checkField_RHO();

    void parseAsterix48_checkField_THETA();



    void asterix34_checkSize_numberRecords();

    void asterix48_checkSize_numberRecords();

    void pcapFile_checkSize_numberRecords();
};

#endif // TEST_ASTERIXREADER_H
