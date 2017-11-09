/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * canvas.cpp
 *
 *
 */

#include "canvas.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPen>

Canvas::Canvas(QObject *parent) : QGraphicsScene(parent)
{
    tool = PenTool;
}

void Canvas::setPrimaryColor(QColor color) {
    primaryColor = color;
}

void Canvas::setSecondaryColor(QColor color) {
    secondaryColor = color;
}

void Canvas::swapColors() {
    QColor temp = secondaryColor;
    secondaryColor = primaryColor;
    primaryColor = temp;
}

void Canvas::setTool(Tool _tool) {
    tool = _tool;
}

void Canvas::setFrame(Frame *frame) {
    this->frame = frame;
    rect = QRect();
    refresh();
}

void Canvas::draw(QPointF point) {
    if (buttonHeld != Qt::NoButton) {
        QPoint convertedPoint = QPoint(point.x() / pixSize.width(), point.y() / pixSize.height());
        QColor color = (buttonHeld == Qt::LeftButton) ? primaryColor : secondaryColor;

        switch (tool) {
        case PenTool:
            frame->drawPen(convertedPoint, color);
            break;
        case MirrorPenTool:
            frame->drawMirrorPen(convertedPoint, color);
            break;
        case EraserTool:
            frame->erase(convertedPoint);
            break;
        case DitheringTool:
            if (convertedPoint.x() % 2 == 0 && convertedPoint.y() % 2 == 0) frame->drawPen(convertedPoint, primaryColor);
            else if (convertedPoint.x() % 2 == 1 && convertedPoint.y() % 2 == 1) frame->drawPen(convertedPoint, primaryColor);
            else if (convertedPoint.x() % 2 == 1 && convertedPoint.y() % 2 == 0) frame->drawPen(convertedPoint, secondaryColor);
            else if (convertedPoint.x() % 2 == 0 && convertedPoint.y() % 2 == 1) frame->drawPen(convertedPoint, secondaryColor);
            break;
        case BucketFillTool:
            if (buttonHeld == Qt::LeftButton) frame->bucketFill(convertedPoint,frame->pixels().pixelColor(convertedPoint) , primaryColor);
            if (buttonHeld == Qt::RightButton) frame->bucketFill(convertedPoint,frame->pixels().pixelColor(convertedPoint)  , secondaryColor);
            break;
        case ColorFillTool:
            if (buttonHeld == Qt::LeftButton) frame->colorFill(convertedPoint, primaryColor);
            if (buttonHeld == Qt::RightButton) frame->colorFill(convertedPoint, secondaryColor);
            break;
        default:
            break;
        }
        refresh();
    }
}

void Canvas::refresh() {
    pixSize = QSizeF(sceneRect().width() / (qreal)frame->size().width(),
                     sceneRect().height() / (qreal)frame->size().width());
    QRect convertedRect = QRect(rect.x() / pixSize.width(), rect.y() / pixSize.height(),
                                rect.size().width() / pixSize.width(), rect.size().height() / pixSize.height());
    clear();

    // Need special handling for ellipses and rectangles
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(pixSize.width());
    pen.setColor(primaryColor);
    switch (tool) {
    case RectangleTool:
        if (buttonHeld != Qt::NoButton) addRect(rect, pen)->setZValue(1);
        else frame->drawRectangle(convertedRect, primaryColor, QColor(0, 0, 0, 0));
        break;
    case EllipseTool:
        if (buttonHeld != Qt::NoButton) addEllipse(rect, pen)->setZValue(1);
        else frame->drawEllipse(convertedRect, primaryColor, QColor(0, 0, 0, 0));
        break;
    case RectSelectTool:
        // I don't really know what we want to be doing here...
        pen.setWidth(2);
        pen.setStyle(Qt::DashLine);
        addRect(rect, pen)->setZValue(1);
        pen.setStyle(Qt::SolidLine);
        break;
    case LineTool:
        if (buttonHeld != Qt::NoButton) addLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height(), pen)->setZValue(1);
        else if (convertedRect != QRect()) frame->drawLine(QPoint(convertedRect.x(), convertedRect.y()),
                             QPoint(convertedRect.x() + convertedRect.width(), convertedRect.y() + convertedRect.height()),
                             primaryColor);
        break;
    default:
        break;
    }
    addPixmap(QPixmap::fromImage(frame->pixels().scaled(sceneRect().width(), sceneRect().height())));

    emit frameUpdated(frame);
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!mouseEnabled) return;

    rect = QRectF(rect.x(), rect.y(), mouseEvent->scenePos().x() - rect.x(), mouseEvent->scenePos().y() - rect.y());
    draw(mouseEvent->scenePos());

//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!mouseEnabled) return;

    buttonHeld = mouseEvent->button();
    rect = QRectF(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), 0, 0);
    draw(mouseEvent->scenePos());
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!mouseEnabled) return;

    buttonHeld = Qt::NoButton;
    if (tool == RectangleTool || tool == EllipseTool || tool == LineTool) refresh();
    emit pixelsModified(frame->pixels());

//    qDebug() << mouseEvent->scenePos();
}
