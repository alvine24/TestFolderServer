#include "TestRenderServer.h"
#include "ui_TestRenderServer.h"

TestRenderServer::TestRenderServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestRenderServer)
{
    ui->setupUi(this);

    connect(ui->hostLineEdit, SIGNAL(textChanged(QString)), SLOT(activateUploadButton()));
    connect(ui->nameFolderLineEdit, SIGNAL(textChanged(QString)), SLOT(activateUploadButton()));
    connect(ui->userNameLineEdit, SIGNAL(textChanged(QString)), SLOT(activateUploadButton()));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), SLOT(activateUploadButton()));

    connect(ui->sceneLineEdit, SIGNAL(textChanged(QString)), SLOT(activateRenderButton()));

    connect(ui->uploadFolderPushButton, SIGNAL(clicked()), SLOT(uploadFolder()));
    connect(ui->renderPushButton, SIGNAL(clicked()), SLOT(askRender()));

    link = NULL;
    serverPath = "";
    resultRender = false;
}

TestRenderServer::~TestRenderServer()
{
    delete ui;
    if( link != NULL)
        delete link;
}

void TestRenderServer::activateUploadButton(){
    bool ok = (!ui->hostLineEdit->text().isEmpty()) && (!ui->nameFolderLineEdit->text().isEmpty())
            && (!ui->userNameLineEdit->text().isEmpty()) && (!ui->passwordLineEdit->text().isEmpty());
    ui->uploadFolderPushButton->setEnabled(ok);
}

void TestRenderServer::activateRenderButton(){
    bool ok = (!ui->hostLineEdit->text().isEmpty()) && (!ui->userNameLineEdit->text().isEmpty())
            && (!ui->passwordLineEdit->text().isEmpty()) && (!ui->sceneLineEdit->text().isEmpty());
    ui->renderPushButton->setEnabled(ok);
}

void TestRenderServer::uploadFolder(){

    ui->uploadFolderPushButton->setEnabled(false);
    ui->renderPushButton->setEnabled(false);

    QString myFolder = ui->nameFolderLineEdit->text();
    createListUpload(myFolder,serverPath);
    fileList = myHash.keys();
    if(!fileList.isEmpty()){
        uploadFile(fileList[0], myHash[fileList[0]]);
    }
}

void TestRenderServer::createListUpload(QString myFolder,QString path){
    QDir dir(myFolder);
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QFileInfo finfo, list){
        if(!finfo.isDir()){ //it's a file; upload to server
            QFile *file = new QFile(finfo.absoluteFilePath());
            file->open(QIODevice::ReadOnly);
            QByteArray massive = file->readAll();
            myHash.insert(path+"/"+finfo.fileName(), massive);
            delete file;
        }else
            createListUpload(finfo.absoluteFilePath(), path + "/" + finfo.fileName());
    }
}

void TestRenderServer::uploadFile(QString resPath, QByteArray data){
    if(link == NULL){
        link = new SheerCloudLink("http://172.245.20.58:8080", "abc", "123");//http://172.245.20.58:8080
        connect(link, SIGNAL(done()), this, SLOT(uploadDone()));
    }

    link->Upload(resPath, data);
    myHash.remove(fileList[0]);
    fileList = myHash.keys();
}

void TestRenderServer::uploadDone(){
    if(fileList.isEmpty()){
        ui->uploadFolderPushButton->setEnabled(true);
        ui->renderPushButton->setEnabled(true);
    }else {
            uploadFile(fileList[0], myHash[fileList[0]]);
    }
}

void TestRenderServer::askRender(){

    ui->uploadFolderPushButton->setEnabled(false);
    ui->renderPushButton->setEnabled(false);

    if(link == NULL){
        link = new SheerCloudLink("http://172.245.20.58:8080", "abc", "123");//http://172.245.20.58:8080
        //link->connect(link, SIGNAL(done()), this, SLOT(progressDone()));
    }

    QString scene = ui->sceneLineEdit->text();
    link->Job(scene, rendering);
    connect(link, SIGNAL(done()), this, SLOT(renderDone()));
}

void TestRenderServer::renderDone(){
    if(link == NULL){
        link = new SheerCloudLink("http://172.245.20.58:8080", "abc", "123");//http://172.245.20.58:8080
        //link->connect(link, SIGNAL(done()), this, SLOT(progressDone()));
    }
    if(!resultRender){
        link->Progress(rendering, resultRender);
        //link->connect(link, SIGNAL(done()), this, SLOT(progressDone()));
        qDebug() << "Progressing : "<< resultRender;
    }else{
        qDebug() << "It is done : "<< resultRender;
        qDebug() << "The result of Rendering :" << rendering;
        ui->uploadFolderPushButton->setEnabled(true);
        ui->renderPushButton->setEnabled(true);
    }
}

/*void TestRenderServer::progressDone(){
    if(resultRender){
        qDebug() << "It is done : "<< resultRender;
        qDebug() << "The result of Rendering :" << rendering;
        ui->uploadFolderPushButton->setEnabled(true);
        ui->renderPushButton->setEnabled(true);
        if(link){
              link->disconnect(link,0,0,0);
              link = NULL;
        }
    }else{
        qDebug() << "Progressing : "<< resultRender;
        renderDone();
    }
}*/
