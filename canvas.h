#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Canvas : public QGraphicsScene
{
    Q_OBJECT

private:
    int size = 32;
    qreal pixSize = 480/32;
    QBrush *brush;
    QImage *data;
    bool buttonHeld = false;

    void putPixel(QPointF);

public:
    explicit Canvas(QObject *parent = nullptr);
    explicit Canvas(int, qreal, QObject *parent = nullptr);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

    void redraw();
    void drawGrid();
};

#endif // CANVAS_H
