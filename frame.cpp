#include "frame.h"
#include <QDebug>
#include <QBitmap>
#include <QStack>

Frame::Frame() {

}

Frame::Frame(Frame& f) {
    this->image = f.image;
    this->undoStack = f.undoStack;
    this->redoStack = f.redoStack;
    painter = new QPainter(&image);
}

Frame::Frame(int dimension) {
    image = QImage(dimension, dimension, QImage::Format_ARGB32);
    image.fill(QColor(0, 0, 0, 0));  // gets rid of weird artifacts that appear on creation
    undoStack.push(image);
    painter = new QPainter(&image);
    painter->setBackgroundMode(Qt::TransparentMode);
}

Frame::~Frame() {
    painter->end();
    undoStack.clear();
    redoStack.clear();
    pixelStack.clear();
}

void Frame::drawEllipse(QRect area, QColor line, QColor fill) {
    painter->setPen(line);
    painter->setBrush(fill);

    painter->drawEllipse(area);
}

void Frame::drawMirrorPen(QPoint point, QColor color) {
    // TODO
}

void Frame::drawPen(QPoint point, QColor color) {
    painter->setPen(color);
    painter->setBrush(QColor(0, 0, 0, 0));

    painter->drawPoint(point);
}

void Frame::drawRectangle(QRect area, QColor line, QColor fill) {
    painter->setPen(line);
    painter->setBrush(fill);

    painter->drawRect(area);
}

void Frame::drawLine(QPoint start, QPoint end, QColor color) {
    painter->setPen(color);
    painter->setBrush(QColor(0, 0, 0, 0));

    painter->drawLine(start, end);
}

void Frame::erase(QPoint point) {
    image.setPixelColor(point, QColor(0, 0, 0, 0));
}

void Frame::bucketFill(QPoint startPoint, QColor initialColor, QColor replacementColor) {
    // Fill behavior is undefined if you click the same color as you want to fill so we do nothing.
    if(initialColor == replacementColor){
        return;
    }
    if(image.pixelColor(startPoint) != initialColor){
        return;
    }
    image.setPixelColor(startPoint, replacementColor);
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
    QColor oldColor = image.pixelColor(startPoint);

    // the below link really helped here
    // https://forum.qt.io/topic/32039/pixmap-mask-not-coloring-over-the-image/2
    QBitmap mask = QPixmap::fromImage(image).createMaskFromColor(oldColor.rgb(), Qt::MaskOutColor);
    painter->setPen(color);
    painter->drawPixmap(image.rect(), mask, mask.rect());
}

void Frame::setPixels(QImage newImage) {
    painter->end();
    image = newImage;
    painter->begin(&image);
}

QImage Frame::pixels() {
    return image;
}

QSize Frame::size() {
    return image.size();
}
