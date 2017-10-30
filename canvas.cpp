#include "canvas.h"
#include <QDebug>
#include <iostream>
#include "model.h"
#include <QGraphicsPixmapItem>
using namespace std;
QBrush *Canvas::brush;
QColor Canvas::c1;
QColor Canvas::c2;
QColor Canvas::c1Last;
QColor Canvas::c2Last;
Canvas::Canvas(QObject *parent) : QGraphicsScene(parent)
{

}

Canvas::Canvas(int size, qreal pixSize, QObject *parent) : QGraphicsScene(parent)
{
    QGraphicsPixmapItem item;
    QString fileNamez = ":/Checkered.png";
    QGraphicsPixmapItem *pm = this->addPixmap( QPixmap(fileNamez) );
    Canvas::brush = new QBrush(Canvas::c1);
    pen = new QPen(QColor::fromRgbF(0.7, 0.8, 0.9, 1.0));
    this->size = size;
    this->pixSize = pixSize;
    this->data = new QImage(size, size, QImage::Format_RGB32);
    // drawBg
    drawGrid();
}

void Canvas::redraw() {
    // This might be necessary later.
}

void Canvas::drawGrid() {
    pen->setWidthF(0.25);
    for (qreal y = 0; y < size; y++) {
        for (qreal x = 0; x < size; x++) {
            addRect(pixSize * x, pixSize * y, pixSize, pixSize, *pen);
        }
    }
}

void Canvas::putPixel(QPointF point, QColor color) {
    qreal x = floor(point.x() / pixSize);
    qreal y = floor(point.y() / pixSize);
    Canvas::brush = new QBrush(color, Qt::SolidPattern);
    // don't place pixels outside the grid
    if (x < 0 || x >= size || y < 0 || y >= size) return;

    data->setPixel(x, y, Canvas::brush->color().rgb());
    addRect(pixSize * x, pixSize * y, pixSize, pixSize, *pen, *brush);
}

void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (buttonHeld){
        if (mouseEvent->buttons() & Qt::LeftButton)
            putPixel(mouseEvent->scenePos(), Canvas::c1);
        else if(mouseEvent->buttons() & Qt::RightButton)
            putPixel(mouseEvent->scenePos(), Canvas::c2);
    }
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = true;
    if (mouseEvent->buttons() & Qt::LeftButton)
        putPixel(mouseEvent->scenePos(), Canvas::c1);
    else if(mouseEvent->buttons() & Qt::RightButton)
        putPixel(mouseEvent->scenePos(), Canvas::c2);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = false;
//    qDebug() << mouseEvent->scenePos();
}
