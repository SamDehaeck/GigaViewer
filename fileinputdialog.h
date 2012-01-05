#ifndef FILEINPUTDIALOG_H
#define FILEINPUTDIALOG_H

#include <QDialog>

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
    void buttonPressed(QString namegiven);

private slots:
    void on_filePushButton_clicked();

private:
    Ui::FileInputDialog *ui;
};

#endif // FILEINPUTDIALOG_H
