#include "playbackdialog.h"
#include "ui_playbackdialog.h"

PlaybackDialog::PlaybackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaybackDialog)
{
    ui->setupUi(this);
}

PlaybackDialog::~PlaybackDialog()
{
    delete ui;
}

void PlaybackDialog::on_stopButton_clicked()
{
    emit stopPlayback();
}
