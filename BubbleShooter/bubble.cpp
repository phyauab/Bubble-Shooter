#include "bubble.h"
#include "resources.h"
#include <QPropertyAnimation>
#include <QDebug>
#include <QVector>
#include <QQueue>

Bubble::Bubble(int row, int col, Kind kind, Color color, QGraphicsItem * parent):QGraphicsItem(parent)
{
    this->row = row;
    this->col = col;
    neighbours = new Bubble*[6];
    for (int i = 0; i < 6; i++) {
        neighbours[i] = nullptr;
    }
    this->color = color;
}

Bubble::Bubble(Kind kind, Color color, QGraphicsItem * parent):Bubble(0, 0, kind, color, parent)
{
}

Bubble::~Bubble()
{
    unlink();
    delete [] neighbours;
}

void Bubble::unlink() {
    if (neighbours[TOP_LEFT] != nullptr) {
        neighbours[TOP_LEFT]->setNeighbour(BOTTOM_RIGHT, nullptr);
    }
    if (neighbours[TOP_RIGHT] != nullptr) {
        neighbours[TOP_RIGHT]->setNeighbour(BOTTOM_LEFT, nullptr);
    }
    if (neighbours[LEFT] != nullptr) {
        neighbours[LEFT]->setNeighbour(RIGHT, nullptr);
    }
    if (neighbours[RIGHT] != nullptr) {
        neighbours[RIGHT]->setNeighbour(LEFT, nullptr);
    }
    if (neighbours[BOTTOM_LEFT] != nullptr) {
        neighbours[BOTTOM_LEFT]->setNeighbour(TOP_RIGHT, nullptr);
    }
    if (neighbours[BOTTOM_RIGHT] != nullptr) {
        neighbours[BOTTOM_RIGHT]->setNeighbour(TOP_LEFT, nullptr);
    }
}

void Bubble::setNeighbour(Direction direction, Bubble* neighbour) {
    neighbours[direction] = neighbour;
}

Bubble** Bubble::getNeighbours() {
    return neighbours;
}

Bubble * Bubble::getNeighbour(Direction direction) {
    return neighbours[direction];
}
Color Bubble::getColor() {
    return color;
}

QRectF Bubble::boundingRect() const {
    return QRect(-width / 2, -height / 2, width, height);
}

void Bubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap * bubbleImage = nullptr;
    switch (color) {
    case BLACK:
        bubbleImage = Resources::getInstance()->getResource("bubble_black");
    break;
    case RED:
        bubbleImage = Resources::getInstance()->getResource("bubble_red");
    break;
    case YELLOW:
        bubbleImage = Resources::getInstance()->getResource("bubble_yellow");
    break;
    case GREEN:
        bubbleImage = Resources::getInstance()->getResource("bubble_green");
    break;
    case BLUE:
        bubbleImage = Resources::getInstance()->getResource("bubble_blue");
    break;
    case PURPLE:
        bubbleImage = Resources::getInstance()->getResource("bubble_purple");
    break;
    }
    if (bubbleImage != nullptr)
        painter->drawPixmap(-width / 2, -height / 2, width, height, *bubbleImage);
}
/*
void Bubble::testConnectivity(bool visited[][9]) {
    for (int dir = TOP_LEFT; dir <= BOTTOM_RIGHT; dir++) {
        if (neighbours[dir] != nullptr) {
            int row = neighbours[dir]->row;
            int col = neighbours[dir]->col;
            if (!visited[row][col]) {
                visited[row][col] = true;
                neighbours[dir]->testConnectivity(visited);
            }
        }
    }
}*/
/*
void Bubble::eliminateB(QMap<int, QVector<Bubble*>> &eliminatedBubbles, int level) {
    // Unlink the bubbles if not having same color
    eliminated = true;
    eliminatedBubbles[level].push_back(this);
    QVector<Bubble *> sameColorNeighbour;
    if (neighbours[TOP_LEFT] != nullptr && !neighbours[TOP_LEFT]->eliminated) {
        if (neighbours[TOP_LEFT]->getColor() != color) {
            neighbours[TOP_LEFT]->setNeighbour(BOTTOM_RIGHT, nullptr);
            neighbours[TOP_LEFT] = nullptr;
        } else {
            neighbours[TOP_LEFT]->eliminate(eliminatedBubbles, level + 1);
            //neighbours[TOP_LEFT]->eliminate(eliminatedBubbles, level + 1);
        }
    }
    if (neighbours[TOP_RIGHT] != nullptr && !neighbours[TOP_RIGHT]->eliminated) {
        if (neighbours[TOP_RIGHT]->getColor() != color) {
            neighbours[TOP_RIGHT]->setNeighbour(BOTTOM_LEFT, nullptr);
            neighbours[TOP_RIGHT] = nullptr;
        } else {
            neighbours[TOP_RIGHT]->eliminate(eliminatedBubbles, level + 1);
        }
    }
    if (neighbours[LEFT] != nullptr && !neighbours[LEFT]->eliminated) {
        if (neighbours[LEFT]->getColor() != color) {
            neighbours[LEFT]->setNeighbour(RIGHT, nullptr);
            neighbours[LEFT] = nullptr;
        } else {
            neighbours[LEFT]->eliminate(eliminatedBubbles, level + 1);
        }
    }
    if (neighbours[RIGHT] != nullptr && !neighbours[RIGHT]->eliminated) {
        if (neighbours[RIGHT]->getColor() != color) {
            neighbours[RIGHT]->setNeighbour(LEFT, nullptr);
            neighbours[RIGHT] = nullptr;
        } else {
            neighbours[RIGHT]->eliminate(eliminatedBubbles, level + 1);
        }
    }
    if (neighbours[BOTTOM_LEFT] != nullptr && !neighbours[BOTTOM_LEFT]->eliminated) {
        if (neighbours[BOTTOM_LEFT]->getColor() != color) {
            neighbours[BOTTOM_LEFT]->setNeighbour(TOP_RIGHT, nullptr);
            neighbours[BOTTOM_LEFT] = nullptr;
        } else {
            neighbours[BOTTOM_LEFT]->eliminate(eliminatedBubbles, level + 1);
        }
    }
    if (neighbours[BOTTOM_RIGHT] != nullptr && !neighbours[BOTTOM_RIGHT]->eliminated) {
        if (neighbours[BOTTOM_RIGHT]->getColor() != color) {
            neighbours[BOTTOM_RIGHT]->setNeighbour(TOP_LEFT, nullptr);
            neighbours[BOTTOM_RIGHT] = nullptr;
        } else {
            neighbours[BOTTOM_RIGHT]->eliminate(eliminatedBubbles, level + 1);
        }
    }
}

*/

bool Bubble::reachTop(QVector<Bubble*>& traversed) {
    if (row == 0) {
        return true;
    }
    if (this->eliminated) {
        return false;
    }
    if (traversed.contains(this)) {
        return false;
    }
    traversed.push_back(this);
    for (int dir = TOP_LEFT; dir <= BOTTOM_RIGHT; dir++) {
        if (neighbours[dir] != nullptr) {
            if (neighbours[dir]->reachTop(traversed)) {
                return true;
            }
        }
    }
    return false;
}

// Reference: https://stackoverflow.com/questions/31247634/how-to-keep-track-of-depth-in-breadth-first-search
// For keep tracking depth for bfs
void Bubble::eliminate(QMap<int, QVector<Bubble*>> &eliminatedBubbles, QVector<Bubble*>& fallingBubbles, int level) {
    QQueue<Bubble*> queue;
    queue.enqueue(this);
    queue.enqueue(nullptr);
    QVector<Bubble*> surroundings; // Neighbours of the eliminated bubbles

    while (!queue.empty()) {
        Bubble * b = queue.dequeue();
        if (b == nullptr) {
            level++;
            queue.enqueue(nullptr);
            if (queue.head() == nullptr) break;
            else continue;
        }
        b->unlink();
        eliminatedBubbles[level].push_back(b);

        for (int dir = TOP_LEFT; dir <= BOTTOM_RIGHT; dir++) {
            if (b->neighbours[dir] != nullptr && !b->neighbours[dir]->eliminated) {
                if (b->neighbours[dir]->getColor() == color) {
                    b->neighbours[dir]->eliminated = true;
                    queue.enqueue(b->neighbours[dir]);
                } else {
                    if (!surroundings.contains(b->neighbours[dir]))
                        surroundings.push_back(b->neighbours[dir]);
                }
            }
        }
    }
    // Falling bubbles must have been attached to the bubbles that were just removed
    // Reference https://gamedev.stackexchange.com/questions/27927/bubble-shooter-falling-algorithm
    for (auto & s : surroundings) {
        // qDebug() << s;
        QVector<Bubble*> temp;
        bool rt = s->reachTop(temp);
        if (!rt) {
            for (auto & t : temp) {
                t->eliminated = true;
            }
            fallingBubbles.append(temp);
        }
    }
}
