#include "playbackdialog.h"
#include "ui_playbackdialog.h"
#include <QDebug>

PlaybackDialog::PlaybackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaybackDialog)
{
    ui->setupUi(this);
    ui->fpsEdit->setValidator(new QIntValidator(0,3600000,this));
    currentTimer=20;
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
    currentTimer=ui->fpsEdit->text().toInt();
    emit newFps(currentTimer);
}

void PlaybackDialog::on_ffwdButton_clicked()
{
    currentTimer=currentTimer/2;
    QString delayTxt=QString("%1").arg(currentTimer);
    ui->fpsEdit->setText(delayTxt);
    emit newFps(currentTimer);
}

void PlaybackDialog::on_rwdButton_clicked()
{
    currentTimer=2*currentTimer;
    QString delayTxt=QString("%1").arg(currentTimer);
    ui->fpsEdit->setText(delayTxt);
    emit newFps(currentTimer);
}

void PlaybackDialog::on_playButton_toggled(bool checked)
{
    if (checked) {
         emit newFps(currentTimer);
    } else {
        emit newFps(3600000);
    }
}

void PlaybackDialog::on_recButton_toggled(bool checked)
{
    emit recordNow(checked);
}

void PlaybackDialog::on_RecSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void PlaybackDialog::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
