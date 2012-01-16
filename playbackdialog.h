#ifndef PLAYBACKDIALOG_H
#define PLAYBACKDIALOG_H

#include <QDialog>

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

private slots:
    void on_stopButton_clicked();

    void on_fpsEdit_editingFinished();

    void on_ffwdButton_clicked();

private:
    Ui::PlaybackDialog *ui;
};

#endif // PLAYBACKDIALOG_H
