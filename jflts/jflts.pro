QT += core gui widgets multimedia printsupport location quickwidgets websockets

CONFIG += console

TARGET = JugendfeuerwehrLeitstelle
TEMPLATE = app

SOURCES += main.cpp \
    durchsage.cpp touchview.cpp \
    karte.cpp \
    einsatz.cpp \
    einsatzform.cpp \
    pagingsystem.cpp \
    printer.cpp \
    dialogbma.cpp \
    audiohandler.cpp

HEADERS  += \
    durchsage.h touchview.h \
    karte.h \
    einsatz.h \
    einsatzform.h \
    pagingsystem.h \
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
dlltarget.commands = windeployqt --release --qmldir $$PWD ../deploy
configuration.path = $$OUT_PWD/../deploy/config
configuration.files = $$PWD/config/*
additionaldll.path = $$OUT_PWD/../deploy

win32-g++:contains(QMAKE_HOST.arch, x86_64): {
    additionaldll.files = $$PWD/dll/64/*
}
else {
    additionaldll.files = $$PWD/dll/32/*
}

INSTALLS += target dlltarget additionaldll configuration
