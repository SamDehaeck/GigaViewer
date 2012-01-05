#include "videoglscene.h"
#include <opencv2/highgui/highgui.hpp>

#include <QtGui>
#include <QtOpenGL>

QDialog * VideoGlScene::createDialog(const QString &windowTitle) const
{
    QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    dialog->setWindowOpacity(0.5);
    dialog->setWindowTitle(windowTitle);
    dialog->setLayout(new QVBoxLayout);

    return dialog;
}

VideoGlScene::VideoGlScene(QObject *parent) :
    QGraphicsScene(parent)
{
    imageBuff=cv::imread("/home/sam/ULB/Fotos/Interf+bel.bmp",0);
//    imageBuff=cv::imread("/home/sam/GridCentralZone.png"); // color images seem to work to

    //now make the control dialogues
    QWidget *fileInput = createDialog(tr("File input"));

    QWidget * m_fileButton = new QPushButton(tr("Load model"));
//    connect(m_modelButton, SIGNAL(clicked()), this, SLOT(loadModel()));
    fileInput->layout()->addWidget(m_fileButton);

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
    proxy->setWidget(fileInput);
    addItem(proxy);

    QPointF pos(10, 10);
    foreach (QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        const QRectF rect = item->boundingRect();
        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
        pos += QPointF(0, 10 + rect.height());
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
        format = GL_BGR;
    } else if (cn==4) {
        format = GL_BGRA;
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

    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0); glVertex2f(0,0);
    glTexCoord2f(1.0,0.0); glVertex2f(imageBuff.cols,0.0);
    glTexCoord2f(1.0,1.0); glVertex2f(imageBuff.cols,imageBuff.rows);
    glTexCoord2f(0.0,1.0); glVertex2f(0.0,imageBuff.rows);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1,&mytex);


    painter->endNativePainting();

    //    QTimer::singleShot(20, this, SLOT(update()));

}



