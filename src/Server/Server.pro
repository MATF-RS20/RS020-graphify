QT += gui
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cachedsong.cpp \
        collaborativefiltering.cpp \
        edge.cpp \
        entity.cpp \
        genre.cpp \
        genretraverse.cpp \
        gentrypoint.cpp \
        knowledgegraph.cpp \
        main.cpp \
        metadata.cpp \
        minheap.cpp \
        pentrypoint.cpp \
        performer.cpp \
        performergenretraverse.cpp \
        performersongtraverse.cpp \
        performertraverse.cpp \
        sentrypoint.cpp \
        server.cpp \
        song.cpp \
        songgenretraverse.cpp \
        songtraverse.cpp \
        traversebehavior.cpp \
        uentrypoint.cpp \
        user.cpp \
        worker.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cachedsong.h \
    collaborativefiltering.h \
    edge.h \
    entity.h \
    genre.h \
    genretraverse.h \
    gentrypoint.h \
    knowledgegraph.h \
    metadata.h \
    minheap.h \
    pentrypoint.h \
    performer.h \
    performergenretraverse.h \
    performersongtraverse.h \
    performertraverse.h \
    sentrypoint.h \
    server.h \
    song.h \
    songgenretraverse.h \
    songtraverse.h \
    traversebehavior.h \
    uentrypoint.h \
    user.h \
    worker.h

RESOURCES += \
    server.qrc
