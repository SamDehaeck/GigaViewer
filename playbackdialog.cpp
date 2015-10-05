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
    connect(&timer1,SIGNAL(timeout()), this, SLOT (finishedFirstTimer()));
    connect(&timer2,SIGNAL(timeout()), this, SLOT (finishedSecondTimer()));
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
        emit newFps(3600000); // such a long time that it appears paused
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

void PlaybackDialog::showNewFps(int msec) {
    int fps=(int)(1000.0/msec);
    QString fpstext="1/"+QString::number(fps);
    ui->fpsEdit->setText(fpstext);
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


// should ask user how long he wants to record
void PlaybackDialog::on_recTimedButton_toggled(bool checked)
{
    recording=checked;
    QString instruct;
    bool ok=true;
    if (recording) {
        instruct=QInputDialog::getText(NULL,"Shutdown timer instructions","Format ~ '2m10s@30fps/3h@0.5fps' : ");
        // check if format is ok
        QRegExp rx("(.+)/(.+)");
        int pos=0;
        pos=rx.indexIn(instruct);
        if (pos==-1) {
            config1=instruct;
            config2="";
        } else {
            config1=rx.cap(1);
            config2=rx.cap(2);
        }

        have2timers=false;
        int tim1,msecs1;
        if (parseInstruct(config1,tim1,msecs1)) {
//            qDebug()<<"Got here with: "<<tim1<<" - "<<msecs1;
            timer1.setInterval(1000*tim1);
            timer1.setSingleShot(true);
            if (msecs1>1) {
                ui->fpsEdit->setText(QString::number(msecs1));
                emit newFps(msecs1);
            }
        } else {
            ok=false;
        }

        int tim2,msecs2;
        if (parseInstruct(config2,tim2,msecs2)) {
//            qDebug()<<"Got here also with: "<<tim2<<" - "<<msecs2;
            if ((msecs2!=msecs1)&&msecs2>1) {
                have2timers=true;
                timer2.setInterval(1000*(tim1+tim2));
                timer2.setSingleShot(true);
                secondDelay=msecs2;
            } else {
                qDebug()<<"Not a correct use of second timer fps: "<<msecs2<<" vs "<<msecs1;
            }

        }

        //time=instruct.toInt(&ok);

        if (!ok) { //exit gracefully
            ui->recTimedButton->toggle();
            return;
        }

    }

    QString recf=ui->RecFolder->text();
    QString cod=ui->codecBox->currentText();
    emit recordNow(checked,recf,cod);
    if (recording) {
        ui->LeftStatus->setText("Recording");
        timer1.start();
        if (have2timers) timer2.start();
    } else {
        ui->LeftStatus->setText("");
    }
}

bool PlaybackDialog::parseInstruct(QString instruct, int& sec, int& msecdelay) {
    QRegExp secSearch("(\\d+)s");
    QRegExp minSearch("(\\d+)m");
    QRegExp hourSearch("(\\d+)h");
    QRegExp adsearch("(.+)@(.+)");
    QRegExp fpsSearch("(.+)fps");
    bool ok=false;
    sec=0;
    msecdelay=-1;
    int pos;
    pos=secSearch.indexIn(instruct);
    if (pos!=-1) {
        int newsec=secSearch.cap(1).toInt(&ok);
        if (ok) sec+=newsec;
    }
    pos=minSearch.indexIn(instruct);
    if (pos!=-1) {
        int newmin=minSearch.cap(1).toInt(&ok);
        if (ok) sec+=(newmin*60);
    }
    pos=hourSearch.indexIn(instruct);
    if (pos!=-1) {
        int newhour=hourSearch.cap(1).toInt(&ok);
        if (ok) sec+=(newhour*3600);
    }

    pos=adsearch.indexIn(instruct);
    if (pos!=-1) {
        QString fpsstring=adsearch.cap(2);
        pos=fpsSearch.indexIn(fpsstring);
        if (pos!=-1) {
            double fp=fpsSearch.cap(1).toDouble(&ok);
            if (ok) {
                msecdelay=1000.0/fp;
            }
        }
    }

    return (sec>0);
}

void PlaybackDialog::finishedFirstTimer() {
    if (recording) {
        if (have2timers) {
            // don't stop recording, just change fps
            ui->fpsEdit->setText(QString::number(secondDelay));
            emit newFps(secondDelay);
        } else {
            ui->recTimedButton->toggle();
            ui->stopButton->click();
        }
    }
}

void PlaybackDialog::finishedSecondTimer() {
    if (recording) {
        ui->recTimedButton->toggle();
        ui->stopButton->click();
    }
}
