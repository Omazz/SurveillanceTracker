
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT += core gui network
CONFIG += c++11
CONFIG -= app_bundle
CONFIG += link_pkgconfig
PKGCONFIG += libpcap
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MessageHandler.cpp \
    RadarView.cpp \
    TargetItem.cpp \
    main.cpp \
    IndicatorWindow.cpp

HEADERS += \
    IndicatorWindow.h \
    MessageHandler.h \
    RadarView.h \
    TargetItem.h

FORMS += \
    IndicatorWindow.ui

TARGET = Indicator

DESTDIR = ../../ProgramBuilds/PROGRAM-Indicator

OBJECTS_DIR = ../../ProgramBuilds/PROGRAM-Indicator

MOC_DIR = ../../ProgramBuilds/PROGRAM-Indicator

UI_DIR = ../../ProgramBuilds/PROGRAM-Indicator

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    ../../ApplicationIcons/IndicatorIcon.ico

include(../../PriFiles/AsterixHandler/AsterixHandler.pri)
include(../../PriFiles/Settings/Settings.pri)
include(../../PriFiles/EncryptionTools/EncryptionTools.pri)

