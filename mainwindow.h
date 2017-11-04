#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include <QColorDialog>
#include <QPalette>
#include <QFrame>
#include "canvas.h"
#include "form.h"
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void updateThis();
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

signals:


public:
    explicit MainWindow(Model *model, QWidget *parent = 0);
    ~MainWindow();

private:
    int imageHeight;
    int pixSize;
    Form popup;
    Model *model;
    Canvas *canvas;
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
    void setPixSize(int sizex, int sizey);
};

#endif // MAINWINDOW_H
