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
