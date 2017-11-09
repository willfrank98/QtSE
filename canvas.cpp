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
    _tool = PenTool;
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

void Canvas::setTool(Tool tool) {
    _tool = tool;
}

void Canvas::setFrame(Frame *frame) {
    this->_frame = frame;
    _rect = QRect();
    refresh();
}

void Canvas::draw(QPointF point) {
    if (buttonHeld != Qt::NoButton) {
        QPoint convertedPoint = QPoint(point.x() / pixSize.width(), point.y() / pixSize.height());
        QColor color = (buttonHeld == Qt::LeftButton) ? primaryColor : secondaryColor;

        switch (_tool) {
        case PenTool:
            _frame->drawPen(convertedPoint, color);
            break;
        case MirrorPenTool:
            _frame->drawMirrorPen(convertedPoint, color);
            break;
        case EraserTool:
            _frame->erase(convertedPoint);
            break;
        case DitheringTool:
            if (convertedPoint.x() % 2 == 0 && convertedPoint.y() % 2 == 0) _frame->drawPen(convertedPoint, primaryColor);
            else if (convertedPoint.x() % 2 == 1 && convertedPoint.y() % 2 == 1) _frame->drawPen(convertedPoint, primaryColor);
            else if (convertedPoint.x() % 2 == 1 && convertedPoint.y() % 2 == 0) _frame->drawPen(convertedPoint, secondaryColor);
            else if (convertedPoint.x() % 2 == 0 && convertedPoint.y() % 2 == 1) _frame->drawPen(convertedPoint, secondaryColor);
            break;
        case BucketFillTool:
            if (buttonHeld == Qt::LeftButton) _frame->bucketFill(convertedPoint,_frame->pixels().pixelColor(convertedPoint) , primaryColor);
            if (buttonHeld == Qt::RightButton) _frame->bucketFill(convertedPoint,_frame->pixels().pixelColor(convertedPoint)  , secondaryColor);
            break;
        case ColorFillTool:
            if (buttonHeld == Qt::LeftButton) _frame->colorSwap(convertedPoint, primaryColor);
            if (buttonHeld == Qt::RightButton) _frame->colorSwap(convertedPoint, secondaryColor);
            break;
        default:
            break;
        }
        refresh();
    }
}

void Canvas::refresh() {
    pixSize = QSizeF(sceneRect().width() / (qreal)_frame->size().width(),
                     sceneRect().height() / (qreal)_frame->size().width());
    QRect convertedRect = QRect(_rect.x() / pixSize.width(), _rect.y() / pixSize.height(),
                                _rect.size().width() / pixSize.width(), _rect.size().height() / pixSize.height());
    clear();

    // Need special handling for ellipses and rectangles
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(pixSize.width());
    pen.setColor(primaryColor);
    switch (_tool) {
    case RectangleTool:
        if (buttonHeld != Qt::NoButton) addRect(_rect, pen)->setZValue(1);
        else _frame->drawRectangle(convertedRect, primaryColor, QColor(0, 0, 0, 0));
        break;
    case EllipseTool:
        if (buttonHeld != Qt::NoButton) addEllipse(_rect, pen)->setZValue(1);
        else _frame->drawEllipse(convertedRect, primaryColor, QColor(0, 0, 0, 0));
        break;
    case RectSelectTool:
        // I don't really know what we want to be doing here...
        pen.setWidth(2);
        pen.setStyle(Qt::DashLine);
        addRect(_rect, pen)->setZValue(1);
        pen.setStyle(Qt::SolidLine);
        break;
    case LineTool:
        if (buttonHeld != Qt::NoButton) addLine(_rect.x(), _rect.y(), _rect.x() + _rect.width(), _rect.y() + _rect.height(), pen)->setZValue(1);
        else if (convertedRect != QRect()) _frame->drawLine(QPoint(convertedRect.x(), convertedRect.y()),
                             QPoint(convertedRect.x() + convertedRect.width(), convertedRect.y() + convertedRect.height()),
                             primaryColor);
        break;
    default:
        break;
    }
    addPixmap(QPixmap::fromImage(_frame->pixels().scaled(sceneRect().width(), sceneRect().height())));

    emit frameUpdated(_frame);
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;

    _rect = QRectF(_rect.x(), _rect.y(), mouseEvent->scenePos().x() - _rect.x(), mouseEvent->scenePos().y() - _rect.y());
    draw(mouseEvent->scenePos());

//    qDebug() << mouseEvent->scenePos();
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;

    buttonHeld = mouseEvent->button();
    _rect = QRectF(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), 0, 0);
    draw(mouseEvent->scenePos());
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;

    buttonHeld = Qt::NoButton;
    if (_tool == RectangleTool || _tool == EllipseTool || _tool == LineTool) refresh();
    emit pixelsModified(_frame->pixels());

//    qDebug() << mouseEvent->scenePos();
}
