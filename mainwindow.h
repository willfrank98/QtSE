#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include "canvas.h"
#include <QColorDialog>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void toolSelection();
    void updateColor(QColor);

public:
    explicit MainWindow(Model& model, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void paletteClicked(int buttonID);
    void updateHistory(QColor);

private:
    Model *model;
    Canvas *canvas = new Canvas();
    Ui::MainWindow *ui;
    int canvasSize;
    qreal pixelSize;
    QVector<QColor> colorHistory;

    QString convertRgbToString(int red, int green, int blue);
};

#endif // MAINWINDOW_H
