QT += quick
QT += xml
QT += widgets

CONFIG += c++11

RC_ICONS = icono.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        edge.cpp \
        graph.cpp \
        graph_object.cpp \
        loader_object.cpp \
        main.cpp \
        map.cpp \
        mapprovider.cpp \
        track.cpp \
        tracksearcher.cpp \
        utm.cpp \
        vertex.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += C:\opencv-build\install\include

LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_calib3d454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_core454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_dnn454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_features2d454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_flann454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_gapi454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_highgui454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_imgcodecs454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_imgproc454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_ml454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_objdetect454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_photo454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_stitching454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_video454.dll
LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_videoio454.dll

HEADERS += \
    edge.h \
    graph.h \
    graph_object.h \
    loader_object.h \
    map.h \
    mapprovider.h \
    track.h \
    tracksearcher.h \
    utm.h \
    vertex.h
