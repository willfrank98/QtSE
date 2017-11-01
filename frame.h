#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <QPainter>
#include <QVector>

class frame
{
private:
    QImage image;
    QPainter painter;
public:
    frame(int dimension);
    void drawEllipse(int x1, int y1, int x2, int y2);
    void drawMirrorPen(QVector<int> pixels);
    void drawPen(QVector<int> pixels);
    void drawRectangle(int x1, int y1, int x2, int y2);
    void erase(QVector<int> pixels);
    void bucketFill(int x, int y);
    void drawDither(QVector<int> pixels);
};

#endif // FRAME_H
