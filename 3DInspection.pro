TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += core
QT += sql
QT += xml

SOURCES += \
    app.cpp \
    sdk/customexception.cpp \
    app/appsetting.cpp \
    app/capturesetting.cpp \
    app/setting.cpp \
    sdk/rectangle.cpp \
    job/measuredobj.cpp \
    job/measuredobjlist.cpp \
    datahelper/formatconvertion.cpp \
    job/board.cpp \
    job/inspectiondata.cpp

HEADERS += \
    sdk/customexception.hpp \
    app/appsetting.hpp \
    app/capturesetting.hpp \
    app/setting.hpp \
    sdk/rectangle.hpp \
    job/measuredobj.hpp \
    job/measuredobjlist.hpp \
    datahelper/formatconvertion.hpp \
    job/board.hpp \
    job/inspectiondata.hpp
