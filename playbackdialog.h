#ifndef PLAYBACKDIALOG_H
#define PLAYBACKDIALOG_H

#include <QDialog>
#include <QtGui>

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

public slots:
    void newFrameNumberReceived(int nr);

private:
    Ui::PlaybackDialog *ui;
    int currentTimer;
    bool recording;
};

#endif // PLAYBACKDIALOG_H
