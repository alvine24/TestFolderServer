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
    connect(ui->cancelPushButton, SIGNAL(clicked()), SLOT(cancelAction()));

    /*ui->fileLineEdit->setText("c:/data/file.txt");
    ui->lineEdit->setText("123");
    ui->userLineEdit->setText("abc");*/
    link = NULL;
    resultFile.clear();
    ui->progressBar->setValue(0);
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
    ui->uploadPushButton->setEnabled(false);
    ui->downloadPushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(true);

    QFile *file = new QFile(ui->fileLineEdit->text());
    if(file->open(QIODevice::ReadOnly)){
        QString myFilename;// = file->fileName().section('/', -1);
        if(!ui->folderPathLineEdit->text().isEmpty())
            myFilename = ui->folderPathLineEdit->text();
        else
            myFilename = file->fileName().section('/', -1);

        QByteArray massive = file->readAll();
        qDebug() << "Size of data: " << massive.size() << myFilename;
        link->Upload(myFilename, massive);
        link->connect( link, SIGNAL(done()), this, SLOT(uploadDone()));
        link->connect(link, SIGNAL(progress(qint64,qint64)), this, SLOT(progressUploadBar(qint64,qint64)));
    }else{
        QMessageBox msgBox;
        msgBox.setText("The document is not sent.");
        msgBox.exec();
        ui->uploadPushButton->setEnabled(true);
        ui->downloadPushButton->setEnabled(true);
        ui->cancelPushButton->setEnabled(false);
    }
}

void TestFolderSetServer::downloadFile(){
    if(link == NULL){
        link = new SheerCloudLink(ui->hostLineEdit->text(), ui->userLineEdit->text(), ui->passwordLineEdit->text());//http://172.245.20.58:8080
    }
    ui->uploadPushButton->setEnabled(false);
    ui->downloadPushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(true);
    link->Download(ui->fileLineEdit->text(), resultFile);
    link->connect(link, SIGNAL(progress(qint64,qint64)), this, SLOT(progressDownloadBar(qint64,qint64)));
    link->connect(link, SIGNAL(done()), this, SLOT(downloadDone()));
}

void TestFolderSetServer::uploadDone(){
    link->deleteLater();
    link = NULL;
    QMessageBox msgBox;
    msgBox.setText("The document is uploaded.");
    msgBox.exec();
    ui->uploadPushButton->setEnabled(true);
    ui->downloadPushButton->setEnabled(true);
    ui->progressBar->setValue(0);
    ui->cancelPushButton->setEnabled(false);
}

void TestFolderSetServer::downloadDone(){
    link->deleteLater();
    link = NULL;
    //save result into temp folder
    QString myTempPath = QDir::homePath()+"/Cairnsmith/server"; //here we set the folder in which server's files will be saved
    QString sFilepath = ui->fileLineEdit->text();
    QFile myFile(myTempPath+'/'+sFilepath);
    if(!myFile.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open : " << myFile.fileName();
    }else {
        //test on the size of the file
        //if(resultFile.size() == totalByte){
            qDebug() << "Data From Server"<< resultFile.size();
            myFile.write(resultFile.data(), resultFile.size());
            myFile.close();
            QMessageBox msgBox;
            msgBox.setText("The document is downloaded.");
            msgBox.exec();
            ui->downloadPushButton->setEnabled(true);
            ui->uploadPushButton->setEnabled(true);
            ui->progressBar->setValue(0);
            ui->cancelPushButton->setEnabled(false);
            qDebug() << "The file is downloaded : " << myFile.fileName();
        //}
    }
}

void TestFolderSetServer::cancelAction(){
    ui->uploadPushButton->setEnabled(true);
    ui->downloadPushButton->setEnabled(true);
    ui->cancelPushButton->setEnabled(false);
    ui->progressBar->setValue(0);

    link->disconnect();
    link->deleteLater();
    link = NULL;
}

void TestFolderSetServer::progressUploadBar(qint64 bytesSent, qint64 bytesTotal){
    qDebug() << "Bytes Sent :" << bytesSent;
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesSent);
}


void TestFolderSetServer::progressDownloadBar(qint64 bytesReceived, qint64 bytesTotal){
    qDebug() << "Bytes Total: " << bytesTotal;
    ui->progressBar->setMaximum(23467684);// here must be set the size of the file that is downloading
    ui->progressBar->setValue(bytesReceived);
}
