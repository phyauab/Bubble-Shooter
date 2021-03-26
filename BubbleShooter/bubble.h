#ifndef BUBBLE_H
#define BUBBLE_H

#include <QGraphicsItem>
#include <QPainter>

enum Direction {
    TOP_LEFT, TOP_RIGHT, LEFT, RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
};

enum Color {
    BLACK, RED, YELLOW, GREEN, BLUE, PURPLE
};

enum Kind {
    NORMAL,
    BONUS,
    EXPLOSIVE
};

class Bubble : public QGraphicsItem
{
public:
    Bubble(Kind kind, Color color, QGraphicsItem* parent = nullptr);
    Bubble(int row, int col, Kind kind, Color color, QGraphicsItem* parent = nullptr);
    ~Bubble() override;
    void setNeighbour(Direction direction, Bubble* neighbour);
    Bubble** getNeighbours();
    Bubble* getNeighbour(Direction direction);
    Color getColor();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void eliminate(QMap<int, QVector<Bubble*>>& bubbles, QVector<Bubble*>& fallingBubbles, int depth = 0);
    // void eliminateB(QMap<int, QVector<Bubble*>>& bubbles, int depth = 0);
    //void testConnectivity(bool visited[][9]);
    bool reachTop(QVector<Bubble*>& traversed);
    const static int width = 60;
    const static int height = 60;

    int row = 0;
    int col = 0;
private:
    Bubble ** neighbours;
    Color color = Color::BLACK;
    bool eliminated = false;
    bool fallen = false;
    void unlink();
};

#endif // BUBBLE_H
