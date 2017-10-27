#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>

MainWindow::MainWindow(Model &model, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);

    // connects the File>New actions
    connect(ui->action8x8, &QAction::triggered, this, [this](){ createCanvas(8); });
    connect(ui->action16x16, &QAction::triggered, this, [this](){ createCanvas(16); });
    connect(ui->action32x32, &QAction::triggered, this, [this](){ createCanvas(32); });
    connect(ui->action64x64, &QAction::triggered, this, [this](){ createCanvas(64); });
    connect(ui->action128x128, &QAction::triggered, this, [this](){ createCanvas(128); });
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createCanvas(int size)
{
    int width = ui->graphicsViewCanvas->width()/(qreal)size;
    int height = ui->graphicsViewCanvas->height()/(qreal)size;
    QGraphicsScene* gScene = new QGraphicsScene();
    QPen* pen = new QPen(QColor::fromRgbF(0.0, 0.0, 0.0, 0.5));
    pen->setWidthF(0.25);

    for (qreal y = 0; y < size; y++) {
        for (qreal x = 0; x < size; x++) {
            // the canvas gains a margin at sizes 64x64 and 128x128 for some reason
            // TODO: fix the issue mentioned above or just remove those sizes (not really the best solution)
            QGraphicsRectItem* cell = new QGraphicsRectItem(width * x, height * y, width, height);
            cell->setZValue(1);
            cell->setPen(*pen);
//            gScene->addRect(cell->boundingRect(), *pen);
            gScene->addItem(cell);
        }
    }

    ui->graphicsViewCanvas->setScene(gScene);
    ui->graphicsViewCanvas->setEnabled(true);
}

