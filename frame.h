#ifndef FRAME_H
#define FRAME_H
#include "canvas.h"
#include <QImage>
#include <QPainter>
#include <QVector>
#include <QGraphicsView>
class Frame : public QGraphicsView
{
Q_OBJECT
signals:
    void updateGV(int framenum);

private:
    QImage image;
    QPainter painter;

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent, int framenum);

public:
    Frame(int sizex,int sizey);
    void drawEllipse(int x1, int y1, int x2, int y2);
    void drawMirrorPen(QVector<int> pixels);
    void drawPen(QVector<int> pixels);
    void drawRectangle(int x1, int y1, int x2, int y2);
    void erase(QVector<int> pixels);
    void bucketFill(int x, int y);
    void drawDither(QVector<int> pixels);

};

#endif // FRAME_H
