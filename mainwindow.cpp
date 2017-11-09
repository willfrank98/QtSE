/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * mainwindow.cpp
 *
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QVector>
#include <QShortcut>
#include <QFileDialog>

MainWindow::MainWindow(Model &model, QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->model = &model;

    // setup the main graphics view
    ui->graphicsViewCanvas->setScene(canvas);
    ui->graphicsViewCanvas->scene()->setSceneRect(ui->graphicsViewCanvas->rect());  // scales the canvas to the QGraphicsView
    ui->graphicsViewCanvas->setEnabled(true);

    QColorDialog *colorPicker1 = new QColorDialog();
    QColorDialog *colorPicker2 = new QColorDialog();
    colorPicker1->setOption(QColorDialog::ShowAlphaChannel, true);
    colorPicker2->setOption(QColorDialog::ShowAlphaChannel, true);

    // adds palette buttons to a list (ordered)
    for (int i = 0; i < ui->paletteButtons->buttons().count(); i++) {
        QString name = QString("palette"+QString::number(i + 1));
        QToolButton *button = ui->frameToolsAndPalette->findChild<QToolButton*>(name);
        paletteHistory.append(QColor(255, 255, 255, 255));

        connect(button, &QToolButton::clicked, this, [=]() {
            canvas->setPrimaryColor(paletteHistory.at(i));
            ui->color1Box->setStyleSheet(colorToString(paletteHistory.at(i)));
        });

        paletteButtons.append(button);
    }

    // Connects primary and secondary color boxes, palette history, and other color things
    connect(ui->color1Box, &QToolButton::clicked, canvas, [=](){ colorPicker1->show(); });
    connect(colorPicker1, &QColorDialog::colorSelected, canvas, &Canvas::setPrimaryColor);
    connect(ui->color2Box, &QToolButton::clicked, canvas, [=](){ colorPicker2->show(); });
    connect(colorPicker2, &QColorDialog::colorSelected, canvas, &Canvas::setSecondaryColor);
    connect(colorPicker1, &QColorDialog::colorSelected, ui->color1Box, [=](QColor color){
        QString newStyle = colorToString(color);
        ui->color1Box->setStyleSheet(newStyle);
        if (paletteHistory.size() == ui->paletteButtons->buttons().count()) paletteHistory.replace(paletteHistoryIndex, color);
        else paletteHistory.insert(paletteHistoryIndex, color);
        updatePaletteHistory();
    });
    connect(colorPicker2, &QColorDialog::colorSelected, ui->color2Box, [=](QColor color){
        QString newStyle = colorToString(color);
        ui->color2Box->setStyleSheet(newStyle);
        if (paletteHistory.size() == ui->paletteButtons->buttons().count()) paletteHistory.replace(paletteHistoryIndex, color);
        else paletteHistory.insert(paletteHistoryIndex, color);
        updatePaletteHistory();
    });
    connect(ui->swapColors, &QToolButton::clicked, this, [=](){
        QString temp = ui->color1Box->styleSheet();
        ui->color1Box->setStyleSheet(ui->color2Box->styleSheet());
        ui->color2Box->setStyleSheet(temp);
        canvas->swapColors();
    });

    // Connects the toolButtons
    connect(ui->penToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(PenTool); });
    connect(ui->mirrorPenToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(MirrorPenTool); });
    connect(ui->eraserToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(EraserTool); });
    connect(ui->ditheringPenToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(DitheringTool); });
    connect(ui->rectangleToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(RectangleTool); });
    connect(ui->bucketPenToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(BucketFillTool); });
    connect(ui->colorFillToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(ColorFillTool); });
    connect(ui->ellipseToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(EllipseTool); });
    connect(ui->rectangularSelectionToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(RectSelectTool); });
    connect(ui->lineToolButton, &QToolButton::clicked, canvas, [=](){ canvas->setTool(LineTool); });

    // Connects the undo/redo actions
    connect(ui->actionUndo, &QAction::triggered, &model, &Model::undo);
    connect(ui->actionRedo, &QAction::triggered, &model, &Model::redo);
    connect(canvas, &Canvas::pixelsModified, &model, &Model::updateUndoRedo);
    connect(&model, &Model::frameUpdated, canvas, &Canvas::setFrame);

    // Connects the File>New actions
    connect(ui->action8x8, &QAction::triggered, this, [=](){ newCanvas(8); });
    connect(ui->action16x16, &QAction::triggered, this, [=](){ newCanvas(16); });
    connect(ui->action32x32, &QAction::triggered, this, [=](){ newCanvas(32); });
    connect(ui->action64x64, &QAction::triggered, this, [=](){ newCanvas(64); });

    // Connects the drawing surfaces (main drawing area, preview area, FPS, etc.)
    connect(ui->spinBoxSpeed, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &model, &Model::setPreviewFPS);
    connect(&model, &Model::frameUpdated, canvas, &Canvas::setFrame);
    connect(&model, &Model::previewFrame, this, [=](QImage image) {
        // TODO: do something with the image from previewFrame
        ui->labelPreview->setPixmap(QPixmap::fromImage(image.scaled(ui->labelPreview->size())));
    });
    connect(ui->zoomLevelCheckbox, &QCheckBox::toggled, this, [=](bool toggled){
        // TODO: handle the zoom toggle
    });
    connect(canvas, &Canvas::frameUpdated, this, [=](Frame *frame){
        QLabel *l = frameButtons.checkedButton()->parent()->findChild<QLabel *>("view");
        l->setPixmap(QPixmap::fromImage(frame->pixels().scaled(l->size())));
    });

    // connects the File>Export actions
    connect(ui->actionCurrentFrame, &QAction::triggered, this, [=]() {
        QString filename = QFileDialog::getSaveFileName(this, "Export to .PNG", "./", "PNG Files (.png)");
        this->model->saveFrameToPNG(filename);
    });
    connect(ui->actionAnimated_GIF, &QAction::triggered, &model, [=](){
        QString filename = QFileDialog::getSaveFileName(this, "Export to animated .GIF", "./", "GIF Files (.gif)");
        this->model->saveAnimatedGIF(filename);
    });

    // connects the File>Exit action
    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);

    connect(ui->pushButtonAddFrame, &QPushButton::clicked, &model, &Model::createFrame);
    connect(&model, &Model::frameCreated, this, [=](int i){ newFrame(i); });

	//	connects File>Save and >Load
		connect(ui->actionSave, &QAction::triggered, this, [=]() {
			QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "./", tr("Sprites (.ssp)"));
			this->model->saveFrameToFile(filename);
		});
		connect(ui->actionLoad, &QAction::triggered, this, [=]() {
			QString filename = QFileDialog::getOpenFileName(this, tr("Load File"), "./", tr("Sprites (.ssp)"));
			this->model->loadFrameFromFile(filename);
		});

    // Connects the Shortcut Keys
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+1"), ui->centralWidget);
    connect(shortcut, SIGNAL(activated()), ui->penToolButton, SLOT(clicked()));

    // I kind of like these one-liner shortcut things better, but we can use whatever
//    ui->penToolButton->setShortcut(Qt::CTRL | Qt::Key_1);
//    ui->actionUndo->setShortcut(Qt::CTRL | Qt::Key_Z);
//    ui->actionRedo->setShortcut(Qt::CTRL | Qt::Key_R);

    model.newSurface(32);
}

MainWindow::~MainWindow()
{
	delete ui;
}

QString MainWindow::colorToString(QColor color) {
    return "background-color: rgb(" +
            QString::number(color.red()) + "," +
            QString::number(color.green()) + "," +
            QString::number(color.blue()) + ");";
}

void MainWindow::updatePaletteHistory() {
    QString newStyle;
    newStyle = colorToString(paletteHistory.at(paletteHistoryIndex));
    paletteButtons.at(paletteHistoryIndex)->setStyleSheet(newStyle);
    paletteHistoryIndex++;

    if (paletteHistoryIndex == ui->paletteButtons->buttons().count()) paletteHistoryIndex = 0;
}

// These signals and slots are getting ridiculous.
void MainWindow::newCanvas(int dimension) {
    emit resetCanvas();
    this->disconnect();
    model->newSurface(dimension);
}

void MainWindow::newFrame(int index) {
    QFrame *newFrame = new QFrame();
    newFrame->setGeometry(0, 0, 75, 75);
    newFrame->setMinimumWidth(75);
    newFrame->setMinimumHeight(75);
    newFrame->setMaximumWidth(75);
    newFrame->setMaximumHeight(75);
    connect(this, &MainWindow::resetCanvas, this, [=]() {
        newFrame->hide();
        ui->frameContainer->layout()->removeWidget(newFrame);
        newFrame->setParent(nullptr);
        delete newFrame;
    });

    QLabel *framePreview = new QLabel();
    framePreview->setGeometry(0, 0, 75, 75);
    framePreview->setObjectName("view");
    framePreview->setMinimumWidth(75);
    framePreview->setMinimumHeight(75);
    framePreview->setMaximumWidth(75);
    framePreview->setMaximumHeight(75);
    framePreview->setAttribute(Qt::WA_TransparentForMouseEvents);

    QPushButton *frameSelected = new QPushButton(newFrame);
    frameSelected->setGeometry(0, 0, 75, 75);
    frameSelected->setObjectName("button");
    frameSelected->setCheckable(true);
    frameButtons.addButton(frameSelected, index);
    connect(frameSelected, &QPushButton::clicked, model, [=](){
        int frameNum = ui->frameContainer->layout()->indexOf(newFrame);
        model->setActiveFrame(frameNum);
    });
    framePreview->setParent(newFrame);

    QToolButton *dupeFrame = new QToolButton(newFrame);
    dupeFrame->setGeometry(55, 55, 20, 20);
    dupeFrame->setAutoRaise(true);
    dupeFrame->setObjectName("dupe");
    dupeFrame->setText("D");
    connect(dupeFrame, &QToolButton::clicked, model, [=](){
        int frameNum = ui->frameContainer->layout()->indexOf(newFrame);
        frameSelected->setChecked(true);
        model->setActiveFrame(frameNum);
        model->dupeFrame(frameButtons.id(frameButtons.checkedButton()));
    });

    QToolButton *removeFrame = new QToolButton(newFrame);
    removeFrame->setGeometry(6, 6, 13, 13);
    removeFrame->setAutoRaise(true);
    removeFrame->setObjectName("remove");
    removeFrame->setText("X");
//   connect(removeFrame, &QToolButton::clicked, model, [=](){
//       int frameNum = ui->frameContainer->layout()->indexOf(newFrame);
//        frameSelected->setChecked(true);
//        model->setActiveFrame(frameNum);
//        model->deleteFrame(frameButtons.id(frameButtons.checkedButton()));
//    });


    // remove the spacer
    ui->frameContainer->layout()->removeItem(ui->frameContainer->layout()->itemAt(ui->frameContainer->layout()->count()-1));

    // add the newFrame
    ui->frameContainer->findChild<QHBoxLayout *>("horizontalLayout")->insertWidget(index, newFrame);
    newFrame->findChild<QPushButton *>("button")->setChecked(true);

    // re-add a horizontal spacer
    ui->frameContainer->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));
}
