QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/GraphicsBuilderWidget.cpp \
    GUI/GraphicsViewZoom.cpp \
    GUI/mainwindow.cpp \
    main.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS += \
    GUI/GraphicsBuilderWidget.h \
    GUI/GraphicsViewZoom.h \
    GUI/mainwindow.h \
    qcustomplot/qcustomplot.h

FORMS += \
    GUI/GraphicsBuilderWidget.ui \
    GUI/mainwindow.ui

TARGET = FilterCheckingProgram

DESTDIR = ../../ProgramBuilds/PROGRAM-FilterCheckingProgram

OBJECTS_DIR = ../../ProgramBuilds/PROGRAM-FilterCheckingProgram

MOC_DIR = ../../ProgramBuilds/PROGRAM-FilterCheckingProgram

UI_DIR = ../../ProgramBuilds/PROGRAM-FilterCheckingProgram


DISTFILES += \
    ../../ApplicationIcons/FilterModelationIcon.ico


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(../../PriFiles/FilterCheckingFiles/FilterFiles.pri)
