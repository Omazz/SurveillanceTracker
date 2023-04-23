QT -= gui
QT += testlib

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        test_alphabetafilter.cpp \
        test_kalmanfilter.cpp \
        test_matrix.cpp

TARGET = FilterCheckingTest

DESTDIR = ../../ProgramBuilds/TEST-FilterCheckingTest

OBJECTS_DIR = ../../ProgramBuilds/TEST-FilterCheckingTest

MOC_DIR = ../../ProgramBuilds/TEST-FilterCheckingTest

UI_DIR = ../../ProgramBuilds/TEST-FilterCheckingTest

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
include(../../PriFiles/FilterCheckingFiles/FilterFiles.pri)

HEADERS += \
    test_alphabetafilter.h \
    test_kalmanfilter.h \
    test_matrix.h
