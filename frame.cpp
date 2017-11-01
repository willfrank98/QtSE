#include "frame.h"

frame::frame(int dimension) {
    image = QImage(dimension, dimension, QImage::Format_ARGB32);
}

void frame::drawEllipse(int x1, int y1, int x2, int y2) {
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

void frame::drawMirrorPen(QVector<int> pixels) {

}

void frame::drawPen(QVector<int> pixels) {
    for (int i = 0; i < pixels.size(); i + 2) {
        painter.drawPoint(pixels[i], pixels[i+1]);
    }
}

void frame::drawRectangle(int x1, int y1, int x2, int y2) {
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

void frame::erase(QVector<int> pixels) {

}

void frame::bucketFill(int x, int y) {

}

void frame::drawDither(QVector<int> pixels) {
    for (int i = 0; i < pixels.size(); i + 2) {
        if((pixels[i] + pixels[i+1]) % 2 != 0) {
            painter.drawPoint(pixels[i], pixels[i+1]);
        }
    }
}
