QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    SettingsWindow.cpp

HEADERS += \
    SettingsWindow.h

FORMS += \
    SettingsWindow.ui

TARGET = Settings

DESTDIR = ../../ProgramBuilds/PROGRAM-Settings

OBJECTS_DIR = ../../ProgramBuilds/PROGRAM-Settings

MOC_DIR = ../../ProgramBuilds/PROGRAM-Settings

UI_DIR = ../../ProgramBuilds/PROGRAM-Settings

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../ApplicationIcons/SettingsIcon.ico

include(../../PriFiles/Settings/Settings.pri)
include(../../PriFiles/LoadingScreen/LoadingScreen.pri)
