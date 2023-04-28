QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    bvh_node.cpp \
    hittable_list.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    postprocess.cpp \
    renderroute.cpp \
    sphere.cpp \
    triangle.cpp

HEADERS += \
    aabb.h \
    bvh_node.h \
    camera.h \
    disney.h \
    hittable.h \
    hittable_list.h \
    mainwindow.h \
    material.h \
    model.h \
    postprocess.h \
    ray.h \
    renderroute.h \
    rtweekend.h \
    scene.h \
    sphere.h \
    texture.h \
    triangle.h \
    vec3.h

FORMS += \
    mainwindow.ui

QMAKE_CXXFLAGS += -fopenmp
LIBS += -lgomp -lpthread
Qt += widgets

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
