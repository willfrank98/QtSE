#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    void createCanvas(int);
};

#endif // MAINWINDOW_H
