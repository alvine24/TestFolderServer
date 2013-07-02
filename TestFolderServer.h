#ifndef TESTFOLDERSERVER_H
#define TESTFOLDERSERVER_H

#include <QWidget>
#include "../../../../Desktop/Cairnsmith13/CairnSmith/CairnSmith/Server/ClientLib/sheer_cloud.h"
#include <QEventLoop>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

namespace Ui {
class TestFolderServer;
}

class TestFolderServer : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestFolderServer(QWidget *parent = 0);
    ~TestFolderServer();

public slots:
    void activateUpload();
    void uploadFile();
    void uploadDone();

private:
    Ui::TestFolderServer *ui;
    SheerCloudLink *link;

};

#endif // TESTFOLDERSERVER_H
