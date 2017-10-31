#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <iostream>
#include <QVector>
using namespace std;


MainWindow::MainWindow(Model &model, QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    cd = new QColorDialog();
    cd->setCurrentColor(*new QColor(Qt::black));


    //This is ugly i know but not sure how to make a better way to iterate through the buttons.
    colorHistoryButtons.append(ui->palette1);
    colorHistoryButtons.append(ui->palette2);
    colorHistoryButtons.append(ui->palette3);
    colorHistoryButtons.append(ui->palette4);
    colorHistoryButtons.append(ui->palette5);
    colorHistoryButtons.append(ui->palette6);
    colorHistoryButtons.append(ui->palette7);
    colorHistoryButtons.append(ui->palette8);
    colorHistoryButtons.append(ui->palette9);
    colorHistoryButtons.append(ui->palette10);
    colorHistoryButtons.append(ui->palette11);
    colorHistoryButtons.append(ui->palette12);
    colorHistoryButtons.append(ui->palette13);
    colorHistoryButtons.append(ui->palette14);

    //set the default colors
    Canvas::c1 = QColor(Qt::black);
    Canvas::c2 = QColor(Qt::white);
    // connects the buttons
    connect(ui->color1Box, &QAbstractButton::clicked, this, &MainWindow::colorBox1Clicked);
    connect(ui->color2Box, &QAbstractButton::clicked, this, &MainWindow::colorBox2Clicked);

    // connects the undo/redo buttons
    //connect(ui->actionUndo, &QAction::triggered, this, Canvas::undo());
    //connect(ui->actionRedo, &QAction::triggered, this, Canvas::redo());

    // connects the File>New actions
    connect(ui->action8x8, &QAction::triggered, this, [this](){ createCanvas(8); });
    connect(ui->action16x16, &QAction::triggered, this, [this](){ createCanvas(16); });
    connect(ui->action32x32, &QAction::triggered, this, [this](){ createCanvas(32); });
    connect(ui->action64x64, &QAction::triggered, this, [this](){ createCanvas(64); });
    connect(ui->action128x128, &QAction::triggered, this, [this](){ createCanvas(128); });
    // connects the File>Export actions
    connect(ui->actionAnimated_GIF, &QAction::triggered, &model, [=](){
        /*
         * This is just a lambda function to check if FreeImage fails for anyone.
         * Looks like Window will be difficult to get linked correctly.
         * Will fix Monday soon.
         */
//        FreeImage_Initialise();
//        qDebug() << "Initialized FreeImage " << FreeImage_GetVersion();
//        FreeImage_DeInitialise();
    });

    // connects the File>Exit action
    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);
    // connects the File>Exit action  
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::colorBox1Clicked(){
    Canvas::c1 = QColor(cd->getColor());
    QString temp = QString::fromStdString("background-color: "+Canvas::c1.name().toStdString()+";");
    colorHistory.append(temp);
    updateColorHistory();
    ui->color1Box->setStyleSheet(temp);
}
void MainWindow::colorBox2Clicked(){
    Canvas::c2 = QColor(cd->getColor());
    QString temp = QString::fromStdString("background-color: "+Canvas::c2.name().toStdString()+";");
    colorHistory.append(temp);
    updateColorHistory();
    ui->color2Box->setStyleSheet(temp);
}

void MainWindow::updateColorHistory(){
    for(int i = 0, j = colorHistory.size(); j > 0 && i < 14 ; i++, j--){
        colorHistoryButtons[i]->setStyleSheet(colorHistory[j - 1]);
    }
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



void MainWindow::on_penTool_clicked()
{
    Canvas::c1 = QColor(Canvas::c1Last);
    Canvas::c2 = QColor(Canvas::c2Last);
}

void MainWindow::on_eraseButton_clicked()
{
    Canvas::c1Last = QColor(Canvas::c1);
    Canvas::c2Last = QColor(Canvas::c2);

    Canvas::c1 = QColor(Qt::white);
    Canvas::c2 = QColor(Qt::white);
}
