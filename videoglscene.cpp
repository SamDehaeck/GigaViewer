#include "videoglscene.h"

#include <QtGui>
#include <QtOpenGL>

VideoGlScene::VideoGlScene(QList<QDialog*> controlDialogs, QObject *parent) :
    QGraphicsScene(parent)
{
    QDialog* dial;

    foreach (dial, controlDialogs) {
        //now make the control dialogues
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
        proxy->setWidget(dial);
        addItem(proxy);
    }

    QPointF pos(10, 10);
    int i(0);

    foreach (QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        switch (i) {
            case 0:
                item->setData(0,"INPUT");
                break;
            case 1:
                item->setData(0,"PLAYBACK");
                break;
            case 2:
                item->setData(0,"CAMERA");
                break;
        }


        if (i!=0) item->setVisible(FALSE);
        const QRectF rect = item->boundingRect();
        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
        pos += QPointF(10 + rect.width(),0);
        i++;
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

    painter->beginNativePainting();

    //place image drawing code here
    int depth = imageBuff.depth();
    int cn = imageBuff.channels();
    GLenum format = GL_LUMINANCE;
    if (cn==3) {
#ifdef Q_OS_WIN32
        format = GL_RGB;
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

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    GLuint mytex;
    glGenTextures(1,&mytex);
    glBindTexture(GL_TEXTURE_2D, mytex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    if (imageBuff.isContinuous()) glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D,0,cn,imageBuff.cols,imageBuff.rows,0,format,gldepth,imageBuff.data);

    //calculate projected size in order to keep aspect ratio intact
    int maxX=1024;
    int maxY=768;
    if (imageBuff.rows!=0) {
        double aspRatio=imageBuff.rows/(double)imageBuff.cols;
        double windowAspRatio=this->height()/(double)this->width();
        if (aspRatio>windowAspRatio) {
            // amount of rows is limiting factor
            maxY=this->height();
            maxX=maxY/aspRatio;
        } else {
            maxX=this->width();
            maxY=maxX*aspRatio;
        }
    }

    glBegin(GL_QUADS);

    glTexCoord2f(0.0,0.0); glVertex2f(0,0);
    glTexCoord2f(1.0,0.0); glVertex2f(maxX,0.0);
    glTexCoord2f(1.0,1.0); glVertex2f(maxX,maxY);
    glTexCoord2f(0.0,1.0); glVertex2f(0.0,maxY);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1,&mytex);


    painter->endNativePainting();

}




