QT -= gui
QT += testlib
TEMPLATE = app
CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        TestEncryptionTools.cpp \
        main.cpp

HEADERS += \
        TestEncryptionTools.h

TARGET = TestEncryptionTools

DESTDIR = ../../ProgramBuilds/TEST-EncryptionTools

OBJECTS_DIR = ../../ProgramBuilds/TEST-EncryptionTools

MOC_DIR = ../../ProgramBuilds/TEST-EncryptionTools

UI_DIR = ../../ProgramBuilds/TEST-EncryptionTools

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


include(../../PriFiles/EncryptionTools/EncryptionTools.pri)

