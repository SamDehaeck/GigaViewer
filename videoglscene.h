#ifndef VIDEOGLSCENE_H
#define VIDEOGLSCENE_H

#include <QGraphicsScene>
#include "opencv2/core/core.hpp"

class VideoGlScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit VideoGlScene(QObject *parent = 0);
    void drawBackground(QPainter *painter, const QRectF &);

    cv::Mat imageBuff;

signals:

public slots:
    void loadImage(QString imName);

private:

};

#endif // VIDEOGLSCENE_H
