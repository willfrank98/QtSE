#include "canvas.h"
#include <QDebug>
#include <iostream>
#include <cmath>
#include "model.h"
#include <QGraphicsPixmapItem>
<<<<<<< HEAD

=======
#include "mainwindow.h"
>>>>>>> 46cdc788b8347550bafb24cd614a118c9e6a71a6
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
    setBG();
    Canvas::brush = new QBrush(Canvas::c1);
    pen = new QPen(QColor::fromRgbF(0.7, 0.8, 0.9, 1.0));
    this->size = size;
    this->pixSize = pixSize;
    this->data = new QImage(size, size, QImage::Format_RGB32);
    // drawBg
    //drawGrid();
}

void Canvas::setBG(){
    QGraphicsPixmapItem item;
    QString fileNamez = ":/Checkered.png";
    QGraphicsPixmapItem *pm = this->addPixmap( QPixmap(fileNamez) );
}

void Canvas::redraw(QPointF point) {

}

void Canvas::drawGrid() {
    pen->setWidthF(0.25);
    for (qreal y = 0; y < size; y++) {
        for (qreal x = 0; x < size; x++) {
            addRect(pixSize * x, pixSize * y, pixSize, pixSize, *pen);
        }
    }
}
void Canvas::deletePixel(QPointF point, QColor color){
    clear();
    setBG();
    QString key = QString::number(floor(point.x()/pixSize)) + "." +QString::number(floor(point.y()/pixSize));
    currentState.erase(key.toStdString());
    for (std::pair<std::string, QColor> element : currentState)
    {
        std::string delimiter = ".";
        std::string s1  = element.first.substr(0, element.first.find(delimiter));
        std::string s2  = element.first.substr(element.first.find(delimiter)+1, element.first.size());
        QPointF temp;
        int x1 = std::stoi(s1);
        int y1 = std::stoi(s2);
        qreal qx1 = static_cast<qreal>(x1);
        qreal qy1 = static_cast<qreal>(y1);
        temp.setX(qx1*pixSize);
        temp.setY(qy1*pixSize);
        putPixel(temp, element.second);
    }
}

void Canvas::putPixel(QPointF point, QColor color) {
    int x = floor(point.x() / pixSize);
    int y = floor(point.y() / pixSize);
    Canvas::brush = new QBrush(color, Qt::SolidPattern);
    QString key = QString::number(floor(point.x()/pixSize)) + "." +QString::number(floor(point.y()/pixSize));
    // don't place pixels outside the grid
    if (x < 0 || x >= size || y < 0 || y >= size) return;
    addRect(pixSize*x,pixSize*y,pixSize,pixSize,*pen,*brush);
    std::string newkey = key.toStdString();
    currentState.try_emplace(newkey, color);
}

void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (buttonHeld){
        if (mouseEvent->buttons() & Qt::LeftButton){
            if (drawMode == 0)
                putPixel(mouseEvent->scenePos(), Canvas::c1);
            else if (drawMode ==1)
                deletePixel(mouseEvent->scenePos(), Canvas::c1);
        }
        else if(mouseEvent->buttons() & Qt::RightButton){
            if (drawMode == 0)
                putPixel(mouseEvent->scenePos(), Canvas::c2);
            else if (drawMode ==1)
                deletePixel(mouseEvent->scenePos(), Canvas::c2);
        }
    }
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = true;
    if (mouseEvent->buttons() & Qt::LeftButton){
        if (drawMode == 0)
            putPixel(mouseEvent->scenePos(), Canvas::c1);
        else if (drawMode ==1)
            deletePixel(mouseEvent->scenePos(), Canvas::c1);
    }
    else if(mouseEvent->buttons() & Qt::RightButton){
        if (drawMode == 0)
            putPixel(mouseEvent->scenePos(), Canvas::c2);
        else if (drawMode ==1)
            deletePixel(mouseEvent->scenePos(), Canvas::c2);
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = false;
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::undo(){
    //If the undo stack has items we first take the current state and put it on the redo stack.
    //Then we pop off the top of the undo stack and set it to the current frame.
    if(!undoStack.isEmpty()){
        redoStack.push(*data);
        QImage temp = undoStack.pop();
        data = &temp;
    }
}

//Need to set up a way to empty the redo stack if a new change (not an undo/redo) is made. Im thinking of using a bool or sending a signal after each change.
void Canvas::redo(){
    //If the redo stack is not empty we first save the current frame to the undo stack.
    //Then we pop off the top of the redo stack and set it to the current frame.
    if(!redoStack.isEmpty()){
        undoStack.push(*data);
        QImage temp =redoStack.pop();
        data = &temp;
    }

}
