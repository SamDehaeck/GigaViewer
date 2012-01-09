#ifndef PICBACKEND_H
#define PICBACKEND_H

#include <QObject>
#include "opencv2/opencv.hpp"

class PicBackend : public QObject
{
    Q_OBJECT
public:
    explicit PicBackend(QObject *parent = 0);

signals:
    void NewImageReady(cv::Mat im);

public slots:
    void LoadNewImage(QString theName);

private:
    cv::Mat Image;

};

#endif // PICBACKEND_H
