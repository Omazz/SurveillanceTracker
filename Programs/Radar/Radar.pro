QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += libpcap
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    RadarWindow.cpp

HEADERS += \
    RadarWindow.h

FORMS += \
    RadarWindow.ui

TARGET = Radar

DESTDIR = ../../ProgramBuilds/PROGRAM-Radar

OBJECTS_DIR = ../../ProgramBuilds/PROGRAM-Radar

MOC_DIR = ../../ProgramBuilds/PROGRAM-Radar

UI_DIR = ../../ProgramBuilds/PROGRAM-Radar

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../ApplicationIcons/RadarIcon.ico

include(../../PriFiles/Settings/Settings.pri)

