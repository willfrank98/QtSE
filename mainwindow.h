#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include <QColorDialog>
#include <QPalette>
#include <QFrame>
#include "canvas.h"
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

	void saveAction();
	void loadAction();

signals:
	void createSaveFile(QString fileName);
	void loadSaveFile(QString fileName);

public:
    explicit MainWindow(Model& model, QWidget *parent = 0);
    ~MainWindow();

private:
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
    void createCanvas(int);
    void updateColorHistory();
};

#endif // MAINWINDOW_H
