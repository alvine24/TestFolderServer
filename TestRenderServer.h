#ifndef TESTRENDERSERVER_H
#define TESTRENDERSERVER_H

#include <QWidget>
#include "sheer_cloud.h"
#include <QDebug>
#include <QByteArray>
#include <QtTest/QTest>
#include "FileHash.h"
#include <QFile>
#include <QDir>


namespace Ui {
class TestRenderServer;
}

class TestRenderServer : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestRenderServer(QWidget *parent = 0);
    ~TestRenderServer();

public slots:
    void activateUploadButton();
    void activateRenderButton();
    void uploadFolder();
    void askRender();
    void uploadDone();
    void renderDone();
    //void progressDone();

private:
    Ui::TestRenderServer *ui;
    SheerCloudLink *link;
    bool isRenderOk;
    void createListUpload(QString myFolder, QString path);
    QFileInfoList myList;
    void uploadFile(QString resPath, QByteArray data);
    QString serverPath;
    QHash<QString, QByteArray> myHash;
    QList<QString> fileList;
    JobID rendering;
    JobResult resultRender;
};

#endif // TESTRENDERSERVER_H
