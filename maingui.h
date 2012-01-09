#ifndef MAINGUI_H
#define MAINGUI_H

#include <QGraphicsView>
#include <QtGui>
#include <QGLWidget>
#include "imagepacket.h"
#include "videoglscene.h"



class MainGui : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainGui(QWidget *parent = 0);

signals:
    void newPicNeeded(QString theString);
    void newOpencvFeedNeeded(bool start);

public slots:
    void newImageReceived(ImagePacket theMatrix);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    VideoGlScene* theScene;

};

#endif // MAINGUI_H
