#include "mainwindow.h"
#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    event->accept();
}
