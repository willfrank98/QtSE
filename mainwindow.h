/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * mainwindow.h
 *
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "model.h"
#include "canvas.h"
#include <QColorDialog>
#include <QToolButton>
#include <QButtonGroup>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void resetCanvas();
    void checkSave();

public slots:
    void saveDialog();

public:
    explicit MainWindow(Model& _model, QWidget *parent = 0);
    ~MainWindow();


private:
    Model *_model;
    Canvas *_canvas = new Canvas();
    Ui::MainWindow *_ui;
    int _canvasSize;
    qreal _pixelSize;
    QList<QToolButton*> _paletteButtons;
    QVector<QColor> _paletteHistory;
    int _paletteHistoryIndex = 0;
    QButtonGroup _frameButtons;

    QString colorToString(QColor color);
    void updatePaletteHistory();
    void newFrame(int index);
    void newCanvas(int dimension);
};

#endif // MAINWINDOW_H
