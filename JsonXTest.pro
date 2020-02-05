CONFIG -= qt
CONFIG += c++11 console

windows: {
    LIBS += -LC:\lib
    INCLUDEPATH += C:\include
}
unix: {
    LIBS += -L/usr/local/lib
    INCLUDEPATH += /usr/local/include
}

SOURCES += \
    io.cpp \
    jsonx.cpp \
    test.cpp

DISTFILES += \
    COPYING \
    README \
    agpl-3.0.html \
    agpl-3.0.txt \
    readme.md

HEADERS += \
    io.hpp \
    jsonx.hpp \
    scanner.hpp
