TEMPLATE = aux
DISTFILES += \
    nc64.exe \
    bma.bat \
    bma.txt

QMAKE_STRIP = echo

bmasender.path = $$OUT_PWD/../deploy/bma
bmasender.files = $$PWD/nc64.exe $$PWD/bma.bat $$PWD/bma.txt
INSTALLS += bmasender
