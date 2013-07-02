#include "TestFolderServer.h"
#include "ui_TestFolderServer.h"

TestFolderServer::TestFolderServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestFolderServer)
{
    ui->setupUi(this);

    connect(ui->userLineEdit, SIGNAL(textChanged(QString)), SLOT(activateUpload()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), SLOT(activateUpload()));
    connect(ui->fileLineEdit, SIGNAL(textChanged(QString)), SLOT(activateUpload()));
    connect(ui->uploadPushButton, SIGNAL(clicked()), SLOT(uploadFile()));

    /*ui->fileLineEdit->setText("c:/data/file.txt");
    ui->lineEdit->setText("123");
    ui->userLineEdit->setText("abc");*/
    link = NULL;
}

TestFolderServer::~TestFolderServer()
{
    delete ui;
    if( link != NULL)
        delete link;
}

void TestFolderServer::activateUpload(){
    if((!ui->userLineEdit->text().isEmpty()) && (!ui->lineEdit->text().isEmpty()) && (!ui->fileLineEdit->text().isEmpty()))
        ui->uploadPushButton->setEnabled(true);
}

void TestFolderServer::uploadFile(){
    if(link == NULL){
        link = new SheerCloudLink("http://localhost:8080", ui->userLineEdit->text(), ui->lineEdit->text());
    }

    QFile *file = new QFile(ui->fileLineEdit->text());
    if(file->open(QIODevice::ReadOnly)){
        QString myFilename = file->fileName().section('/', -1);
        qDebug() << myFilename << "File is ok";
        QByteArray massive = file->readAll();
        link->Upload(myFilename, massive);
        //loop.exec();
        link->connect( link, SIGNAL(done()), this, SLOT(uploadDone()));
        ui->uploadPushButton->setEnabled(false);
    }else{
        QMessageBox msgBox;
        msgBox.setText("The document is not sent.");
        msgBox.exec();
    }
}

void TestFolderServer::uploadDone(){
    QMessageBox msgBox;
    msgBox.setText("The document is sent.");
    msgBox.exec();
    ui->uploadPushButton->setEnabled(true);
}

