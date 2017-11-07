#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QVector>
#include <QObject>
#include <QFileDialog>
#include <QShortcut>

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

    //To make iterating easy, i gave each an unique ID.
    ui->paletteButtons->setId(ui->palette1, 0);
    ui->paletteButtons->setId(ui->palette2, 1);
    ui->paletteButtons->setId(ui->palette3, 2);
    ui->paletteButtons->setId(ui->palette4, 3);
    ui->paletteButtons->setId(ui->palette5, 4);
    ui->paletteButtons->setId(ui->palette6, 5);
    ui->paletteButtons->setId(ui->palette7, 6);
    ui->paletteButtons->setId(ui->palette8, 7);
    ui->paletteButtons->setId(ui->palette9, 8);
    ui->paletteButtons->setId(ui->palette10, 9);
    ui->paletteButtons->setId(ui->palette11, 10);
    ui->paletteButtons->setId(ui->palette12, 11);
    ui->paletteButtons->setId(ui->palette13, 12);
    ui->paletteButtons->setId(ui->palette14, 13);
    ui->paletteButtons->setId(ui->palette15, 14);
    ui->paletteButtons->setId(ui->palette16, 15);


    QColorDialog *colorPicker1 = new QColorDialog();
    QColorDialog *colorPicker2 = new QColorDialog();
    colorPicker1->setOption(QColorDialog::ShowAlphaChannel, true);
    colorPicker2->setOption(QColorDialog::ShowAlphaChannel, true);

    // connects the color selection buttons
    connect(ui->color1Box, &QToolButton::clicked, canvas, [=](){ colorPicker1->show(); });
    connect(colorPicker1, &QColorDialog::colorSelected, canvas, &Canvas::setPrimaryColor);
    connect(colorPicker1, &QColorDialog::colorSelected, this, &MainWindow::updateHistory);
    connect(ui->color2Box, &QToolButton::clicked, canvas, [=](){ colorPicker2->show(); });
    connect(colorPicker2, &QColorDialog::colorSelected, canvas, &Canvas::setSecondaryColor);
    connect(colorPicker2, &QColorDialog::colorSelected, this, &MainWindow::updateHistory);
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

    // Connects the Shortcut Keys
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+1"), ui->centralWidget);
    connect(shortcut, SIGNAL(activated()), ui->penToolButton, SLOT(clicked()));

    // Connects the toolButtons
    //connect(ui->toolButtons, SIGNAL(buttonClicked(int)), &model, SLOT(changeTool(int)));

    // Connects the colorPaletteHistory
    connect(ui->paletteButtons, SIGNAL(buttonClicked(int)), this, SLOT(paletteClicked(int)));
    connect(this, SIGNAL(updateColor(QColor)), canvas, SLOT(setPrimaryColor(QColor)));

    // connects the undo/redo buttons
    connect(ui->actionUndo, &QAction::triggered, this, [=](){this->model->undo();});
    connect(ui->actionRedo, &QAction::triggered, this, [=](){this->model->redo();});

    // connects the File>New actions
    connect(ui->action8x8, &QAction::triggered, this, [=](){ this->model->createFrame(8); });
    connect(ui->action16x16, &QAction::triggered, this, [=](){ this->model->createFrame(16); });
    connect(ui->action32x32, &QAction::triggered, this, [=](){ this->model->createFrame(32); });
    connect(ui->action64x64, &QAction::triggered, this, [=](){ this->model->createFrame(64); });

    // I think something like this is what you wanted? Sorry if I still didn't get it right.
    connect(canvas, &Canvas::modifiedPixels, &model, &Model::modifyFrame);
    connect(&model, &Model::frameUpdated, canvas, &Canvas::displayImage);

    // connects the File>Export actions
    connect(ui->actionCurrentFrame, &QAction::triggered, this, [=](){
        QString filename = QFileDialog::getSaveFileName(this, tr("Export to .PNG"), "./", tr("PNG Files (.png)"));
        this->model->saveCurrentFrameToPNG(filename);
    });
    connect(ui->actionAnimated_GIF, &QAction::triggered, &model, [=](){
        /*
         * Gonna likely replace this with gif.h
         */
//        FreeImage_Initialise();
//        qDebug() << "Initialized FreeImage " << FreeImage_GetVersion();
//        FreeImage_DeInitialise();
    });

    connect(&model, &Model::framePreview, this, [=](){ &MainWindow::addFramePreview; });

    // connects the File>Exit action
    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);
}

void MainWindow::paletteClicked(int buttonID){
    if (colorHistory.size() - buttonID > 0){
        QColor newColor = colorHistory[colorHistory.size() - buttonID - 1];
        QString newStyle = convertRgbToString(newColor.red(),newColor.green(),newColor.blue());
        ui->color1Box->setStyleSheet(newStyle);
        emit updateColor(newColor);
    }
}

void MainWindow::updateHistory(QColor newColor){
    colorHistory.append(newColor);
    for(int i = 0, j = colorHistory.size(); i < 16 && j > 0;i++, j--){
        QString temp = convertRgbToString(colorHistory[j-1].red(), colorHistory[j-1].green(), colorHistory[j-1].blue());
        ui->paletteButtons->button(i)->setStyleSheet(temp);
    }

}

QString MainWindow::convertRgbToString(int red, int green, int blue){
    QString output = "background-color: rgb(" +
            QString::number(red) + "," +
            QString::number(green) + "," +
            QString::number(blue) + ");";
    return output;
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
