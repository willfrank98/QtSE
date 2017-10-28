#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include <QColorDialog>
#include <QPalette>
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void colorBox1Clicked();
    void colorBox2Clicked();
    void on_penTool_clicked();
    void on_eraseButton_clicked();

public:
    explicit MainWindow(Model& model, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int canvasSize;
    qreal pixelSize;
    QColorDialog *cd ;
    void addBackground(QGraphicsScene&);
    void addSurface(QGraphicsScene&);
    void addGrid(QGraphicsScene&);
    void createCanvas(int);
};

#endif // MAINWINDOW_H
