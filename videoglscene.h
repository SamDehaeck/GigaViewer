#ifndef VIDEOGLSCENE_H
#define VIDEOGLSCENE_H

#include <QGraphicsScene>
#include "opencv2/opencv.hpp"
#include "fileinputdialog.h"

class VideoGlScene : public QGraphicsScene
{
    Q_OBJECT
public:
    VideoGlScene(QList<QDialog*> controlDialogs,QObject *parent = 0);
    void drawBackground(QPainter *painter, const QRectF &);

    cv::Mat imageBuff;

signals:

public slots:

private:


};

#endif // VIDEOGLSCENE_H
