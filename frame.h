#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <QPainter>
#include <QVector>

class Frame
{
private:
    QImage image;
    //You cannot copy a Qpainter object. Therefore we cannot copy frames.
    QPainter painter;
public:
    Frame();
    Frame(int dimension);
    void drawEllipse(QVector<int> pixels);
    void drawMirrorPen(QVector<int> pixels);
    void drawPen(QVector<QPoint> pixels, QColor color);
    void drawRectangle(QVector<int> pixels);
    void erase(QVector<int> pixels);
    void bucketFill(QVector<int> pixels);
    void drawDither(QVector<int> pixels);
    void setPixels(QImage newImage);

    void storeUndoImage();
    void undo();
    void redo();
    QImage pixels();
    QSize size();
};

#endif // FRAME_H
