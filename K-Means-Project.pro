QT -= gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ChartView.cpp \
        ChartViewWidget.cpp \
        KMeansDialog.cpp \
        K_Means.cpp \
        MainWindow.cpp \
        ScatterWidget3D.cpp \
        initialization.cpp \
        main.cpp \
        metrics.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ChartView.h \
    ChartViewWidget.h \
    KMeansDialog.h \
    K_Means.h \
    MainWindow.h \
    ScatterWidget3D.h \
    initialization.h \
    metrics.h

FORMS += \
    ChartViewWidget.ui \
    KMeansDialog.ui \
    MainWindow.ui \
    ScatterWidget3D.ui
