#include <QApplication>
#include "TestFolderServer.h"
#include "TestFolderSetServer.h"
#include "TestListFolderServer.h"
#include "TestRenderServer.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /*TestFolderServer myTest;
    myTest.show();*/

    TestFolderSetServer myTest1;
    myTest1.show();

    /*TestListFolderServer myTest2;
    myTest2.show();*/

    /*TestRenderServer myTest3;
    myTest3.show();*/

    return app.exec();
}
