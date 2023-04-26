QT -= gui
QT += testlib
TEMPLATE = app
CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += link_pkgconfig
PKGCONFIG += libpcap

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        test_asterixreader.cpp

TARGET = TestTracker

DESTDIR = ../../ProgramBuilds/TEST-Tracker

OBJECTS_DIR = ../../ProgramBuilds/TEST-Tracker

MOC_DIR = ../../ProgramBuilds/TEST-Tracker

UI_DIR = ../../ProgramBuilds/TEST-Tracker

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    $$PWD/TestRecords/recordAsterix34_1packet_NorthMarker.pcapng \
    $$PWD/TestRecords/recordAsterix34_1packet_sectorCrossed.pcapng \
    $$PWD/TestRecords/recordAsterix48_1packet.pcapng \
    $$PWD/TestRecords/recordsAsterix_100packets.pcapng

include(../../PriFiles/TrackerFiles/TrackerFiles.pri)

HEADERS += \
    test_asterixreader.h
