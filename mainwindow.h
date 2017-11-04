#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include "canvas.h"
#include <QColorDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void toolSelection();

public:
    explicit MainWindow(Model& model, QWidget *parent = 0);
    void addFramePreview(QList<Frame>);
    ~MainWindow();

private:
    Model *model;
    Canvas *canvas = new Canvas();
    Ui::MainWindow *ui;
    int canvasSize;
    qreal pixelSize;
    int frameNum;
};

#endif // MAINWINDOW_H
