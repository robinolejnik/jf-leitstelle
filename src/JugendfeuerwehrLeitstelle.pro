QT       += core gui widgets multimedia printsupport location quickwidgets

CONFIG += console

TARGET = JugendfeuerwehrLeitstelle
TEMPLATE = app


SOURCES += main.cpp \
    durchsage.cpp touchview.cpp \
    karte.cpp \
    einsatz.cpp \
    einsatzform.cpp \
    printer.cpp \
    dialogbma.cpp

HEADERS  += \
    durchsage.h touchview.h \
    karte.h \
    einsatz.h \
    einsatzform.h \
    printer.h \
    dialogbma.h

FORMS    += \
    touchview.ui \
    einsatzform.ui \
    dialogbma.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    map.qml \
    config/print.html \
    config/stichworte.txt \
    config/streets.txt
