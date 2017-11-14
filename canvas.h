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
#include <QKeyEvent>
#include <tool.h>
#include <frame.h>

class Canvas : public QGraphicsScene
{
    Q_OBJECT
signals:
    void frameUpdated(Frame *_frame);
    void pixelsModified(QImage);
    void updateUndo(QImage);

public slots:
    void setPrimaryColor(QColor color);
    void setSecondaryColor(QColor color);
    void swapColors();
    void setTool(Tool tool);
    void setFrame(Frame *frame);
    //void drawSlot(QPoint point, QColor color);	//intended to be used for loading files. Attempting to be more like human drawing to see if that fixes things
    //void refreshSlot();

private:
    int _lastX;
    int _lastY;
    int _lastButton;
    QColor _primaryColor = QColor(0, 0, 0, 255);
    QColor _secondaryColor = QColor(255, 255, 255, 255);
    QVector<QPoint> _points = QVector<QPoint>();
    Qt::MouseButton _buttonHeld = Qt::NoButton;
    QSizeF _pixSize = QSizeF(32, 32);
    Frame *_frame;
    Tool _tool;
    QRectF _rect;
    QRectF _prevRect;
    QPoint _convertedPoint;
    QRect _convertedRect;
    bool _isRectSelected;
    bool _mouseEnabled = true;
    bool _blankCanvas = true;
    Tool _lastTool;
    void draw(QPointF point);
    void refresh();

public:
    explicit Canvas(QObject *parent = nullptr);
    void setDisableMouse(bool val) { _mouseEnabled = !val; }
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
};

#endif // CANVAS_H
