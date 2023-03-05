#include "test_asterixreader.h"

Test_AsterixReader::Test_AsterixReader(QObject *parent)
    : QObject{parent}
{

}

void Test_AsterixReader::parseAsterix34_checkField_LEN() {
    AsterixReader asterixReader;

    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    QCOMPARE(records.second.back().LEN, 0x0A);
}

void Test_AsterixReader::parseAsterix34_checkField_FSPEC() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    QCOMPARE(records.second.back().FSPEC, 0xE000);
}

void Test_AsterixReader::parseAsterix34_checkField_DataSourceIdentifier() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    QCOMPARE(records.second.back().DataSourceIdentifier, 0x0A33);
}

void Test_AsterixReader::parseAsterix34_checkField_MessageType() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    QCOMPARE(records.second.back().MessageType, 0x01);
}

void Test_AsterixReader::parseAsterix34_checkField_TimeOfDay() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    QCOMPARE(records.second.back().TimeOfDay, 0x4E9652);
}

void Test_AsterixReader::parseAsterix48_checkField_LEN() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix48_1packet.pcapng"
                );
    QCOMPARE(records.first.back().LEN, 0x0E);
}

void Test_AsterixReader::parseAsterix48_checkField_FSPEC() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix48_1packet.pcapng"
                );
    QCOMPARE(records.first.back().FSPEC, 0xF0000000);
}

void Test_AsterixReader::parseAsterix48_checkField_DataSourceIdentifier() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix48_1packet.pcapng"
                );
    QCOMPARE(records.first.back().DataSourceIdentifier, 0x0A33);
}

void Test_AsterixReader::parseAsterix48_checkField_TimeOfDay() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix48_1packet.pcapng"
                );
    QCOMPARE(records.first.back().TimeOfDay, 0x4E93DF);
}

void Test_AsterixReader::parseAsterix48_checkField_TargetReportDescriptor() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix48_1packet.pcapng"
                );
    QCOMPARE(records.first.back().TargetReportDescriptor.back(), 0x20);
}

void Test_AsterixReader::parseAsterix48_checkField_RHO() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix48_1packet.pcapng"
                );
    QCOMPARE(records.first.back().MeasuredPositionInSlantPolarCoordinates & 0xFFFF0000, 0x30220000);
}

void Test_AsterixReader::parseAsterix48_checkField_THETA() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordAsterix48_1packet.pcapng"
                );
    QCOMPARE(records.first.back().MeasuredPositionInSlantPolarCoordinates & 0x0000FFFF, 0x0000049C);
}

void Test_AsterixReader::asterix34_checkSize_numberRecords() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordsAsterix_100packets.pcapng"
                );
    QCOMPARE(records.first.size(), 50);
}

void Test_AsterixReader::asterix48_checkSize_numberRecords() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordsAsterix_100packets.pcapng"
                );
    QCOMPARE(records.second.size(), 50);
}

void Test_AsterixReader::pcapFile_checkSize_numberRecords() {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../TestTracker/TestRecords/recordsAsterix_100packets.pcapng"
                );
    QCOMPARE(records.first.size() + records.second.size(), 100);
}
