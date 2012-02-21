#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void windowClosed();

public slots:
    void toggleFullscreen();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
