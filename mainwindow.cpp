#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QVector>
#include <QObject>

MainWindow::MainWindow(Model &model, QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model.createFrame(32);
    ui->graphicsViewCanvas->setScene(canvas);
    ui->graphicsViewCanvas->scene()->setSceneRect(ui->graphicsViewCanvas->rect());  // scales the canvas to the QGraphicsView
    ui->graphicsViewCanvas->setEnabled(true);


    // this feels hacky; if anyone can get the connections to work without this, that'd be great
    this->model = &model;

    QColorDialog *colorPicker1 = new QColorDialog();
    QColorDialog *colorPicker2 = new QColorDialog();

    // connects the color selection buttons
    connect(ui->color1Box, &QToolButton::clicked, canvas, [=](){ colorPicker1->show(); });
    connect(colorPicker1, &QColorDialog::colorSelected, canvas, &Canvas::setPrimaryColor);
    connect(ui->color2Box, &QToolButton::clicked, canvas, [=](){ colorPicker2->show(); });
    connect(colorPicker2, &QColorDialog::colorSelected, canvas, &Canvas::setSecondaryColor);
    connect(colorPicker1, &QColorDialog::colorSelected, ui->color1Box, [=](QColor color){
        QString newStyle = "background-color: rgb(" +
                QString::number(color.red()) + "," +
                QString::number(color.green()) + "," +
                QString::number(color.blue()) + ");";
        ui->color1Box->setStyleSheet(newStyle);
    });
    connect(colorPicker2, &QColorDialog::colorSelected, ui->color2Box, [=](QColor color){
        QString newStyle = "background-color: rgb(" +
                QString::number(color.red()) + "," +
                QString::number(color.green()) + "," +
                QString::number(color.blue()) + ");";
        ui->color2Box->setStyleSheet(newStyle);
    });

    // Connects the toolButtons


    // connects the undo/redo buttons
    //connect(ui->actionUndo, &QAction::triggered, this, Canvas::undo());
    //connect(ui->actionRedo, &QAction::triggered, this, Canvas::redo());

    // connects the File>New actions
    connect(ui->action8x8, &QAction::triggered, this, [=](){ this->model->createFrame(8); });
    connect(ui->action16x16, &QAction::triggered, this, [=](){ this->model->createFrame(16); });
    connect(ui->action32x32, &QAction::triggered, this, [=](){ this->model->createFrame(32); });
    connect(ui->action64x64, &QAction::triggered, this, [=](){ this->model->createFrame(64); });

    // I think something like this is what you wanted? Sorry if I still didn't get it right.
    connect(canvas, &Canvas::modifiedPixels, &model, &Model::modifyFrame);
    connect(&model, &Model::frameUpdated, canvas, &Canvas::displayImage);

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

    connect(&model, &Model::framePreview, this, [=](){ &MainWindow::addFramePreview; });

    // connects the File>Exit action
    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);
}

MainWindow::~MainWindow()
{

	delete ui;
}


void MainWindow::addFramePreview(QList<Frame> frames){

    //QWidget *window = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    QString name = "";

    for (int i = 0; i< frames.size(); i++)
    {
    name = "Frame " + i;
    QLabel *name = new QLabel();
    name-> setFrameStyle(QLabel::Sunken | QLabel::Box);
    //name->setPixmap(&QPixmap::fromImage(&frames.at(i).pixels()));    //This line needs work
    name->setVisible(true);
    layout->addWidget(name);
    }

    ui->frameContainer->addLayout(layout);

}
