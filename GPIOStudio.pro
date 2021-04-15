QT += gui core widgets

SOURCES += src/main.cpp

HEADERS += src/main.hpp src/thirdparty/**

CONFIG += release

QMAKE_CXXFLAGS += -std=c++11