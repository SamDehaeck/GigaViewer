#include "playbackdialog.h"
#include "ui_playbackdialog.h"
#include <QDebug>

PlaybackDialog::PlaybackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaybackDialog)
{
    ui->setupUi(this);
    ui->fpsEdit->setValidator(new QIntValidator(-3600000,3600000,this));
    currentTimer=100;
    ui->RecFolder->setText(QDir::homePath());
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
    QString recf=ui->RecFolder->text();
    QString cod=ui->codecBox->currentText();
    emit recordNow(checked,recf,cod);
}

void PlaybackDialog::on_RecSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void PlaybackDialog::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void PlaybackDialog::on_toolButton_clicked()
{
    QString fold= QFileDialog::getExistingDirectory(this,tr("Select recording folder"),QDir::homePath(),QFileDialog::ShowDirsOnly);
    ui->RecFolder->setText(fold);
}
