#ifndef CANVAS_H
#define CANVAS_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QStack>

class Canvas : public QGraphicsScene
{
    Q_OBJECT
signals:
    void modifiedPixels(QVector<QPoint> pixels);

public slots:
    void setPrimaryColor(QColor);
    void setSecondaryColor(QColor);
    void displayImage(QImage);

private:
    QColor primaryColor = QColor(0, 0, 0, 255);
    QColor secondaryColor = QColor(255, 255, 255, 255);
    QVector<QPoint> points = QVector<QPoint>();
    bool buttonHeld = false;

    // TODO: move these to model
//    QStack<QImage> undoStack;
//    QStack<QImage> redoStack;

public:
    explicit Canvas(QObject *parent = nullptr);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

    // TODO: move these to model
//    void undo();
//    void redo();
};

#endif // CANVAS_H
