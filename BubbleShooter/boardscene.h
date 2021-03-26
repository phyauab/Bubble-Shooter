#ifndef BOARDSCENE_H
#define BOARDSCENE_H


#include <QGraphicsScene>
#include <QGraphicsView>
#include "scene.h"
#include "bubble.h"
#include <QQueue>

class BoardScene : public Scene
{
    Q_OBJECT
public:
    BoardScene(QGraphicsView* graphicsView);
private:
    Bubble*** bubbles;
    void initBoard();
    void initShooter();
    Bubble** bubbleQueue;
    //Bubble* eliminatedBubbles;
    QMap<int, QVector<Bubble*>> vanishingBubbles;
    QVector<Bubble*> fallingBubbles;
    int vanishingDepth = 0;
    QTimer *timer;
    void fallBubbles();
protected slots:
    void updateBubbles();
};

#endif // BOARDSCENE_H
