#include "playbackdialog.h"
#include "ui_playbackdialog.h"
#include <QDebug>

PlaybackDialog::PlaybackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaybackDialog),recording(FALSE)
{
    ui->setupUi(this);
    currentTimer=100;
    ui->RecFolder->setText(QDir::homePath());

}

PlaybackDialog::~PlaybackDialog()
{
    delete ui;
}

void PlaybackDialog::on_stopButton_clicked()
{
    ui->playButton->setChecked(TRUE);
    ui->recButton->setChecked(FALSE);
    ui->fpsEdit->setText("100");
    recording=FALSE;
    emit stopPlayback();
}

void PlaybackDialog::on_ffwdButton_clicked()
{
    emit jumpFrames(TRUE);
}

void PlaybackDialog::on_rwdButton_clicked()
{
    emit jumpFrames(FALSE);
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
    recording=checked;
    QString recf=ui->RecFolder->text();
    QString cod=ui->codecBox->currentText();
    emit recordNow(checked,recf,cod);
    if (recording) {
        ui->LeftStatus->setText("Recording");
    } else {
        ui->LeftStatus->setText("");
    }
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
    QString old = ui->RecFolder->text();
//    if (old=="") {
        QString fold= QFileDialog::getExistingDirectory(this,tr("Select recording folder"),QDir::homePath(),QFileDialog::ShowDirsOnly);
//    } else {
//        QString fold= QFileDialog::getExistingDirectory(this,tr("Select recording folder"),QDir::homePath(),QFileDialog::ShowDirsOnly);
//    }
    ui->RecFolder->setText(fold);
}

void PlaybackDialog::togglePlay()
{
    ui->playButton->toggle();
}

void PlaybackDialog::reversePlay()
{
    currentTimer=-abs(currentTimer);
    QString delayTxt=QString("%1").arg(currentTimer);
    ui->fpsEdit->setText(delayTxt);
    ui->playButton->setChecked(TRUE);
    emit newFps(currentTimer);
}

void PlaybackDialog::forwardPlay()
{
    currentTimer=abs(currentTimer);
    QString delayTxt=QString("%1").arg(currentTimer);
    ui->fpsEdit->setText(delayTxt);
    ui->playButton->setChecked(TRUE);
    emit newFps(currentTimer);
}

void PlaybackDialog::on_fpsEdit_returnPressed()
{
    QString valStr=ui->fpsEdit->text();
    if (valStr.contains("/")) {
        QStringList vl=valStr.split("/");
        if (vl.count()==2) {
            int newVal=1000/vl[1].toInt();
            currentTimer=newVal;
        } else {
            qDebug()<<"Could not understand fps setting";
        }
    } else {
        currentTimer=valStr.toInt();
    }

    emit newFps(currentTimer);
}
