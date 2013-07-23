#ifndef TESTFOLDERSETSERVER_H
#define TESTFOLDERSETSERVER_H

#include <QWidget>
#include "sheer_cloud.h"
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
    void cancelAction();
    void progressUploadBar(qint64 bytesSent, qint64 bytesTotal);
    void progressDownloadBar(qint64 bytesReceived, qint64 bytesTotal);
    
private:
    Ui::TestFolderSetServer *ui;
    SheerCloudLink *link;
    QByteArray resultFile;
};

#endif // TESTFOLDERSETSERVER_H
