#ifndef TESTLISTFOLDERSERVER_H
#define TESTLISTFOLDERSERVER_H

#include <QWidget>
#include <QFileSystemModel>
#include "../../../../Desktop/Cairnsmith13/CairnSmith/CairnSmith/Server/ClientLib/sheer_cloud.h"
#include <QEventLoop>
#include <QMessageBox>
#include <QDebug>
#include <QByteArray>
#include <QtTest/QTest>
#include "FileHash.h"

const QString sPathServer = QDir::homePath()+"/Cairnsmith/server/CSLibrairies";
const QString sPathLocal = QDir::homePath()+"/Cairnsmith/abc/CSLibrairies";

namespace Ui {
class TestListFolderServer;
}

class TestListFolderServer : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestListFolderServer(QWidget *parent = 0);
    ~TestListFolderServer();

    signals:
    void ready();

public slots:
    void on_treeView_clicked(const QModelIndex &index);
    void clickModel();
    void clickMaterial();
    void listDownloadMaterial();
    void listDownloadModel();
    void downloadDone();



private:
    Ui::TestListFolderServer *ui;
    SheerCloudLink *link;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    QModelIndex dirRootIndex;
    QModelIndex fileRootIndex;
    void getListFileServer(QString spath);
    void materialDone();
    void modelDone();
    int taille;
    QByteArray result;
    FileHash m_oHashMD5;
    QHash<QString, QString> mylistServer;
    QHash<QString, QString> mylistLocal;
    QHash<QString, QString> listFiles(QString mDir);
    QList<QString> compareFileList(QHash<QString, QString> local, QHash<QString, QString> remote);

};

#endif // TESTLISTFOLDERSERVER_H
