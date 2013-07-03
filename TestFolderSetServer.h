#ifndef TESTFOLDERSETSERVER_H
#define TESTFOLDERSETSERVER_H

#include <QWidget>
#include "../../../../Desktop/Cairnsmith13/CairnSmith/CairnSmith/Server/ClientLib/sheer_cloud.h"
#include <QEventLoop>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

namespace Ui {
class TestFolderSetServer;
}

class TestFolderSetServer : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestFolderSetServer(QWidget *parent = 0);
    ~TestFolderSetServer();

public slots:
    void uploadFile();
    void downloadFile();
    void activateButton();
    void uploadDone();
    void downloadDone();
    
private:
    Ui::TestFolderSetServer *ui;
    SheerCloudLink *link;
    QByteArray resultFile;
};

#endif // TESTFOLDERSETSERVER_H
