TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT+=core
QT+=sql

SOURCES += \
    src/app.cpp \
    sdk/customexception.cpp \
    src/appsetting.cpp \
    src/capturesetting.cpp \
    src/setting.cpp

HEADERS += \
    sdk/customexception.hpp \
    src/appsetting.hpp \
    src/capturesetting.hpp \
    src/setting.hpp
