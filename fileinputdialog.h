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

private:
    Ui::FileInputDialog *ui;
};

#endif // FILEINPUTDIALOG_H
