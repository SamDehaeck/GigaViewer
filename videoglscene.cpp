#include "videoglscene.h"

#include <QtGui>
#include <QGraphicsProxyWidget>
#include <QOpenGLFunctions_3_0>

VideoGlScene::VideoGlScene(QList<QDialog*> controlDialogs, QObject *parent) :
    QGraphicsScene(parent),didInitOpengl(0)
{
    QDialog* dial;
    int ini=0;
    QPointF pos(10, 10);
    foreach (dial, controlDialogs) {
        //now make the control dialogues
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(nullptr, Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
        proxy->setWidget(dial);
        addItem(proxy);

        switch (ini) {
            case 0:
                proxy->setData(0,"INPUT");
                break;
            case 1:
                proxy->setData(0,"PLAYBACK");
                break;
            case 2:
                proxy->setData(0,"CAMERA");
                break;
            case 3:
                proxy->setData(0,"PLUGIN");
                break;
        }
        proxy->setFlag(QGraphicsItem::ItemIsMovable);
        proxy->setCacheMode(QGraphicsItem::DeviceCoordinateCache);


        if (ini!=0) proxy->setVisible(false);

        const QRectF rect = proxy->boundingRect();
        proxy->setPos(pos.x() - rect.x(), pos.y() - rect.y());

        // now will add the offset for placing the next dialog as we have the size now
        if (ini==0) pos += QPointF(650,-350);  //This is the base Y offset for the extra panels
        pos += QPointF(0,10 + rect.height());

        ini=ini+1;
    }


}

void VideoGlScene::drawBackground(QPainter *painter, const QRectF &)
{
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL
            && painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        return;
    }

/*    if (didInitOpengl==0) {
        initializeOpenGLFunctions();
        didInitOpengl=1;
    }*/
    QOpenGLFunctions_3_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();

//    QOpenGLFunctions_3_3_Compatibility *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Compatibility>();
    f->initializeOpenGLFunctions();
    painter->beginNativePainting();



    //place image drawing code here
    int depth = imageBuff.depth();
    int cn = imageBuff.channels();
    GLenum format = GL_LUMINANCE;
    if (cn==3) {
#ifdef Q_OS_WIN32
//        format = GL_RGB; // this setting was used for correct webcam viewing but is not working for Vimba colour cameras
        format = GL_BGR;
#else
        format = GL_BGR;
#endif

    } else if (cn==4) {
#ifdef Q_OS_WIN32
        format = GL_RGBA;
#else
        format = GL_BGRA;
#endif
    }
    GLenum gldepth = GL_UNSIGNED_BYTE;
    if (depth==CV_16U) gldepth=GL_UNSIGNED_SHORT;

    f->glEnable(GL_TEXTURE_2D);
    f->glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    GLuint mytex;
    f->glGenTextures(1,&mytex);
    f->glBindTexture(GL_TEXTURE_2D, mytex);
    f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    if (imageBuff.isContinuous()) glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    f->glTexImage2D(GL_TEXTURE_2D,0,cn,imageBuff.cols,imageBuff.rows,0,format,gldepth,imageBuff.data);

    //calculate projected size in order to keep aspect ratio intact
    int maxX=1024;
    int maxY=768;
    if (imageBuff.rows!=0) {
        double aspRatio=static_cast<double>(imageBuff.rows)/imageBuff.cols;
        double windowAspRatio=static_cast<double>(this->height())/this->width();
        if (aspRatio>windowAspRatio) {
            // amount of rows is limiting factor
            maxY=static_cast<int>(round(this->height()));
            maxX=static_cast<int>(round(maxY/aspRatio));
        } else {
            maxX=static_cast<int>(round(this->width()));
            maxY=static_cast<int>(round(maxX*aspRatio));
        }
    }

    f->glBegin(GL_QUADS);

    f->glTexCoord2f(0.0,0.0); f->glVertex2f(0,0);
    f->glTexCoord2f(1.0,0.0); f->glVertex2f(maxX,0.0);
    f->glTexCoord2f(1.0,1.0); f->glVertex2f(maxX,maxY);
    f->glTexCoord2f(0.0,1.0); f->glVertex2f(0.0,maxY);

    f->glEnd();
    f->glDisable(GL_TEXTURE_2D);
    f->glDeleteTextures(1,&mytex);


    painter->endNativePainting();

}




