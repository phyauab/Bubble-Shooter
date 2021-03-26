#include "game.h"
#include "menuscene.h"
#include "boardscene.h"
#include <QDebug>

Game::Game(QGraphicsView * graphicsView)
{
    this->graphicsView = graphicsView;

    this->init();
    qDebug() << "width: " << graphicsView->width() << ", height: " << graphicsView->height();


}

Game::~Game()
{
    if (scene != nullptr) {
        delete scene;
    }
    delete graphicsView;
}

void Game::init() {
    scene = new BoardScene(graphicsView);
}

QGraphicsScene* Game::getScene() {
    return scene;
}
