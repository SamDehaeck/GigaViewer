#include "fileinputdialog.h"
#include "ui_fileinputdialog.h"

FileInputDialog::FileInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInputDialog)
{
    ui->setupUi(this);
    currentDir=QDir::home();
}

FileInputDialog::~FileInputDialog()
{
    delete ui;
}

void FileInputDialog::on_filePushButton_clicked()
{
    QString resp = QFileDialog::getOpenFileName(this,tr("Open Picture"),
    currentDir.absolutePath(), tr("All files (*.*)") );
    if (resp=="") {
        currentDir=QDir::home();
    } else {
        currentDir=QDir(resp);
    }
    emit StaticPicPressed(resp);
}

void FileInputDialog::on_camButton_clicked() {
    emit OpencvFeedPressed();
}

// load movie button
void FileInputDialog::on_MovieButton_clicked()
{
    QString resp = QFileDialog::getOpenFileName(this,tr("Open Movie"),
    currentDir.absolutePath(), tr("All files (*.*)") );
    if (resp=="") {
        currentDir=QDir::home();
    } else {
        currentDir=QDir(resp);
    }
    emit MoviePressed(resp);
}

void FileInputDialog::on_pushButton_2_clicked()
{
    emit CloseApp();
}

void FileInputDialog::on_AvtButton_clicked()
{
    emit AvtFeedPressed();
}
