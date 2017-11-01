#ifndef CANVAS_H
#define CANVAS_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QStack>
#include<QRectF>
#include <unordered_map>
class Canvas : public QGraphicsScene
{
    Q_OBJECT
signals:
    void updateGV();
private:
    bool *hasPixel[128][128];
    QPen *pen;
    int size = 32;
    qreal pixSize = 480/32;
    QImage *data;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;
    bool buttonHeld = false;
    //bool imageChanged = false;
    void putPixel(QPointF, QColor);
    void deletePixel(QPointF, QColor);
    void setBG();

public:
    std::unordered_map<std::string, QColor> currentState;
    int drawMode;
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
    void redraw(QPointF );
    void drawGrid();
    void undo();
    void redo();
};

#endif // CANVAS_H
