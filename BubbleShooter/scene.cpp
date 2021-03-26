#include "scene.h"
#include <QImage>

Scene::Scene(QGraphicsView * graphicsView)
{
    this->graphicsView = graphicsView;
    // graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    graphicsView->setScene(this);
    // graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    //graphicsView->fitInView(this->sceneRect(), Qt::KeepAspectRatio);
    // graphicsView->fitInView(sceneRect(), Qt::KeepAspectRatio);
    //setSceneRect(0, 0, 768, 1024);
    QBrush bg;
    bg.setColor(QColor(29, 32, 33));
    bg.setStyle(Qt::SolidPattern);
    setBackgroundBrush(bg);
}
