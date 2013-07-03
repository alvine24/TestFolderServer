#include "TestFolderSetServer.h"
#include "ui_TestFolderSetServer.h"

TestFolderSetServer::TestFolderSetServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestFolderSetServer)
{
    ui->setupUi(this);

    connect(ui->hostLineEdit, SIGNAL(textChanged(QString)), SLOT(activateButton()));
    connect(ui->userLineEdit, SIGNAL(textChanged(QString)), SLOT(activateButton()));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), SLOT(activateButton()));
    connect(ui->fileLineEdit, SIGNAL(textChanged(QString)), SLOT(activateButton()));

    connect(ui->uploadPushButton, SIGNAL(clicked()), SLOT(uploadFile()));
    connect(ui->downloadPushButton, SIGNAL(clicked()), SLOT(downloadFile()));

    /*ui->fileLineEdit->setText("c:/data/file.txt");
    ui->lineEdit->setText("123");
    ui->userLineEdit->setText("abc");*/
    link = NULL;
    resultFile.clear();
}

TestFolderSetServer::~TestFolderSetServer()
{
    delete ui;
    if( link != NULL)
        delete link;
}

void TestFolderSetServer::activateButton(){
    bool ok = !ui->hostLineEdit->text().isEmpty() && !ui->userLineEdit->text().isEmpty() && !ui->passwordLineEdit->text().isEmpty() && !ui->fileLineEdit->text().isEmpty();
    ui->downloadPushButton->setEnabled(ok);
    ui->uploadPushButton->setEnabled(ok);
}

void TestFolderSetServer::uploadFile(){
    if(link == NULL){
        link = new SheerCloudLink(ui->hostLineEdit->text(), ui->userLineEdit->text(), ui->passwordLineEdit->text());
    }

    QFile *file = new QFile(ui->fileLineEdit->text());
    if(file->open(QIODevice::ReadOnly)){
        QString myFilename = file->fileName().section('/', -1);
        QByteArray massive = file->readAll();
        qDebug() << "Size of data: " << massive.size();
        link->Upload(myFilename, massive);
        link->connect( link, SIGNAL(done()), this, SLOT(uploadDone()));
        ui->uploadPushButton->setEnabled(false);
    }else{
        QMessageBox msgBox;
        msgBox.setText("The document is not sent.");
        msgBox.exec();
    }
}

void TestFolderSetServer::downloadFile(){
    if(link == NULL){
        link = new SheerCloudLink(ui->hostLineEdit->text(), ui->userLineEdit->text(), ui->passwordLineEdit->text());//http://172.245.20.58:8080/
    }

    link->Download(ui->fileLineEdit->text(), resultFile);
    link->connect(link, SIGNAL(done()), this, SLOT(downloadDone()));
}

void TestFolderSetServer::uploadDone(){
    QMessageBox msgBox;
    msgBox.setText("The document is uploaded.");
    msgBox.exec();
    ui->uploadPushButton->setEnabled(true);
}

void TestFolderSetServer::downloadDone(){

    //save result into temp folder
    ui->downloadPushButton->setEnabled(false);
    QString myTempPath = QDir::homePath()+"/Cairnsmith/server"; //here we set the folder in which server's files will be saved
    QString sFilepath = ui->fileLineEdit->text();
    QFile myFile(myTempPath+'/'+sFilepath);
    if(!myFile.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open : " << myFile.fileName();
    }else {
        myFile.write(resultFile.data(), resultFile.size());
        myFile.close();
        QMessageBox msgBox;
        msgBox.setText("The document is downloaded.");
        msgBox.exec();
        ui->downloadPushButton->setEnabled(true);
        qDebug() << "The file is downloaded : " << myFile.fileName();
    }
}
