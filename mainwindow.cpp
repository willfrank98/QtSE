#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <iostream>
#include <QVector>
#include <QFileDialog>
#include <QBuffer>
#include "frame.h"
using namespace std;


MainWindow::MainWindow(Model *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    this->model = model;
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

    // connects the File>New actions\

    connect(ui->actioncustom_2, &QAction::triggered, [this](){ custom_clicked(); });
    connect(ui->action2x3, &QAction::triggered, this, [this](){ createCanvas(2,3); });
    connect(ui->action4x8, &QAction::triggered, this, [this](){ createCanvas(4,8); });
    connect(ui->action8x4, &QAction::triggered, this, [this](){ createCanvas(8,4); });
    connect(ui->action16x16, &QAction::triggered, this, [this](){ createCanvas(8,8); });
    connect(ui->action32x32, &QAction::triggered, this, [this](){ createCanvas(32,32); });
    connect(ui->action64x64, &QAction::triggered, this, [this](){ createCanvas(64,64); });
    connect(ui->action128x128, &QAction::triggered, this, [this](){ createCanvas(128,128); });
    // connects the File>Export actions
//    connect(ui->actionAnimated_GIF, &QAction::triggered, &model, [=](){
        /*
         * This is just a lambda function to check if FreeImage fails for anyone.
         * Looks like Window will be difficult to get linked correctly.
         * Will fix Monday soon.
         */
//        FreeImage_Initialise();
//        qDebug() << "Initialized FreeImage " << FreeImage_GetVersion();
//        FreeImage_DeInitialise();
    //});

    // connects the File>Exit action
//    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);
    // connects the File>Exit action  

	//connects save and load actions to functions which bring up a FileDialog, then call to the model
	connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveAction);
	connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadAction);

//	connect(this, &MainWindow::createSaveFile, &model, &Model::saveFramesToFile);
//	connect(this, &MainWindow::loadSaveFile, &model, &Model::loadFramesFromFile);
}

MainWindow::~MainWindow()
{
	delete ui;
}
//is called immediately after the save button is called to get a file path
void MainWindow::saveAction()
{
    QString filename = QFileDialog::getSaveFileName();
    QFile f( filename );    // store data in f
    //emit MainWindow::createSaveFile(fileName);
    f.open(QIODevice::WriteOnly);
    QDataStream out(&f);   // we will serialize the data into the file
    out << QImage();
    f.close();
}

void MainWindow::custom_clicked()
{
    popup.show();
}

void MainWindow::loadAction()
{
    QString filename = QFileDialog::getOpenFileName();
    QFile f( filename );
    f.open(QIODevice::ReadOnly);
    QTextStream in(&f);
    QRegExp rx("(\\ )"); //RegEx  ' '
    int lineCounter, width, height, numberOfFrames, heightCounter;
    lineCounter = 0;
    heightCounter = 0;
    //read the first line
    QString line = in.readLine();
    QStringList query = line.split(rx);
    QString qs = query.at(0);
    height = qs.toInt();
    qs = query.at(1);
    width = qs.toInt();
    //read the 2nd line
    line = in.readLine();
    query = line.split(rx);
    numberOfFrames = line.toInt();
    QImage i(width, height, QImage::Format_ARGB32);
    while (!in.atEnd()) {
        heightCounter = lineCounter % height;
        line = in.readLine();
        query = line.split(rx);
        int widthCounter = 0;
        for(int x1 = 0; x1 < width; x1++) {
            int r,g,b,a;
            for (int x2 = 0; x2< 4; x2++){
                qs = query.at(x1*4+x2);
                int val = qs.toInt();
                QPoint qp;
                qp.setX(widthCounter);

                QString str;
                if (x2==0)
                    r = val;
                if (x2==1)
                    g = val;
                if (x2==2)
                    b = val;
                if (x2==3)
                    a = val;
            }
            //cout<<r<<" "<<g<<" "<<" "<<b<<" "<<a<<" "<<endl;
            cout<<x1<<" "<<heightCounter<<endl;
            QColor color;
            color.setRgb(r,g,b,a);
            QBrush brush(color, Qt::SolidPattern);
            i.setPixel(x1, heightCounter, brush.color().rgb());
        }
        QRgb *values = reinterpret_cast<QRgb *>(i.scanLine(0));
        cout<<values<<endl;
        Frame fr(width,height);
        lineCounter++;
        heightCounter++;
    }
    f.close();
    i = i.scaledToHeight(400, Qt::TransformationMode::FastTransformation);
    QPixmap map;
    map.fromImage(i, Qt::AutoColor);
    QGraphicsPixmapItem gpi;
    gpi.setPixmap(map);
    gpi.show();
    gpi.setZValue(2);
    createCanvas(width, height);
    this->canvas->addItem(&gpi);
    canvas->update();
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    cout<<i.save(&buffer, "PNG")<<endl; // writes image into ba in PNG formal. output bool if save was succes.
    QFile filez("a.png"); //write into the debug folder
    filez.open(QIODevice::WriteOnly);
    filez.write(ba);
    filez.close();
}

void MainWindow::updateThis(){
    update();
    ui->graphicsViewCanvas->update();
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

void MainWindow::createCanvas(int sizex, int sizey)
{
    int pixSize;
    if (sizex > sizey)
        pixSize = ui->graphicsViewCanvas->width()/(qreal)sizex;
    else if (sizey > sizex)
        pixSize = ui->graphicsViewCanvas->width()/(qreal)sizey;
    this->canvas = new Canvas(sizex,sizey, pixSize);
    //canvas->setSceneRect(pos);
    ui->graphicsViewCanvas->setScene(canvas);
    ui->graphicsViewCanvas->setEnabled(true);
    this->canvas->drawMode = 0;
    connect(this->canvas, SIGNAL(updateGV()),this, SLOT(updateThis()));
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

}

void MainWindow::on_eraseButton_clicked()
{

}

void MainWindow::on_penToolButton_clicked()
{
    this->canvas->drawMode = 0;
}

void MainWindow::on_eraserToolButton_clicked()
{
    this->canvas->drawMode = 1;
}

void MainWindow::on_rectangleToolButton_clicked()
{
    this->canvas->drawMode = 3;
}

void MainWindow::on_ellipseToolButton_clicked()
{
    this->canvas->drawMode = 2;
}
