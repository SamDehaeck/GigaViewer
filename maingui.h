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
    void newMovieNeeded(QString theString);
    void newOpencvFeedNeeded(bool start);
    void implementNewFps(int fps);

public slots:
    void newImageReceived(ImagePacket theMatrix);
    void openCvFeedPressed();
    void newMoviePressed(QString theString);
    void stopButtonPressed();
    void gotNewFps(int fps);

protected:
    void resizeEvent(QResizeEvent *event);
    void showPlaybackControls(bool visible);
    void showInputControls(bool visible);

private:
    VideoGlScene* theScene;

};

#endif // MAINGUI_H
