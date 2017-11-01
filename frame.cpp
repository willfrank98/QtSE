#include "frame.h"

frame::frame(int dimension) {
    image = QImage(dimension, dimension, QImage::Format_ARGB32);
}

void frame::drawEllipse(int x1, int y1, int x2, int y2) {

}

void frame::drawMirrorPen(QVector<int> pixels) {

}

void frame::drawPen(QVector<int> pixels) {

}

void frame::drawRectangle(int x1, int y1, int x2, int y2) {

}

void frame::erase(QVector<int> pixels) {

}

void frame::bucketFill(int x, int y) {

}

void frame::drawDither(QVector<int> pixels) {
    for (int i = 0; i < pixels.size()/2; i++) {
        painter.drawPoint(pixels[i], pixels[i+1]);
    }
}
