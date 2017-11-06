#ifndef CANVAS_H
#define CANVAS_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QStack>
#include<QRectF>
#include <unordered_map>
#include <frame.h>
class Canvas : public QGraphicsScene
{
    Q_OBJECT
signals:
    void clickToGV(QGraphicsSceneMouseEvent*, int frameNum);
private:
    std::unordered_map<std::string, QColor> previousState;
    std::unordered_map<std::string, QColor> currentState;
    std::unordered_map<std::string, QColor> currentRect;
    std::unordered_map<std::string, QColor> currentCirc;
    QGraphicsRectItem *rect[128][128];
    QPen *pen;
    qreal lastx,lasty;
    int sizex = 32;
    int sizey = 32;
    QImage *data;
    QPointF startingPoint;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;
    bool buttonHeld = false;
    //bool imageChanged = false;
    void putPixel(QPointF, QColor);
    void deletePixel(QPointF, QColor);
    void setBG();
    void drawRect(QPointF, QColor);
    void drawCirc(QPointF, QColor);
    std::unordered_map<std::string, QColor> getRectPoints(QPointF endingPoint, QColor color);
    std::unordered_map<std::string, QColor> getCircPoints(QPointF endingPoint, QColor color);


public:
    QImage image;
    void setMap();
    int frameNumber;
    qreal pixSize = 480/32;
    int drawMode;
    static QColor c1;
    static QColor c2;
    static QColor c1Last;
    static QColor c2Last;
    static QBrush *brush;
    explicit Canvas(QObject *parent = nullptr);
    explicit Canvas(QImage, int, int, int, QObject *parent = nullptr);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void redraw(std::unordered_map<std::string, QColor> points, int mode);
    void drawGrid();
    void undo();
    void redo();
};

#endif // CANVAS_H
