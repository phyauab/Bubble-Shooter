#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include <QGraphicsView>

class Game
{
public:
    Game(QGraphicsView * graphicsView);
    ~Game();
    QGraphicsScene * getScene();
private:
    QGraphicsView * graphicsView = nullptr;
    QGraphicsScene * scene = nullptr;
    void init();
};

#endif // GAME_H
