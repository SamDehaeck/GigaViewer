#include "videoglscene.h"

#include <QtGui>
#include <QtOpenGL>

VideoGlScene::VideoGlScene(QObject *parent) :
    QGraphicsScene(parent)
{
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

    QColor m_backgroundColor("lightGray");

    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //place image drawing code here





    painter->endNativePainting();

//    QTimer::singleShot(20, this, SLOT(update()));

}

