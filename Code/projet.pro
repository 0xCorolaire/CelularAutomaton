QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11

QT += widgets
QT += xml

SOURCES += \
    main.cpp \
    automate.cpp \
    autocell.cpp \
    simulateur.cpp \
    etats.cpp \
    choix.cpp


HEADERS += \
    autocell.h \
    automate.h \
    etats.h \
    simulateur.h \ 
    choix.h \
