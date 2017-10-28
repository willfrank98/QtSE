#include "canvas.h"
#include <QDebug>

Canvas::Canvas(QObject *parent) : QGraphicsScene(parent)
{

}

Canvas::Canvas(int size, qreal pixSize, QObject *parent) : QGraphicsScene(parent)
{
    this->size = size;
    this->pixSize = pixSize;
    this->data = new QImage(size, size, QImage::Format_RGB32);
    this->brush = new QBrush(QColor::fromRgbF(0.0, 0.0, 0.0, 1.0));

    // drawBg
    drawGrid();

}

void Canvas::redraw() {
    // This might be necessary later.
}

void Canvas::drawGrid() {
    QPen *pen = new QPen(QColor::fromRgbF(0.7, 0.8, 0.9, 1.0));
    pen->setWidthF(0.25);

    for (qreal y = 0; y < size; y++) {
        for (qreal x = 0; x < size; x++) {
            addRect(pixSize * x, pixSize * y, pixSize, pixSize, *pen);
        }
    }
}

void Canvas::putPixel(QPointF point) {
    qreal x = floor(point.x() / pixSize);
    qreal y = floor(point.y() / pixSize);
    QPen *pen = new QPen(QColor::fromRgb(255 - brush->color().red(),
                         255 - brush->color().green(),
                         255 - brush->color().blue()));

    // don't place pixels outside the grid
    if (x < 0 || x >= size || y < 0 || y >= size) return;

    data->setPixel(x, y, brush->color().rgb());
    addRect(pixSize * x, pixSize * y, pixSize, pixSize, *pen, *brush);
}

void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (buttonHeld)
        putPixel(mouseEvent->scenePos());
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = true;
    putPixel(mouseEvent->scenePos());
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = false;
//    qDebug() << mouseEvent->scenePos();
}
