#ifndef MAINGUI_H
#define MAINGUI_H

#include <QGraphicsView>
#include <QtGui>
#include <QGLWidget>
#include "opencv2/opencv.hpp"
#include "videoglscene.h"



class MainGui : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainGui(QWidget *parent = 0);

signals:
    void newPicNeeded(QString theString);

public slots:
    void newPicReceived(cv::Mat theMatrix);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    VideoGlScene* theScene;

};

#endif // MAINGUI_H
