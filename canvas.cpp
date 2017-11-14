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
    _isRectSelected = false;

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

    _lastTool = _tool;
    if (_lastTool == RectSelectTool){
        _isRectSelected = false;
        _frame->setupDraw(Qt::transparent, Qt::transparent, _frame->_tempImage);
    }
    _tool = tool;
     qDebug()<<tool<<" "<<_lastTool;
}

void Canvas::setFrame(Frame *frame) {
    this->_frame = frame;
    _rect = QRect();
    refresh();
}

void Canvas::draw(QPointF point) {
    if (_buttonHeld != Qt::NoButton) {
        _convertedPoint = QPoint(point.x() / _pixSize.width(), point.y() / _pixSize.height());
        QColor color = (_buttonHeld == Qt::LeftButton) ? _primaryColor : _secondaryColor;
        switch (_tool) {
        case PenTool:
            _frame->drawPen(_convertedPoint, color);
            break;
        case MirrorPenTool:
            _frame->drawMirrorPen(_convertedPoint, color);
            break;
        case EraserTool:
            _frame->erase(_convertedPoint);
            break;
        case DitheringTool:
            if (_convertedPoint.x() % 2 == 0 && _convertedPoint.y() % 2 == 0) _frame->drawPen(_convertedPoint, _primaryColor);
            else if (_convertedPoint.x() % 2 == 1 && _convertedPoint.y() % 2 == 1) _frame->drawPen(_convertedPoint, _primaryColor);
            else if (_convertedPoint.x() % 2 == 1 && _convertedPoint.y() % 2 == 0) _frame->drawPen(_convertedPoint, _secondaryColor);
            else if (_convertedPoint.x() % 2 == 0 && _convertedPoint.y() % 2 == 1) _frame->drawPen(_convertedPoint, _secondaryColor);
            break;
        case BucketFillTool:
            if (_buttonHeld == Qt::LeftButton) _frame->bucketFill(_convertedPoint,_frame->pixels().pixelColor(_convertedPoint) , _primaryColor);
            if (_buttonHeld == Qt::RightButton) _frame->bucketFill(_convertedPoint,_frame->pixels().pixelColor(_convertedPoint)  , _secondaryColor);
            break;
        case ColorFillTool:
            if (_buttonHeld == Qt::LeftButton) _frame->colorSwap(_convertedPoint, _primaryColor);
            if (_buttonHeld == Qt::RightButton) _frame->colorSwap(_convertedPoint, _secondaryColor);
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
    _convertedRect = QRect(_rect.x() / _pixSize.width(), _rect.y() / _pixSize.height(),
                                _rect.size().width() / _pixSize.width(), _rect.size().height() / _pixSize.height());
    clear();
    QColor color = (_lastButton == 1) ? _primaryColor : _secondaryColor;
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(_pixSize.width());
    pen.setColor(color);
    switch (_tool) {
    case RectangleTool:
        _frame->drawRectangle(_convertedRect, color, QColor(0, 0, 0, 0));
        break;
    case EllipseTool:
        _frame->drawEllipse(_convertedRect, color, QColor(0, 0, 0, 0));
        break;
    case RectSelectTool:
        if (!_isRectSelected){
            _prevRect = _convertedRect;
            _frame->selectRegion(_convertedRect, color, QColor(0, 40, 50, 50));
        }
        else {
            _convertedRect = QRect(_prevRect.x() + _convertedPoint.x() -_convertedRect.x(),
                                   _prevRect.y() + _convertedPoint.y() -_convertedRect.y(),
                                   _prevRect.width(), _prevRect.height());
            _frame->selectRegion(_convertedRect, color, QColor(0, 40, 50, 50));
        }
        break;
    case LineTool:
        if (_convertedRect != QRect()) _frame->drawLine(QPoint(_convertedRect.x(), _convertedRect.y()),
                             QPoint(_convertedRect.x() + _convertedRect.width(), _convertedRect.y() + _convertedRect.height()),
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
    if((_lastX != currentX) | (_lastY != currentY))
    {
        _rect = QRectF(_rect.x(), _rect.y(), mouseEvent->scenePos().x() - _rect.x(), mouseEvent->scenePos().y() - _rect.y());
        draw(mouseEvent->scenePos());
        _lastX = currentX;
        _lastY = currentY;
    }
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;
    if (!_isRectSelected)
        _frame->_tempImage = _frame->_image;
    _rect = QRectF(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), 0, 0);
    if (_tool == RectSelectTool){
         QPoint _point(_rect.x() / _pixSize.width(), _rect.y() / _pixSize.height());
        int top, bottom, left, right;
        if (_prevRect.top()> _prevRect.bottom()) {
            top = _prevRect.top();
            bottom = _prevRect.bottom();
        }
        else{
            top = _prevRect.bottom();
            bottom = _prevRect.top();
        }
        if (_prevRect.left() < _prevRect.right()) {
            left = _prevRect.left();
            right = _prevRect.right();
        }
        else {
            left = _prevRect.right();
            right = _prevRect.left();
        }
        if ((_point.x() >= left) && (_point.x() <= right)
                && (_point.y() <= top ) && (_point.y() >= bottom)){
            _isRectSelected = true;
        }
        else {
           _isRectSelected = false;
        }
    }
    else{
        //QRect _prevRect;
    }
    emit updateUndo(_frame->pixels());
    _buttonHeld = mouseEvent->button();
    draw(mouseEvent->scenePos());
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
  // if (event->matches(QKeySequence::Cut))
       //qDebug()<<"cut"<<endl;
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;

    _buttonHeld = Qt::NoButton;
    if (_tool == RectangleTool || _tool == EllipseTool || _tool == LineTool) {
        refresh();
        _frame->_tempImage = _frame->_image;
    }
    if (_tool == RectSelectTool){
        _prevRect = _convertedRect;
        _isRectSelected = true;
    }
    emit pixelsModified(_frame->pixels());
}

//Used for loading a file
//void Canvas::drawSlot(QPoint point, QColor color)
//{
//	_frame->drawPen(point, color);
//}

//Used for loading a file
//void Canvas::refreshSlot()
//{
//    refresh();
//}
