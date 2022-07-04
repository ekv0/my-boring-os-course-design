QT       += core gui \
            charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 \
    resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpupage.cc \
    detail.cc \
    gui_common.cc \
    load.cc \
    mempage.cc \
    main.cc \
    performance.cc \
    taskmgr.cc \
    thumbnail.cc \
    user.cc \
    win32_common.cc

HEADERS += \
    cpupage.h \
    detail.h \
    gui_common.h \
    load.h \
    mempage.h \
    performance.h \
    taskmgr.h \
    thumbnail.h \
    user.h \
    win32_common.h

FORMS += \
    load.ui \
    taskmgr.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lPdh \
                -lpowrprof

INCLUDEPATH += 'D:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0'
DEPENDPATH += 'D:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0'

RC_ICONS = Taskmgr.ico
