#include "TestListFolderServer.h"
#include "ui_TestListFolderServer.h"

TestListFolderServer::TestListFolderServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestListFolderServer)
{
    ui->setupUi(this);
    connect(ui->materialsPushButton, SIGNAL(clicked()), SLOT(clickMaterial()));
    connect(ui->modelsPushButton, SIGNAL(clicked()), SLOT(clickModel()));

    link = NULL;
    taille = 0;
    resultList.clear();
    resultFile.clear();
    spathResFile.clear();
    spathResList.clear();
}

TestListFolderServer::~TestListFolderServer()
{
    delete ui;
    if( link != NULL)
        delete link;
}

void TestListFolderServer::on_treeView_clicked(const QModelIndex &index){
    QString snPath = dirModel->filePath(index);
    ui->listView->setRootIndex(fileModel->setRootPath(snPath));
    fileModel->setNameFilters(QStringList());
}

void TestListFolderServer::clickMaterial(){
    if(link == NULL){
        link = new SheerCloudLink("http://172.245.20.58:8080", "abc", "123");//http://172.245.20.58:8080
    }

    spathResList.clear();
    resultList.clear();
    spathResList = "CSLibrairies/"+ui->materialsPushButton->text()+"/Color";
    qDebug() << "Folder :" << spathResList;
    link->List(spathResList, resultList);

    link->connect(link, SIGNAL(done()), this, SLOT(listDownload()));
    iDownloadwhat = 1;
}


void TestListFolderServer::listDownload(){
    qDebug()<< "TestListFolderServer::listDownload";
    link->deleteLater();
    link = NULL;

    ui->materialsPushButton->setEnabled(false);
    ui->modelsPushButton->setEnabled(false);

    list.clear();
    QString myResult = resultList.data();
    qDebug()<< "The List: " << myResult;
    QStringList myListResult = myResult.split("\n");
    foreach(QString s, myListResult){
        if(s.contains('/')){
            list.append(s);
        }
    }
    qDebug()<< "The List: " << list;
    //get file from the first
    if (!list.empty()) {
        QString s = list.at(0);
        list.pop_front();
        getListFileServer(s);
    }
}


void TestListFolderServer::getListFileServer(QString spath){
    qDebug() << "TestListFolderServer::getListFileServer";
    if(link == NULL){
        link = new SheerCloudLink("http://172.245.20.58:8080", "abc", "123");//http://172.245.20.58:8080
    }

    //QByteArray res;
    spathResFile.clear();
    resultFile.clear();
    spathResFile = spath;

    link->Download(spath, resultFile);
    link->connect(link, SIGNAL(done()), this, SLOT(downloadDone()));
}

void TestListFolderServer::materialDone(){

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirRootIndex = dirModel->setRootPath(sPathServer);
    ui->treeView->setModel(dirModel);
    ui->treeView->setRootIndex(dirRootIndex);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    fileRootIndex = fileModel->setRootPath(sPathServer);

    ui->listView->setModel(fileModel);
    ui->listView->setRootIndex(fileRootIndex);

    ui->materialsPushButton->setEnabled(true);
    ui->modelsPushButton->setEnabled(true);
}

void TestListFolderServer::downloadDone(){
    link->deleteLater();
    link = NULL;
    qDebug() << "TestListFolderServer::downloadDone";

    QFile myFile(myTempPath+'/'+spathResFile);
    if(!myFile.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open : " << myFile.fileName();
    }else {
        myFile.write(resultFile.data(), resultFile.size());
        myFile.close();
    }
    if (list.empty()) {
        if(iDownloadwhat == 1)
            materialDone();
        else if (iDownloadwhat == 2)
            modelDone();
    }
    else {
        QString s = list.at(0);
        list.pop_front();
        getListFileServer(s);
    }
}


void TestListFolderServer::clickModel(){
    if(link == NULL){
        link = new SheerCloudLink("http://172.245.20.58:8080", "abc", "123");//http://172.245.20.58:8080
    }
    resultList.clear();
    spathResList.clear();
    spathResList = ui->modelsPushButton->text();

    link->List(spathResList, resultList);

    link->connect(link, SIGNAL(done()), this, SLOT(listDownload()));
    iDownloadwhat = 2;
}


void TestListFolderServer::modelDone(){

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirRootIndex = dirModel->setRootPath(sPathServer);
    ui->treeView->setModel(dirModel);
    ui->treeView->setRootIndex(dirRootIndex);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    fileRootIndex = fileModel->setRootPath(sPathServer);

    ui->listView->setModel(fileModel);
    ui->listView->setRootIndex(fileRootIndex);

    ui->materialsPushButton->setEnabled(true);
    ui->modelsPushButton->setEnabled(true);
}

QHash<QString, QString> TestListFolderServer::listFiles(QString sDir){
    QHash<QString, QString> myResult;
    QDir dir(sDir);
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QFileInfo finfo, list) {
        if (finfo.isDir()) {
            listFiles(finfo.absoluteFilePath());
        } else {
            //Calcul MD5 and store in a QHash
            QString m_sResValCryptoMD5;
            m_oHashMD5.SetInFile(finfo.absoluteFilePath());
            m_oHashMD5.SetOutFile(QDir::tempPath());
            m_oHashMD5.CalculateCryptoMD5();
            if(m_oHashMD5.IsOk() == true){
                m_sResValCryptoMD5 = m_oHashMD5.GetResCryptoMD5();
                myResult.insert(m_sResValCryptoMD5,finfo.absoluteDir().dirName()+"/"+finfo.fileName());
            }
        }
    }
    return myResult;
}

QList<QString> TestListFolderServer::compareFileList(QHash<QString, QString> local, QHash<QString, QString> remote){
    QString key;
    QList<QString> resultList;
    foreach(key, local.keys()){
        if(!remote.contains(key) || remote[key] != local[key]){
            resultList.append(local[key]);
        }
    }
    return resultList;
}
