CONFIG -= core gui qt
TEMPLATE = lib
CONFIG += c++11 staticlib debug_and_release build_all

CONFIG(debug, debug|release) {
    TARGET = jsonxd
} else {
    TARGET = jsonx
}

DISTFILES += \
    COPYING \
    Doxyfile \
    README \
    agpl-3.0.html \
    agpl-3.0.txt \
    readme.md

SOURCES += \
    io.cpp \
    jsonx.cpp

HEADERS += \
    io.hpp \
    jsonx.hpp

headerFiles.files = jsonx.hpp

unix {
    INCLUDEPATH += /usr/local/include
    target.path = /usr/local/lib
    headerFiles.path = /usr/local/include
}
windows {
    INCLUDEPATH += C:\include
    LIBS += -lsetupapi
    target.path = C:\lib
    headerFiles.path = C:\include
}

!isEmpty(target.path): INSTALLS += target headerFiles
