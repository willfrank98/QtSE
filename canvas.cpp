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
    _primaryColor = color;
}

void Canvas::setSecondaryColor(QColor color) {
    _secondaryColor = color;
}

void Canvas::swapColors() {
    QColor temp = _secondaryColor;
    _secondaryColor = _primaryColor;
    _primaryColor = temp;
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
    if (_buttonHeld != Qt::NoButton) {
        QPoint convertedPoint = QPoint(point.x() / _pixSize.width(), point.y() / _pixSize.height());
        QColor color = (_buttonHeld == Qt::LeftButton) ? _primaryColor : _secondaryColor;
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
            if (convertedPoint.x() % 2 == 0 && convertedPoint.y() % 2 == 0) _frame->drawPen(convertedPoint, _primaryColor);
            else if (convertedPoint.x() % 2 == 1 && convertedPoint.y() % 2 == 1) _frame->drawPen(convertedPoint, _primaryColor);
            else if (convertedPoint.x() % 2 == 1 && convertedPoint.y() % 2 == 0) _frame->drawPen(convertedPoint, _secondaryColor);
            else if (convertedPoint.x() % 2 == 0 && convertedPoint.y() % 2 == 1) _frame->drawPen(convertedPoint, _secondaryColor);
            break;
        case BucketFillTool:
            if (_buttonHeld == Qt::LeftButton) _frame->bucketFill(convertedPoint,_frame->pixels().pixelColor(convertedPoint) , _primaryColor);
            if (_buttonHeld == Qt::RightButton) _frame->bucketFill(convertedPoint,_frame->pixels().pixelColor(convertedPoint)  , _secondaryColor);
            break;
        case ColorFillTool:
            if (_buttonHeld == Qt::LeftButton) _frame->colorSwap(convertedPoint, _primaryColor);
            if (_buttonHeld == Qt::RightButton) _frame->colorSwap(convertedPoint, _secondaryColor);
            break;
        default:
            break;
        }
        refresh();
    }
}

void Canvas::refresh() {
    _pixSize = QSizeF(sceneRect().width() / (qreal)_frame->size().width(),
                     sceneRect().height() / (qreal)_frame->size().width());
    QRect convertedRect = QRect(_rect.x() / _pixSize.width(), _rect.y() / _pixSize.height(),
                                _rect.size().width() / _pixSize.width(), _rect.size().height() / _pixSize.height());
    clear();
    QColor color = (_lastButton == 1) ? _primaryColor : _secondaryColor;
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(_pixSize.width());
    pen.setColor(color);
    switch (_tool) {
    case RectangleTool:
        if (_buttonHeld != Qt::NoButton)
            addRect(_rect, pen)->setZValue(1);
        else _frame->drawRectangle(convertedRect, color, QColor(0, 0, 0, 0));
        break;
    case EllipseTool:
        if (_buttonHeld != Qt::NoButton) addEllipse(_rect, pen)->setZValue(1);
        else _frame->drawEllipse(convertedRect, color, QColor(0, 0, 0, 0));
        break;
    case RectSelectTool:
        if (_buttonHeld != Qt::NoButton)_frame->selectRegion(convertedRect);
        break;
    case LineTool:
        if (_buttonHeld != Qt::NoButton) addLine(_rect.x(), _rect.y(), _rect.x() + _rect.width(), _rect.y() + _rect.height(), pen)->setZValue(1);
        else if (convertedRect != QRect()) _frame->drawLine(QPoint(convertedRect.x(), convertedRect.y()),
                             QPoint(convertedRect.x() + convertedRect.width(), convertedRect.y() + convertedRect.height()),
                             _primaryColor);
        break;
    default:
        break;
    }
    _lastButton = _buttonHeld;
    addPixmap(QPixmap::fromImage(_frame->pixels().scaled(sceneRect().width(), sceneRect().height())));
    emit frameUpdated(_frame);
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;
    int currentX = mouseEvent->scenePos().rx()/_pixSize.width();
    int currentY = mouseEvent->scenePos().ry()/_pixSize.height();
    if((_lastX != currentX) | (_lastY != currentY)){
        _rect = QRectF(_rect.x(), _rect.y(), mouseEvent->scenePos().x() - _rect.x(), mouseEvent->scenePos().y() - _rect.y());
        draw(mouseEvent->scenePos());
        _lastX = currentX;
        _lastY = currentY;
    }
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;

    _buttonHeld = mouseEvent->button();
    _rect = QRectF(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), 0, 0);
    draw(mouseEvent->scenePos());
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;

    _buttonHeld = Qt::NoButton;
    if (_tool == RectangleTool || _tool == EllipseTool || _tool == LineTool) refresh();
    emit pixelsModified(_frame->pixels());

//    qDebug() << mouseEvent->scenePos();
}
