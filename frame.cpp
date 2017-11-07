#include "frame.h"
#include <QDebug>

Frame::Frame(){

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

void Frame::bucketFill(QPoint startPoint, QColor fill) {

}

void Frame::drawDither(QPoint point, QColor color1, QColor color2) {

}

void Frame::colorSwap(QPoint startPoint, QColor color) {
    QColor oldColor = image.pixelColor(startPoint);

    // the below link really helped here
    // https://forum.qt.io/topic/32039/pixmap-mask-not-coloring-over-the-image/2
    QBitmap mask = QPixmap::fromImage(image).createMaskFromColor(oldColor.rgb(), Qt::MaskOutColor);
    painter->setPen(color);
    painter->drawPixmap(image.rect(), mask, mask.rect());
}

void Frame::undo() {
    if (!undoStack.isEmpty()) {
        painter->end();
        QImage temp = undoStack.pop();
        if (image == temp && !undoStack.isEmpty()) {
            redoStack.push(temp);
            temp = undoStack.pop();
        }
        else {
            image = temp;
            painter->begin(&image);
            return;
        }
        redoStack.push(temp);
        image = temp;
        painter->begin(&image);
    }
}

void Frame::redo() {
    if (!redoStack.isEmpty()) {
        painter->end();
        QImage temp = redoStack.pop();
        if (image == temp) {
            undoStack.push(temp);
            temp = redoStack.pop();
        }
        undoStack.push(temp);
        image = temp;
        painter->begin(&image);
    }
}

void Frame::storeUndoImage() {
    redoStack.clear();
    if (undoStack.size() == 10) undoStack.pop_back();
    undoStack.push(image);
}

QImage Frame::pixels() {
    return image;
}

QSize Frame::size() {
    return image.size();
}
