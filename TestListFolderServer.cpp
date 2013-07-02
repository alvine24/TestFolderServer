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
    result = 0;
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
        link = new SheerCloudLink("http://localhost:8080", "abc", "123");//http://172.245.20.58:8080
    }
    result = 0;

    QString spath = "CSLibrairies/"+ui->materialsPushButton->text();
    link->List(spath, result);

    link->connect(link, SIGNAL(done()), this, SLOT(listDownloadMaterial()));
}


void TestListFolderServer::materialDone(){

    //QString sPath = QDir::homePath()+"/Cairnsmith/server/CSLibrairies";

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirRootIndex = dirModel->setRootPath(sPathServer);
    ui->treeView->setModel(dirModel);
    ui->treeView->setRootIndex(dirRootIndex);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileRootIndex = fileModel->setRootPath(sPathServer);

    ui->listView->setModel(fileModel);
    ui->listView->setRootIndex(fileRootIndex);

    ui->materialsPushButton->setEnabled(true);
    ui->modelsPushButton->setEnabled(true);
}

void TestListFolderServer::listDownloadMaterial(){
    ui->materialsPushButton->setEnabled(false);
    ui->modelsPushButton->setEnabled(false);

    QString myResult = result.data();
    QStringList list = myResult.split("\n");
    foreach(QString s, list){
        if(s.contains('/')){
            qDebug() << "Ici on a : " << s;
            getListFileServer(s);
            taille+=1;
        }
    }
    materialDone();
}

void TestListFolderServer::downloadDone(){
    /*QMessageBox msgBox;
    msgBox.setText("The document is downloaded.");
    msgBox.exec();*/
}

void TestListFolderServer::getListFileServer(QString spath){
    if(link == NULL){
        link = new SheerCloudLink("http://localhost:8080", "abc", "123");//http://172.245.20.58:8080
    }
    QByteArray resultat;
    link->Download(spath, resultat);
    link->connect(link, SIGNAL(done()), this, SLOT(downloadDone()));

    //save result into temp folder
    QString myTempPath = QDir::homePath()+"/Cairnsmith/server"; //here we set the folder in which server's files will be saved
    QFile myFile(myTempPath+'/'+spath);
    qDebug() << "le chemin : " << myTempPath+'/'+spath;
    if(!myFile.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open : " << myFile.fileName();
    }else {
        myFile.write(resultat.data());
        myFile.close();
    }
}


void TestListFolderServer::clickModel(){
    if(link == NULL){
        link = new SheerCloudLink("http://localhost:8080", "abc", "123");//http://172.245.20.58:8080
    }
    result =0;
    QString spath = "CSLibrairies/"+ui->modelsPushButton->text();

    link->List(spath, result);

    link->connect(link, SIGNAL(done()), this, SLOT(listDownloadModel()));
}


void TestListFolderServer::listDownloadModel(){
    ui->materialsPushButton->setEnabled(false);
    ui->modelsPushButton->setEnabled(false);

    QString myResult = result.data();
    QStringList list = myResult.split("\n");
    foreach(QString s, list){
        if(s.contains('/')){
            qDebug() << "Ici on a : " << s;
            getListFileServer(s);
            taille+=1;
        }
    }
    modelDone();
}

void TestListFolderServer::modelDone(){
    //QString sPath = QDir::homePath()+"/Cairnsmith/server/CSLibrairies";
    qDebug() << "My current Path is :" << sPathServer;

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirRootIndex = dirModel->setRootPath(sPathServer);
    ui->treeView->setModel(dirModel);
    ui->treeView->setRootIndex(dirRootIndex);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
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
