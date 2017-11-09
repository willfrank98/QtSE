/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * canvas.h
 *
 *
 */

#ifndef CANVAS_H
#define CANVAS_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QStack>
#include <tool.h>
#include <frame.h>

class Canvas : public QGraphicsScene
{
    Q_OBJECT
signals:
    void frameUpdated(Frame *frame);
    void pixelsModified(QImage);

public slots:
    void setPrimaryColor(QColor color);
    void setSecondaryColor(QColor color);
    void swapColors();
    void setTool(Tool _tool);
    void setFrame(Frame *frame);

private:
    QColor primaryColor = QColor(0, 0, 0, 255);
    QColor secondaryColor = QColor(255, 255, 255, 255);
    QVector<QPoint> points = QVector<QPoint>();
    Qt::MouseButton buttonHeld = Qt::NoButton;
    QSizeF pixSize = QSizeF(32, 32);
    Frame *frame;
    Tool tool;
    QRectF rect;
    bool mouseEnabled = true;

    void draw(QPointF point);
    void refresh();

public:
    explicit Canvas(QObject *parent = nullptr);
    void setDisableMouse(bool val) { mouseEnabled = !val; }
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
};

#endif // CANVAS_H
