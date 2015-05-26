TARGET = places_map
TEMPLATE = app

QT += quick qml network declarative widgets
SOURCES = \
    main.cpp \
    myclass.cpp \
    qmlapplicationviewer.cpp \
    auxiliary.cpp \
    ballcalc.cpp

CONFIG+=qml_debug


qmlcontent.files += $$files(*.qml) \
                    marker.png

target.path = $$[QT_INSTALL_EXAMPLES]/location/places_map
qmlcontent.path = $$[QT_INSTALL_EXAMPLES]/location/places_map

INSTALLS += target qmlcontent

HEADERS += \
    myclass.h \
    qmlapplicationviewer.h \
    auxiliary.h \
    ballcalc.h \

DISTFILES +=
    main.qml

RESOURCES += \
    res.qrc
