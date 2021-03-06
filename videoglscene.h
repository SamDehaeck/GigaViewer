#ifndef VIDEOGLSCENE_H
#define VIDEOGLSCENE_H

#include <QGraphicsScene>
//#include <qopenglfunctions_3_3_compatibility.h>
#include "opencv2/opencv.hpp"
#include "fileinputdialog.h"

class VideoGlScene : public QGraphicsScene//, protected QOpenGLFunctions_3_3_Compatibility
{
    Q_OBJECT
public:
    VideoGlScene(QList<QDialog*> controlDialogs,QObject *parent = 0);
    void drawBackground(QPainter *painter, const QRectF &);

    cv::Mat imageBuff;

signals:

public slots:

private:
    int didInitOpengl;


};

#endif // VIDEOGLSCENE_H
