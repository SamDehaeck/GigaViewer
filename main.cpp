#include <QtGui>
#include <QGLWidget>

#include "videoglscene.h"

#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>


class GraphicsView : public QGraphicsView
{
public:
    GraphicsView()
    {
        setWindowTitle(tr("Gige Viewer"));
    }

protected:
    void resizeEvent(QResizeEvent *event) {
        if (scene())
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        QGraphicsView::resizeEvent(event);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GraphicsView view;
//    view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view.setViewport(new QGLWidget);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setScene(new VideoGlScene);
    view.show();

    view.resize(1024, 768);



    return a.exec();
}
