#include "fileinputdialog.h"
#include "ui_fileinputdialog.h"

FileInputDialog::FileInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInputDialog)
{
    ui->setupUi(this);
}

FileInputDialog::~FileInputDialog()
{
    delete ui;
}

void FileInputDialog::on_filePushButton_clicked()
{
    QString resp = QFileDialog::getOpenFileName(this,tr("Open Picture"),
    QDir::currentPath(), tr("All files (*.*)") );

    emit StaticPicPressed(resp);
}

void FileInputDialog::on_camButton_toggled(bool checked)
{
    emit OpencvFeedPressed(checked);
}
