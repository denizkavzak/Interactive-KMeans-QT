QT -= gui
QT += charts
QT += datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ChartView.cpp \
        ChartViewWidget.cpp \
        InputHandlerPanning.cpp \
        KMeansDialog.cpp \
        K_Means.cpp \
        MainWindow.cpp \
        NDViewWidget.cpp \
        Scatter3DVis.cpp \
        Scatter3DWidget.cpp \
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
    InputHandlerPanning.h \
    KMeansDialog.h \
    K_Means.h \
    MainWindow.h \
    NDViewWidget.h \
    Scatter3DVis.h \
    Scatter3DWidget.h \
    initialization.h \
    metrics.h

FORMS += \
    ChartViewWidget.ui \
    KMeansDialog.ui \
    MainWindow.ui \
    NDViewWidget.ui \
    Scatter3DWidget.ui
