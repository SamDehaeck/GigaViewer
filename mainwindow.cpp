#include "mainwindow.h"
#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"Inside main close";
    event->accept();
}
