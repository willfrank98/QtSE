#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include <QColorDialog>
#include <QPalette>
#include <QQueue>
#include <QFrame>
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
    QVector<QString> colorHistory;
    QVector<QFrame*> colorHistoryButtons;
    void addBackground(QGraphicsScene&);
    void addSurface(QGraphicsScene&);
    void addGrid(QGraphicsScene&);
    void createCanvas(int);
    void updateColorHistory();
};

#endif // MAINWINDOW_H
