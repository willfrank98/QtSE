#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QVector>

MainWindow::MainWindow(Model &model, QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsViewCanvas->setScene(canvas);
    ui->graphicsViewCanvas->setEnabled(true);
    this->model = &model;
    QColorDialog *colorPicker1 = new QColorDialog();
    QColorDialog *colorPicker2 = new QColorDialog();

    // connects the color selection buttons
    connect(ui->color1Box, &QToolButton::clicked, canvas, [=](){ colorPicker1->show(); });
    connect(colorPicker1, &QColorDialog::colorSelected, canvas, &Canvas::setPrimaryColor);
    connect(ui->color2Box, &QToolButton::clicked, canvas, [=](){ colorPicker2->show(); });
    connect(colorPicker2, &QColorDialog::colorSelected, canvas, &Canvas::setSecondaryColor);

    // connects the undo/redo buttons
    //connect(ui->actionUndo, &QAction::triggered, this, Canvas::undo());
    //connect(ui->actionRedo, &QAction::triggered, this, Canvas::redo());

    // connects the File>New actions
    connect(ui->action8x8, &QAction::triggered, this, [=](){ this->model->createFrame(8); });
    connect(ui->action16x16, &QAction::triggered, this, [=](){ this->model->createFrame(16); });
    connect(ui->action32x32, &QAction::triggered, this, [=](){ this->model->createFrame(32); });
    connect(ui->action64x64, &QAction::triggered, this, [=](){ this->model->createFrame(64); });
    connect(&model, &Model::frameCreated, canvas, &Canvas::displayImage);

    // connects the File>Export actions
    connect(ui->actionAnimated_GIF, &QAction::triggered, &model, [=](){
        /*
         * This is just a lambda function to check if FreeImage fails for anyone.
         * Looks like Window will be difficult to get linked correctly.
         */
//        FreeImage_Initialise();
//        qDebug() << "Initialized FreeImage " << FreeImage_GetVersion();
//        FreeImage_DeInitialise();
    });

    // connects the File>Exit action
    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);
}

MainWindow::~MainWindow()
{
	delete ui;
}
