#include "canvas.h"
#include <QDebug>
#include <iostream>
#include <cmath>
#include "model.h"
#include "mainwindow.h"
using namespace std;
QBrush *Canvas::brush;
QColor Canvas::c1;
QColor Canvas::c2;
QColor Canvas::c1Last;
QColor Canvas::c2Last;
int Canvas::drawMode;
Canvas::Canvas(QObject *parent) : QGraphicsScene(parent)
{

}

Canvas::Canvas(QImage i, int sizex, int sizey,  int frame, QObject *parent) : QGraphicsScene(parent)
{
    drawMode = 0;
    QPixmap *pix = new QPixmap(QPixmap::fromImage(i));
    //graphicspixmap->paint(painter, );
    //addItem(graphicspixmap);
    frameNumber = frame;
    for (int i = 0; i < 128; i++)
        for (int j = 0; j < 128; j++)
            rect[i][j] = NULL;
    setBG();
    Canvas::brush = new QBrush(Canvas::c1);
    pen = new QPen(QColor::fromRgbF(0.7, 0.8, 0.9, 1.0));
    this->sizex = sizex;
    this->sizey = sizey;
    this->pixSize = 1;
    this->data = new QImage(sizex, sizey, QImage::Format_RGB32);
    drawGrid();
    QPainter *painter = new QPainter(pix);
    QRect r(0,0,3,2);
    painter->drawRect(r);
    QGraphicsPixmapItem *graphicspixmap = new QGraphicsPixmapItem(*pix);
    addItem(graphicspixmap);
}

void Canvas::setBG(){

}

void Canvas::redraw(std::unordered_map<std::string, QColor> points, int mode) {
    for (std::pair<std::string, QColor> element : points)
    {
        std::string delimiter = ".";
        std::string s1  = element.first.substr(0, element.first.find(delimiter));
        std::string s2  = element.first.substr(element.first.find(delimiter)+1, element.first.size());
        QPointF temp;
        int x1 = std::stoi(s1);
        int y1 = std::stoi(s2);
        if (x1 < 0 | x1 > sizex | y1 < 0 | y1 > sizey)
            return;
        qreal qx1 = static_cast<qreal>(x1);
        qreal qy1 = static_cast<qreal>(y1);
        temp.setX(qx1*pixSize);
        temp.setY(qy1*pixSize);
        if (mode == 0)
            putPixel(temp, element.second);
        else if (mode == 1)
            deletePixel(temp, element.second);
    }
}

void Canvas::drawGrid() {
    pen->setWidthF(0);
    for (qreal y = 0; y < sizey; y++) {
        for (qreal x = 0; x < sizex; x++) {
            addRect(pixSize * x, pixSize * y, pixSize, pixSize, *pen);
        }
    }
}
void Canvas::deletePixel(QPointF point, QColor color){
    int x = floor(point.x() / pixSize);
    int y = floor(point.y() / pixSize);
    if(rect[x][y]!=NULL){
        removeItem(rect[x][y]);
        rect[x][y]==NULL;
        currentState.erase(std::to_string(x)+"."+std::to_string(y));
    }
}

void Canvas::putPixel(QPointF point, QColor color) {
    int x = floor(point.x() / pixSize);
    int y = floor(point.y() / pixSize);
    Canvas::brush = new QBrush(color, Qt::SolidPattern);
    QString key = QString::number(floor(point.x()/pixSize)) + "." +QString::number(floor(point.y()/pixSize));
    // don't place pixels outside the grid
    if (x < 0 || x >= sizex || y < 0 || y >= sizey) return;
    if (rect[x][y] != NULL){
        removeItem(rect[x][y]);
        rect[x][y]==NULL;
    }

    rect[x][y] = addRect(pixSize*x,pixSize*y,pixSize,pixSize,*pen,*brush);
    data->setPixel(x, y, Canvas::brush->color().rgb());
    std::string newkey = key.toStdString();
    currentState.try_emplace(newkey, color);
}


//starting point is saved in startingPoint private member. endingPoint is the current point the cursor is over.
std::unordered_map<std::string, QColor> Canvas::getRectPoints(QPointF endingPoint, QColor color){
    std::unordered_map<std::string, QColor> vals;
    std::string str;
    int xEnd = floor(endingPoint.x() / pixSize);
    int yEnd = floor(endingPoint.y() / pixSize);
    int xStart = floor(startingPoint.x() / pixSize);
    int yStart = floor(startingPoint.y() / pixSize);
    int dx = std::abs(xStart-xEnd);
    int dy = std::abs(yStart-yEnd);
    if (xStart >= xEnd){
        for (int i = 0; i <= dx; i++){
            vals.try_emplace(std::to_string(xStart-i)+"."+std::to_string(yStart), color);
            vals.try_emplace(std::to_string(xStart-i)+"."+std::to_string(yEnd), color);
        }
    }
    else if (xStart <= xEnd){
        for (int i = 0; i <= dx; i++){
            vals.try_emplace(std::to_string(xStart+i)+"."+std::to_string(yStart), color);
            vals.try_emplace(std::to_string(xStart+i)+"."+std::to_string(yEnd), color);
        }
    }
    if (yStart >= yEnd){
        for (int i = 0; i <= dy; i++){
            vals.try_emplace(std::to_string(xStart)+"."+std::to_string(yStart-i), color);
            vals.try_emplace(std::to_string(xEnd)+"."+std::to_string(yStart-i), color);
        }
    }
    else if (yStart <= yEnd){
        for (int i = 0; i <= dy; i++){
            vals.try_emplace(std::to_string(xStart)+"."+std::to_string(yStart+i), color);
            vals.try_emplace(std::to_string(xEnd)+"."+std::to_string(yStart+i), color);
        }
    }
    return vals;
}


void Canvas::drawRect(QPointF point, QColor color){
    int x = floor(point.x() / pixSize);
    int y = floor(point.y() / pixSize);
    if (x < 0 || x >= sizex || y < 0 || y >= sizey) return;
    redraw(currentRect, 1);  //mode 1 for erase mode 0 for draw
    currentRect = getRectPoints(point,color);
    redraw(previousState, 0);
    redraw(currentRect, 0);
}

void Canvas::drawCirc(QPointF point, QColor color){
    redraw(currentCirc, 1);  //mode 1 for erase mode 0 for draw
    currentCirc = getCircPoints(point,color);
    redraw(previousState, 0);
    redraw(currentCirc, 0);
}

std::unordered_map<std::string, QColor> Canvas::getCircPoints(QPointF endingPoint, QColor color){
    std::unordered_map<std::string, QColor> vals;
    int startx = floor(startingPoint.x()/pixSize)*pixSize; //get the top corner.
    int starty = floor(startingPoint.y()/pixSize)*pixSize;
    double startXMid = startx + pixSize/2;
    double startYMid = starty + pixSize/2;
    int endx = floor(endingPoint.x()/pixSize)*pixSize; //get the top corner.
    int endy = floor(endingPoint.y()/pixSize)*pixSize;
    double endXMid = endx + pixSize/2;
    double endYMid = endy + pixSize/2;
    cout<<startx <<" "<<starty <<" "<<pixSize<<endl;
    qreal xmid = (startXMid + endXMid) /2;
    qreal ymid = (startYMid + endYMid) /2;
    qreal pi = atan(1)*4;
    qreal xlen = std::abs(startXMid - endXMid);
    qreal ylen = std::abs(startYMid - endYMid);
    qreal r = std::sqrt(xlen*xlen + ylen*ylen);
    qreal circum = r * pi;
    qreal arclen = circum / pixSize * 3;
    qreal theta = 2*pi / arclen;
    for (double angle=0; angle<=2*pi; angle+=theta){
        int x = floor(xmid+ r*cos( angle ))/pixSize;
        int y = floor(ymid + r *sin( angle ))/pixSize;
        std::string temp = std::to_string(x)+"."+std::to_string(y);
        vals.try_emplace(temp, color);
   }
    redraw(vals, 1);
   return vals;
}
void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (buttonHeld){
        if(lastx != floor(mouseEvent->scenePos().x()/pixSize) && lasty != mouseEvent->scenePos().y()/pixSize){

            if (mouseEvent->buttons() & Qt::LeftButton){
                if (drawMode == 0)
                    putPixel(mouseEvent->scenePos(), Canvas::c1);
                else if (drawMode == 1)
                    deletePixel(mouseEvent->scenePos(), Canvas::c1);
                else if (drawMode == 2)
                    drawCirc(mouseEvent->scenePos(), Canvas::c1);
                else if (drawMode == 3)
                    drawRect(mouseEvent->scenePos(), Canvas::c1);
            }
            else if(mouseEvent->buttons() & Qt::RightButton){
                if (drawMode == 0)
                    putPixel(mouseEvent->scenePos(), Canvas::c2);
                else if (drawMode == 1)
                    deletePixel(mouseEvent->scenePos(), Canvas::c2);
                else if (drawMode == 2)
                    drawCirc(mouseEvent->scenePos(), Canvas::c1);
                else if (drawMode == 3)
                    drawRect(mouseEvent->scenePos(), Canvas::c2);
            }
        }
    }
    lastx = floor(mouseEvent->scenePos().x()/pixSize);
    lasty = floor(mouseEvent->scenePos().y()/pixSize);
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    startingPoint = mouseEvent->scenePos();
    buttonHeld = true;
    previousState = currentState;
    if (mouseEvent->buttons() & Qt::LeftButton){
        if (drawMode == 0 | drawMode==3)
            putPixel(mouseEvent->scenePos(), Canvas::c1);
        else if (drawMode ==1 )
            deletePixel(mouseEvent->scenePos(), Canvas::c1);
    }
    else if(mouseEvent->buttons() & Qt::RightButton){
        if (drawMode == 0 | drawMode==3)
            putPixel(mouseEvent->scenePos(), Canvas::c2);
        else if (drawMode ==1)
            deletePixel(mouseEvent->scenePos(), Canvas::c2);
    }
    emit clickToGV(mouseEvent, frameNumber);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = false;
//    qDebug() << mouseEvent->scenePos();
    if (drawMode == 3){
        previousState.insert(currentRect.begin(), currentRect.end());
    }
    if (drawMode == 2){
        previousState.insert(currentCirc.begin(), currentCirc.end());
    }
    currentState = previousState;
    redraw(currentState, 0);
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
