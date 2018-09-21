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
    dialogbma.cpp \
    audiohandler.cpp

HEADERS  += \
    durchsage.h touchview.h \
    karte.h \
    einsatz.h \
    einsatzform.h \
    printer.h \
    dialogbma.h \
    audiohandler.h

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

QMAKE_STRIP = echo
target.path = $$OUT_PWD/../deploy
dlltarget.path = $$OUT_PWD/../deploy
dlltarget.commands = windeployqt --qmldir $$PWD ../deploy
configuration.path = $$OUT_PWD/../deploy/config
configuration.files = $$PWD/config/*
additionaldll.path = $$OUT_PWD/../deploy
additionaldll.files = $$PWD/dll/*
INSTALLS += target dlltarget additionaldll configuration
