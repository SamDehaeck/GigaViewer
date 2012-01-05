#ifndef VIDEOGLSCENE_H
#define VIDEOGLSCENE_H

#include <QGraphicsScene>
#include "opencv2/opencv.hpp"

class VideoGlScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit VideoGlScene(QObject *parent = 0);
    void drawBackground(QPainter *painter, const QRectF &);

signals:

public slots:
    void loadImage(QString imName);

private:
    cv::VideoCapture cam;
    cv::Mat imageBuff;

};

#endif // VIDEOGLSCENE_H
