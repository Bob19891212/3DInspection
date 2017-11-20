TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/app.cpp \
    sdk/customexception.cpp

HEADERS += \
    sdk/customexception.hpp
