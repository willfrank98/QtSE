#include "frame.h"
#include <QDebug>

Frame::Frame(){


}

Frame::Frame(int dimension) {
    image = QImage(dimension, dimension, QImage::Format_ARGB32);
    image.fill(QColor(0, 0, 0, 0));  // gets rid of weird artifacts that appear on creation
    painter.begin(&image);
}

void Frame::drawEllipse(QVector<int> pixels) {
    int width = abs(pixels[0] - pixels[2]);
    int height = abs(pixels[1] - pixels[3]);

    int x, y;
    if(pixels[0] < pixels[2]) {
        x = pixels[0];
    }
    else {
        x = pixels[2];
    }
    if (pixels[1] < pixels[3]) {
        y = pixels[1];
    }
    else {
        y = pixels[3];
    }

    painter.drawEllipse(x, y, width, height);
}

void Frame::drawMirrorPen(QVector<int> pixels) {

}

void Frame::drawPen(QVector<QPoint> pixels, QColor color) {
    painter.setPen(color);
    for (QPoint point : pixels) {
        painter.drawPoint(point);
    }
}

void Frame::drawRectangle(QVector<int> pixels) {
    int width = abs(pixels[0] - pixels[2]);
    int height = abs(pixels[1] - pixels[3]);

    int x, y;
    if(pixels[0] < pixels[2]) {
        x = pixels[0];
    }
    else {
        x = pixels[2];
    }
    if (pixels[1] < pixels[3]) {
        y = pixels[1];
    }
    else {
        y = pixels[3];
    }

    painter.drawRect(x, y, width, height);
}

void Frame::erase(QVector<int> pixels) {

}

void Frame::bucketFill(QVector<int> pixels) {

}

void Frame::drawDither(QVector<int> pixels) {
    for (int i = 0; i < pixels.size(); i + 2) {
        if((pixels[i] + pixels[i+1]) % 2 != 0) {
            painter.drawPoint(pixels[i], pixels[i+1]);
        }
    }
}

void Frame::setPixels(QImage newImage){
    painter.end();
    image = newImage;
    painter.begin(&image);
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
