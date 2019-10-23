TEMPLATE = aux
DISTFILES += \
    nc32.exe \
    nc64.exe \
    bma32.bat \
    bma64.bat \
    bma.txt

QMAKE_STRIP = echo

bmasender.path = $$OUT_PWD/../deploy/bma

win32-g++:contains(QMAKE_HOST.arch, x86_64): {
    bmasender.files = $$PWD/nc64.exe $$PWD/bma64.bat $$PWD/bma.txt
}
else {
    bmasender.files = $$PWD/nc32.exe $$PWD/bma32.bat $$PWD/bma.txt
}
INSTALLS += bmasender
