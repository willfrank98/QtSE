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
    pixSize = QSizeF(sceneRect().width() / (qreal)image.size().width(),
                                                  sceneRect().height() / (qreal)image.size().height());
    clear();
    addPixmap(QPixmap::fromImage(image.scaled(sceneRect().width(), sceneRect().height())));
}

void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (buttonHeld != Qt::NoButton){
        QPoint convertedPoint = QPoint(mouseEvent->scenePos().x() / pixSize.width(), mouseEvent->scenePos().y() / pixSize.height());
        points.append(convertedPoint);
    }
//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    buttonHeld = mouseEvent->button();

    QPoint convertedPoint = QPoint(mouseEvent->scenePos().x() / pixSize.width(), mouseEvent->scenePos().y() / pixSize.height());
    points.append(convertedPoint);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPoint convertedPoint = QPoint(mouseEvent->scenePos().x() / pixSize.width(), mouseEvent->scenePos().y() / pixSize.height());
    points.append(convertedPoint);
    if (buttonHeld == Qt::LeftButton) emit modifiedPixels(points, primaryColor);
    else if (buttonHeld == Qt::RightButton) emit modifiedPixels(points, secondaryColor);
    points.clear();
    buttonHeld = Qt::NoButton;

//    qDebug() << mouseEvent->scenePos();
}

