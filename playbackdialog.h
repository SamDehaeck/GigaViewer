#ifndef PLAYBACKDIALOG_H
#define PLAYBACKDIALOG_H

#include <QDialog>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

namespace Ui {
    class PlaybackDialog;
}

class PlaybackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaybackDialog(QWidget *parent = 0);
    ~PlaybackDialog();

signals:
    void stopPlayback();
    void newFps(int fps);
    void jumpFrames(bool forward);
    void recordNow(bool checked,QString recFold, QString codec);

private slots:
    void on_stopButton_clicked();
    void on_ffwdButton_clicked();
    void on_rwdButton_clicked();
    void on_playButton_toggled(bool checked);
    void on_recButton_toggled(bool checked);
    void on_RecSettings_clicked();
    void on_backButton_clicked();
    void on_toolButton_clicked();
    void togglePlay();
    void reversePlay();
    void forwardPlay();
    void on_fpsEdit_returnPressed();
    void on_horizontalSlider_valueChanged(int value);
    void on_recTimedButton_toggled(bool checked);
    void finishedFirstTimer();
    void finishedSecondTimer();

public slots:
    void newFrameNumberReceived(int nr);
    void showNewFps(int msec);

private:
    bool parseInstruct(QString instruct,int& sec,int& msecdelay);


    Ui::PlaybackDialog *ui;
    int currentTimer;
    bool recording;
    bool have2timers;
    int secondDelay;
    QTimer timer1,timer2;  // typical use case only requires two timers
    QString config1,config2; // to go with the two timers
};

#endif // PLAYBACKDIALOG_H
