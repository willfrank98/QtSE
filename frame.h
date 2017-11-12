/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * frame.h
 *
 *
 */

#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <QPainter>
#include <QStack>
#include <QMap>

class Frame
{
private:

    QPainter *_painter;
    QStack<QPoint> _pixelStack;
    QStack<QImage> _undoStack;
    QStack<QImage> _redoStack;

public:
    QImage _image;
    QImage _tempImage;
    Frame();
    Frame(Frame& f);
    Frame(int dimension);
    ~Frame();
    void drawEllipse(QRect area, QColor line, QColor fill);
    void drawMirrorPen(QPoint point, QColor color);
    void drawPen(QPoint point, QColor color);
    void drawRectangle(QRect area, QColor line, QColor fill);
    void setupDraw(QRect area, QColor line, QColor fill);
    void drawLine(QPoint start, QPoint end, QColor color);
    void erase(QPoint point);
    void bucketFill(QPoint startPoint, QColor initialColor, QColor replacementColor);
    void colorSwap(QPoint startPoint, QColor color);
    void drawDither(QPoint point, QColor color1, QColor color2);
    void selectRegion(QRectF);
    void undo();
    void redo();
    void updateUndoRedo(QImage newImage);
    void setPixels(QImage newImage);
    QImage pixels();
    QSize size();
};

#endif // FRAME_H
