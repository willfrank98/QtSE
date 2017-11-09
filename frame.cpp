/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * frame.cpp
 *
 *
 */

#include "frame.h"
#include <QDebug>
#include <QBitmap>
#include <QStack>

Frame::Frame() {

}

Frame::Frame(Frame& f) {
    this->_image = f._image;
    this->_undoStack = f._undoStack;
    this->_redoStack = f._redoStack;
    _painter = new QPainter(&_image);
}

Frame::Frame(int dimension) {
    _image = QImage(dimension, dimension, QImage::Format_ARGB32);
    _image.fill(QColor(0, 0, 0, 0));  // gets rid of weird artifacts that appear on creation
    _undoStack.push(_image);
    _painter = new QPainter(&_image);
    _painter->setBackgroundMode(Qt::TransparentMode);
}

Frame::~Frame() {
    _painter->end();
    _undoStack.clear();
    _redoStack.clear();
    _pixelStack.clear();
}

void Frame::drawEllipse(QRect area, QColor line, QColor fill) {
    _painter->setPen(line);
    _painter->setBrush(fill);

    _painter->drawEllipse(area);
}

void Frame::drawMirrorPen(QPoint point, QColor color) {
    _painter->setPen(color);
    _painter->setBrush(QColor(0, 0, 0, 0));

    _painter->drawPoint(point);
    _painter->drawPoint(-point.x() + _image.size().width() - 1, point.y());
}

void Frame::drawPen(QPoint point, QColor color) {
    _painter->setPen(color);
    _painter->setBrush(QColor(0, 0, 0, 0));

    _painter->drawPoint(point);
}

void Frame::drawRectangle(QRect area, QColor line, QColor fill) {
    _painter->setPen(line);
    _painter->setBrush(fill);

    _painter->drawRect(area);
}

void Frame::drawLine(QPoint start, QPoint end, QColor color) {
    _painter->setPen(color);
    _painter->setBrush(QColor(0, 0, 0, 0));

    _painter->drawLine(start, end);
}

void Frame::erase(QPoint point) {
    _image.setPixelColor(point, QColor(0, 0, 0, 0));
}

void Frame::bucketFill(QPoint startPoint, QColor initialColor, QColor replacementColor) {
    // Fill behavior is undefined if you click the same color as you want to fill so we do nothing.
    if(initialColor == replacementColor){
        return;
    }
    if(_image.pixelColor(startPoint) != initialColor){
        return;
    }
    _image.setPixelColor(startPoint, replacementColor);
    //We recursively look in cardinal directions (up, down, left, right).
    if(startPoint.y()+1 < size().height()){
        bucketFill(QPoint(startPoint.x(), startPoint.y()+1), initialColor, replacementColor);
    }
    if(startPoint.y()-1 >= 0){
        bucketFill(QPoint(startPoint.x(), startPoint.y()-1), initialColor, replacementColor);
    }
    if(startPoint.x()-1 >= 0){
        bucketFill(QPoint(startPoint.x()-1, startPoint.y()), initialColor, replacementColor);
    }
    if(startPoint.x()+1 < size().width()){
        bucketFill(QPoint(startPoint.x()+1, startPoint.y()), initialColor, replacementColor);
    }
}

void Frame::drawDither(QPoint point, QColor color1, QColor color2) {
    // TODO
}

// This is the color-fill tool.  We might want to rename it to better reflect that.
void Frame::colorSwap(QPoint startPoint, QColor color) {
    QColor oldColor = _image.pixelColor(startPoint);

    // the below link really helped here
    // https://forum.qt.io/topic/32039/pixmap-mask-not-coloring-over-the-image/2
    QBitmap mask = QPixmap::fromImage(_image).createMaskFromColor(oldColor.rgb(), Qt::MaskOutColor);
    _painter->setPen(color);
    _painter->drawPixmap(_image.rect(), mask, mask.rect());
}

void Frame::setPixels(QImage newImage) {
    _painter->end();
    _image = newImage;
    _painter->begin(&_image);
}

QImage Frame::pixels() {
    return _image;
}

QSize Frame::size() {
    return _image.size();
}
