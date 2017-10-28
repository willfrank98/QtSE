#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(Model& model, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int canvasSize;
    qreal pixelSize;

    void addBackground(QGraphicsScene&);
    void addSurface(QGraphicsScene&);
    void addGrid(QGraphicsScene&);
    void createCanvas(int);
};

#endif // MAINWINDOW_H
