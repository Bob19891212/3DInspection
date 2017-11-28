TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += core
QT += sql
QT += xml

SOURCES += \
    sdk/customexception.cpp \
    app/appsetting.cpp \
    app/capturesetting.cpp \
    sdk/rectangle.cpp \
    job/measuredobj.cpp \
    job/measuredobjlist.cpp \
    job/board.cpp \
    job/inspectiondata.cpp \
    main.cpp \
    sdk/formatconvertion.cpp \
    app/app.cpp \
    app/datageneration.cpp

HEADERS += \
    sdk/customexception.hpp \
    app/appsetting.hpp \
    app/capturesetting.hpp \
    sdk/rectangle.hpp \
    job/measuredobj.hpp \
    job/measuredobjlist.hpp \
    job/board.hpp \
    job/inspectiondata.hpp \
    sdk/formatconvertion.hpp \
    app/app.hpp \
    app/datageneration.hpp
