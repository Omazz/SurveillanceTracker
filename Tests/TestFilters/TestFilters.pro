QT -= gui
QT += testlib

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        TestAlphaBetaFilter.cpp \
        TestAlphaBetaLeastSquaresFilter.cpp \
        TestKalmanConstAccelerationFilter.cpp \
        TestKalmanConstVelocityFilter.cpp \
        main.cpp \
        TestMatrix.cpp

HEADERS += \
    TestAlphaBetaFilter.h \
    TestAlphaBetaLeastSquaresFilter.h \
    TestKalmanConstAccelerationFilter.h \
    TestKalmanConstVelocityFilter.h \
    TestMatrix.h

TARGET = TestFilters

DESTDIR = ../../ProgramBuilds/TEST-Filters

OBJECTS_DIR = ../../ProgramBuilds/TEST-Filters

MOC_DIR = ../../ProgramBuilds/TEST-Filters

UI_DIR = ../../ProgramBuilds/TEST-Filters

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
include(../../PriFiles/Filters/Filters.pri)
