#ifndef CANVAS_H
#define CANVAS_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
class Canvas : public QGraphicsScene
{
    Q_OBJECT

private:
    QPen *pen;
    int size = 32;
    qreal pixSize = 480/32;
    QImage *data;
    bool buttonHeld = false;
    void putPixel(QPointF, QColor);

public:
    static QColor c1;
    static QColor c2;
    static QColor c1Last;
    static QColor c2Last;
    static QBrush *brush;
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
