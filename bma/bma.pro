TEMPLATE = aux
DISTFILES += \
    ncat.exe \
    bma.bat \
    bma.txt

QMAKE_STRIP = echo

bmasender.path = $$OUT_PWD/../deploy/bma
bmasender.files = $$PWD/ncat.exe $$PWD/bma.bat $$PWD/bma.txt

INSTALLS += bmasender
