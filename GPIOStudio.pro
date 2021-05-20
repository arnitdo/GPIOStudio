QT += gui core widgets

SOURCES += src/main.cpp

HEADERS += src/main.hpp src/thirdparty/**

CONFIG += release

QMAKE_CXXFLAGS_RELEASE += -std=c++11 -v -O0

QMAKE_CXXFLAGS_RELEASE -= -O2