#include "playbackdialog.h"
#include "ui_playbackdialog.h"
#include <QDebug>

PlaybackDialog::PlaybackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaybackDialog),recording(false)
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
    ui->playButton->setChecked(true);
    ui->recButton->setChecked(false);
    ui->fpsEdit->setText("100");
    recording=false;
    emit stopPlayback();
}

void PlaybackDialog::on_ffwdButton_clicked()
{
    emit jumpFrames(true);
}

void PlaybackDialog::on_rwdButton_clicked()
{
    emit jumpFrames(false);
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
    ui->playButton->setChecked(true);
    emit newFps(currentTimer);
}

void PlaybackDialog::forwardPlay()
{
    currentTimer=abs(currentTimer);
    QString delayTxt=QString("%1").arg(currentTimer);
    ui->fpsEdit->setText(delayTxt);
    ui->playButton->setChecked(true);
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
        if (valStr.at(0)=='-') currentTimer=-currentTimer;
    } else {
        currentTimer=valStr.toInt();
    }

    emit newFps(currentTimer);
}

void PlaybackDialog::newFrameNumberReceived(int nr)
{
    QString frameTxt=QString("%1").arg(nr);
    ui->RightStatus->setText(frameTxt);
}

void PlaybackDialog::on_horizontalSlider_valueChanged(int value)
{
    double newFrameRate=1;
    if (value>10) {
        newFrameRate=value-9;
    } else {
        newFrameRate=value/10.0;
    }
//    qDebug()<<"Converted new value "<<value<<" into new fps "<<newFrameRate;
    QString oldText=ui->fpsEdit->text();
    QString delayTxt;
    int newVal;
    if (oldText.at(0)=='-') {
        delayTxt=QString("-1/%1").arg(newFrameRate);
        newVal=(int)(-1000/newFrameRate);
    } else {
        delayTxt=QString("1/%1").arg(newFrameRate);
        newVal=(int)(1000/newFrameRate);
    }
    ui->fpsEdit->setText(delayTxt);

    currentTimer=newVal;
    emit newFps(currentTimer);
    ui->playButton->setChecked(true);
}
