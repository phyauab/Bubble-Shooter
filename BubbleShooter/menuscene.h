#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "scene.h"

class MenuScene : public Scene
{
public:
    MenuScene(QGraphicsView* graphicsView);
};

#endif // MENUSCENE_H
