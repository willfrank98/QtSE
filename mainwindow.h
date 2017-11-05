#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "model.h"
#include <QColorDialog>
#include <QPalette>
#include <QFrame>
#include <QGraphicsRectItem>
#include <QDebug>
#include <iostream>
#include <QVector>
#include <QFileDialog>
#include <QBuffer>
#include "canvas.h"
#include "form.h"
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void updateFocus();
private slots:
    void colorBox1Clicked();
    void colorBox2Clicked();
    void on_penTool_clicked();
    void on_eraseButton_clicked();
    void on_penToolButton_clicked();
    void on_eraserToolButton_clicked();
    void on_rectangleToolButton_clicked();
    void on_ellipseToolButton_clicked();
    void custom_clicked();
	void saveAction();
	void loadAction();
    void on_pushButtonAddFrame_clicked();

signals:


public:
    QVector <QGraphicsScene*> scenes;
    Canvas *canvas;
    explicit MainWindow(Model *model, QWidget *parent = 0);
    ~MainWindow();

private:

    QList <QImage> images;
    int imageHeight;
    int pixSize;
    int sizeX;
    int sizeY;
    Form popup;
    Model *model;
    Ui::MainWindow *ui;
    int canvasSize;
    qreal pixelSize;
    QColorDialog *cd ;
    QVector<QString> colorHistory;
    QVector<QFrame*> colorHistoryButtons;
    void addBackground(QGraphicsScene&);
    void addSurface(QGraphicsScene&);
    void addGrid(QGraphicsScene&);
    void createCanvas(int,int);
    void updateColorHistory();
    void createSaveFile(QString fileName);
    void loadSaveFile(QString fileName);
    void setPixSize(int sizex, int sizey, int gvsize);
    void addFramePreview(QImage image);
};

#endif // MAINWINDOW_H
