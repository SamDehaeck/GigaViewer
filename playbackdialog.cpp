#include "playbackdialog.h"
#include "ui_playbackdialog.h"
#include <QDebug>

PlaybackDialog::PlaybackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaybackDialog)
{
    ui->setupUi(this);
    ui->fpsEdit->setValidator(new QIntValidator(0,3600000,this));
}

PlaybackDialog::~PlaybackDialog()
{
    delete ui;
}

void PlaybackDialog::on_stopButton_clicked()
{
    emit stopPlayback();
}

void PlaybackDialog::on_fpsEdit_editingFinished()
{
    emit newFps(ui->fpsEdit->text().toInt());
}
