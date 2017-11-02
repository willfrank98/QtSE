#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <QPainter>
#include <QVector>

class Frame
{
private:
    QImage image;
    QPainter painter;
public:
    Frame(int dimension);
    void drawEllipse(int x1, int y1, int x2, int y2);
    void drawMirrorPen(QVector<int> pixels);
    void drawPen(QVector<QPoint> pixels, QColor color);
    void drawRectangle(int x1, int y1, int x2, int y2);
    void erase(QVector<int> pixels);
    void bucketFill(int x, int y);
    void drawDither(QVector<int> pixels);
    QImage pixels();
    QSize size();
};

#endif // FRAME_H
