#include "boardscene.h"
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>
#include <QVector>
BoardScene::BoardScene(QGraphicsView* graphicsView):Scene(graphicsView) {
    initBoard();
    initShooter();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateBubbles()));
    timer->start(30);
}

void BoardScene::initBoard() {
    this->bubbles = new Bubble **[8];
    for (int row = 0; row < 8; row++) {
        bubbles[row] = new Bubble *[9];
        for (int col = 0; col < 9; col++) {
            Color randomColor = static_cast<Color>(QRandomGenerator::global()->generate() % 2);
            bubbles[row][col] = new Bubble(row, col, Kind::NORMAL, randomColor);
        }
    }
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 9; col++) {
            bool isEven = row % 2 == 0;

            if (row != 0 && (!isEven || col != 0)) {
                // qDebug() << "TOP LEFT of " << row << "," << col << " is set to be " << row - 1 << "," << col- isEven;
                bubbles[row][col]->setNeighbour(TOP_LEFT, bubbles[row - 1][col - isEven]);
            }
            if (row != 0 && (isEven || col != 8)) {
                // qDebug() << "TOP RIGHT of " << row << "," << col << " is set to be " << row - 1 << "," << col + !isEven;
                bubbles[row][col]->setNeighbour(TOP_RIGHT, bubbles[row - 1][col + !isEven]);
            }
            if (col != 0) {
                // qDebug() << "LEFT of " << row << "," << col << " is set to be " << row << "," << col - 1;
                bubbles[row][col]->setNeighbour(LEFT, bubbles[row][col - 1]);
            }
            if (col != 8) {
                // qDebug() << "RIGHT of " << row << "," << col << " is set to be " << row << "," << col + 1;
                bubbles[row][col]->setNeighbour(RIGHT, bubbles[row][col + 1]);
            }
            if (row != 7 && (!isEven || col != 0)) {
                // qDebug() << "BOTTOM LEFT of " << row << "," << col << " is set to be " << row + 1 << "," << col - isEven;
                bubbles[row][col]->setNeighbour(BOTTOM_LEFT, bubbles[row + 1][col - isEven]);
            }
            if (row != 7 && (isEven || col != 8)) {
                // qDebug() << "BOTTOM RIGHT of " << row << "," << col << " is set to be " << row + 1 << "," << col + !isEven;
                bubbles[row][col]->setNeighbour(BOTTOM_RIGHT, bubbles[row + 1][col + !isEven]);
            }
            int offsetX = row % 2 == 1 ? Bubble::width * 0.25 : Bubble::width * -0.25;
            bubbles[row][col]->setPos(col * Bubble::width + col * 5 + offsetX, row * Bubble::height);
            addItem(bubbles[row][col]);
        }
    }
    update();
    // bubbles[3][3]->eliminate();
    vanishingDepth = 0;
    bubbles[3][4]->eliminate(vanishingBubbles, fallingBubbles);
    //fallBubbles();
    //

    //eliminatedBubbles = bubbles[3][3];
    // vanishingBubbles.push_back(bubbles[3][3]);
    //Bubble * start = bubbles[4][4];

}

void BoardScene::initShooter() {
    int size = 5;
    Bubble** bubbles = new Bubble*[size];
    for (int i = 0; i < size; i++) {
        Color randomColor = static_cast<Color>(QRandomGenerator::global()->generate() % 6);
        bubbles[i] = new Bubble(Kind::NORMAL, randomColor);
    }
    for (int i = 0; i < size; i++) {
        if (i + 1 < size)
            bubbles[i]->setNeighbour(RIGHT, bubbles[i + 1]);
        if (i > 0)
            bubbles[i]->setNeighbour(LEFT, bubbles[i - 1]);
        bubbles[i]->setPos(i * Bubble::width + i * 5, 768);
        addItem(bubbles[i]);
    }
}

void BoardScene::fallBubbles() {
    bool visited[8][9] = { {false} };
    for (int col = 0; col < 9; col++) {
        Bubble * topBubble = bubbles[0][col];
        if (topBubble == nullptr)
            continue;
        qDebug() << "col "<< col;
        //topBubble->testConnectivity(visited);
    }
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 9; j++)
            qDebug() << i << "," << j << visited[i][j];
}

void BoardScene::updateBubbles()
{
    if (!vanishingBubbles[vanishingDepth].empty()) {
        const QVector<Bubble*> &bubbles = vanishingBubbles[vanishingDepth];
        bool nextLevel = false;
        for (auto& b : bubbles) {
            b->setScale(b->scale() * 0.85);
            if (b->scale() < 0.4) {
                b->setScale(b->scale() * 0.35);
            }
            if (b->scale() < 0.2) {
                removeItem(b);
                delete b;
                nextLevel = true;
            }
        }
        if (nextLevel)
            vanishingDepth++;
    } else {
        vanishingBubbles.clear();
        vanishingDepth = 0;

        if (fallingBubbles.size() > 0) {
            //qDebug() << "size greater than 0";
            bool clear = false;
            for (auto &f : fallingBubbles) {
                f->setScale(f->scale() * 0.85);
                if (f->scale() < 0.4) {
                    f->setScale(f->scale() * 0.35);
                }
                if (f->scale() < 0.2) {
                    removeItem(f);
                    delete f;
                    clear = true;
                }
            }
            if (clear) fallingBubbles.clear();
        }
    }
/*
    QQueue<Bubble*>::iterator i;
    for (i = vanishingBubbles.begin(); i != vanishingBubbles.end(); ++i) {
        Bubble * b = *i;
        b->setScale(b->scale() * 0.85);
        if (b->scale() < 0.4) {
            b->setScale(b->scale() * 0.35);
        }
        if (b->scale() < 0.2) {
            Bubble * tl = b->getNeighbour(TOP_LEFT);
            Bubble * tr = b->getNeighbour(TOP_RIGHT);
            Bubble * l = b->getNeighbour(LEFT);
            Bubble * r = b->getNeighbour(RIGHT);
            Bubble * bl = b->getNeighbour(BOTTOM_LEFT);
            Bubble * br = b->getNeighbour(BOTTOM_RIGHT);
            if (tl != nullptr && !vanishingBubbles.contains(tl))
                vanishingBubbles.push_back(tl);
            if (tr != nullptr && !vanishingBubbles.contains(tr))
                vanishingBubbles.push_back(tr);
            if (l != nullptr && !vanishingBubbles.contains(l))
                vanishingBubbles.push_back(l);
            if (r != nullptr && !vanishingBubbles.contains(r))
                vanishingBubbles.push_back(r);
            if (bl != nullptr && !vanishingBubbles.contains(bl))
                vanishingBubbles.push_back(bl);
            if (br != nullptr && !vanishingBubbles.contains(br))
                vanishingBubbles.push_back(br);
            // Remove
            //nextIterator.remove();
            //vanishingBubbles.
            removeItem(b);
            delete b;
        }
    }*/
    /*if (!vanishingBubbles.isEmpty()) {
        Bubble * b = vanishingBubbles.head();
        b->setScale(b->scale() * 0.85);
        if (b->scale() < 0.4) {
            b->setScale(b->scale() * 0.35);
        }
        if (b->scale() < 0.2) {
            Bubble * tl = b->getNeighbour(TOP_LEFT);
            Bubble * tr = b->getNeighbour(TOP_RIGHT);
            Bubble * l = b->getNeighbour(LEFT);
            Bubble * r = b->getNeighbour(RIGHT);
            Bubble * bl = b->getNeighbour(BOTTOM_LEFT);
            Bubble * br = b->getNeighbour(BOTTOM_RIGHT);
            if (tl != nullptr && !vanishingBubbles.contains(tl))
                vanishingBubbles.push_back(tl);
            if (tr != nullptr && !vanishingBubbles.contains(tr))
                vanishingBubbles.push_back(tr);
            if (l != nullptr && !vanishingBubbles.contains(l))
                vanishingBubbles.push_back(l);
            if (r != nullptr && !vanishingBubbles.contains(r))
                vanishingBubbles.push_back(r);
            if (bl != nullptr && !vanishingBubbles.contains(bl))
                vanishingBubbles.push_back(bl);
            if (br != nullptr && !vanishingBubbles.contains(br))
                vanishingBubbles.push_back(br);
            // Remove
            //nextIterator.remove();
            //vanishingBubbles.
            vanishingBubbles.dequeue();
            removeItem(b);
            delete b;
        }
    }*/
    /*
    while (!vanishingBubbles.ha) {
        Bubble * b = vanishingBubbles.head();
        b->setScale(b->scale() * 0.85);
        if (b->scale() < 0.4) {
            b->setScale(b->scale() * 0.35);
        }
        if (b->scale() < 0.2) {
            Bubble * tl = b->getNeighbour(TOP_LEFT);
            Bubble * tr = b->getNeighbour(TOP_RIGHT);
            Bubble * l = b->getNeighbour(LEFT);
            Bubble * r = b->getNeighbour(RIGHT);
            Bubble * bl = b->getNeighbour(BOTTOM_LEFT);
            Bubble * br = b->getNeighbour(BOTTOM_RIGHT);
            if (tl != nullptr && !vanishingBubbles.contains(tl))
                vanishingBubbles.push_back(tl);
            if (tr != nullptr && !vanishingBubbles.contains(tr))
                vanishingBubbles.push_back(tr);
            if (l != nullptr && !vanishingBubbles.contains(l))
                vanishingBubbles.push_back(l);
            if (r != nullptr && !vanishingBubbles.contains(r))
                vanishingBubbles.push_back(r);
            if (bl != nullptr && !vanishingBubbles.contains(bl))
                vanishingBubbles.push_back(bl);
            if (br != nullptr && !vanishingBubbles.contains(br))
                vanishingBubbles.push_back(br);
            // Remove
            //nextIterator.remove();
            //vanishingBubbles.
            vanishingBubbles.dequeue();
            removeItem(b);
            delete b;
        }
    }*/
/*
    //Q<Bubble*> nextIterator(vanishingBubbles);
    // QVector<Bubble *> pending;
    while (nextIterator.hasNext()) {
        Bubble * b = nextIterator.next();
        b->setScale(b->scale() * 0.85);
        if (b->scale() < 0.4) {
            b->setScale(b->scale() * 0.35);
        }
        if (b->scale() < 0.2) {
            Bubble * tl = b->getNeighbour(TOP_LEFT);
            Bubble * tr = b->getNeighbour(TOP_RIGHT);
            Bubble * l = b->getNeighbour(LEFT);
            Bubble * r = b->getNeighbour(RIGHT);
            Bubble * bl = b->getNeighbour(BOTTOM_LEFT);
            Bubble * br = b->getNeighbour(BOTTOM_RIGHT);
            if (tl != nullptr && !next.contains(tl))
                next.push_back(tl);
            if (tr != nullptr && !next.contains(tr))
                next.push_back(tr);
            if (l != nullptr && !next.contains(l))
                next.push_back(l);
            if (r != nullptr && !next.contains(r))
                next.push_back(r);
            if (bl != nullptr && !next.contains(bl))
                next.push_back(bl);
            if (br != nullptr && !next.contains(br))
                next.push_back(br);
            // Remove
            nextIterator.remove();
            removeItem(b);
            delete b;
        }
    }
    qDebug() << next.size();
    //if (!pending.isEmpty())
    //    next.append(pending);
    //update();*/
}
