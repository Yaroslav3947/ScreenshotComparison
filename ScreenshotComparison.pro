QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comparisonresult.cpp \
    databasemanager.cpp \
    imagecomparator.cpp \
    main.cpp \
    mainwindow.cpp \
    screenshotsnap.cpp

HEADERS += \
    SimilarityCalculationTask.h \
    comparisonresult.h \
    databasemanager.h \
    imagecomparator.h \
    mainwindow.h \
    screenshotsnap.h

FORMS += \
    mainwindow.ui

win32: LIBS += -lopencv_world460
else: unix: LIBS += -lopencv_world460

win32: LIBS += -L$$PWD/../../../../opencv/build/x64/vc15/lib/
else: unix: LIBS += -L/usr/local/lib/

INCLUDEPATH += $$PWD/../../../../opencv/build/include
DEPENDPATH += $$PWD/../../../../opencv/build/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
