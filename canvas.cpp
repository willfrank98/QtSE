#include "canvas.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

Canvas::Canvas(QObject *parent) : QGraphicsScene(parent)
{

}

void Canvas::setPrimaryColor(QColor color) {
    primaryColor = color;
}

void Canvas::setSecondaryColor(QColor color) {
    secondaryColor = color;
}

void Canvas::displayImage(QImage image) {
    clear();
    addPixmap(QPixmap::fromImage(image));
}

void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (buttonHeld){
        QPoint convertedPoint = QPoint(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    }
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = true;
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = false;
//    qDebug() << mouseEvent->scenePos();
}

//void Canvas::undo(){
//    //If the undo stack has items we first take the current state and put it on the redo stack.
//    //Then we pop off the top of the undo stack and set it to the current frame.
//    if(!undoStack.isEmpty()){
////        redoStack.push(*data);
//        QImage temp = undoStack.pop();
////        data = &temp;
//    }
//}

//Need to set up a way to empty the redo stack if a new change (not an undo/redo) is made. Im thinking of using a bool or sending a signal after each change.
//void Canvas::redo(){
//    //If the redo stack is not empty we first save the current frame to the undo stack.
//    //Then we pop off the top of the redo stack and set it to the current frame.

//    if(!redoStack.isEmpty()){
//        undoStack.push(*data);
//        QImage temp =redoStack.pop();
//        data = &temp;
//    }

//}
