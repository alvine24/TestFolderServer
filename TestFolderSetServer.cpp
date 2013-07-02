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
        link = new SheerCloudLink(ui->hostLineEdit->text(), ui->userLineEdit->text(), ui->passwordLineEdit->text());
    }

    QByteArray result;
    link->Download(ui->fileLineEdit->text(), result);
    link->connect(link, SIGNAL(done()), this, SLOT(downloadDone()));
    ui->downloadPushButton->setEnabled(false);

    //save result into temp folder
    QString myTempPath = QDir::homePath()+"/Cairnsmith/server"; //here we set the folder in which server's files will be saved
    QString sFilepath = ui->fileLineEdit->text();
    QFile myFile(myTempPath+'/'+sFilepath);
    qDebug() << "le chemin : " << myTempPath+'/'+sFilepath;
    if(!myFile.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open : " << myFile.fileName();
        QMessageBox msgBox;
        msgBox.setText("The file is not saved.");//.arg(myFile.fileName()));
        msgBox.exec();
    }else {
        myFile.write(result.data());
        myFile.close();
        qDebug() << "The file is downloaded : " << myFile.fileName();
        QMessageBox msgBox;
        msgBox.setText("The file %1 is saved.");//.arg(myFile.fileName()));
        msgBox.exec();
    }
}

void TestFolderSetServer::uploadDone(){
    QMessageBox msgBox;
    msgBox.setText("The document is uploaded.");
    msgBox.exec();
    ui->uploadPushButton->setEnabled(true);
}

void TestFolderSetServer::downloadDone(){
    QMessageBox msgBox;
    msgBox.setText("The document is downloaded.");
    msgBox.exec();
    ui->downloadPushButton->setEnabled(true);
}
