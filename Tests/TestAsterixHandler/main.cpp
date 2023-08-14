#include <gtest/gtest.h>
#include "AsterixReader.h"


TEST(parseAsterix34, checkField_LEN) {
    AsterixReader asterixReader;

    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    ASSERT_EQ(records.second.back().LEN, 0x0A);
}

TEST(parseAsterix34, checkField_FSPEC) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    ASSERT_EQ(records.second.back().FSPEC, 224);
}

TEST(parseAsterix34, checkField_DataSourceIdentifier) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    ASSERT_EQ(records.second.back().DataSourceIdentifier, 0x0A33);
}

TEST(parseAsterix34, checkField_MessageType) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    ASSERT_EQ(records.second.back().MessageType, 0x01);
}

TEST(parseAsterix34, checkField_TimeOfDay) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng"
                );
    ASSERT_EQ(records.second.back().TimeOfDay, 0x4E9652);
}

TEST(parseAsterix48, checkField_LEN) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix48_1packet.pcapng"
                );
    ASSERT_EQ(records.first.back().LEN, 0x0E);
}

TEST(parseAsterix48, checkField_FSPEC) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix48_1packet.pcapng"
                );
    ASSERT_EQ(records.first.back().FSPEC, 240);
}

TEST(parseAsterix48, checkField_DataSourceIdentifier) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix48_1packet.pcapng"
                );
    ASSERT_EQ(records.first.back().DataSourceIdentifier, 0x0A33);
}

TEST(parseAsterix48, checkField_TimeOfDay) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix48_1packet.pcapng"
                );
    ASSERT_EQ(records.first.back().TimeOfDay, 0x4E93DF);
}

TEST(parseAsterix48, checkField_TargetReportDescriptor) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix48_1packet.pcapng"
                );
    ASSERT_EQ(records.first.back().TargetReportDescriptor.back(), 0x20);
}

TEST(parseAsterix48, checkField_RHO) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix48_1packet.pcapng"
                );
    ASSERT_EQ(records.first.back().MeasuredPositionInSlantPolarCoordinates & 0xFFFF0000, 0x30220000);
}

TEST(parseAsterix48, checkField_THETA) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordAsterix48_1packet.pcapng"
                );
    ASSERT_EQ(records.first.back().MeasuredPositionInSlantPolarCoordinates & 0x0000FFFF, 0x0000049C);
}

TEST(readPcapFile, checkAsterix34Records) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordsAsterix_100packets.pcapng"
                );
    ASSERT_EQ(records.first.size(), 50);
}

TEST(readPcapFile, checkAsterix48Records) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordsAsterix_100packets.pcapng"
                );
    ASSERT_EQ(records.second.size(), 50);
}

TEST(readPcapFile, checkTotalRecords) {
    AsterixReader asterixReader;
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records = asterixReader.readPcapFile(
                "../../../Tests/TestAsterixHandler/TestRecords/recordsAsterix_100packets.pcapng"
                );
    ASSERT_EQ(records.first.size() + records.second.size(), 100);
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
