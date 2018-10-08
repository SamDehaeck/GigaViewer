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
    int maxX;
    int maxY;

signals:
    void LclickOnImage(QPointF p);
    void RclickOnImage(QPointF p);

public slots:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)override;

private:


};

#endif // VIDEOGLSCENE_H
