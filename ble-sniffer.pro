QT       += core gui\
            sql\
            network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11\
          no_keywords
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    advacombobox.cpp \
    bladerfdriver.cpp \
    blepacket.cpp \
    databuffer.cpp \
    demodulator.cpp \
    functionwidget.cpp \
    imagedelegate.cpp \
    main.cpp \
    mainwindow.cpp \
    settingswidget.cpp \
    thirdparty/lodepng/lodepng.cpp

HEADERS += \
    advacombobox.h \
    bladerfdriver.h \
    blepacket.h \
    databuffer.h \
    demodulator.h \
    functionwidget.h \
    imagedelegate.h \
    mainwindow.h\
    settingswidget.h \ \
    thirdparty/lodepng/lodepng.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    FPGA.qrc \
    Icon.qrc

FORMS += \
    functionwidget.ui \
    settingswidget.ui

unix:!macx: LIBS += -lbladeRF

DISTFILES += \
    regeister.py
