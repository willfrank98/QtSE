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
    void drawEllipse(QVector<int> pixels);
    void drawMirrorPen(QVector<int> pixels);
    void drawPen(QVector<QPoint> pixels, QColor color);
    void drawRectangle(QVector<int> pixels);
    void erase(QVector<int> pixels);
    void bucketFill(QVector<int> pixels);
    void drawDither(QVector<int> pixels);
    QImage pixels();
    QSize size();
};

#endif // FRAME_H
