#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include "canvas.h"
#include <QColorDialog>
#include <QToolButton>
#include <QButtonGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void resetCanvas();

public:
    explicit MainWindow(Model& model, QWidget *parent = 0);
    ~MainWindow();

private:
    Model *model;
    Canvas *canvas = new Canvas();
    Ui::MainWindow *ui;
    int canvasSize;
    qreal pixelSize;
    QList<QToolButton*> paletteButtons;
    QVector<QColor> paletteHistory;
    int paletteHistoryIndex = 0;
    QButtonGroup frameButtons;

    QString colorToString(QColor color);
    void updatePaletteHistory();
    void newFrame(int index);
    void newCanvas(int dimension);
};

#endif // MAINWINDOW_H
