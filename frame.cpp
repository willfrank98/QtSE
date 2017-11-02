#include "frame.h"
#include <QDebug>

Frame::Frame(int dimension) {
    image = QImage(dimension, dimension, QImage::Format_ARGB32);
    image.fill(QColor(0, 0, 0, 0));  // gets rid of weird artifacts that appear on creation
    painter.begin(&image);
}

void Frame::drawEllipse(int x1, int y1, int x2, int y2) {
    int width = abs(x1 - x2);
    int height = abs(y1 - y2);

    int x, y;
    if(x1 < x2) {
        x = x1;
    }
    else {
        x = x2;
    }
    if (y1 < y2) {
        y = y1;
    }
    else {
        y = y2;
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

void Frame::drawRectangle(int x1, int y1, int x2, int y2) {
    int width = abs(x1 - x2);
    int height = abs(y1 - y2);

    int x, y;
    if(x1 < x2) {
        x = x1;
    }
    else {
        x = x2;
    }
    if (y1 < y2) {
        y = y1;
    }
    else {
        y = y2;
    }

    painter.drawRect(x, y, width, height);
}

void Frame::erase(QVector<int> pixels) {

}

void Frame::bucketFill(int x, int y) {

}

void Frame::drawDither(QVector<int> pixels) {
    for (int i = 0; i < pixels.size(); i + 2) {
        if((pixels[i] + pixels[i+1]) % 2 != 0) {
            painter.drawPoint(pixels[i], pixels[i+1]);
        }
    }
}

QImage Frame::pixels() {
    return image;
}

QSize Frame::size() {
    return image.size();
}
