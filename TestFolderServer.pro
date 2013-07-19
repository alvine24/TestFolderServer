#The file .pro to build and run the project must contain at least :

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestFolderServer
TEMPLATE = app


SOURCES += TestFolderServer.cpp \
    main.cpp \
    C:/Users/sheerServer/Desktop/CairnsmithNew/CairnSmith/CairnSmith/Server/ClientLib/sheer_cloud.cpp \
    TestFolderSetServer.cpp \
    TestListFolderServer.cpp \
    TestRenderServer.cpp

HEADERS  += TestFolderServer.h \
    C:/Users/sheerServer/Desktop/CairnsmithNew/CairnSmith/CairnSmith/Server/ClientLib/sheer_cloud.h \
    TestFolderSetServer.h \
    TestListFolderServer.h \
    TestRenderServer.h

INCLUDEPATH += C:/Users/sheerServer/Projets/Qt/FileHash
INCLUDEPATH += C:/Users/sheerServer/Desktop/CairnsmithNew/CairnSmith/CairnSmith/Server/ClientLib
LIBS+=  -LC:/Users/sheerServer/Projets/Qt/FileHash-build-Desktop_Qt_5_0_1_MSVC2010_32bit-Debug/debug -lFileHash

#INCLUDEPATH += C:/Users/sheerServer/Desktop/Cairnsmith13/CairnSmith/CairnSmith/Server/ClientLib

FORMS += \
    TestFolderServer.ui \
    TestFolderSetServer.ui \
    TestListFolderServer.ui \
    TestRenderServer.ui
