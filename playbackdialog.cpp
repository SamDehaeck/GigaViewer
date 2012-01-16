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

void PlaybackDialog::on_ffwdButton_clicked()
{
    int delayVal=(ui->fpsEdit->text().toInt())/2;
    QString delayTxt=QString("%1").arg(delayVal);
    ui->fpsEdit->setText(delayTxt);
    emit newFps(delayVal);
}

void PlaybackDialog::on_rwdButton_clicked()
{
    int delayVal=2*(ui->fpsEdit->text().toInt());
    QString delayTxt=QString("%1").arg(delayVal);
    ui->fpsEdit->setText(delayTxt);
    emit newFps(delayVal);
}
