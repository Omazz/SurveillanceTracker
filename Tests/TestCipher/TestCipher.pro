QT -= gui
QT += testlib
TEMPLATE = app
CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        TestCipher.cpp \
        main.cpp

TARGET = TestCipher

DESTDIR = ../../ProgramBuilds/TEST-Cipher

OBJECTS_DIR = ../../ProgramBuilds/TEST-Cipher

MOC_DIR = ../../ProgramBuilds/TEST-Cipher

UI_DIR = ../../ProgramBuilds/TEST-Cipher

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


include(../../PriFiles/Сipher/CipherFiles.pri)

HEADERS += \
    TestCipher.h

