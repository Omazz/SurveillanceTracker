QT += core gui network testlib
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += libpcap

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../../Programs/Tracker/Airplane.cpp \
        ../../Programs/Tracker/AirplaneHandler.cpp \
        ../../Programs/Tracker/MessageHandler.cpp \
        ../../Programs/Tracker/Plot.cpp \
        TestAirplaneHandler.cpp \
        main.cpp

HEADERS += \
    ../../Programs/Tracker/Airplane.h \
    ../../Programs/Tracker/AirplaneHandler.h \
    ../../Programs/Tracker/MessageHandler.h \
    ../../Programs/Tracker/Plot.h \
    TestAirplaneHandler.h

TARGET = TestTracker

DESTDIR = ../../ProgramBuilds/TEST-Tracker

OBJECTS_DIR = ../../ProgramBuilds/TEST-Tracker

MOC_DIR = ../../ProgramBuilds/TEST-Tracker

UI_DIR = ../../ProgramBuilds/TEST-Tracker

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(../../PriFiles/EncryptionTools/EncryptionTools.pri)
include(../../PriFiles/AsterixHandler/AsterixHandler.pri)
include(../../PriFiles/Settings/Settings.pri)
