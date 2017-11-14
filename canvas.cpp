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
#include <QLabel>

Canvas::Canvas(QObject *parent) : QGraphicsScene(parent)
{
    _tool = PenTool;
    _isRectSelected = false;
    _isCut = false;
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
        QRect _prevRect;
        _frame->setupDraw(Qt::transparent, Qt::transparent, _frame->_tempImage, _frame->_tempImage.rect());
    }
    _tool = tool;
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
        if (_convertedRect != QRect()) {
            _frame->drawRectangle(_convertedRect, color, QColor(0, 0, 0, 0));
        }
        break;
    case EllipseTool:
        if (_convertedRect != QRect()) {
            _frame->drawEllipse(_convertedRect, color, QColor(0, 0, 0, 0));
        }
        break;
    case RectSelectTool:
<<<<<<< HEAD
        if (_isCut){
            _frame->_tempImage = _frame->_image;
            _isCut = false;
            break;
        }
        if (!_isRectSelected){
            _convertedRect = _convertedRect.normalized();
=======
        if (!_isRectSelected) {
>>>>>>> e39cbd8dfe78a13e2fbbdb78f1fd36c982a6a941
            _prevRect = _convertedRect;
            _frame->selectRegion(_convertedRect, QColor(0, 40, 50, 50), QColor(0, 40, 50, 50));
        }
        else
        {
            _convertedRect = QRect(_prevRect.x() + _convertedPoint.x() -_convertedRect.x(),
                                   _prevRect.y() + _convertedPoint.y() -_convertedRect.y(),
                                   _prevRect.width(), _prevRect.height());
            _frame->selectRegion(_convertedRect, QColor(0, 40, 50, 50), QColor(0, 40, 50, 50));
        }
        break;
    case LineTool:
        if (_convertedRect != QRect()) {
            _frame->drawLine(QPoint(_convertedRect.x(), _convertedRect.y()),
                             QPoint(_convertedRect.x() + _convertedRect.width(), _convertedRect.y() + _convertedRect.height()),
                             _primaryColor);
        }
        break;
    default:
        break;
    }
    _lastButton = _buttonHeld;
    addPixmap(QPixmap::fromImage(_frame->_image.scaled(sceneRect().width(), sceneRect().height())));
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

void Canvas::normalizeRectSides(QRect r){
    if (_prevRect.top() > r.bottom()) {
        _lastTop = r.top();
        _lastBottom = r.bottom();
    }
    else{
        _lastTop = r.bottom();
        _lastBottom = r.top();
    }
    if (_prevRect.left() < r.right()) {
        _lastLeft = r.left();
        _lastRight = r.right();
    }
    else {
        _lastLeft = r.right();
        _lastRight = r.left();
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
         normalizeRectSides(_prevRect);
        if ((_point.x() >= _lastLeft) && (_point.x() <= _lastRight)
                && (_point.y() <= _lastTop ) && (_point.y() >= _lastBottom)){
            _isRectSelected = true;
        }
        else {
           _isRectSelected = false;
        }
    }
    else{
        //QRect _prevRect;
    }
    _buttonHeld = mouseEvent->button();
    draw(mouseEvent->scenePos());
}


void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (_tool == RectSelectTool && _isRectSelected){
         //qDebug()<<_convertedRect.topLeft()<<" "<<_convertedRect.topRight()<<" "<<_convertedRect.bottomLeft()<<" "<<_convertedRect.bottomRight()<<endl;
        if (event->matches(QKeySequence::Cut)){
            _isCut = true;
             _frame->setupDraw(Qt::transparent, Qt::transparent, _frame->_tempImage, _frame->_tempImage.rect());
            _convertedRect = _convertedRect.normalized();
            int top,bot, left,right;
            if (_prevRect.top() < _prevRect.bottom()) {
                top = _convertedRect.top();
                bot = _convertedRect.bottom();
            }
            else{
                top = _convertedRect.bottom();
                bot = _convertedRect.top();
            }
            if (_prevRect.left() < _prevRect.right()) {
                left = _convertedRect.left();
                right = _convertedRect.right();
            }
            else {
                left = _convertedRect.right();
                right = _convertedRect.left();
            }
            if (top<0) top = 0;
            if (bot>_frame->_dimension) bot = _frame->_dimension;
            if (left < 0) left = 0;
            if (right > _frame->_dimension) right = _frame->_dimension;
            for(int i = top; i <= bot; i++)
                for (int j = left; j<= right; j++){
                    _frame->erase(QPoint(j,i));
                }
            refresh();

        }
    }
}
void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!_mouseEnabled) return;

    _buttonHeld = Qt::NoButton;
    if (_tool == RectangleTool || _tool == EllipseTool || _tool == LineTool) refresh();
    if (_tool == RectSelectTool){
        _prevRect = _convertedRect;
        _isRectSelected = true;
    }
    emit pixelsModified(_frame->pixels());
}
