#ifndef FILEINPUTDIALOG_H
#define FILEINPUTDIALOG_H

#include <QDialog>
#include <QtGui>

namespace Ui {
    class FileInputDialog;
}

class FileInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileInputDialog(QWidget *parent = 0);
    ~FileInputDialog();

signals:
    void StaticPicPressed(QString namegiven);
    void OpencvFeedPressed();
    void MoviePressed(QString namegiven);
    void CloseApp();

private slots:
    void on_filePushButton_clicked();
    void on_camButton_clicked();

    void on_MovieButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FileInputDialog *ui;
};

#endif // FILEINPUTDIALOG_H
