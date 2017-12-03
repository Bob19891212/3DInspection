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
    app/datageneration.cpp \
    sdk/DB/blob.cpp \
    sdk/DB/sqlitedb.cpp \
    app/mainwindow.cpp \
    app/config.cpp \
    sdk/numrandom.cpp

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
    app/datageneration.hpp \
    sdk/DB/blob.hpp \
    sdk/DB/sqlitedb.hpp \
    app/mainwindow.hpp \
    app/config.hpp \
    sdk/numrandom.hpp

INCLUDEPATH += $$PWD/include/sqlits
INCLUDEPATH += $$PWD/include

unix::LIBS += -L$$PWD/lib/ -lsqlite3

unix:LIBS += -L/usr/lib/x86_64-linux-gnu\
-ldl
