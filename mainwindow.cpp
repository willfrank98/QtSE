#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>

MainWindow::MainWindow(Model &model, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    // connects the File>New actions
    connect(ui->action8x8, &QAction::triggered, this, [this](){ createCanvas(8); });
    connect(ui->action16x16, &QAction::triggered, this, [this](){ createCanvas(16); });
    connect(ui->action32x32, &QAction::triggered, this, [this](){ createCanvas(32); });
    connect(ui->action64x64, &QAction::triggered, this, [this](){ createCanvas(64); });
    connect(ui->action128x128, &QAction::triggered, this, [this](){ createCanvas(128); });

    // connects the File>Exit action
    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createCanvas(int size)
{
    Canvas *canvas = new Canvas(size, ui->graphicsViewCanvas->width()/(qreal)size);
    ui->graphicsViewCanvas->setScene(canvas);
    ui->graphicsViewCanvas->setEnabled(true);
}

// Adds a background to the canvas at Z-layer -1 (background)
void MainWindow::addBackground(QGraphicsScene &scene) {
    // TODO
}

void MainWindow::addSurface(QGraphicsScene &scene) {
    QBrush *brush = new QBrush(QColor::fromRgbF(0.0, 0.0, 0.0, 1.0));

    for (qreal y = 0; y < canvasSize; y++) {
        for (qreal x = 0; x < canvasSize; x++) {

        }
    }
}

// Adds a grid to the canvas at Z-layer 1 (foreground)
// Interestingly, the canvas can pan very slightly regardless of size.
void MainWindow::addGrid(QGraphicsScene &scene) {
    QPen *pen = new QPen(QColor::fromRgbF(0.2, 0.2, 0.2, 0.75));
    pen->setWidthF(0.25);

    for (qreal y = 0; y < canvasSize; y++) {
        for (qreal x = 0; x < canvasSize; x++) {
            scene.addRect(pixelSize * x, pixelSize * y, pixelSize, pixelSize, *pen);
        }
    }
    scene.createItemGroup(scene.items())->setZValue(1);
}

