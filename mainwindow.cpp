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
