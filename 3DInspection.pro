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
    src/setting.cpp \
    sdk/rectangle.cpp \
    job/measuredobj.cpp \
    job/measuredobjlist.cpp \
    datahelper/formatconvertion.cpp \
    job/board.cpp

HEADERS += \
    sdk/customexception.hpp \
    src/appsetting.hpp \
    src/capturesetting.hpp \
    src/setting.hpp \
    sdk/rectangle.hpp \
    job/measuredobj.hpp \
    job/measuredobjlist.hpp \
    datahelper/formatconvertion.hpp \
    job/board.hpp
