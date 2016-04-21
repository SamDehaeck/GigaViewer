#include "fileinputdialog.h"
#include "ui_fileinputdialog.h"
#include <QFileDialog>

FileInputDialog::FileInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInputDialog)
{
    ui->setupUi(this);

#ifndef PVAPI
    ui->AvtButton->setEnabled(false);
#endif


#ifndef IDS
    ui->idsButton->setEnabled(false);
#endif

#ifndef VIMBA
    ui->vimbaButton->setEnabled(false);
#endif

    currentDir=QDir::home();
}

FileInputDialog::~FileInputDialog()
{
    delete ui;
}

// load picture from disk button
void FileInputDialog::on_filePushButton_clicked()
{
    QString resp = QFileDialog::getOpenFileName(0,tr("Open Picture"),
    currentDir.absolutePath(), tr("All files (*.*)") );
    if (resp=="") {
        currentDir=QDir::home();
    } else {
        currentDir=QDir(resp);
    }
    emit StaticPicPressed(resp);
}

// load opencvFeed
void FileInputDialog::on_camButton_clicked() {
    emit OpencvFeedPressed();
}

// load movie (from disk) button
void FileInputDialog::on_MovieButton_clicked()
{
    QString resp = QFileDialog::getOpenFileName(0,tr("Open Movie"),
    currentDir.absolutePath(), tr("All files (*.*)") );
    if (resp=="") {
        currentDir=QDir::home();
    } else {
        currentDir=QDir(resp);
    }
    emit MoviePressed(resp);
}

// exit button
void FileInputDialog::on_pushButton_2_clicked()
{
    emit CloseApp();
}

// Avt (PvAPI) feed (Prosilica)
void FileInputDialog::on_AvtButton_clicked()
{
    emit AvtFeedPressed();
}

void FileInputDialog::on_vimbaButton_clicked()
{
    emit VimbaFeedPressed();
}

void FileInputDialog::on_idsButton_clicked()
{
    emit IdsFeedPressed();
}
